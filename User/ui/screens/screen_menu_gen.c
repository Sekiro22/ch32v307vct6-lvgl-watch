/**
 * @file screen_menu_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "screen_menu_gen.h"
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

lv_obj_t * screen_menu_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if CH32V307_GC9A01_UI_CHECK_COMPILE_TARGET(CH32V307_GC9A01_UI_TARGET_ALL)
    if (ch32v307_gc9a01_ui_check_target(CH32V307_GC9A01_UI_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "screen_menu_#");
        lv_obj_set_style_bg_color(lv_obj_0, lv_color_hex(0x070809), 0);
        lv_obj_set_style_bg_opa(lv_obj_0, (255 * 100 / 100), 0);
        lv_obj_set_style_pad_all(lv_obj_0, 0, 0);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_t * menu_outer = lv_obj_create(lv_obj_0);
        lv_obj_set_name(menu_outer, "menu_outer");
        lv_obj_set_width(menu_outer, 236);
        lv_obj_set_height(menu_outer, 236);
        lv_obj_set_align(menu_outer, LV_ALIGN_CENTER);
        lv_obj_set_style_radius(menu_outer, 118, 0);
        lv_obj_set_style_bg_color(menu_outer, lv_color_hex(0x0D0F12), 0);
        lv_obj_set_style_bg_opa(menu_outer, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(menu_outer, 3, 0);
        lv_obj_set_style_border_color(menu_outer, lv_color_hex(0x555960), 0);
        lv_obj_set_style_pad_all(menu_outer, 0, 0);
        lv_obj_set_flag(menu_outer, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(menu_outer, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_t * menu_row_minus2 = lv_obj_create(menu_outer);
        lv_obj_set_name(menu_row_minus2, "menu_row_minus2");
        lv_obj_set_width(menu_row_minus2, 128);
        lv_obj_set_height(menu_row_minus2, 28);
        lv_obj_set_align(menu_row_minus2, LV_ALIGN_CENTER);
        lv_obj_set_y(menu_row_minus2, -78);
        lv_obj_set_style_radius(menu_row_minus2, 14, 0);
        lv_obj_set_style_bg_color(menu_row_minus2, lv_color_hex(0x171A1F), 0);
        lv_obj_set_style_bg_opa(menu_row_minus2, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(menu_row_minus2, 1, 0);
        lv_obj_set_style_border_color(menu_row_minus2, lv_color_hex(0x34373C), 0);
        lv_obj_set_style_pad_all(menu_row_minus2, 0, 0);
        lv_obj_set_flag(menu_row_minus2, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * menu_label_minus2 = lv_label_create(menu_row_minus2);
        lv_obj_set_name(menu_label_minus2, "menu_label_minus2");
        lv_label_set_text(menu_label_minus2, "锻炼");
        lv_obj_set_align(menu_label_minus2, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(menu_label_minus2, font_menu_14, 0);
        lv_obj_set_style_text_color(menu_label_minus2, lv_color_hex(0x8D9097), 0);

        lv_obj_t * menu_row_minus1 = lv_obj_create(menu_outer);
        lv_obj_set_name(menu_row_minus1, "menu_row_minus1");
        lv_obj_set_width(menu_row_minus1, 174);
        lv_obj_set_height(menu_row_minus1, 30);
        lv_obj_set_align(menu_row_minus1, LV_ALIGN_CENTER);
        lv_obj_set_y(menu_row_minus1, -39);
        lv_obj_set_style_radius(menu_row_minus1, 15, 0);
        lv_obj_set_style_bg_color(menu_row_minus1, lv_color_hex(0x171A1F), 0);
        lv_obj_set_style_bg_opa(menu_row_minus1, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(menu_row_minus1, 1, 0);
        lv_obj_set_style_border_color(menu_row_minus1, lv_color_hex(0x34373C), 0);
        lv_obj_set_style_pad_all(menu_row_minus1, 0, 0);
        lv_obj_set_flag(menu_row_minus1, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * menu_label_minus1 = lv_label_create(menu_row_minus1);
        lv_obj_set_name(menu_label_minus1, "menu_label_minus1");
        lv_label_set_text(menu_label_minus1, "闹钟");
        lv_obj_set_align(menu_label_minus1, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(menu_label_minus1, font_menu_16, 0);
        lv_obj_set_style_text_color(menu_label_minus1, lv_color_hex(0xD9DCE1), 0);

        lv_obj_t * menu_row_selected = lv_obj_create(menu_outer);
        lv_obj_set_name(menu_row_selected, "menu_row_selected");
        lv_obj_set_width(menu_row_selected, 190);
        lv_obj_set_height(menu_row_selected, 36);
        lv_obj_set_align(menu_row_selected, LV_ALIGN_CENTER);
        lv_obj_set_style_radius(menu_row_selected, 18, 0);
        lv_obj_set_style_bg_color(menu_row_selected, lv_color_hex(0xED1B2F), 0);
        lv_obj_set_style_bg_opa(menu_row_selected, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(menu_row_selected, 0, 0);
        lv_obj_set_style_pad_all(menu_row_selected, 0, 0);
        lv_obj_set_flag(menu_row_selected, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * menu_label_selected = lv_label_create(menu_row_selected);
        lv_obj_set_name(menu_label_selected, "menu_label_selected");
        lv_label_set_text(menu_label_selected, "设置");
        lv_obj_set_align(menu_label_selected, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(menu_label_selected, font_menu_20, 0);
        lv_obj_set_style_text_color(menu_label_selected, lv_color_hex(0xFFFFFF), 0);

        lv_obj_t * menu_row_plus1 = lv_obj_create(menu_outer);
        lv_obj_set_name(menu_row_plus1, "menu_row_plus1");
        lv_obj_set_width(menu_row_plus1, 174);
        lv_obj_set_height(menu_row_plus1, 30);
        lv_obj_set_align(menu_row_plus1, LV_ALIGN_CENTER);
        lv_obj_set_y(menu_row_plus1, 39);
        lv_obj_set_style_radius(menu_row_plus1, 15, 0);
        lv_obj_set_style_bg_color(menu_row_plus1, lv_color_hex(0x171A1F), 0);
        lv_obj_set_style_bg_opa(menu_row_plus1, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(menu_row_plus1, 1, 0);
        lv_obj_set_style_border_color(menu_row_plus1, lv_color_hex(0x34373C), 0);
        lv_obj_set_style_pad_all(menu_row_plus1, 0, 0);
        lv_obj_set_flag(menu_row_plus1, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * menu_label_plus1 = lv_label_create(menu_row_plus1);
        lv_obj_set_name(menu_label_plus1, "menu_label_plus1");
        lv_label_set_text(menu_label_plus1, "秒表");
        lv_obj_set_align(menu_label_plus1, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(menu_label_plus1, font_menu_16, 0);
        lv_obj_set_style_text_color(menu_label_plus1, lv_color_hex(0xD9DCE1), 0);

        lv_obj_t * menu_row_plus2 = lv_obj_create(menu_outer);
        lv_obj_set_name(menu_row_plus2, "menu_row_plus2");
        lv_obj_set_width(menu_row_plus2, 128);
        lv_obj_set_height(menu_row_plus2, 28);
        lv_obj_set_align(menu_row_plus2, LV_ALIGN_CENTER);
        lv_obj_set_y(menu_row_plus2, 78);
        lv_obj_set_style_radius(menu_row_plus2, 14, 0);
        lv_obj_set_style_bg_color(menu_row_plus2, lv_color_hex(0x171A1F), 0);
        lv_obj_set_style_bg_opa(menu_row_plus2, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(menu_row_plus2, 1, 0);
        lv_obj_set_style_border_color(menu_row_plus2, lv_color_hex(0x34373C), 0);
        lv_obj_set_style_pad_all(menu_row_plus2, 0, 0);
        lv_obj_set_flag(menu_row_plus2, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * menu_label_plus2 = lv_label_create(menu_row_plus2);
        lv_obj_set_name(menu_label_plus2, "menu_label_plus2");
        lv_label_set_text(menu_label_plus2, "计时器");
        lv_obj_set_align(menu_label_plus2, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(menu_label_plus2, font_menu_14, 0);
        lv_obj_set_style_text_color(menu_label_plus2, lv_color_hex(0x8D9097), 0);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

