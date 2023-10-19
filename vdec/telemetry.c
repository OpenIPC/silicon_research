#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;
  
static uint16_t rgb888_to_rgb565 (uint8_t r, uint8_t g, uint8_t b) {
  return (((uint16_t)r & 0xF8) << 8) | (((uint16_t)g & 0xFC) << 3) | (b >> 3);
}
  
void writePixel(uint8_t* buffer,  uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t offset = (y * finfo.line_length) + (x * vinfo.bits_per_pixel / 8);
    
    uint16_t value = rgb888_to_rgb565(r, g, b);;
    memcpy(buffer + offset, &value, 2);
}

int main() {
  int fbfd = open ("/dev/fb0", O_RDWR);
  
  ioctl (fbfd, FBIOGET_FSCREENINFO, &finfo);
  ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo);

  int fb_width      = vinfo.xres;
  int fb_height     = vinfo.yres;
  int fb_bpp        = vinfo.bits_per_pixel;
  int fb_bytes      = fb_bpp / 8;
  int fb_stride     = finfo.line_length;
  int fb_data_size  = fb_height * fb_stride;

  char *fbdata = mmap (0, fb_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, (off_t)0);
  
  printf(
    "FrameBuffer ready.\n"
    "  + Width : %d\n"
    "  + Height: %d\n"
    "  + BPP   : %d\n",
    fb_width, fb_height, fb_bpp
  );
  
  while (1) { 
    for (int x = 0; x < fb_width / 2; x++) {
      for (int y = 100; y < 150; y++) {
        uint8_t r = rand() % 255;
        uint8_t g = rand() % 255;
        uint8_t b = rand() % 255;
        writePixel(fbdata, x, y, r, g, b);
      }
    }
    usleep(25000);
  }

  return 0;
}