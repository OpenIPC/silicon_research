/*
 * gcc vdec-sample.c -o vdec-sample -s -Wall
 *
 * Usage:
 * ./vdec-sample 5600 192.168.1.10 6000
 * gst-launch-1.0 udpsrc port=6000 ! application/x-rtp ! rtph265depay ! avdec_h265 ! fpsdisplaysink sync=false
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <endian.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 512 * 512
#define MAX_SIZE 1200

struct RTPHeader {
	uint8_t version;
	uint8_t payload_type;
	uint16_t sequence;
	uint32_t timestamp;
	uint32_t ssrc_id;
};

struct sockaddr_in tx_address;
static bool debug = false;
static int nal_size = 0;
static int rtp_sequence = 0;

static void create_stream(int port, char *data, int size) {
	struct RTPHeader rtp_header;
	rtp_header.version = 0x80;
	rtp_header.payload_type = 0x60;
	rtp_header.sequence = htobe16(rtp_sequence++);
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
	msg.msg_name = &tx_address;
	msg.msg_namelen = sizeof(struct sockaddr_in);

	if (debug) {
		printf("SX: ");
		for (int i = 0; i < 16; i++) {
			printf("0x%02X, ", data[i] & 0xFF);
		}

		printf("len: %d\n", size);
	}

	if (sendmsg(port, &msg, 0) < 0) {
		printf("> Cannot send packet: %s [%dKB]\n", strerror(errno), size / 1024);
	}
}

static void create_fragment(int port, char *data, int size) {
	char nal_prefix = 4;
	data += nal_prefix;
	size -= nal_prefix;

	if (size > MAX_SIZE) {
		char nal_type_avc = data[0] & 0x1F;
		char nal_type_hevc = (data[0] >> 1) & 0x3F;
		char nal_bits_avc = data[0] & 0xE0;
		char nal_bits_hevc = data[0] & 0x81;

		char tx_buffer[4096];
		char nal_bits = 2;
		bool start_bit = true;

		while (size) {
			int chunk = size > MAX_SIZE ? MAX_SIZE : size;
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
				nal_bits = 3;

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

			memcpy(tx_buffer + nal_bits, data, chunk + nal_bits);
			create_stream(port, tx_buffer, chunk + nal_bits);

			data += chunk;
			size -= chunk;
		}
	} else {
		create_stream(port, data, size);
	}
}

static int decode_frame(char *rx_buffer, int rx_length, int header_size, char *nal_buffer) {
	rx_buffer += header_size;
	rx_length -= header_size;

	char fragment_avc = rx_buffer[0] & 0x1F;
	char fragment_hevc = (rx_buffer[0] >> 1) & 0x3F;

	char start_bit = 0;
	char end_bit = 0;
	char nal_code = 4;

	if (fragment_avc == 28 || fragment_hevc == 49) {
		if (fragment_avc == 28) {
			start_bit = rx_buffer[1] & 0x80;
			end_bit = rx_buffer[1] & 0x40;
			nal_buffer[4] = (rx_buffer[0] & 0xE0) | (rx_buffer[1] & 0x1F);
		} else {
			start_bit = rx_buffer[2] & 0x80;
			end_bit = rx_buffer[2] & 0x40;
			nal_buffer[4] = (rx_buffer[0] & 0x81) | (rx_buffer[2] & 0x3F) << 1;
			nal_buffer[5] = 1;

			nal_code++;
			rx_buffer++;
			rx_length--;
		}

		rx_buffer++;
		rx_length--;

		if (start_bit) {
			nal_buffer[0] = 0;
			nal_buffer[1] = 0;
			nal_buffer[2] = 0;
			nal_buffer[3] = 1;

			memcpy(nal_buffer + nal_code, rx_buffer, rx_length);
			nal_size = rx_length + nal_code;
		} else {
			rx_buffer++;
			rx_length--;

			memcpy(nal_buffer + nal_size, rx_buffer, rx_length);
			nal_size += rx_length;

			if (end_bit) {
				return nal_size;
			}
		}
	} else {
		nal_buffer[0] = 0;
		nal_buffer[1] = 0;
		nal_buffer[2] = 0;
		nal_buffer[3] = 1;
		memcpy(nal_buffer + nal_code, rx_buffer, rx_length);

		return rx_length + nal_code;
	}

	return 0;
}

int main(int argc, const char *argv[]) {
	char output_addr[64];
	int input_port = 5600;
	int output_port = 6000;

	if (argc > 1) {
		input_port = atoi(argv[1]);
	}

	struct sockaddr_in rx_address;
	rx_address.sin_family = AF_INET;
	rx_address.sin_port = htons(input_port);
	rx_address.sin_addr.s_addr = INADDR_ANY;

	int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	bind(udp_sock, (struct sockaddr*)&rx_address, sizeof(struct sockaddr_in));

	char *rx_buffer = malloc(BUF_SIZE);
	char *nal_buffer = malloc(BUF_SIZE);

	char *local_host = "127.0.0.1";
	if (argc > 2) {
		strcpy(output_addr, argv[2]);
	} else {
		strcpy(output_addr, local_host);
	}

	if (argc > 3) {
		output_port = atoi(argv[3]);
	}

	tx_address.sin_family = AF_INET;
	tx_address.sin_port = htons(output_port);
	tx_address.sin_addr.s_addr = inet_addr(output_addr);

	if (argc > 4) {
		debug = atoi(argv[4]);
	}

	printf("Input: %s:%d, Output: %s:%d [%d]\n",
		local_host, input_port, output_addr, output_port, debug);

	while (true) {
		int rx_length = recv(udp_sock, rx_buffer, BUF_SIZE, 0);
		if (rx_length < 0) {
			usleep(1);
			continue;
		}

		int rtp_header = 0;
		if (rx_buffer[0] & 0x80 && rx_buffer[1] & 0x60) {
			rtp_header = 12;
		}

		if (debug) {
			printf("RX: ");
			for (int i = rtp_header; i < 16 + rtp_header; i++) {
				printf("0x%02X, ", rx_buffer[i] & 0xFF);
			}

			printf("len: %d\n", rx_length - rtp_header);
		}

		int nal_size = decode_frame(rx_buffer, rx_length, rtp_header, nal_buffer);
		if (!nal_size) {
			continue;
		}

		if (debug) {
			printf("TX: ");
			for (int i = 4; i < 20; i++) {
				printf("0x%02X, ", nal_buffer[i] & 0xFF);
			}

			printf("len: %d\n", nal_size - 4);
		}

		create_fragment(udp_sock, nal_buffer, nal_size);
	}

	free(rx_buffer);
	free(nal_buffer);

	return 0;
}
