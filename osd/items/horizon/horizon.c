#include "horizon.h"
#include "math.h"

/**
 * @brief 
 */
lv_obj_t * horizon_create() {
  /*Create an array for the points of the line*/
  static lv_point_t line_points[] = { {0, 0},  {400, 0} };

  /*Create style*/
  static lv_style_t style_line;
  lv_style_init(&style_line);
  lv_style_set_line_width(&style_line, 4);
  lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
  lv_style_set_line_rounded(&style_line, true);
  lv_style_set_border_width(&style_line, 0);
  
  // - Create line
  lv_obj_t * line;
  line = lv_line_create(lv_scr_act());
  lv_line_set_points(line, line_points, 2);     /*Set the points*/
  lv_obj_add_style(line, &style_line, 0);
  
  // - Set pivot point
  lv_obj_set_style_transform_pivot_x(line, 200,  0);
  lv_obj_set_style_transform_pivot_y(line, 0, 0);
  lv_obj_center(line);
  
  
  
  static lv_style_t dir_style;
  lv_style_init(&dir_style);
  lv_style_set_line_width(&dir_style, 8);
  lv_style_set_line_color(&dir_style, lv_palette_main(LV_PALETTE_GREEN));
  lv_style_set_line_rounded(&dir_style, false);
  
  // - Draw director lines
  { lv_obj_t * dir;
    dir = lv_line_create(lv_scr_act());
    static lv_point_t dir_points[] = { {0, 0},  {50, 0} };
    lv_line_set_points(dir, dir_points, 2);  
    lv_obj_add_style(dir, &dir_style, 0);
    lv_obj_center(dir);
    lv_obj_set_style_translate_x(dir, -250, 0);
  }
  { lv_obj_t * dir;
    dir = lv_line_create(lv_scr_act());
    static lv_point_t dir_points[] = { {0, 0},  {50, 0} };
    lv_line_set_points(dir, dir_points, 2);  
    lv_obj_add_style(dir, &dir_style, 0);
    lv_obj_center(dir);
    lv_obj_set_style_line_width(dir, 4, 0);
  }
  { lv_obj_t * dir;
    dir = lv_line_create(lv_scr_act());
    static lv_point_t dir_points[] = { {0, 0},  {0, 50} };
    lv_line_set_points(dir, dir_points, 2);  
    lv_obj_add_style(dir, &dir_style, 0);
    lv_obj_center(dir);
    lv_obj_set_style_line_width(dir, 4, 0);
  }
  { lv_obj_t * dir;
    dir = lv_line_create(lv_scr_act());
    static lv_point_t dir_points[] = { {0, 0},  {50, 0} };
    lv_line_set_points(dir, dir_points, 2);  
    lv_obj_add_style(dir, &dir_style, 0);
    lv_obj_center(dir);
    lv_obj_set_style_translate_x(dir, 250, 0);
  }
  
  return line;
}

void horizon_setRoll(lv_obj_t * horizon, float angle) {
  lv_obj_set_style_transform_angle(horizon, angle * 10, 0);
}

void horizon_setPitch(lv_obj_t * horizon, float angle) {
  lv_obj_center(horizon);
  lv_obj_set_style_translate_y(horizon, sin(angle * 3.14159 / 180) * 200, 0);
}


static void set_value_roll(void * horizon, int32_t v) {
  horizon_setRoll(horizon, v);
}

static void set_value_pitch(void * horizon, int32_t v) {
  horizon_setPitch(horizon, v);
}

void horizon_demo(lv_obj_t* horizon) {
  /*Create an animation to set the value*/
  { lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_value_roll);
    lv_anim_set_var(&a, horizon);
    lv_anim_set_values(&a, -90, 90);
    lv_anim_set_time(&a, 2000);
    lv_anim_set_repeat_delay(&a, 100);
    lv_anim_set_playback_time(&a, 500);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
  }
  
  { lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_value_pitch);
    lv_anim_set_var(&a, horizon);
    lv_anim_set_values(&a, -90, 90);
    lv_anim_set_time(&a, 2000);
    lv_anim_set_repeat_delay(&a, 100);
    lv_anim_set_playback_time(&a, 500);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
  }
}