#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define DISP_BUF_SIZE (320 * 240)

void fbdev_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p);

/* initial app_lvgl */
void app_lvgl_init();


#ifdef __cplusplus
}
#endif