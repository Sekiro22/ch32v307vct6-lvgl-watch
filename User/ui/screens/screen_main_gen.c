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
        lv_obj_set_style_bg_color(lv_obj_0, lv_color_hex(0x101820), 0);

        lv_obj_t * lv_label_0 = lv_label_create(lv_obj_0);
        lv_label_set_text(lv_label_0, "LVGL UI");
        lv_obj_set_align(lv_label_0, LV_ALIGN_CENTER);
        lv_obj_set_y(lv_label_0, -30);
        lv_obj_set_style_text_color(lv_label_0, lv_color_hex(0xffffff), 0);

        lv_obj_t * lv_button_0 = lv_button_create(lv_obj_0);
        lv_obj_set_align(lv_button_0, LV_ALIGN_CENTER);
        lv_obj_set_y(lv_button_0, 30);
        lv_obj_set_width(lv_button_0, 100);
        lv_obj_set_height(lv_button_0, 40);
        lv_obj_t * lv_label_1 = lv_label_create(lv_button_0);
        lv_label_set_text(lv_label_1, "OK");
        lv_obj_set_x(lv_label_1, 0);
        lv_obj_set_y(lv_label_1, 0);
        lv_label_set_recolor(lv_label_1, false);
        lv_label_set_long_mode(lv_label_1, LV_LABEL_LONG_MODE_WRAP);
        lv_obj_set_align(lv_label_1, LV_ALIGN_CENTER);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

