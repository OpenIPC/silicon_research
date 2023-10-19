// Author: Dinh Cong Bang from Viet Nam
// LVGL version: 8.2.0

#include "../ui.h"


// COMPONENT itemTextText

lv_obj_t *ui_itemTextText_create(lv_obj_t *comp_parent) {

lv_obj_t *cui_itemTextText;
cui_itemTextText = lv_obj_create(comp_parent);
lv_obj_set_width( cui_itemTextText, lv_pct(11));
lv_obj_set_height( cui_itemTextText, lv_pct(9));
lv_obj_set_x( cui_itemTextText, lv_pct(42) );
lv_obj_set_y( cui_itemTextText, lv_pct(30) );
lv_obj_set_align( cui_itemTextText, LV_ALIGN_CENTER );
lv_obj_clear_flag( cui_itemTextText, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(cui_itemTextText, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(cui_itemTextText, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(cui_itemTextText, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(cui_itemTextText, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(cui_itemTextText, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_t *cui_Label4;
cui_Label4 = lv_label_create(cui_itemTextText);
lv_obj_set_width( cui_Label4, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( cui_Label4, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( cui_Label4, -38 );
lv_obj_set_y( cui_Label4, 0 );
lv_obj_set_align( cui_Label4, LV_ALIGN_CENTER );
lv_obj_set_style_text_color(cui_Label4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(cui_Label4, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(cui_Label4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(cui_Label4, &lv_font_montserrat_28, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_t *cui_Label1;
cui_Label1 = lv_label_create(cui_itemTextText);
lv_obj_set_width( cui_Label1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( cui_Label1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( cui_Label1, 40 );
lv_obj_set_y( cui_Label1, -5 );
lv_obj_set_style_text_color(cui_Label1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(cui_Label1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(cui_Label1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(cui_Label1, &lv_font_montserrat_28, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_t ** children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_ITEMTEXTTEXT_NUM);
children[UI_COMP_ITEMTEXTTEXT_ITEMTEXTTEXT] = cui_itemTextText;
children[UI_COMP_ITEMTEXTTEXT_LABEL4] = cui_Label4;
children[UI_COMP_ITEMTEXTTEXT_LABEL1] = cui_Label1;
lv_obj_add_event_cb(cui_itemTextText, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
lv_obj_add_event_cb(cui_itemTextText, del_component_child_event_cb, LV_EVENT_DELETE, children);
ui_comp_itemTextText_create_hook(cui_itemTextText);
return cui_itemTextText; 
}

