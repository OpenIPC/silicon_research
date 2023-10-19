/**
 * @file fbdev.h
 *
 */

#ifndef FBDEV_H
#define FBDEV_H
#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void fbdev_init(void);
void fbdev_exit(void);
void fbdev_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p);
void fbdev_get_sizes(uint16_t *width, uint16_t *height, uint16_t *dpi);
/**
 * Set the X and Y offset in the variable framebuffer info.
 * @param xoffset horizontal offset
 * @param yoffset vertical offset
 */
void fbdev_set_offset(uint32_t xoffset, uint32_t yoffset);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*FBDEV_H*/
