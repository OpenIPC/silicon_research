// Author: Dinh Cong Bang from Viet Nam
// LVGL version: 8.2.0

#include "../ui.h"


// COMPONENT itemImgText

lv_obj_t *ui_itemImgText_create(lv_obj_t *comp_parent) {

lv_obj_t *cui_itemImgText;
cui_itemImgText = lv_obj_create(comp_parent);
lv_obj_set_width( cui_itemImgText, lv_pct(20));
lv_obj_set_height( cui_itemImgText, lv_pct(9));
lv_obj_set_x( cui_itemImgText, lv_pct(-39) );
lv_obj_set_y( cui_itemImgText, lv_pct(-44) );
lv_obj_set_align( cui_itemImgText, LV_ALIGN_CENTER );
lv_obj_clear_flag( cui_itemImgText, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(cui_itemImgText, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(cui_itemImgText, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(cui_itemImgText, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(cui_itemImgText, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(cui_itemImgText, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_t *cui_Image4;
cui_Image4 = lv_img_create(cui_itemImgText);
lv_obj_set_width( cui_Image4, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( cui_Image4, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( cui_Image4, -104 );
lv_obj_set_y( cui_Image4, 0 );
lv_obj_set_align( cui_Image4, LV_ALIGN_CENTER );
lv_obj_add_flag( cui_Image4, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( cui_Image4, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(cui_Image4,160);

lv_obj_t *cui_Label2;
cui_Label2 = lv_label_create(cui_itemImgText);
lv_obj_set_width( cui_Label2, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( cui_Label2, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( cui_Label2, 31 );
lv_obj_set_y( cui_Label2, -3 );
lv_obj_set_style_text_color(cui_Label2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(cui_Label2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(cui_Label2, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(cui_Label2, &lv_font_montserrat_28, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_t ** children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_ITEMIMGTEXT_NUM);
children[UI_COMP_ITEMIMGTEXT_ITEMIMGTEXT] = cui_itemImgText;
children[UI_COMP_ITEMIMGTEXT_IMAGE4] = cui_Image4;
children[UI_COMP_ITEMIMGTEXT_LABEL2] = cui_Label2;
lv_obj_add_event_cb(cui_itemImgText, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
lv_obj_add_event_cb(cui_itemImgText, del_component_child_event_cb, LV_EVENT_DELETE, children);
ui_comp_itemImgText_create_hook(cui_itemImgText);
return cui_itemImgText; 
}

