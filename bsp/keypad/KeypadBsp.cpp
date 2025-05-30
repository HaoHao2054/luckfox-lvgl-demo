#include "KeypadBsp.h"
#include <thread>        // 用于 std::this_thread::sleep_for
#include <chrono>        // 用于 std::chrono::milliseconds
#include <iostream>      // 用于 std::cerr, std::cout

namespace Bsp {

KeypadBsp::KeypadBsp() : m_isInitialized(false) {
    // 构造函数本身不立即初始化引脚，需要调用 initialize() 方法。
}

// 初始化单个按键引脚的辅助函数
bool KeypadBsp::initPin(KeyId id, int pinNumber) {
    auto gpio = std::make_unique<SysfsGpio>(pinNumber); // 创建 SysfsGpio 对象
    if (!gpio->exportPin()) { // 导出引脚
        std::cerr << "KeypadBsp Error: 导出按键 " << static_cast<int>(id) << " (引脚 " << pinNumber << ") 失败。" << std::endl;
        return false;
    }
    // DEV_GPIO_Mode 中设置方向为输入
    if (!gpio->setDirection(GpioDirection::IN)) { // 设置方向为输入
        std::cerr << "KeypadBsp Error: 设置按键 " << static_cast<int>(id) << " (引脚 " << pinNumber << ") 方向为 IN 失败。" << std::endl;
        // 如果设置方向失败，但之前导出成功，尝试取消导出
        gpio->unexportPin();
        return false;
    }
    m_keys[id] = std::move(gpio); // 将成功初始化的 GPIO 对象存入 map
    return true;
}

// 初始化所有按键
bool KeypadBsp::initialize() {
    if (m_isInitialized) { // 如果已初始化，则直接返回 true
        return true;
    }

    std::cout << "KeypadBsp: Initializing keys..." << std::endl;
    bool success = true;
    // 初始化所有在 DEV_GPIO_Init 中定义的按键
    success &= initPin(KeyId::UP,    KeyPins::UP_PIN);
    success &= initPin(KeyId::DOWN,  KeyPins::DOWN_PIN);
    success &= initPin(KeyId::LEFT,  KeyPins::LEFT_PIN);
    success &= initPin(KeyId::RIGHT, KeyPins::RIGHT_PIN);
    success &= initPin(KeyId::PRESS, KeyPins::PRESS_PIN);
    success &= initPin(KeyId::A,     KeyPins::A_PIN);
    success &= initPin(KeyId::B,     KeyPins::B_PIN);
    success &= initPin(KeyId::X,     KeyPins::X_PIN);
    success &= initPin(KeyId::Y,     KeyPins::Y_PIN);

    if (!success) {
        std::cerr << "KeypadBsp Error: 一个或多个按键初始化失败。正在清理..." << std::endl;
        m_keys.clear(); // 清理会调用 unique_ptr 的析构函数，从而释放 SysfsGpio 对象并取消导出
        m_isInitialized = false;
        return false;
    }

    m_isInitialized = true;
    std::cout << "KeypadBsp: 所有按键初始化成功。" << std::endl;
    return true;
}

KeypadBsp::~KeypadBsp() {
    // 当 m_keys 这个 map 被销毁时，它包含的 unique_ptr 会自动调用其指向的
    // SysfsGpio 对象的析构函数。这些析构函数会负责取消导出由它们管理的引脚。
    if (m_isInitialized) {
         std::cout << "KeypadBsp: 正在关闭并取消导出引脚。" << std::endl;
    }
    m_keys.clear(); // 显式清除，或让 map 的析构函数自动处理
}

KeyState KeypadBsp::readKey(KeyId key) {
    if (!m_isInitialized) {
        std::cerr << "KeypadBsp Warning: 在未初始化状态下读取按键。" << std::endl;
        return KeyState::RELEASED; // 或抛出异常
    }

    auto it = m_keys.find(key); // 在 map 中查找对应的按键
    if (it == m_keys.end() || !it->second) { // 确保找到且 GPIO 对象有效
        std::cerr << "KeypadBsp Error: 按键 ID " << static_cast<int>(key) << " 未找到或 GPIO 对象为空。" << std::endl;
        return KeyState::RELEASED; // 或抛出异常
    }

    bool readSuccess = false;
    GpioValue val = it->second->read(readSuccess); // 读取 GPIO 电平

    if (!readSuccess) {
        std::cerr << "KeypadBsp Warning: 读取按键 ID " << static_cast<int>(key) << " 的 GPIO 失败。" << std::endl;
        return KeyState::RELEASED; //读取失败时默认为释放状态
    }

    // 根据原始 C 代码的检查 `GET_KEYA == 0`，假设低电平 (0) 表示按下
    return (val == GpioValue::LOW) ? KeyState::PRESSED : KeyState::RELEASED;
}

bool KeypadBsp::isKeyPressed(KeyId key) {
    return readKey(key) == KeyState::PRESSED;
}

void KeypadBsp::delayMs(uint32_t milliseconds) {
    // usleep 的参数是微秒。原始 DEV_Delay_ms 是循环调用 usleep(1000)。
    // 一次完整的休眠更好。
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

} // namespace Bsp