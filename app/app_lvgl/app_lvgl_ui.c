#include "app_lvgl_ui.h"

/*Image declare*/
LV_IMG_DECLARE(Luckfox);
LV_IMG_DECLARE(cat);
LV_IMG_DECLARE(galaxy);
LV_IMG_DECLARE(saint);

void lv_show_img(lv_obj_t *img, const lv_img_dsc_t img_dat) {
    lv_obj_clean(img);
    lv_img_set_src(img, &img_dat);
    lv_obj_center(img);
}

void app_lvgl_ui_init() {
    /*Show an image*/
    lv_obj_t *scr = lv_disp_get_scr_act(NULL);
    lv_obj_t *img = lv_img_create(scr);
    lv_show_img(img, Luckfox);
    lv_obj_center(img);

    /*Create a cursor*/
    lv_obj_t *cursor = lv_img_create(scr);
    lv_img_set_src(cursor, LV_SYMBOL_GPS);
    lv_obj_set_pos(cursor, 70, 120);
    int x = 70, y = 120, move = 0;
}
