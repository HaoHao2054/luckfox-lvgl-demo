#include "app_keypad_interface.hpp"
#include "app_itc/KeyMessageQueue.hpp"
#include "app_itc/ItcDataBroker.hpp"

#include <iostream>
#include <string>
#include <thread>

const KeyDef keys[] = {
        {"UP", 57, Bsp::GpioValue::LOW},   {"LEFT", 65, Bsp::GpioValue::LOW},  {"RIGHT", 67, Bsp::GpioValue::LOW},
        {"DOWN", 66, Bsp::GpioValue::LOW}, {"PRESS", 68, Bsp::GpioValue::LOW}, {"A", 69, Bsp::GpioValue::LOW},
        {"B", 54, Bsp::GpioValue::LOW},    {"X", 64, Bsp::GpioValue::LOW},     {"Y", 55, Bsp::GpioValue::LOW},
};


int app_keypad_thread() {
    /*Initialize the keypad interface*/
    Bsp::KeypadManager myKeypad;

    /*Initialize keypad via myKeypad*/
    keypad_init(myKeypad);

    /*Initialize the ITC data broker*/
    auto& broker = itc::ItcDataBroker::instance();


    while (1) {
        /*Sleep for a short duration to avoid busy waiting*/
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        /*Check if any key is pressed*/
        std::string pressed_key_name = is_keypad_pressed(myKeypad);
        if (!pressed_key_name.empty()) {
            std::cout << "Key pressed: " << pressed_key_name << std::endl;
            
            /*Send the pressed key name to the ITC queue*/
            // g_key_event_queue.send(pressed_key_name); 
            broker.set<std::string>("keypad_last_key", pressed_key_name);
        }
    }
    return 0;
}

int keypad_init(Bsp::KeypadManager &keypadManager) {
    using namespace Bsp;

    for (const auto &key: keys) {
        if (!keypadManager.addKey(key.name, key.gpio, key.activeLogic)) {
            std::cerr << "Failed to add " << key.name << " key. Exiting." << std::endl;
            return 1;
        }
    }
    return 0;
}

std::string is_keypad_pressed(Bsp::KeypadManager &keypadManager) {
    using namespace Bsp;

    // // 静态变量存储上次检查时间
    // static auto last_check_time = std::chrono::steady_clock::now();
    // auto now = std::chrono::steady_clock::now();

    // // 如果未超过 100ms，直接返回空字符串
    // if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_check_time) <
    // std::chrono::milliseconds(100)) {
    //     return "";
    // }

    // // 更新检查时间
    // last_check_time = now;

    for (const auto &key: keys) {
        if (keypadManager.isKeyPressed(key.name)) {
            return key.name;
        }
    }
    return ""; // Return empty string if no key is pressed
}
