#include "app_keypad_init.h"
#include "app_keypad_interface.h"
#include "c_keypad/DEV_Config.h"
#include "c_keypad/sysfs_gpio.h"

#include <iostream>
#include <thread>



void app_keypad_thread() {
    /*Initialize the keypad interface*/
    DEV_ModuleInit();

    while (1) {
        /*Sleep for a short duration to avoid busy waiting*/
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        int i = 0;
        /*Check if any key is pressed*/
        if (GET_KEY_UP == 0) {
            // Handle UP key press
            i = 1; // Example action for UP key
        } else if (GET_KEY_DOWN == 0) {
            // Handle DOWN key press
            i = 2;
        } else if (GET_KEY_LEFT == 0) {
            // Handle LEFT key press
            i = 3;
        } else if (GET_KEY_RIGHT == 0) {
            // Handle RIGHT key press
            i = 4;
        } else if (GET_KEY_PRESS == 0) {
            // Handle PRESS key action
            i = 5; // Example action for PRESS key
        }

        if (i != 0) {
            // If a key was pressed, print the action
            std::cout << "Key action: " << i << std::endl;
            // Here you can add code to handle the key action, e.g., update UI or perform an action
        }
    }
}
