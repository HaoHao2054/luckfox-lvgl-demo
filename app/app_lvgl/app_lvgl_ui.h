#pragma once 

#include "lvgl/lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

void lv_show_img(lv_obj_t *img, const lv_img_dsc_t img_dat);
void app_lvgl_ui_init(void);
void lv_official_demo(void);
#ifdef __cplusplus
}
#endif