#include "app_keypad_interface.h"
#include "drivers/Keypad/KeypadManager.hpp"

#include <iostream>
#include <thread>



int app_keypad_thread() {
    /*Initialize the keypad interface*/
    Bsp::KeypadManager myKeypad;

    /* initialize keypad via myKeypad */
    if (!myKeypad.addKey("UP", 57, Bsp::GpioValue::LOW)) {
        std::cerr << "Failed to add UP key. Exiting." << std::endl;
        return 1;
    }
    if (!myKeypad.addKey("LEFT", 65, Bsp::GpioValue::LOW)) {
        std::cerr << "Failed to add LEFT key. Exiting." << std::endl;
        return 1;
    }
    if (!myKeypad.addKey("RIGHT", 67, Bsp::GpioValue::LOW)) {
        std::cerr << "Failed to add RIGHT key. Exiting." << std::endl;
        return 1;
    }
    if (!myKeypad.addKey("DOWN", 66, Bsp::GpioValue::LOW)) {
        std::cerr << "Failed to add DOWN key. Exiting." << std::endl;
        return 1;
    }
    if (!myKeypad.addKey("PRESS", 68, Bsp::GpioValue::LOW)) {
        std::cerr << "Failed to add PRESS key. Exiting." << std::endl;
        return 1;
    }
    if (!myKeypad.addKey("A", 69, Bsp::GpioValue::LOW)) {
        std::cerr << "Failed to add A key. Exiting." << std::endl;
        return 1;
    }
    if (!myKeypad.addKey("B", 54, Bsp::GpioValue::LOW)) {
        std::cerr << "Failed to add B key. Exiting." << std::endl;
        return 1;
    }
    if (!myKeypad.addKey("X", 64, Bsp::GpioValue::LOW)) {
        std::cerr << "Failed to add X key. Exiting." << std::endl;
        return 1;
    }
    if (!myKeypad.addKey("Y", 55, Bsp::GpioValue::LOW)) {
        std::cerr << "Failed to add Y key. Exiting." << std::endl;
        return 1;
    }

    while (1) {
        /*Sleep for a short duration to avoid busy waiting*/
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        int i = 0;
        if (myKeypad.isKeyPressed("UP")) {
            i = 1; 
        } else if (myKeypad.isKeyPressed("DOWN")) {
            i = 2; 
        } else if (myKeypad.isKeyPressed("LEFT")) {
            i = 3; 
        } else if (myKeypad.isKeyPressed("RIGHT")) {
            i = 4; 
        } else if (myKeypad.isKeyPressed("PRESS")) {
            i = 5; 
        } else if (myKeypad.isKeyPressed("A")) {
            i = 6; 
        } else if (myKeypad.isKeyPressed("B")) {
            i = 7; 
        } else if (myKeypad.isKeyPressed("X")) {
            i = 8; // Action for X key
        } else if (myKeypad.isKeyPressed("Y")) {
            i = 9; 
        }

        if (i != 0) {
            // If a key was pressed, print the action
            std::cout << "Key action: " << i << std::endl;
            // Here you can add code to handle the key action, e.g., update UI or perform an action
        }
    }
}
