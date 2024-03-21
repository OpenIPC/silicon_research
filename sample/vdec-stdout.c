/*
 * gcc vdec-stdout.c -o vdec-stdout -s -Wall
 *
 * Usage:
 * ./vdec-stdout 5600 | ffplay -i -
 * ./vdec-stdout 5600 | gst-launch-1.0 fdsrc ! decodebin ! fpsdisplaysink sync=false
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 512 * 512

static int nal_size = 0;
static bool nal_start = false;

static int decode_frame(char *rx_buffer, int rx_length, int header_size, char *nal_buffer) {
	rx_buffer += header_size;
	rx_length -= header_size;

	char fragment_avc = rx_buffer[0] & 0x1F;
	char fragment_hevc = (rx_buffer[0] >> 1) & 0x3F;

	char start_bit = 0;
	char end_bit = 0;
	char nal_code = 4;

	if (fragment_avc == 7 || fragment_hevc == 32) {
		nal_start = true;
	}

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
	int rtp_port = 5600;

	if (argc > 1) {
		rtp_port = atoi(argv[1]);
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(rtp_port);
	address.sin_addr.s_addr = INADDR_ANY;

	int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	bind(udp_sock, (struct sockaddr*)&address, sizeof(struct sockaddr_in));

	char *rx_buffer = malloc(BUFFER_SIZE);
	char *nal_buffer = malloc(BUFFER_SIZE);

	while (true) {
		int rx_length = recv(udp_sock, rx_buffer, BUFFER_SIZE, 0);
		if (rx_length < 0) {
			usleep(1);
			continue;
		}

		int rtp_header = 0;
		if (rx_buffer[0] & 0x80 && rx_buffer[1] & 0x60) {
			rtp_header = 12;
		}

		int nal_size = decode_frame(rx_buffer, rx_length, rtp_header, nal_buffer);
		if (!nal_size || !nal_start) {
			continue;
		}

		fwrite(nal_buffer, nal_size, 1, stdout);
		fflush(stdout);
	}

	free(rx_buffer);
	free(nal_buffer);

	return 0;
}
