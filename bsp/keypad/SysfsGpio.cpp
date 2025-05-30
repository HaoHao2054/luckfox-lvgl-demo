#include "SysfsGpio.h"
#include <fstream>   // 用于 std::ofstream, std::ifstream
#include <unistd.h>  // 用于 access() (检查文件存在)
#include <iostream>  // 用于 std::cerr, std::cout
#include <thread>    // 用于 std::this_thread::sleep_for
#include <chrono>    // 用于 std::chrono::milliseconds

namespace Bsp {

const std::string GPIO_SYSFS_BASE_PATH = "/sys/class/gpio/";

// 静态辅助函数：检查路径（文件/目录）是否存在
bool SysfsGpio::pathExists(const std::string& path) {
    return access(path.c_str(), F_OK) == 0;
}

// 静态辅助函数：将字符串写入文件
bool SysfsGpio::writeToFile(const std::string& filePath, const std::string& value) {
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        std::cerr << "SysfsGpio Error: 无法打开文件: " << filePath << std::endl;
        return false;
    }
    outFile << value;
    if (outFile.fail()) {
        std::cerr << "SysfsGpio Error: 无法将 '" << value << "' 写入文件: " << filePath << std::endl;
        outFile.close();
        return false;
    }
    outFile.close();
    return true;
}

SysfsGpio::SysfsGpio(int pinNumber)
    : m_pinNumber(pinNumber),
      m_gpioPath(GPIO_SYSFS_BASE_PATH + "gpio" + std::to_string(pinNumber) + "/"),
      m_successfullyExportedByThisInstance(false) {
}

SysfsGpio::~SysfsGpio() {
    if (m_successfullyExportedByThisInstance) {
        // 在取消导出前稍作延迟，sysfs 有时需要这个
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (!unexportPin()) { // unexportPin 现在只在由该实例导出时才取消导出
            std::cerr << "SysfsGpio Warning: 析构函数未能取消导出引脚 " << m_pinNumber << std::endl;
        }
    }
}

bool SysfsGpio::exportPin() {
    if (isExported()) { // 检查 gpioN 目录是否已存在
        // 如果已导出（可能由其他实体导出），我们不声明所有权，
        // 但认为操作成功以供使用。然而，为了让这个类实例管理取消导出，
        // 它应该是执行导出的那一个。
        // 为简单起见，如果调用此函数，我们尝试导出。
        // 如果已导出，那也没关系。我们从现在开始管理它。
        m_successfullyExportedByThisInstance = true; // 假设我们现在可以管理它
        return true;
    }

    if (!writeToFile(GPIO_SYSFS_BASE_PATH + "export", std::to_string(m_pinNumber))) {
        return false;
    }

    // Sysfs 需要一些时间来创建目录。
    // 带超时的循环比固定睡眠更健壮。
    int retries = 20; // 增加重试次数或延迟时间
    bool exported_check_success = false;
    while (retries-- > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(25)); // 稍长一点的延迟
        if (isExported()) {
            exported_check_success = true;
            break;
        }
    }

    if (!exported_check_success) {
         std::cerr << "SysfsGpio Error: 引脚 " << m_pinNumber << " 目录在导出后未找到。" << std::endl;
         // 尝试回滚导出操作
         writeToFile(GPIO_SYSFS_BASE_PATH + "unexport", std::to_string(m_pinNumber));
         return false;
    }

    m_successfullyExportedByThisInstance = true;
    return true;
}

bool SysfsGpio::unexportPin() {
    // 即使不是由这个实例导出的，如果被要求 unexport 并且它当前是导出的，也尝试 unexport。
    if (!isExported()) {
        m_successfullyExportedByThisInstance = false; // 如果之前设置了标志，则清除
        return true; // 引脚未导出，无需操作
    }

    if (!writeToFile(GPIO_SYSFS_BASE_PATH + "unexport", std::to_string(m_pinNumber))) {
        return false;
    }
    m_successfullyExportedByThisInstance = false; // 成功取消导出后清除标志
    return true;
}

bool SysfsGpio::isExported() const {
    return pathExists(m_gpioPath);
}

bool SysfsGpio::setDirection(GpioDirection direction) {
    if (!isExported()) { // 在设置方向之前确保引脚已导出
        std::cerr << "SysfsGpio Error: 引脚 " << m_pinNumber << " 未导出。无法设置方向。" << std::endl;
        return false;
    }
    std::string dirStr = (direction == GpioDirection::IN) ? "in" : "out";
    if (!writeToFile(m_gpioPath + "direction", dirStr)) {
        std::cerr << "SysfsGpio Error: 设置引脚 " << m_pinNumber << " 方向为 " << dirStr << " 失败。" << std::endl;
        return false;
    }
    // 在C代码中，设置方向后会打印信息。这里可以通过返回值和日志来体现。
    // printf("IN Pin = %d\r\n",Pin); or printf("OUT Pin = %d\r\n",Pin);
    if (direction == GpioDirection::IN) {
        std::cout << "SysfsGpio Info: Pin " << m_pinNumber << " direction set to IN." << std::endl;
    } else {
        std::cout << "SysfsGpio Info: Pin " << m_pinNumber << " direction set to OUT." << std::endl;
    }
    return true;
}

GpioValue SysfsGpio::read(bool& success) {
    success = false;
    if (!isExported()) {
        std::cerr << "SysfsGpio Error: 引脚 " << m_pinNumber << " 未导出。无法读取。" << std::endl;
        return GpioValue::LOW;
    }
    std::ifstream valueFile(m_gpioPath + "value");
    if (!valueFile.is_open()) {
        std::cerr << "SysfsGpio Error: 无法打开引脚 " << m_pinNumber << " 的值文件进行读取。" << std::endl;
        return GpioValue::LOW;
    }
    char val_char;
    valueFile >> val_char;
    if (valueFile.fail() || (val_char != '0' && val_char != '1')) {
        std::cerr << "SysfsGpio Error: 从引脚 " << m_pinNumber << " 读取到无效值。" << std::endl;
        valueFile.close();
        return GpioValue::LOW;
    }
    valueFile.close();
    success = true;
    return (val_char == '0') ? GpioValue::LOW : GpioValue::HIGH;
}

bool SysfsGpio::write(GpioValue value) {
    if (!isExported()) {
        std::cerr << "SysfsGpio Error: 引脚 " << m_pinNumber << " 未导出。无法写入。" << std::endl;
        return false;
    }
    char val_char = (value == GpioValue::LOW) ? '0' : '1';
    return writeToFile(m_gpioPath + "value", std::string(1, val_char));
}

} // namespace Bsp