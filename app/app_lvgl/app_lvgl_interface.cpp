#include "app_lvgl_ui.h"
#include "app_lvgl_init.h"
#include "app_lvgl_interface.h"



#include <iostream>
#include <thread>


/* 
lvgl thread in c++11, 其他线程调用lvgl的api都要通过线程间通信来处理

*/
void app_lvgl_thread() {
    /*Initialize LittlevGL*/
    app_lvgl_init();

    /*Initialize UI*/
    // app_lvgl_ui_init();
    lv_official_demo();

    while (1) {
        // usleep(5000); // Sleep for 5 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        /*Handle LittlevGL tasks*/
        lv_timer_handler();

    }
}