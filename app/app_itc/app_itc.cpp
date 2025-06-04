#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <chrono>
#include "app_itc.hpp"
#include "app_keypad/app_keypad_interface.hpp"
#include "app_lvgl/app_lvgl_interface.h"


/* 用于实现app中的各个线程间的通信 */