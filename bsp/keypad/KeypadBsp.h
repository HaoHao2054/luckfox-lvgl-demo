#ifndef KEYPAD_BSP_HPP
#define KEYPAD_BSP_HPP

#include "SysfsGpio.h" // 我们新创建的 C++ SysfsGpio 类
#include <vector>
#include <map>
#include <cstdint>      // 用于 uintX_t 类型
#include <memory>       // 用于 std::unique_ptr

namespace Bsp {

// 替换 DEV_Config.h 中的 #define 引脚定义
// 这些也可以是静态常量成员或枚举类（如果值不稀疏）。
// 由于它们是特定的引脚号，将它们保留为常量是合适的。
namespace KeyPins {
    const int UP_PIN    = 57;
    const int DOWN_PIN  = 66;
    const int LEFT_PIN  = 65;
    const int RIGHT_PIN = 67;
    const int PRESS_PIN = 68;
    const int A_PIN     = 69;
    const int B_PIN     = 54;
    const int X_PIN     = 64;
    const int Y_PIN     = 55;
} // namespace KeyPins

// 按键标识符枚举
enum class KeyId {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    PRESS,
    A,
    B,
    X,
    Y
};

// 按键状态枚举
enum class KeyState {
    RELEASED, // 假设高电平=释放, 低电平=按下
    PRESSED
};


class KeypadBsp {
public:
    KeypadBsp();
    ~KeypadBsp(); // 将调用 SysfsGpio 成员的析构函数，这些析构函数处理取消导出

    // 禁用拷贝和移动构造/赋值
    KeypadBsp(const KeypadBsp&) = delete;
    KeypadBsp& operator=(const KeypadBsp&) = delete;
    KeypadBsp(KeypadBsp&&) = delete;
    KeypadBsp& operator=(KeypadBsp&&) = delete;

    bool initialize(); // 显式的初始化函数
    KeyState readKey(KeyId key); // 读取指定按键的状态
    bool isKeyPressed(KeyId key); // 辅助函数：检查按键是否被按下

    static void delayMs(uint32_t milliseconds); // 静态延时函数

private:
    // 使用 KeyId 映射到对应的 SysfsGpio 对象智能指针
    std::map<KeyId, std::unique_ptr<SysfsGpio>> m_keys;
    bool m_isInitialized; // 标记是否已成功初始化

    // 辅助函数：初始化单个引脚
    bool initPin(KeyId id, int pinNumber);
};

} // namespace Bsp
#endif // KEYPAD_BSP_HPP