// Author: Dinh Cong Bang from Viet Nam
// LVGL version: 8.2.0

#include <math.h>
#include "ui.h"
#include "ui_helpers.h"
#include "queue.h"
#include "util.h"

///////////////////// VARIABLES ////////////////////
extern bool s_bHasReceivedGPSInfo;
extern bool s_bHasReceivedGPSPos;

// SCREEN: ui_mainOSD1
void ui_mainOSD1_screen_init(void);
lv_obj_t *ui_mainOSD1;
lv_obj_t *ui_Home;
lv_obj_t *ui_TotalVolt;
lv_obj_t *ui_CellVolt;
lv_obj_t *ui_Current;
lv_obj_t *ui_Speed;
lv_obj_t *ui_Thermo;
lv_obj_t *ui_WifiSignal;
lv_obj_t *ui_Navigation;
lv_obj_t *ui_Time;
lv_obj_t *ui_BitRate;
lv_obj_t *ui_Long;
lv_obj_t *ui_Lat;
lv_obj_t *ui_RCSignal;
lv_obj_t *ui_Record;
lv_obj_t *ui_logo;
lv_obj_t *ui_GPS;
lv_obj_t *ui_flyMode;
lv_obj_t *ui_imgHorizon;
lv_obj_t *ui_imgVertical;
lv_obj_t *ui_imgCross;
lv_obj_t *ui_StatusText;
lv_obj_t *ui_Throttle;
lv_obj_t *ui_Altitude;
lv_obj_t *ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 32
    #error "LV_COLOR_DEPTH should be 32bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////

void ui_init( void )
{
    LV_EVENT_GET_COMP_CHILD = lv_event_register_id();
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_mainOSD1_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr( ui_mainOSD1);
}

extern Queue queue;
void* ui_updating_thread (void* arg)
{
    while(1)
    {
        Data item = dequeue(&queue);
        MSG_TYPE type = item.id;
        switch ((int)type)
        {
            case E_MPU:
            {
                MPUData mpu = *(MPUData*)item.data;
                float roll_angle = mapf(mpu.roll, -180, 180, 0, 3600);
                lv_img_set_angle(ui_imgVertical, roll_angle);
                lv_obj_center(ui_imgVertical);
                lv_obj_set_style_translate_y(ui_imgVertical, sin(deg2rad(mpu.pitch)) * 180, 0);
            }
            break;
            //float
            case E_BATTERY:
            {
                static uint8_t numCell = 1;
                float volt = *(float*)item.data;
                char tmp[10] = "";
                sprintf(tmp, "%.2f", volt);
                set_imgtext(ui_TotalVolt, tmp, "V");

                float cell = 0.0f;
                if(numCell == 1)
                {
                    for(; numCell < 10; numCell++)
                    {
                        cell = volt/numCell;
                        if(cell >= 2.80f && cell <= 4.35f)
                        {
                            break;
                        }
                    }
                }
                cell = volt/numCell;
                memset(tmp, 0, 10);
                sprintf(tmp, "%.2f", cell);
                set_imgtext(ui_CellVolt, tmp, "V");
            }
            break;
            //uint16_t
            case E_CURRENT_CONSUMED:
            {
                uint16_t curr_consumed = *(uint16_t*)item.data;
                char tmp[20] = "";
                sprintf(tmp, "%d", curr_consumed);
                set_texttext(ui_Current, tmp, " mAh");
            }
            break;
            //uint16_t
            case E_TEMP:
            {
                uint16_t temp = *(uint16_t*)item.data;
                char tmp[20] = "";
                sprintf(tmp, "%d", temp);
                set_imgtext(ui_Thermo, tmp, " C");
            }
            break;
            // char
            case E_SATS:
            {
                if(s_bHasReceivedGPSInfo)
                {
                    char numSats = *(char*)item.data;
                    char tmp[5] = "";
                    sprintf(tmp, "%d", numSats);
                    set_imgtext(ui_GPS, tmp, "");
                    lv_obj_clear_flag(ui_GPS, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                    lv_obj_add_flag(ui_GPS, LV_OBJ_FLAG_HIDDEN);
                }
            }
            break;
            // uint16_t
            case E_HDG:
            {
                uint16_t hdg = *(uint16_t*)item.data;
                set_arrow_to_home(hdg, s_bHasReceivedGPSInfo);
            }
            break;
            // double
            case E_GPS_DISTANCE:
            {
                if(s_bHasReceivedGPSInfo && s_bHasReceivedGPSPos)
                {
                    double dist = *(double*)item.data;
                    char tmp[10] = "";
                    sprintf(tmp, "%d", dist);
                    set_imgtext(ui_Home, tmp, "m");
                    lv_obj_clear_flag(ui_Home, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                    lv_obj_add_flag(ui_Home, LV_OBJ_FLAG_HIDDEN);
                }
            }
            break;
            //GPS cordinate
            case E_GPS_CORDINATE:
            {
                if(s_bHasReceivedGPSInfo && s_bHasReceivedGPSPos)
                {
                    GPSCordinates gps = *(GPSCordinates*)item.data;
                    char tmp[15] = "";
                    sprintf(tmp, "%d", gps.alt);
                    set_imgtext(ui_Altitude, tmp, "m");

                    memset(tmp, 0, 15);
                    sprintf(tmp, "%.7f", gps.lat);
                    set_imgtext(ui_Lat, tmp, "");

                    memset(tmp, 0, 15);
                    sprintf(tmp, "%.7f", gps.lon);
                    set_imgtext(ui_Long, tmp, "");

                    lv_obj_clear_flag(ui_Altitude, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui_Lat, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui_Long, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                    lv_obj_add_flag(ui_Altitude, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui_Lat, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui_Long, LV_OBJ_FLAG_HIDDEN);
                }
            }
            break;
            //Float
            case E_GSPEED:
            {
                if(s_bHasReceivedGPSInfo && s_bHasReceivedGPSPos)
                {
                    float speed = *(float*)item.data;
                    char tmp[10] = "";
                    sprintf(tmp, "%.1f", speed);
                    set_imgtext(ui_Speed, tmp, "Km/h");
                    lv_obj_clear_flag(ui_Speed, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                    lv_obj_add_flag(ui_Speed, LV_OBJ_FLAG_HIDDEN);
                }
            }
            break;
            //uint16_t
            case E_RSSI:
            {
                uint16_t rssi = 0;
                char tmp[10] = "";
                rssi = mapu32(*(uint16_t*)item.data, 0, 255, 0, 100);
                sprintf(tmp, "%d", rssi);
                set_imgtext(ui_RCSignal, tmp, "");
            }
            break;
            //uint16_t
            case E_THROTTLE:
            {
                uint16_t throttle = *(uint16_t*)item.data;
                char tmp[10] = "";
                sprintf(tmp, "%d", throttle);
                set_imgtext(ui_Throttle, tmp, "");
            }
            break;
            //int
            case E_ARMED:
            {
                static lv_timer_t *timer_flight = NULL;
                static int16_t seconds = -1;
                static lv_timer_t *timer_arm = NULL;
                char arm = *(char*)item.data;                
                lv_label_set_text(ui_StatusText, (arm == 0) ? "Armed" : "Disarmed");
                
                void update_labelArmed_timer(lv_timer_t *timer_arm) {
                    lv_timer_pause(timer_arm);
                    lv_label_set_text(ui_StatusText, "");
                }

                /*Handle for arm text*/
                if(timer_arm == NULL)
                {
                    timer_arm = lv_timer_create(update_labelArmed_timer, 5000, NULL);
                }
                else
                {
                    lv_timer_reset(timer_arm);
                    lv_timer_resume(timer_arm);
                }

                /*Handle for flight time*/
                void update_flight_timer(lv_timer_t *timer_flight) {
                    seconds++;
                    uint8_t hours, minutes, remainingSeconds;

                    hours = seconds / 3600; // Calculate the number of hours
                    minutes = (seconds % 3600) / 60; // Calculate the number of minutes
                    remainingSeconds = seconds % 60; // Calculate the remaining seconds
                    char tmp[15] = "";
                    sprintf(tmp, "%d:%02d:%02d", hours, minutes, remainingSeconds);
                    lv_label_set_text(ui_comp_get_child(ui_Time, UI_COMP_ITEMIMGTEXT_LABEL2),tmp);
                }
                if(seconds == -1 && arm == 1)
                {
                    seconds = 0;
                    timer_flight = lv_timer_create(update_flight_timer, 1000, NULL);
                }
                else if(arm == 0 && timer_flight != NULL)
                {
                    lv_timer_pause(timer_flight);
                }
                else if(arm == 1 && timer_flight != NULL)
                {
                    lv_timer_resume(timer_flight);
                }
            }
            break;
            //text
            case E_STATUS_TEXT:
            {
                set_text(ui_StatusText, (char*)item.data);
            }
            break;
            //text
            case E_FLY_MODE:
            {
                set_text(ui_flyMode, (char*)item.data);
            }
            break;

            default:
                break;
        }
        free(item.data);
        item.data = NULL;

    }
}
