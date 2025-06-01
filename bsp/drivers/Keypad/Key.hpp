#ifndef KEY_HPP
#define KEY_HPP

#include "../../hal/SysfsGpio/SysfsGpio.hpp" // Uses GpioValue and SysfsGpio
#include <string>
#include <memory> // For std::unique_ptr if we decide to heap allocate SysfsGpio, but member is fine

namespace Bsp {

    // Key state enum (can be shared or specific to keypad context)
    enum class KeyState {
        RELEASED,
        PRESSED,
        READ_ERROR // Optional: to indicate a problem reading the key
    };

    class Key {
    public:
        // Constructor: name, pin, and which electrical level means "pressed"
        Key(std::string keyName, int pinNumber, GpioValue activeLogic);
        // Destructor will be default, SysfsGpio member handles its own unexport

        // Deleted copy/move constructors and assignment operators
        Key(const Key&) = delete;
        Key& operator=(const Key&) = delete;
        Key(Key&&) = delete;
        Key& operator=(Key&&) = delete;

        bool initialize();       // Exports and sets direction for the GPIO pin
        KeyState getState();     // Reads the current state (PRESSED or RELEASED)
        bool isPressed();        // Convenience: true if current state is PRESSED
        const std::string& getName() const;
        int getPinNumber() const;
        bool isInitialized() const;

    private:
        std::string m_name;
        SysfsGpio m_gpio; // Composition: Key owns its SysfsGpio object
        GpioValue m_activeLogic; // Defines if LOW or HIGH means pressed
        bool m_isInitialized;
    };

} // namespace Bsp
#endif // KEY_HPP