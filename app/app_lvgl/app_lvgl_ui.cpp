#include "app_lvgl_ui.hpp"
#include "app_itc/KeyMessageQueue.hpp"
#include "lvgl/demos/benchmark/lv_demo_benchmark.h"
#include "lvgl/demos/lv_demos.h"
#include "app_itc/ItcDataBroker.hpp"

/*Image declare*/
LV_IMG_DECLARE(Luckfox);
LV_IMG_DECLARE(cat);
LV_IMG_DECLARE(galaxy);
LV_IMG_DECLARE(saint);

namespace ui {

    static main_screen main_screen_handles;



    void lv_show_img(lv_obj_t *img, const lv_img_dsc_t img_dat) {
        lv_obj_clean(img);
        lv_img_set_src(img, &img_dat);
        lv_obj_center(img);
    }

    void app_lvgl_ui_init() {
        /*Initialize LVGL*/
        create_main_screen(main_screen_handles);
    }

    void lv_official_demo(void) {

        lv_demo_widgets();
        // lv_demo_benchmark();
        // lv_demo_music();
        // lv_demo_widgets();
        // lv_demo_keypad_encoder();
    }

    

    void create_main_screen(main_screen& ui_handles) {
        /*Create a screen*/
        ui_handles.screen_container = lv_obj_create(lv_scr_act());
        lv_obj_clear_flag(ui_handles.screen_container, LV_OBJ_FLAG_SCROLLABLE); // Disable scrollable
        lv_obj_set_style_bg_color(ui_handles.screen_container, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_bg_opa(ui_handles.screen_container, LV_OPA_COVER, 0);
        lv_obj_set_size(ui_handles.screen_container, 320, 240);
        lv_scr_load(ui_handles.screen_container);

        /*Create a title label*/
        ui_handles.title_label = lv_label_create(ui_handles.screen_container);
        lv_label_set_text(ui_handles.title_label, "Main Screen");
        lv_obj_align(ui_handles.title_label, LV_ALIGN_TOP_MID, 0, 10);

        /*Create a value label*/
        ui_handles.value_label = lv_label_create(ui_handles.screen_container);
        lv_label_set_text(ui_handles.value_label, "Value: 0");
        lv_obj_align(ui_handles.value_label, LV_ALIGN_CENTER, 0, -20);

        /*Create a status icon*/
        // ui_handles.status_icon = lv_img_create(ui_handles.screen_container);
        // lv_img_set_src(ui_handles.status_icon, &Luckfox);
        // lv_obj_align(ui_handles.status_icon, LV_ALIGN_CENTER, 0, 20);

        /*Create an action button*/
        ui_handles.action_button = lv_btn_create(ui_handles.screen_container);
        lv_obj_set_size(ui_handles.action_button, 100, 50);
        lv_obj_align(ui_handles.action_button, LV_ALIGN_BOTTOM_MID, 0, -10);

    }

    // 创建一个LVGL定时器，周期性地更新UI
    void create_ui_update_timer() {
        lv_timer_create([](lv_timer_t* timer) {
            // 定时器回调函数内部
            auto& broker = itc::ItcDataBroker::instance();

            // 1. 从broker获取最新的按键名
            auto key_name_opt = broker.get<std::string>("keypad_last_key");
            
            // 2. 如果成功获取到数据 (optional不为空)
            if (key_name_opt) {
                // 3. 使用原生LVGL API更新Label
                std::string text_to_set = "Key: " + *key_name_opt;
                // lv_label_set_text(main_screen_handles.value_label, text_to_set.c_str());
                lv_label_set_text(main_screen_handles.value_label, "hello world");

                // 4. 清除broker中的数据，避免重复更新
                broker.remove("keypad_last_key");
                
                // 打印调试信息
                std::cout << "Updated label with key: " << *key_name_opt << std::endl;
            }

        }, 50, nullptr); // 每50毫秒检查一次
    }

    // 简单测试lv_label_set_text能不能更新
    void test_lv_label_update(const int& i) {
        // 将i转换为字符串char*
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "Value: %d", i);

        lv_label_set_text(main_screen_handles.value_label, buffer);
        // std::cout << "Label updated to: Hello, LVGL!" << std::endl;
    }
} // namespace ui
