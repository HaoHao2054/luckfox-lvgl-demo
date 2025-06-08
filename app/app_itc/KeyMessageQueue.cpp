#include "KeyMessageQueue.hpp"
#include "app_keypad/app_keypad_interface.hpp"
#include "app_lvgl/app_lvgl_interface.h"


/* 用于实现app中的各个线程间的通信 */
KeyMessageQueue g_key_event_queue; // 全局实例定义

void KeyMessageQueue::send(const std::string& key_name) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(key_name);
    cond_var_.notify_one(); // 如果有线程在阻塞等待，则唤醒它
}

// 非阻塞接收
bool KeyMessageQueue::receive(std::string& key_name) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return false;
    }
    key_name = queue_.front();
    queue_.pop();
    return true;
}

// 阻塞接收 (可选)
std::string KeyMessageQueue::receive_blocking() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_var_.wait(lock, [this] { return !queue_.empty(); });
    std::string key_name = queue_.front();
    queue_.pop();
    return key_name;
}