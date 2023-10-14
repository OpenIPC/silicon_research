// Author: Dinh Cong Bang from Viet Nam
// LVGL version: 8.2.0

#include "ui.h"
#include "ui_helpers.h"
#include "queue.h"
#include "util.h"

///////////////////// VARIABLES ////////////////////


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
            //uint16_t
            case E_ALTITUDE:
            {
                uint16_t alt = byteArrayToUint16(item.data);
                char tmp[10] = "";
                sprintf(tmp, "%d", alt);
                set_imgtext(ui_Altitude, tmp, "m");
            }
            break;
            //float
            case E_BATTERY:
            {
                float volt = byteArrayToFloat(item.data);
                char tmp[10] = "";
                sprintf(tmp, "%.00f", volt);
                set_imgtext(ui_TotalVolt, tmp, "V");
            }
            break;
            //uint16_t
            case E_CURRENT:
            {
                uint16_t curr = byteArrayToUint16(item.data);
                char tmp[10] = "";
                sprintf(tmp, "%d", curr);
                set_imgtext(ui_Current, tmp, "mAh");
            }
            break;
            //float
            case E_CELL:
            {
                float volt = byteArrayToFloat(item.data);
                char tmp[10] = "";
                sprintf(tmp, "%.00f", volt);
                set_imgtext(ui_CellVolt, tmp, "V");
            }
            break;
            // uint16_t
            case E_SATS:
            {
                char numSats = item.data[0];
                char tmp[5] = "";
                sprintf(tmp, "%d", numSats);
                set_imgtext(ui_GPS, tmp, "");
            }
            break;
            // uint16_t
            case E_HDG:
            {
                uint16_t hdg = byteArrayToUint16(item.data);
                set_arrow_to_home(hdg);
            }
            break;
            // double
            case E_GPS_DISTANCE:
            {
                double dist = byteArrayToDouble(item.data);
                char tmp[10] = "";
                sprintf(tmp, "%d", dist);
                set_imgtext(ui_Home, tmp, "m");
            }
            break;
            // double
            case E_LAT:
            {
                double lat = byteArrayToDouble(item.data);
                char tmp[15] = "";
                sprintf(tmp, "%.00000000f", lat);
                set_imgtext(ui_Lat, tmp, "");
            }
            break;
            // double
            case E_LON:
            {
                double lon = byteArrayToDouble(item.data);
                char tmp[15] = "";
                sprintf(tmp, "%.00000000f", lon);
                set_imgtext(ui_Long, tmp, "");
            }
            break;
            //Float
            case E_GSPEED:
            {
                float speed = byteArrayToFloat(item.data);
                char tmp[10] = "";
                sprintf(tmp, "%.0f", speed);
                set_imgtext(ui_Speed, tmp, "Km/h");
            }
            break;
            //uint16_t
            case E_RSSI:
            {
                uint16_t rssi = byteArrayToUint16(item.data);
                char tmp[10] = "";
                sprintf(tmp, "%d", rssi);
                set_imgtext(ui_RCSignal, tmp, "");
            }
            break;
            //uint16_t
            case E_THROTTLE:
            {
                uint16_t throttle = byteArrayToUint16(item.data);
                char tmp[10] = "";
                sprintf(tmp, "%d", throttle);
                set_imgtext(ui_Throttle, tmp, "");
            }
            break;
            //int
            case E_ARMED:
            {

            }
            break;
            //text
            case E_STATUS_TEXT:
            {
                set_text(ui_StatusText, item.data);
            }
            break;
            //text
            case E_FLY_MODE:
            {
                set_text(ui_flyMode, item.data);
            }
            break;
            //Todo:
            //Receive Roll, Pitch to control vertical line
            default:
                break;
        }

    }
}
