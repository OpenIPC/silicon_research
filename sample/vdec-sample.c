/*
 * x86_64-linux-musl-gcc vdec-sample.c -o vdec-sample -s -static -Wall
 * 
 * Usage:
 * ./vdec 5600 192.168.1.10 6000 0
 * gst-launch-1.0 udpsrc port=6000 ! application/x-rtp ! rtph265depay ! avdec_h265 ! autovideosink sync=false
 * 
 */

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
static uint32_t debug = 0;

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

	if (debug > 1) {
		int sx_size = (debug > 2) ? size : 8;
		printf("SX: ");
		for (int i = 0; i < sx_size; i++) {
			printf("0x%02X, ", data[i]);
		}
		printf("len: %d\n", size);
	}
}

static void create_fragment(int port, uint8_t* data, uint32_t size) {
	uint8_t prefix = 4;
	data += prefix;
	size -= prefix;

	if (size > MAX_SIZE + prefix) {
		uint8_t nal_type_avc = data[0] & 0x1F;
		uint8_t nal_type_hevc = (data[0] >> 1) & 0x3F;
		uint8_t nal_bits_avc = data[0] & 0xE0;
		uint8_t nal_bits_hevc = data[0] & 0x81;

		uint8_t tx_buffer[4096];
		uint8_t tx_size = 2;
		bool start_bit = true;

		while (size) {
			uint32_t chunk = size > MAX_SIZE ? MAX_SIZE : size;
			if (nal_type_avc == 1 || nal_type_avc == 5) {
				tx_buffer[0] = nal_bits_avc | 28;
				tx_buffer[1] = nal_type_avc;

				if (start_bit) {
					data++;
					size--;
					tx_buffer[1] = 0x80 | nal_type_avc;
					start_bit = false;
				}

				if (chunk == size) {
					tx_buffer[1] |= 0x40;
				}
			}

			if (nal_type_hevc == 1 || nal_type_hevc == 19) {
				tx_buffer[0] = nal_bits_hevc | 49 << 1;
				tx_buffer[1] = 1;
				tx_buffer[2] = nal_type_hevc;
				tx_size = 3;

				if (start_bit) {
					data += 2;
					size -= 2;
					tx_buffer[2] = 0x80 | nal_type_hevc;
					start_bit = false;
				}

				if (chunk == size) {
					tx_buffer[2] |= 0x40;
				}
			}

			memcpy(tx_buffer + tx_size, data, chunk + tx_size);
			send_data(port, tx_buffer, chunk + tx_size);

			data += chunk;
			size -= chunk;
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

		uint32_t rtp_header = 0;
		if (rx_buffer[8] & 0x80 && rx_buffer[9] & 0x60) {
			rtp_header = 12;
		}

		if (debug) {
			int rx_size = (debug > 2) ? rx + 8 : 16 + rtp_header;
			printf("RX: ");
			for (int i = 8 + rtp_header; i < rx_size; i++) {
				printf("0x%02X, ", rx_buffer[i]);
			}
			printf("len: %d\n", rx - rtp_header);
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
			int tx_size = (debug > 2) ? size : 12;
			printf("TX: ");
			for (int i = 4; i < tx_size; i++) {
				printf("0x%02X, ", buffer[i]);
			}
			printf("len: %d\n", size - 4);
		}

		create_fragment(port, buffer, size);
	}

	return 0;
}
