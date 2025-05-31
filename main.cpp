#include "app_keypad/app_keypad_interface.h"
#include "app_lvgl/app_lvgl_interface.h"

#include <thread>


int main(void) {
    std::thread thread_lvgl(app_lvgl_thread);
    std::thread thread_keypad(app_keypad_thread);

    thread_lvgl.join();
    thread_keypad.join();


    return 0;
}
