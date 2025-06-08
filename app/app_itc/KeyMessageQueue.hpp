#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <chrono>


class KeyMessageQueue {
public:
    void send(const std::string& key_name);
    bool receive(std::string& key_name); // 非阻塞接收
    std::string receive_blocking();       // 阻塞接收 (如果需要)

private:
    std::queue<std::string> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_; // 主要用于阻塞接收
};

// 全局实例或通过其他方式使其在两个线程中可访问
extern KeyMessageQueue g_key_event_queue;