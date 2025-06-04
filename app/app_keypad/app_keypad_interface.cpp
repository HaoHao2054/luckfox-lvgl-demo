#include "app_itc/app_itc.hpp"
#include "app_keypad_interface.hpp"

#include <iostream>
#include <string>
#include <thread>

const KeyDef keys[] = {
        {"UP", 57, Bsp::GpioValue::LOW},
        {"LEFT", 65, Bsp::GpioValue::LOW},
        {"RIGHT", 67, Bsp::GpioValue::LOW},
        {"DOWN", 66, Bsp::GpioValue::LOW},
        {"PRESS", 68, Bsp::GpioValue::LOW},
        {"A", 69, Bsp::GpioValue::LOW},
        {"B", 54, Bsp::GpioValue::LOW},
        {"X", 64, Bsp::GpioValue::LOW},
        {"Y", 55, Bsp::GpioValue::LOW},
};


int app_keypad_thread() {
    /*Initialize the keypad interface*/
    Bsp::KeypadManager myKeypad;

    /* initialize keypad via myKeypad */
    keypad_init(myKeypad);
    

    while (1) {
        /*Sleep for a short duration to avoid busy waiting*/
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        /*Check if any key is pressed*/
        std::string pressed_key_name = is_keypad_pressed(myKeypad);
        if (!pressed_key_name.empty()) {
             std::cout << "Key pressed: " << pressed_key_name << std::endl;

            /*Send the pressed key name to the ITC queue*/
            
        }
    }
    return 0;
}

int keypad_init(Bsp::KeypadManager& keypadManager) {
    using namespace Bsp;

    for (const auto& key : keys) {
        if (!keypadManager.addKey(key.name, key.gpio, key.activeLogic)) {
            std::cerr << "Failed to add " << key.name << " key. Exiting." << std::endl;
            return 1;
        }
    }
    return 0;
}

std::string is_keypad_pressed(Bsp::KeypadManager& keypadManager) {
    using namespace Bsp;

    for (const auto& key : keys) {
        if (keypadManager.isKeyPressed(key.name)) {
            return key.name;
        } 
    }
    return ""; // Return empty string if no key is pressed
}
