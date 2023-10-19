#include "osd.h"
#include "horizon.h"
#include "ui.h"
#include "queue.h"
#include "mavlink_parser.h"

// - Temporary buffer for LVGL
#define DISPLAY_BUFFER_SIZE (1024 * 1024)
lv_color_t display_buffer[DISPLAY_BUFFER_SIZE];

Queue queue;

/**
 * @brief Entry point
 * @param argc - Argument count
 * @param argv - Array of arguments
 */
int main(int argc, const char* argv[]) {
  // - Init LVGL
  lv_init();
  
  // - Initialize famebuffer
  fbdev_init();
  
  // - Initialize descriptor for display buffer
  lv_disp_draw_buf_t display_buffer_descriptor;
  lv_disp_draw_buf_init(&display_buffer_descriptor, display_buffer, NULL, DISPLAY_BUFFER_SIZE);

  // - Initialize display driver
  lv_disp_drv_t display_driver;
  lv_disp_drv_init(&display_driver);

  // - Request display size
  fbdev_get_sizes(&display_driver.hor_res, &display_driver.ver_res, NULL);
  
  // - Register display driver
  display_driver.draw_buf   = &display_buffer_descriptor;
  display_driver.flush_cb   = fbdev_flush;
  lv_disp_drv_register(&display_driver);

  // - Enable transparent background
  lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_TRANSP, LV_PART_MAIN);
  lv_disp_set_bg_opa(NULL, LV_OPA_TRANSP);
  
  
  initQueue(&queue);
  
  ui_init();

  pthread_t ui_thread;
  pthread_t mavlink_thread;

  pthread_create(&ui_thread, NULL, ui_updating_thread, 0);
  pthread_create(&mavlink_thread, NULL, mavlink_handler, 0);

  // - Run display service in tickless mode
  while(1) {
    lv_timer_handler();
    usleep(1);
  }
    
  return 0;
}


/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
  
  return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}