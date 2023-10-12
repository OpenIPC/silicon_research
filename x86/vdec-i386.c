// x86_64-linux-musl-gcc vdec.c -o vdec -s -static

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

static uint8_t* decode_frame(uint8_t* rx_buffer, uint32_t rx_size, uint32_t header_size,
	uint8_t* nal_buffer, uint32_t* nal_buffer_used, uint32_t* out_nal_size) {
	rx_buffer += header_size;
	rx_size -= header_size;

	// Get NAL type
	uint8_t fragment_type_avc = rx_buffer[0] & 0x1F;
	uint8_t fragment_type_hevc = (rx_buffer[0] >> 1) & 0x3F;

	uint8_t nal_type = 0;
	uint8_t start_bit = 0;
	uint8_t end_bit = 0;
	uint8_t copy_size = 4;

	uint8_t* in_buffer = NULL;
	uint32_t in_size = 0;

	if (fragment_type_avc == 28 || fragment_type_hevc == 49) {
		if (fragment_type_avc == 28) {
			start_bit = rx_buffer[1] & 0x80;
			end_bit = rx_buffer[1] & 0x40;
			nal_buffer[4] = (rx_buffer[0] & 0xE0) | (rx_buffer[1] & 0x1F);
		} else {
			start_bit = rx_buffer[2] & 0x80;
			end_bit = rx_buffer[2] & 0x40;
			nal_buffer[4] = (rx_buffer[0] & 0x81) | (rx_buffer[2] & 0x3F) << 1;
			nal_buffer[5] = rx_buffer[1];
			copy_size++;
			rx_buffer++;
			rx_size--;
		}

		rx_buffer++;
		rx_size--;

		if (start_bit && !end_bit && (*nal_buffer_used) == 0) {
			// Write NAL header
			nal_buffer[0] = 0;
			nal_buffer[1] = 0;
			nal_buffer[2] = 0;
			nal_buffer[3] = 1;

			// Copy data
			memcpy(nal_buffer + copy_size, rx_buffer, rx_size);
			(*nal_buffer_used) = rx_size + copy_size;
		} else if (!start_bit && !end_bit && (*nal_buffer_used) != 0) {
			rx_buffer++;
			rx_size--;
			memcpy(nal_buffer + (*nal_buffer_used), rx_buffer, rx_size);
			*nal_buffer_used += rx_size;
		} else if (!start_bit && end_bit && (*nal_buffer_used) != 0) {
			rx_buffer++;
			rx_size--;

			memcpy(nal_buffer + (*nal_buffer_used), rx_buffer, rx_size);
			*nal_buffer_used += rx_size;

			// Store NAL size
			*out_nal_size = *nal_buffer_used;
			*nal_buffer_used = 0;

			return nal_buffer;
		}

		return NULL;
	} else {
		rx_buffer[-4] = 0;
		rx_buffer[-3] = 0;
		rx_buffer[-2] = 0;
		rx_buffer[-1] = 1;

		*out_nal_size = rx_size + 4;
		*nal_buffer_used = 0;

		return rx_buffer - 4;
	}
}

int main(int argc, const char* argv[]) {
	struct sockaddr_in address;
	uint16_t listen_port = 5600;

	uint8_t* rx_buffer = malloc(1024 * 1024);
	uint8_t* nal_buffer = malloc(1024 * 1024);
	uint32_t nal_buffer_used = 0;
	uint32_t rtp_header = 0;

	int port = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	memset(&address, 0x00, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(listen_port);
	bind(port, (struct sockaddr*)&address, sizeof(struct sockaddr_in));

	if (argc > 1) {
		listen_port = atoi(argv[1]);
	}

	FILE *file = fopen("file.mp4", "wb");
	printf("Monitor port: %d\n", listen_port);

	while (true) {
		int rx = recv(port, rx_buffer + 8, 2048, 0);
		if (rx <= 0) {
			usleep(1);
			continue;
		}

#if 0
		int r = 20;
		printf("RX: 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\n",
			rx_buffer[0 + r], rx_buffer[1 + r], rx_buffer[2 + r], rx_buffer[3 + r],
			rx_buffer[4 + r], rx_buffer[5 + r], rx_buffer[6 + r], rx_buffer[7 + r]);
#endif

		if (rx_buffer[8] == 0x80) {
			rtp_header = 12;
		}

		uint32_t size = 0;
		uint8_t* buffer = decode_frame(rx_buffer + 8, rx, rtp_header,
			nal_buffer, &nal_buffer_used, &size);
		if (!buffer) {
			continue;
		}

		if (size < 5) {
			printf("> Broken frame\n");
		}

#if 0
		int t = 4;
		printf("TX: 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\n",
			buffer[0 + t], buffer[1 + t], buffer[2 + t], buffer[3 + t],
			buffer[4 + t], buffer[5 + t], buffer[6 + t], buffer[7 + t]);
#endif

		fwrite(buffer, sizeof(char), size, file);
	}

	fclose(file);
	return 0;
}
