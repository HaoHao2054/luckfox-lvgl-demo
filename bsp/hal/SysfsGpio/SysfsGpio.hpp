#ifndef SYSFS_GPIO_HPP
#define SYSFS_GPIO_HPP

#include <cstdint> // For uintX_t types
#include <string>
#include <vector> // Required by some compilers for std::to_string with int

namespace Bsp {

    // GPIO Direction enum
    enum class GpioDirection { IN, OUT };

    // GPIO Value enum
    enum class GpioValue : uint8_t { LOW = 0, HIGH = 1 };

    class SysfsGpio {
    public:
        explicit SysfsGpio(int pinNumber);
        ~SysfsGpio();

        SysfsGpio(const SysfsGpio &) = delete;
        SysfsGpio &operator=(const SysfsGpio &) = delete;
        SysfsGpio(SysfsGpio &&) = delete;
        SysfsGpio &operator=(SysfsGpio &&) = delete;

        bool exportPin();
        bool unexportPin();
        bool setDirection(GpioDirection direction);
        GpioValue read(bool &success);
        bool write(GpioValue value);
        bool isExported() const;

        int getPinNumber() const { return m_pinNumber; }

    private:
        int m_pinNumber;
        std::string m_gpioPath;
        bool m_successfullyExportedByThisInstance;

        static bool writeToFile(const std::string &filePath, const std::string &value);
        static bool pathExists(const std::string &path);
    };

} // namespace Bsp
#endif // SYSFS_GPIO_HPP