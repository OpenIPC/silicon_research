/**
 * @brief Algorithm of NAL reassembly
 * 
 * _nal_buffer - External buffer to accumulate fragments
 * 
 * @param rx_buffer - Received UDP payload
 * @param rx_size - Size of received UDP payload
 */
void processPacket(uint8_t* rx_buffer, uint32_t rx_size) {
  // - Get NAL type
  uint8_t fragment_type = rx_buffer[0]  & 0x1F;
  uint8_t nal_type      = rx_buffer[1]  & 0x1F;
  uint8_t start_bit     = rx_buffer[1]  & 0x80;
  uint8_t end_bit       = rx_buffer[1]  & 0x40;

  uint8_t * in_buffer = nullptr;
  uint32_t  in_size = 0;
  
  if (fragment_type == 28) {
    uint8_t new_nal = (rx_buffer[0] & 0xE0) | (rx_buffer[1] & 0x1F);
    
    rx_buffer ++;
    rx_size   --;
    
    bool nal_ready = false;
    
    if (start_bit && !end_bit &&_nal_buffer.isEmpty()) {
      // - Write NAL header
      _nal_buffer[0] = 0;
      _nal_buffer[1] = 0;
      _nal_buffer[2] = 1;
    
      // - Copy data
      memcpy(_nal_buffer.data() + 3, rx_buffer, rx_size);
      
      // - Patch NAL 
      _nal_buffer[3] = new_nal;
      _nal_buffer.setSize(rx_size + 3);
    
    } else if (!start_bit && !end_bit && !_nal_buffer.isEmpty()) {
      rx_buffer ++;
      rx_size   --;
      memcpy(_nal_buffer.data() + _nal_buffer.getSize(), rx_buffer, rx_size);
      _nal_buffer.addSize(rx_size);
      
    } else if (!start_bit && end_bit && !_nal_buffer.isEmpty()) {
      rx_buffer ++;
      rx_size   --;

      memcpy(_nal_buffer.data() + _nal_buffer.getSize(), rx_buffer, rx_size);
      _nal_buffer.addSize(rx_size);
     
      in_buffer = _nal_buffer.data();
      in_size   = _nal_buffer.getSize();
      
      printf("> DEFRAG NAL %d :  Size = %d\n", _nal_buffer[3] & 0x1F, _nal_buffer.getSize());
      nal_ready = true;
      _nal_buffer.setSize(0);
    }
    
    if (!nal_ready) {
      return;
    }
  } else {
    rx_buffer[-1] = 1;
    rx_buffer[-2] = 0;
    rx_buffer[-3] = 0;
    in_buffer   = rx_buffer - 3;
    in_size     = rx_size + 3;
  }
  
  // - At this point we have reassembled NAL with prefix (00 00 01)
  h264decode(in_buffer, in_size);
}