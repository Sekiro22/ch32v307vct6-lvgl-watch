/**
 * @file screen_main_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "screen_main_gen.h"
#include "../ch32v307_gc9a01_ui.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * screen_main_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if CH32V307_GC9A01_UI_CHECK_COMPILE_TARGET(CH32V307_GC9A01_UI_TARGET_ALL)
    if (ch32v307_gc9a01_ui_check_target(CH32V307_GC9A01_UI_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "screen_main_#");
        lv_obj_set_style_bg_color(lv_obj_0, lv_color_hex(0x070809), 0);
        lv_obj_set_style_bg_opa(lv_obj_0, (255 * 100 / 100), 0);
        lv_obj_set_style_pad_all(lv_obj_0, 0, 0);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_t * dial_outer = lv_obj_create(lv_obj_0);
        lv_obj_set_name(dial_outer, "dial_outer");
        lv_obj_set_width(dial_outer, 236);
        lv_obj_set_height(dial_outer, 236);
        lv_obj_set_align(dial_outer, LV_ALIGN_CENTER);
        lv_obj_set_style_radius(dial_outer, 118, 0);
        lv_obj_set_style_bg_color(dial_outer, lv_color_hex(0x111317), 0);
        lv_obj_set_style_bg_opa(dial_outer, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(dial_outer, 3, 0);
        lv_obj_set_style_border_color(dial_outer, lv_color_hex(0x555960), 0);
        lv_obj_set_style_border_opa(dial_outer, (255 * 100 / 100), 0);
        lv_obj_set_style_pad_all(dial_outer, 0, 0);
        lv_obj_set_flag(dial_outer, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(dial_outer, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_t * dial_inner = lv_obj_create(dial_outer);
        lv_obj_set_name(dial_inner, "dial_inner");
        lv_obj_set_width(dial_inner, 218);
        lv_obj_set_height(dial_inner, 218);
        lv_obj_set_align(dial_inner, LV_ALIGN_CENTER);
        lv_obj_set_style_radius(dial_inner, 109, 0);
        lv_obj_set_style_bg_color(dial_inner, lv_color_hex(0x0D0F12), 0);
        lv_obj_set_style_bg_opa(dial_inner, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(dial_inner, 1, 0);
        lv_obj_set_style_border_color(dial_inner, lv_color_hex(0x34373C), 0);
        lv_obj_set_style_border_opa(dial_inner, (255 * 100 / 100), 0);
        lv_obj_set_style_pad_all(dial_inner, 0, 0);
        lv_obj_set_flag(dial_inner, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(dial_inner, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_t * scale_minutes = lv_scale_create(dial_inner);
        lv_obj_set_name(scale_minutes, "scale_minutes");
        lv_obj_set_width(scale_minutes, 204);
        lv_obj_set_height(scale_minutes, 204);
        lv_obj_set_align(scale_minutes, LV_ALIGN_CENTER);
        lv_scale_set_mode(scale_minutes, LV_SCALE_MODE_ROUND_INNER);
        lv_scale_set_min_value(scale_minutes, 0);
        lv_scale_set_max_value(scale_minutes, 60);
        lv_scale_set_total_tick_count(scale_minutes, 61);
        lv_scale_set_major_tick_every(scale_minutes, 5);
        lv_scale_set_label_show(scale_minutes, false);
        lv_scale_set_angle_range(scale_minutes, 360);
        lv_scale_set_rotation(scale_minutes, 270);
        lv_scale_set_draw_ticks_on_top(scale_minutes, true);
        lv_obj_set_style_arc_width(scale_minutes, 1, 0);
        lv_obj_set_style_arc_color(scale_minutes, lv_color_hex(0x45484E), 0);
        lv_obj_set_style_arc_opa(scale_minutes, (255 * 80 / 100), 0);
        lv_obj_set_style_length(scale_minutes, 5, LV_PART_ITEMS);
        lv_obj_set_style_line_width(scale_minutes, 1, LV_PART_ITEMS);
        lv_obj_set_style_line_color(scale_minutes, lv_color_hex(0x74777D), LV_PART_ITEMS);
        lv_obj_set_style_line_opa(scale_minutes, (255 * 80 / 100), LV_PART_ITEMS);
        lv_obj_set_style_length(scale_minutes, 9, LV_PART_INDICATOR);
        lv_obj_set_style_line_width(scale_minutes, 2, LV_PART_INDICATOR);
        lv_obj_set_style_line_color(scale_minutes, lv_color_hex(0xF2F3F4), LV_PART_INDICATOR);
        lv_obj_set_style_line_opa(scale_minutes, (255 * 100 / 100), LV_PART_INDICATOR);
        lv_obj_set_flag(scale_minutes, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(scale_minutes, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_t * arc_battery = lv_arc_create(dial_inner);
        lv_obj_set_name(arc_battery, "arc_battery");
        lv_obj_set_width(arc_battery, 174);
        lv_obj_set_height(arc_battery, 174);
        lv_obj_set_align(arc_battery, LV_ALIGN_CENTER);
        lv_arc_set_min_value(arc_battery, 0);
        lv_arc_set_max_value(arc_battery, 100);
        lv_arc_set_value(arc_battery, 97);
        lv_arc_set_mode(arc_battery, LV_ARC_MODE_NORMAL);
        lv_arc_set_bg_start_angle(arc_battery, 145);
        lv_arc_set_bg_end_angle(arc_battery, 205);
        lv_arc_set_rotation(arc_battery, 0);
        lv_obj_set_style_arc_width(arc_battery, 5, 0);
        lv_obj_set_style_arc_color(arc_battery, lv_color_hex(0x35383E), 0);
        lv_obj_set_style_arc_opa(arc_battery, (255 * 100 / 100), 0);
        lv_obj_set_style_arc_rounded(arc_battery, true, 0);
        lv_obj_set_style_arc_width(arc_battery, 5, LV_PART_INDICATOR);
        lv_obj_set_style_arc_color(arc_battery, lv_color_hex(0xED1B2F), LV_PART_INDICATOR);
        lv_obj_set_style_arc_opa(arc_battery, (255 * 100 / 100), LV_PART_INDICATOR);
        lv_obj_set_style_arc_rounded(arc_battery, true, LV_PART_INDICATOR);
        lv_obj_set_style_bg_opa(arc_battery, (255 * 0 / 100), LV_PART_KNOB);
        lv_obj_set_style_border_opa(arc_battery, (255 * 0 / 100), LV_PART_KNOB);
        lv_obj_set_style_outline_opa(arc_battery, (255 * 0 / 100), LV_PART_KNOB);
        lv_obj_set_style_shadow_opa(arc_battery, (255 * 0 / 100), LV_PART_KNOB);
        lv_obj_set_flag(arc_battery, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(arc_battery, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_t * label_steps_title = lv_label_create(dial_inner);
        lv_obj_set_name(label_steps_title, "label_steps_title");
        lv_label_set_text(label_steps_title, "STEP");
        lv_obj_set_align(label_steps_title, LV_ALIGN_CENTER);
        lv_obj_set_x(label_steps_title, 48);
        lv_obj_set_y(label_steps_title, -47);
        lv_obj_set_style_text_font(label_steps_title, font_data_10, 0);
        lv_obj_set_style_text_color(label_steps_title, lv_color_hex(0x8D9097), 0);
        lv_obj_set_style_text_opa(label_steps_title, (255 * 100 / 100), 0);

        lv_obj_t * label_steps_value = lv_label_create(dial_inner);
        lv_obj_set_name(label_steps_value, "label_steps_value");
        lv_label_set_text(label_steps_value, "7641");
        lv_obj_set_align(label_steps_value, LV_ALIGN_CENTER);
        lv_obj_set_x(label_steps_value, 48);
        lv_obj_set_y(label_steps_value, -31);
        lv_obj_set_style_text_font(label_steps_value, font_data_12, 0);
        lv_obj_set_style_text_color(label_steps_value, lv_color_hex(0xED1B2F), 0);
        lv_obj_set_style_text_opa(label_steps_value, (255 * 100 / 100), 0);

        lv_obj_t * label_altitude_title = lv_label_create(dial_inner);
        lv_obj_set_name(label_altitude_title, "label_altitude_title");
        lv_label_set_text(label_altitude_title, "ALT");
        lv_obj_set_align(label_altitude_title, LV_ALIGN_CENTER);
        lv_obj_set_x(label_altitude_title, 65);
        lv_obj_set_y(label_altitude_title, -12);
        lv_obj_set_style_text_font(label_altitude_title, font_data_10, 0);
        lv_obj_set_style_text_color(label_altitude_title, lv_color_hex(0x8D9097), 0);
        lv_obj_set_style_text_opa(label_altitude_title, (255 * 100 / 100), 0);

        lv_obj_t * label_altitude_value = lv_label_create(dial_inner);
        lv_obj_set_name(label_altitude_value, "label_altitude_value");
        lv_label_set_text(label_altitude_value, "2552m");
        lv_obj_set_align(label_altitude_value, LV_ALIGN_CENTER);
        lv_obj_set_x(label_altitude_value, 65);
        lv_obj_set_y(label_altitude_value, 4);
        lv_obj_set_style_text_font(label_altitude_value, font_data_12, 0);
        lv_obj_set_style_text_color(label_altitude_value, lv_color_hex(0xED1B2F), 0);
        lv_obj_set_style_text_opa(label_altitude_value, (255 * 100 / 100), 0);

        lv_obj_t * label_heart_title = lv_label_create(dial_inner);
        lv_obj_set_name(label_heart_title, "label_heart_title");
        lv_label_set_text(label_heart_title, "HR");
        lv_obj_set_align(label_heart_title, LV_ALIGN_CENTER);
        lv_obj_set_x(label_heart_title, 48);
        lv_obj_set_y(label_heart_title, 23);
        lv_obj_set_style_text_font(label_heart_title, font_data_10, 0);
        lv_obj_set_style_text_color(label_heart_title, lv_color_hex(0x8D9097), 0);
        lv_obj_set_style_text_opa(label_heart_title, (255 * 100 / 100), 0);

        lv_obj_t * label_heart_value = lv_label_create(dial_inner);
        lv_obj_set_name(label_heart_value, "label_heart_value");
        lv_label_set_text(label_heart_value, "97");
        lv_obj_set_align(label_heart_value, LV_ALIGN_CENTER);
        lv_obj_set_x(label_heart_value, 48);
        lv_obj_set_y(label_heart_value, 39);
        lv_obj_set_style_text_font(label_heart_value, font_data_12, 0);
        lv_obj_set_style_text_color(label_heart_value, lv_color_hex(0xED1B2F), 0);
        lv_obj_set_style_text_opa(label_heart_value, (255 * 100 / 100), 0);

        lv_obj_t * label_battery_value = lv_label_create(dial_inner);
        lv_obj_set_name(label_battery_value, "label_battery_value");
        lv_label_set_text(label_battery_value, "97%");
        lv_obj_set_align(label_battery_value, LV_ALIGN_CENTER);
        lv_obj_set_x(label_battery_value, -64);
        lv_obj_set_y(label_battery_value, 6);
        lv_obj_set_style_text_font(label_battery_value, font_data_12, 0);
        lv_obj_set_style_text_color(label_battery_value, lv_color_hex(0xED1B2F), 0);
        lv_obj_set_style_text_opa(label_battery_value, (255 * 100 / 100), 0);
        lv_obj_set_style_transform_pivot_x(label_battery_value, lv_pct(50), 0);
        lv_obj_set_style_transform_pivot_y(label_battery_value, lv_pct(50), 0);

        lv_obj_t * hand_hour = lv_obj_create(dial_inner);
        lv_obj_set_name(hand_hour, "hand_hour");
        lv_obj_set_width(hand_hour, 7);
        lv_obj_set_height(hand_hour, 46);
        lv_obj_set_align(hand_hour, LV_ALIGN_CENTER);
        lv_obj_set_y(hand_hour, -23);
        lv_obj_set_style_radius(hand_hour, 3, 0);
        lv_obj_set_style_bg_color(hand_hour, lv_color_hex(0xD9DCE1), 0);
        lv_obj_set_style_bg_opa(hand_hour, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(hand_hour, 1, 0);
        lv_obj_set_style_border_color(hand_hour, lv_color_hex(0x5A5D64), 0);
        lv_obj_set_style_transform_rotation(hand_hour, 0, 0);
        lv_obj_set_style_transform_pivot_x(hand_hour, lv_pct(50), 0);
        lv_obj_set_style_transform_pivot_y(hand_hour, lv_pct(100), 0);
        lv_obj_set_flag(hand_hour, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(hand_hour, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_t * hand_minute = lv_obj_create(dial_inner);
        lv_obj_set_name(hand_minute, "hand_minute");
        lv_obj_set_width(hand_minute, 5);
        lv_obj_set_height(hand_minute, 70);
        lv_obj_set_align(hand_minute, LV_ALIGN_CENTER);
        lv_obj_set_y(hand_minute, -35);
        lv_obj_set_style_radius(hand_minute, 2, 0);
        lv_obj_set_style_bg_color(hand_minute, lv_color_hex(0xF4F5F6), 0);
        lv_obj_set_style_bg_opa(hand_minute, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(hand_minute, 1, 0);
        lv_obj_set_style_border_color(hand_minute, lv_color_hex(0x6A6D73), 0);
        lv_obj_set_style_transform_rotation(hand_minute, 0, 0);
        lv_obj_set_style_transform_pivot_x(hand_minute, lv_pct(50), 0);
        lv_obj_set_style_transform_pivot_y(hand_minute, lv_pct(100), 0);
        lv_obj_set_flag(hand_minute, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(hand_minute, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_t * hand_second = lv_obj_create(dial_inner);
        lv_obj_set_name(hand_second, "hand_second");
        lv_obj_set_width(hand_second, 2);
        lv_obj_set_height(hand_second, 82);
        lv_obj_set_align(hand_second, LV_ALIGN_CENTER);
        lv_obj_set_y(hand_second, -41);
        lv_obj_set_style_radius(hand_second, 1, 0);
        lv_obj_set_style_bg_color(hand_second, lv_color_hex(0xED1B2F), 0);
        lv_obj_set_style_bg_opa(hand_second, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(hand_second, 0, 0);
        lv_obj_set_style_transform_rotation(hand_second, 0, 0);
        lv_obj_set_style_transform_pivot_x(hand_second, lv_pct(50), 0);
        lv_obj_set_style_transform_pivot_y(hand_second, lv_pct(100), 0);
        lv_obj_set_flag(hand_second, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(hand_second, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_t * hand_hub = lv_obj_create(dial_inner);
        lv_obj_set_name(hand_hub, "hand_hub");
        lv_obj_set_width(hand_hub, 15);
        lv_obj_set_height(hand_hub, 15);
        lv_obj_set_align(hand_hub, LV_ALIGN_CENTER);
        lv_obj_set_style_radius(hand_hub, 8, 0);
        lv_obj_set_style_bg_color(hand_hub, lv_color_hex(0xED1B2F), 0);
        lv_obj_set_style_bg_opa(hand_hub, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(hand_hub, 3, 0);
        lv_obj_set_style_border_color(hand_hub, lv_color_hex(0xF4F5F6), 0);
        lv_obj_set_style_border_opa(hand_hub, (255 * 100 / 100), 0);
        lv_obj_set_flag(hand_hub, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(hand_hub, LV_OBJ_FLAG_SCROLLABLE, false);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

