#ifndef SYSFS_GPIO_HPP
#define SYSFS_GPIO_HPP

#include <string>
#include <cstdint> // 用于 uintX_t 类型

namespace Bsp {

// GPIO 方向枚举 (替代 SYSFS_GPIO_IN, SYSFS_GPIO_OUT)
enum class GpioDirection {
    IN,
    OUT
};

// GPIO 电平值枚举 (替代 SYSFS_GPIO_LOW, SYSFS_GPIO_HIGH)
enum class GpioValue : uint8_t {
    LOW = 0,
    HIGH = 1
};

class SysfsGpio {
public:
    explicit SysfsGpio(int pinNumber); // 构造函数，指定 GPIO 引脚号
    ~SysfsGpio();                     // 析构函数，如果由此实例导出，则取消导出引脚

    // 禁用拷贝和移动构造/赋值，或根据需要正确实现它们
    SysfsGpio(const SysfsGpio&) = delete;
    SysfsGpio& operator=(const SysfsGpio&) = delete;
    SysfsGpio(SysfsGpio&&) = delete;
    SysfsGpio& operator=(SysfsGpio&&) = delete;

    bool exportPin();    // 导出 GPIO 引脚
    bool unexportPin();  // 取消导出 GPIO 引脚
    bool setDirection(GpioDirection direction); // 设置 GPIO 方向
    GpioValue read(bool& success); // 读取 GPIO 电平值，通过引用传递成功状态
    bool write(GpioValue value);   // 写入 GPIO 电平值
    bool isExported() const;       // 检查引脚是否已导出

    int getPinNumber() const { return m_pinNumber; }

private:
    int m_pinNumber;
    std::string m_gpioPath; // GPIO 引脚的 sysfs 路径，例如 /sys/class/gpio/gpioX/
    bool m_successfullyExportedByThisInstance; // 标记此实例是否成功导出了该引脚

    // 静态辅助函数，用于向文件写入字符串
    static bool writeToFile(const std::string& filePath, const std::string& value);
    // 静态辅助函数，用于检查路径是否存在
    static bool pathExists(const std::string& path);
};

} // namespace Bsp
#endif // SYSFS_GPIO_HPP