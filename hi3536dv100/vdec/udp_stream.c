#include "main.h"

uint32_t prev_frame_id = 0;
uint32_t frames_received = 0;
uint32_t frames_lost = 0;

#pragma pack(push, 1)
struct VideoFrame {
  //! Sequence identifier
  uint32_t  sequence_id;
  
  //! Frame identtifier
  uint32_t  frame_id;
  
  //! Payload size
  uint16_t  payload_size;
};

#pragma pack(pop)

static uint32_t prev_sequence_id = 0;
static uint32_t in_nal_size = 0;

uint8_t* decodeUDPFrame(uint8_t* rx_buffer, uint32_t rx_size,
	uint32_t header_size, uint8_t* nal_buffer, uint32_t* out_nal_size) {
  // - Access video frame information
  /*struct VideoFrame* frame = (struct VideoFrame*)rx_buffer;
  
  if (!prev_sequence_id) {
    prev_sequence_id = frame->sequence_id;
  }
  
  if (prev_sequence_id + 1 != frame->sequence_id) {
    frames_lost += frame->sequence_id - prev_sequence_id - 1;
    printf("> Lost: %d (%.02f\%) | Total Received: %d, Total Lost: %d\n",
      frame->sequence_id - prev_sequence_id - 1, (double)frames_lost / ((double)(frames_received + frames_lost)) * 100 ,
      frames_received, frames_lost
    );
    
    // - Reset Rx buffer
    *nal_buffer_used = 0;
  }
  
  prev_sequence_id = frame->sequence_id;
    
  // - Skip frame header
  rx_buffer += sizeof(struct VideoFrame);
  rx_size   -= sizeof(struct VideoFrame);*/

  rx_buffer += header_size;
  rx_size -= header_size;
  
  // - Get NAL type
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
      // - Write NAL header
      nal_buffer[0] = 0;
      nal_buffer[1] = 0;
      nal_buffer[2] = 0;
      nal_buffer[3] = 1;

      // - Copy data
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
        frames_received++;
        return nal_buffer;
    }

    return NULL;
  } else {
    // - Create frame prefix
    rx_buffer[-4] = 0;
    rx_buffer[-3] = 0;
    rx_buffer[-2] = 0;
    rx_buffer[-1] = 1;

    *out_nal_size = rx_size + copy_size;
    in_nal_size = 0;

    // - Return NAL
    //printf("> GOOD NAL %d : Size = %d bytes\n", fragment_type, *out_nal_size);
    frames_received++;
    return rx_buffer - copy_size;
  }
}
