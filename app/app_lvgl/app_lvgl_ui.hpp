#pragma once

#include "lvgl/lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif


namespace ui {

    void lv_show_img(lv_obj_t *img, const lv_img_dsc_t img_dat);
    void app_lvgl_ui_init(void);
    void lv_official_demo(void);

    struct main_screen {
        lv_obj_t *screen_container;
        lv_obj_t *title_label;
        lv_obj_t *value_label;
        lv_obj_t *status_icon;
        lv_obj_t *action_button;
    };

    void create_main_screen(main_screen& ui_handles);
    void create_ui_update_timer();
    void test_lv_label_update(const int& i);

} // namespace ui


#ifdef __cplusplus
}
#endif
