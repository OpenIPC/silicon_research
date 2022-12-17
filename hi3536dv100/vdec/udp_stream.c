#include "main.h"

uint8_t* decodeUDPFrame(uint8_t* rx_buffer, uint32_t rx_size, uint32_t header_size, uint8_t* nal_buffer, uint32_t* nal_buffer_used, uint32_t* out_nal_size) {
  rx_buffer += header_size;
  rx_size   -= header_size;
  
  // - Get NAL type
  uint8_t fragment_type = rx_buffer[0]  & 0x1F;
  uint8_t nal_type      = rx_buffer[1]  & 0x1F;
  uint8_t start_bit     = rx_buffer[1]  & 0x80;
  uint8_t end_bit       = rx_buffer[1]  & 0x40;

  uint8_t * in_buffer   = NULL;
  uint32_t  in_size     = 0;

  if (fragment_type == 28) {
    uint8_t new_nal = (rx_buffer[0] & 0xE0) | (rx_buffer[1] & 0x1F);
    
    rx_buffer ++;
    rx_size   --;
    
    if (start_bit && !end_bit && (*nal_buffer_used) == 0) {
      // - Write NAL header
      nal_buffer[0] = 0;
      nal_buffer[1] = 0;
      nal_buffer[2] = 1;
  
      // - Copy data
      memcpy(nal_buffer + 3, rx_buffer, rx_size);
      
      // - Patch NAL 
      nal_buffer[3]       = new_nal;
      (*nal_buffer_used)  = rx_size + 3;
      
    } else if (!start_bit && !end_bit && (*nal_buffer_used) != 0) {
      rx_buffer ++;
      rx_size   --;
      memcpy(nal_buffer + (*nal_buffer_used), rx_buffer, rx_size);
      *nal_buffer_used += rx_size;
      
    } else if (!start_bit && end_bit && (*nal_buffer_used) != 0) {
      rx_buffer ++;
      rx_size   --;
      
      memcpy(nal_buffer + (*nal_buffer_used), rx_buffer, rx_size);
      *nal_buffer_used += rx_size;
     
     
      // - Store NAL size
      *out_nal_size = *nal_buffer_used;
      
      // - Reset NAL buffer
      *nal_buffer_used = 0;
      
      // - Return NAL
      printf("> DEFRAG NAL %d : Size = %d\n", nal_buffer[3] & 0x1F, *out_nal_size);
      return nal_buffer;
    }
    
    // - No frame yet
    return NULL;
    
  } else {
    // - Create frame prefix
    rx_buffer[-1] = 1;
    rx_buffer[-2] = 0;
    rx_buffer[-3] = 0;

    *out_nal_size     = rx_size + 3;
    *nal_buffer_used  = 0;
    
    // - Return NAL
    printf("> GOOD NAL %d : Size = %d bytes\n", fragment_type, *out_nal_size);
    return rx_buffer - 3;
  }
}