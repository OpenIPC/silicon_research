// x86_64-linux-musl-gcc vdec-i386.c -o vdec -s -static -Wall

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <endian.h>
#include <arpa/inet.h>

struct RTPHeader {
	uint8_t version;
	uint8_t payload_type;
	uint16_t sequence;
	uint32_t timestamp;
	uint32_t ssrc_id;
};

#define MAX_SIZE 1400

static struct sockaddr_in address;
static uint32_t in_nal_size = 0;
static uint16_t rtp_sequence = 0;
static bool debug = false;

static void send_data(int port, uint8_t* data, uint32_t size) {
	struct RTPHeader rtp_header;
	rtp_header.version = 0x80;
	rtp_header.sequence = htobe16(rtp_sequence++);
	rtp_header.payload_type = 0x60;
	rtp_header.timestamp = 0;
	rtp_header.ssrc_id = 0xDEADBEEF;

	struct iovec iov[2];
	iov[0].iov_base = &rtp_header;
	iov[0].iov_len = sizeof(struct RTPHeader);
	iov[1].iov_base = data;
	iov[1].iov_len = size;

	struct msghdr msg = {0};
	msg.msg_iovlen = 2;
	msg.msg_iov = iov;
	msg.msg_name = &address;
	msg.msg_namelen = sizeof(struct sockaddr_in);

	if (sendmsg(port, &msg, 0) < 0) {
		printf("> Cannot send packet: %s [%dKB]\n", strerror(errno), size / 1024);
	}

	if (debug) {
		printf("SX: ");
		for (int i = 0; i < size; i++) {
			printf("0x%02X, ", data[i]);
		}
		printf("len: %d\n", size);
	}
}

static void create_fragment(int port, uint8_t* data, uint32_t size) {
	uint8_t tx_buffer[1024 * 64];
	data += 4;
	size -= 4;

	if (size > MAX_SIZE) {
		uint8_t nal_type_avc = data[0] & 0x1F;
		uint8_t nal_type_hevc = (data[0] >> 1) & 0x3F;
		uint8_t nal_bits_avc = data[0] & 0xE0;
		uint8_t nal_bits_hevc = data[0] & 0x81;
		uint8_t rx_size = 0;
		bool first = true;

		while (size) {
			uint32_t chunk_size = size > MAX_SIZE ? MAX_SIZE : size;
			if (nal_type_avc == 1 || nal_type_avc == 5) {
				rx_size = 2;
				if (first) {
					data[-1] = nal_bits_avc | 28;
					data[0] = 0x80 | nal_type_avc;
				} else {
					data[-1] = nal_bits_avc | 28;
					data[0] = nal_type_avc;
					if (chunk_size == size) {
						data[0] |= 0x40;
						rx_size--;
					}
				}
			}

			if (nal_type_hevc == 1 || nal_type_hevc == 19) {
				rx_size = 3;
				if (first) {
					data[-1] = nal_bits_hevc | 49 << 1;
					data[0] = 1;
					data[1] = 0x80 | nal_type_hevc;
				} else {
					data[-1] = nal_bits_hevc | 49 << 1;
					data[0] = 1;
					data[1] = nal_type_hevc;
					if (chunk_size == size) {
						data[1] |= 0x40;
						rx_size--;
					}
				}
			}

			memcpy(tx_buffer, data - 1, chunk_size + rx_size);
			send_data(port, tx_buffer, chunk_size + rx_size);

			data += chunk_size;
			size -= chunk_size;
			first = false;
		}
	} else {
		send_data(port, data, size);
	}
}

static uint8_t* decode_frame(uint8_t* rx_buffer, uint32_t rx_size,
		uint32_t header_size, uint8_t* nal_buffer, uint32_t* out_nal_size) {
	rx_buffer += header_size;
	rx_size -= header_size;

	uint8_t fragment_type_avc = rx_buffer[0] & 0x1F;
	uint8_t fragment_type_hevc = (rx_buffer[0] >> 1) & 0x3F;

	uint8_t start_bit = 0;
	uint8_t end_bit = 0;
	uint8_t copy_size = 4;

	if (fragment_type_avc == 28 || fragment_type_hevc == 49) {
		if (fragment_type_avc == 28) {
			start_bit = rx_buffer[1] & 0x80;
			end_bit = rx_buffer[1] & 0x40;
			nal_buffer[4] = (rx_buffer[0] & 0xE0) | (rx_buffer[1] & 0x1F);
		} else {
			start_bit = rx_buffer[2] & 0x80;
			end_bit = rx_buffer[2] & 0x40;
			nal_buffer[4] = (rx_buffer[0] & 0x81) | (rx_buffer[2] & 0x3F) << 1;
			nal_buffer[5] = 1;
			copy_size++;
			rx_buffer++;
			rx_size--;
		}

		rx_buffer++;
		rx_size--;

		if (start_bit) {
			nal_buffer[0] = 0;
			nal_buffer[1] = 0;
			nal_buffer[2] = 0;
			nal_buffer[3] = 1;

			memcpy(nal_buffer + copy_size, rx_buffer, rx_size);
			in_nal_size = rx_size + copy_size;
		} else {
			rx_buffer++;
			rx_size--;
			memcpy(nal_buffer + in_nal_size, rx_buffer, rx_size);
			in_nal_size += rx_size;

			if (end_bit) {
				*out_nal_size = in_nal_size;
				in_nal_size = 0;
				return nal_buffer;
			}
		}

		return NULL;
	} else {
		rx_buffer[-4] = 0;
		rx_buffer[-3] = 0;
		rx_buffer[-2] = 0;
		rx_buffer[-1] = 1;

		*out_nal_size = rx_size + copy_size;
		in_nal_size = 0;
		return rx_buffer - copy_size;
	}
}

int main(int argc, const char* argv[]) {
	uint16_t input_port = 5600;
	uint16_t output_port = 6000;
	char output_addr[64];

	uint8_t* rx_buffer = malloc(1024 * 1024);
	uint8_t* nal_buffer = malloc(1024 * 1024);
	int port = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (argc > 1) {
		input_port = atoi(argv[1]);
	}

	memset(&address, 0x00, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(input_port);
	bind(port, (struct sockaddr*)&address, sizeof(struct sockaddr_in));

	if (argc > 2) {
		strcpy(output_addr, argv[2]);
	} else {
		strcpy(output_addr, "127.0.0.1");
	}

	if (argc > 3) {
		output_port = atoi(argv[3]);
	}

	if (argc > 4) {
		debug = atoi(argv[4]);
	}

	address.sin_port = htons(output_port);
	address.sin_addr.s_addr = inet_addr(output_addr);

	printf("Input: %d, Output: %s:%d [%d]\n", input_port, output_addr, output_port, debug);

	while (true) {
		int rx = recv(port, rx_buffer + 8, 1024 * 20, 0);
		if (rx <= 0) {
			usleep(1);
			continue;
		}

		if (debug) {
			printf("RX: ");
			for (int i = 20; i < rx + 8; i++) {
				printf("0x%02X, ", rx_buffer[i]);
			}
			printf("len: %d\n", rx - 12);
		}

		uint32_t rtp_header = 0;
		if (rx_buffer[8] & 0x80 && rx_buffer[9] & 0x60) {
			rtp_header = 12;
		}

		uint32_t size = 0;
		uint8_t* buffer = decode_frame(rx_buffer + 8, rx, rtp_header, nal_buffer, &size);
		if (!buffer) {
			continue;
		}

		if (size < 5) {
			printf("> Broken frame\n");
		}

		if (debug) {
			printf("TX: ");
			for (int i = 4; i < size; i++) {
				printf("0x%02X, ", buffer[i]);
			}
			printf("len: %d\n", size - 4);
		}

		create_fragment(port, buffer, size);
	}

	return 0;
}
