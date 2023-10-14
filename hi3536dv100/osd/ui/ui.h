// Author: Dinh Cong Bang from Viet Nam
// LVGL version: 8.2.0

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "lvgl/lvgl.h"

#include "ui_helpers.h"
#include "components/ui_comp.h"
#include "components/ui_comp_hook.h"
#include "ui_events.h"
// SCREEN: ui_mainOSD1
void ui_mainOSD1_screen_init(void);
extern lv_obj_t *ui_mainOSD1;
extern lv_obj_t *ui_Home;
extern lv_obj_t *ui_TotalVolt;
extern lv_obj_t *ui_CellVolt;
extern lv_obj_t *ui_Current;
extern lv_obj_t *ui_Speed;
extern lv_obj_t *ui_Thermo;
extern lv_obj_t *ui_WifiSignal;
extern lv_obj_t *ui_Navigation;
extern lv_obj_t *ui_Time;
extern lv_obj_t *ui_BitRate;
extern lv_obj_t *ui_Long;
extern lv_obj_t *ui_Lat;
extern lv_obj_t *ui_RCSignal;
extern lv_obj_t *ui_Record;
extern lv_obj_t *ui_logo;
extern lv_obj_t *ui_GPS;
extern lv_obj_t *ui_flyMode;
extern lv_obj_t *ui_imgHorizon;
extern lv_obj_t *ui_imgVertical;
extern lv_obj_t *ui_imgCross;
extern lv_obj_t *ui_StatusText;
extern lv_obj_t *ui_Throttle;
extern lv_obj_t *ui_Altitude;
extern lv_obj_t *ui____initial_actions0;

LV_IMG_DECLARE( ui_img_icon_home_png);   // assets\icon_home.png
LV_IMG_DECLARE( ui_img_icon_battery_png);   // assets\icon_battery.png
LV_IMG_DECLARE( ui_img_icon_speed_png);   // assets\icon_speed.png
LV_IMG_DECLARE( ui_img_icon_thermometer_png);   // assets\icon_thermometer.png
LV_IMG_DECLARE( ui_img_icon_signal_png);   // assets\icon_signal.png
LV_IMG_DECLARE( ui_img_iconnavi_png);   // assets\iconnavi.png
LV_IMG_DECLARE( ui_img_icon_clock_png);   // assets\icon_clock.png
LV_IMG_DECLARE( ui_img_icon_arrow_left_png);   // assets\icon_arrow_left.png
LV_IMG_DECLARE( ui_img_ico_record_png);   // assets\ico_record.png
LV_IMG_DECLARE( ui_img_logo_png);   // assets\logo.png
LV_IMG_DECLARE( ui_img_icon_gps_png);   // assets\icon_gps.png
LV_IMG_DECLARE( ui_img_arhorizon_png);   // assets\arhorizon.png
LV_IMG_DECLARE( ui_img_icon_vertical_png);   // assets\icon_vertical.png
LV_IMG_DECLARE( ui_img_icon_crosshair_png);   // assets\icon_crosshair.png
LV_IMG_DECLARE( ui_img_icon_throttle_png);   // assets\icon_throttle.png
LV_IMG_DECLARE( ui_img_icon_altw_png);   // assets\icon_altw.png



void ui_init(void);
void* ui_updating_thread (void* arg);
void set_arrow_to_home(uint16_t hdg);
void set_fly_time(int mins, int seconds);
void set_text(lv_obj_t* object, const char* text1);
void set_texttext(lv_obj_t* object, const char* text1, const char* text2);
void set_imgtext(lv_obj_t* object, const char* text1, const char* text2);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
