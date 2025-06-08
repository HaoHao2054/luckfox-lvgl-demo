#include "app_lvgl_ui.hpp"
#include "app_lvgl_init.h"
#include "app_lvgl_interface.h"
#include "app_itc/KeyMessageQueue.hpp"


#include <iostream>
#include <thread>


/* 
lvgl thread in c++11, 其他线程调用lvgl的api都要通过线程间通信来处理

*/
void app_lvgl_thread() {
    /*Initialize LittlevGL*/
    app_lvgl_init();

    /*Initialize UI*/
    ui::app_lvgl_ui_init();
    // ui::lv_official_demo();

    /*Create UI update timer*/
    ui::create_ui_update_timer();

    while (1) {
        // Sleep for 5 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        int i = 0;
        for (i = 0; i < 100; i++) {
            /*Update the label text with the current value*/
            ui::test_lv_label_update(i);

            if (i == 99) {
                i = 0; // Reset i to 0 after reaching 99
            }
        }

        /*Handle LittlevGL tasks*/
        lv_timer_handler();

    }
}