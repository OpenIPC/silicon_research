// x86_64-linux-musl-gcc vdec-i386.c -o vdec -s -static

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

static uint32_t in_nal_size = 0;

static uint8_t* decode_frame(uint8_t* rx_buffer, uint32_t rx_size,
		uint32_t header_size, uint8_t* nal_buffer, uint32_t* out_nal_size) {
	rx_buffer += header_size;
	rx_size -= header_size;

	// Get NAL type
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
			// Write NAL header
			nal_buffer[0] = 0;
			nal_buffer[1] = 0;
			nal_buffer[2] = 0;
			nal_buffer[3] = 1;

			// Copy data
			memcpy(nal_buffer + copy_size, rx_buffer, rx_size);
			in_nal_size = rx_size + copy_size;
		} else if (end_bit) {
			rx_buffer++;
			rx_size--;

			memcpy(nal_buffer + in_nal_size, rx_buffer, rx_size);
			in_nal_size += rx_size;

			// Store NAL size
			*out_nal_size = in_nal_size;
			in_nal_size = 0;

			return nal_buffer;
		} else {
			rx_buffer++;
			rx_size--;
			memcpy(nal_buffer + in_nal_size, rx_buffer, rx_size);
			in_nal_size += rx_size;
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
	struct sockaddr_in address;
	uint16_t input_port = 5600;
	uint16_t output_port = 6000;
	char output_addr[64];
	bool debug = false;

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
			int r = 20;
			printf("RX: 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, len: %d\n",
				rx_buffer[0 + r], rx_buffer[1 + r], rx_buffer[2 + r], rx_buffer[3 + r],
				rx_buffer[4 + r], rx_buffer[5 + r], rx_buffer[6 + r], rx_buffer[7 + r], rx);
		}

		uint32_t rtp_header = 0;
		if (rx_buffer[8] == 0x80) {
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
			int t = 4;
			printf("TX: 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, len: %d\n",
				buffer[0 + t], buffer[1 + t], buffer[2 + t], buffer[3 + t],
				buffer[4 + t], buffer[5 + t], buffer[6 + t], buffer[7 + t], size);
		}

		sendto(port, buffer, size, 0, (struct sockaddr*)&address, sizeof(struct sockaddr_in));
	}

	return 0;
}
