#pragma once


#include "drivers/Keypad/KeypadManager.hpp"
#include "hal/SysfsGpio/SysfsGpio.hpp"

int app_keypad_thread();
int keypad_init(Bsp::KeypadManager &keypadManager);
std::string is_keypad_pressed(Bsp::KeypadManager& keypadManager);

struct KeyDef {
    std::string name;
    int gpio;
    Bsp::GpioValue activeLogic;
};
