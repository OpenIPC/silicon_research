// Author: Dinh Cong Bang from Viet Nam
// LVGL version: 8.2.0

#include "../ui.h"

void set_arrow_to_home(uint16_t hdg, bool hasGPS)
{
    lv_img_set_angle(ui_Navigation, hdg);
    if(hasGPS == false)
    {
        lv_label_set_text(ui_comp_get_child(ui_Navigation, UI_COMP_ITEMIMGTEXT_LABEL2),"N/A");
    }
    else
    {
        lv_label_set_text(ui_comp_get_child(ui_Navigation, UI_COMP_ITEMIMGTEXT_LABEL2),"GPS");
    }
}

void set_fly_time(int mins, int seconds)
{
    char tmp[50] = "";
    sprintf(tmp, "%d:%d", mins, seconds);
    lv_label_set_text(ui_comp_get_child(ui_Time, UI_COMP_ITEMIMGTEXT_LABEL2),tmp);
    lv_obj_set_style_blend_mode(ui_comp_get_child(ui_Time, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void set_text(lv_obj_t* object, const char* text1)
{
    lv_label_set_text(object, text1);
    lv_obj_set_align( object, LV_ALIGN_CENTER );
    lv_obj_set_style_text_align(object, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(object, LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void set_texttext(lv_obj_t* object, const char* text1, const char* text2)
{
    lv_label_set_text(ui_comp_get_child(object, UI_COMP_ITEMTEXTTEXT_LABEL4),text1);
    lv_label_set_text(ui_comp_get_child(object, UI_COMP_ITEMTEXTTEXT_LABEL1),text2);
    lv_obj_set_style_blend_mode(ui_comp_get_child(object, UI_COMP_ITEMTEXTTEXT_LABEL4), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_comp_get_child(object, UI_COMP_ITEMTEXTTEXT_LABEL1), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void set_imgtext(lv_obj_t* object, const char* text1, const char* text2)
{
    char tmp[50] = "";
    sprintf(tmp, "%s%s", text1, text2);
    lv_label_set_text(ui_comp_get_child(object, UI_COMP_ITEMIMGTEXT_LABEL2),tmp);
    lv_obj_set_style_blend_mode(ui_comp_get_child(object, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void ui_mainOSD1_screen_init(void)
{
ui_mainOSD1 = lv_obj_create(NULL);
lv_obj_clear_flag( ui_mainOSD1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
// lv_obj_set_style_bg_opa(ui_mainOSD1, 0, LV_OPA_TRANSP);
lv_obj_set_style_bg_opa(ui_mainOSD1, LV_OPA_TRANSP, LV_PART_MAIN);
lv_disp_set_bg_opa(NULL, LV_OPA_TRANSP);

ui_Home = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_Home, lv_pct(-40) );
lv_obj_set_y( ui_Home, lv_pct(-35) );

lv_img_set_src(ui_comp_get_child(ui_Home, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_home_png);

lv_label_set_text(ui_comp_get_child(ui_Home, UI_COMP_ITEMIMGTEXT_LABEL2),"0m");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_Home, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_add_flag(ui_Home, LV_OBJ_FLAG_HIDDEN);

ui_TotalVolt = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_TotalVolt, lv_pct(-40) );
lv_obj_set_y( ui_TotalVolt, lv_pct(-12) );
lv_img_set_src(ui_comp_get_child(ui_TotalVolt, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_battery_png);
lv_label_set_text(ui_comp_get_child(ui_TotalVolt, UI_COMP_ITEMIMGTEXT_LABEL2),"0.0V");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_TotalVolt, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_CellVolt = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_CellVolt, lv_pct(-40) );
lv_obj_set_y( ui_CellVolt, lv_pct(-4) );

lv_img_set_src(ui_comp_get_child(ui_CellVolt, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_battery_png);
lv_label_set_text(ui_comp_get_child(ui_CellVolt, UI_COMP_ITEMIMGTEXT_LABEL2),"0.0V");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_CellVolt, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_Current = ui_itemTextText_create(ui_mainOSD1);
lv_obj_set_x( ui_Current, lv_pct(-14) );
lv_obj_set_y( ui_Current, lv_pct(41) );
lv_label_set_text(ui_comp_get_child(ui_Current, UI_COMP_ITEMTEXTTEXT_LABEL4),"00000");
lv_label_set_text(ui_comp_get_child(ui_Current, UI_COMP_ITEMTEXTTEXT_LABEL1)," mAh");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_Current, UI_COMP_ITEMTEXTTEXT_LABEL4), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_Current, UI_COMP_ITEMTEXTTEXT_LABEL1), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_Speed = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_Speed, lv_pct(-40) );
lv_obj_set_y( ui_Speed, lv_pct(4) );

lv_img_set_src(ui_comp_get_child(ui_Speed, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_speed_png);
lv_label_set_text(ui_comp_get_child(ui_Speed, UI_COMP_ITEMIMGTEXT_LABEL2),"0 km/h");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_Speed, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_add_flag(ui_Speed, LV_OBJ_FLAG_HIDDEN);

ui_Thermo = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_Thermo, lv_pct(-27) );
lv_obj_set_y( ui_Thermo, lv_pct(41) );
lv_img_set_src(ui_comp_get_child(ui_Thermo, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_thermometer_png);
lv_label_set_text(ui_comp_get_child(ui_Thermo, UI_COMP_ITEMIMGTEXT_LABEL2),"40 C");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_Thermo, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_WifiSignal = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_WifiSignal, lv_pct(-40) );
lv_obj_set_y( ui_WifiSignal, lv_pct(41) );
lv_img_set_src(ui_comp_get_child(ui_WifiSignal, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_signal_png);
lv_label_set_text(ui_comp_get_child(ui_WifiSignal, UI_COMP_ITEMIMGTEXT_LABEL2),"CH7");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_WifiSignal, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_Navigation = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_Navigation, lv_pct(14) );
lv_obj_set_y( ui_Navigation, lv_pct(41) );
lv_img_set_src(ui_comp_get_child(ui_Navigation, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_iconnavi_png);
lv_img_set_zoom(ui_comp_get_child(ui_Navigation, UI_COMP_ITEMIMGTEXT_IMAGE4),250);
lv_label_set_text(ui_comp_get_child(ui_Navigation, UI_COMP_ITEMIMGTEXT_LABEL2),"N/A");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_Navigation, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_Time = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_Time, lv_pct(26) );
lv_obj_set_y( ui_Time, lv_pct(41) );
lv_img_set_src(ui_comp_get_child(ui_Time, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_clock_png);
lv_label_set_text(ui_comp_get_child(ui_Time, UI_COMP_ITEMIMGTEXT_LABEL2),"0:00:00");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_Time, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_BitRate = ui_itemTextText_create(ui_mainOSD1);
lv_obj_set_x( ui_BitRate, lv_pct(39) );
lv_obj_set_y( ui_BitRate, lv_pct(41) );

lv_label_set_text(ui_comp_get_child(ui_BitRate, UI_COMP_ITEMTEXTTEXT_LABEL4),"25.4");
lv_label_set_text(ui_comp_get_child(ui_BitRate, UI_COMP_ITEMTEXTTEXT_LABEL1),"Mbps\n");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_BitRate, UI_COMP_ITEMTEXTTEXT_LABEL4), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_BitRate, UI_COMP_ITEMTEXTTEXT_LABEL1), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_Long = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_Long, lv_pct(0) );
lv_obj_set_y( ui_Long, lv_pct(-37) );
lv_img_set_src(ui_comp_get_child(ui_Long, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_arrow_left_png);
lv_img_set_angle(ui_comp_get_child(ui_Long, UI_COMP_ITEMIMGTEXT_IMAGE4),-900);
lv_label_set_text(ui_comp_get_child(ui_Long, UI_COMP_ITEMIMGTEXT_LABEL2),"000.0000000");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_Long, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_add_flag(ui_Long, LV_OBJ_FLAG_HIDDEN);

ui_Lat = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_Lat, lv_pct(0) );
lv_obj_set_y( ui_Lat, lv_pct(-43) );
lv_img_set_src(ui_comp_get_child(ui_Lat, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_arrow_left_png);
lv_label_set_text(ui_comp_get_child(ui_Lat, UI_COMP_ITEMIMGTEXT_LABEL2),"000.0000000");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_Lat, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_add_flag(ui_Lat, LV_OBJ_FLAG_HIDDEN);

ui_RCSignal = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_RCSignal, lv_pct(23) );
lv_obj_set_y( ui_RCSignal, lv_pct(-43) );

lv_img_set_src(ui_comp_get_child(ui_RCSignal, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_signal_png);
lv_label_set_text(ui_comp_get_child(ui_RCSignal, UI_COMP_ITEMIMGTEXT_LABEL2),"00");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_RCSignal, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_Record = lv_img_create(ui_mainOSD1);
lv_img_set_src(ui_Record, &ui_img_ico_record_png);
lv_obj_set_width( ui_Record, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Record, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Record, 596 );
lv_obj_set_y( ui_Record, 14 );
lv_obj_set_align( ui_Record, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Record, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Record, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Record,180);
lv_obj_add_flag(ui_Record, LV_OBJ_FLAG_HIDDEN);

ui_logo = lv_img_create(ui_mainOSD1);
lv_img_set_src(ui_logo, &ui_img_logo_png);
lv_obj_set_width( ui_logo, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_logo, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_logo, 568 );
lv_obj_set_y( ui_logo, -310 );
lv_obj_set_align( ui_logo, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_logo, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_logo, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_GPS = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_GPS, lv_pct(-40) );
lv_obj_set_y( ui_GPS, lv_pct(-44) );
lv_img_set_src(ui_comp_get_child(ui_GPS, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_gps_png);
lv_img_set_zoom(ui_comp_get_child(ui_GPS, UI_COMP_ITEMIMGTEXT_IMAGE4),160);

lv_label_set_text(ui_comp_get_child(ui_GPS, UI_COMP_ITEMIMGTEXT_LABEL2),"0");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_GPS, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_add_flag(ui_GPS, LV_OBJ_FLAG_HIDDEN);

ui_flyMode = lv_label_create(ui_mainOSD1);
lv_obj_set_width( ui_flyMode, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_flyMode, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_flyMode, lv_pct(1) );
lv_obj_set_y( ui_flyMode, lv_pct(68) );
//lv_obj_set_align( ui_flyMode, LV_TEXT_ALIGN_LEFT );
lv_label_set_text(ui_flyMode,"AIR");
lv_obj_set_style_text_color(ui_flyMode, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_flyMode, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_flyMode, &lv_font_montserrat_28, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_blend_mode(ui_flyMode, LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_imgHorizon = lv_img_create(ui_mainOSD1);
lv_img_set_src(ui_imgHorizon, &ui_img_arhorizon_png);
lv_obj_set_width( ui_imgHorizon, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_imgHorizon, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_imgHorizon, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_imgHorizon, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_imgHorizon, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_imgHorizon,255);

ui_imgVertical = lv_img_create(ui_mainOSD1);
lv_img_set_src(ui_imgVertical, &ui_img_icon_vertical_png);
lv_obj_set_width( ui_imgVertical, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_imgVertical, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_imgVertical, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_imgVertical, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
// lv_obj_clear_flag( ui_imgVertical, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_imgVertical,256);

ui_imgCross = lv_img_create(ui_mainOSD1);
lv_img_set_src(ui_imgCross, &ui_img_icon_crosshair_png);
lv_obj_set_width( ui_imgCross, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_imgCross, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_imgCross, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_imgCross, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_imgCross, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_imgCross,250);

ui_StatusText = lv_label_create(ui_mainOSD1);
lv_obj_set_width( ui_StatusText, lv_pct(50));
lv_obj_set_height( ui_StatusText, LV_SIZE_CONTENT);   /// 4
lv_obj_set_x( ui_StatusText, 0 );
lv_obj_set_y( ui_StatusText, 193 );
lv_obj_set_align( ui_StatusText, LV_ALIGN_CENTER );
lv_label_set_text(ui_StatusText,"");
lv_obj_set_style_text_color(ui_StatusText, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_StatusText, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_StatusText, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_StatusText, &lv_font_montserrat_28, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_blend_mode(ui_StatusText, LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_Throttle = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_Throttle, lv_pct(-40) );
lv_obj_set_y( ui_Throttle, lv_pct(12) );

lv_img_set_src(ui_comp_get_child(ui_Throttle, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_throttle_png);
lv_label_set_text(ui_comp_get_child(ui_Throttle, UI_COMP_ITEMIMGTEXT_LABEL2),"0");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_Throttle, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_img_set_zoom(ui_comp_get_child(ui_Throttle, UI_COMP_ITEMIMGTEXT_IMAGE4),190);

ui_Altitude = ui_itemImgText_create(ui_mainOSD1);
lv_obj_set_x( ui_Altitude, lv_pct(-40) );
lv_obj_set_y( ui_Altitude, lv_pct(-29) );
lv_img_set_src(ui_comp_get_child(ui_Altitude, UI_COMP_ITEMIMGTEXT_IMAGE4), &ui_img_icon_altw_png);
lv_label_set_text(ui_comp_get_child(ui_Altitude, UI_COMP_ITEMIMGTEXT_LABEL2),"0m");
lv_obj_set_style_blend_mode(ui_comp_get_child(ui_Altitude, UI_COMP_ITEMIMGTEXT_LABEL2), LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_add_flag(ui_Altitude, LV_OBJ_FLAG_HIDDEN);

}
