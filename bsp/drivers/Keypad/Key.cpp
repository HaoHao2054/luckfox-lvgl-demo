#include "Key.hpp"
#include <iostream> // For std::cerr, std::cout

namespace Bsp {

    Key::Key(std::string keyName, int pinNumber, GpioValue activeLogic) :
        m_name(std::move(keyName)),
        m_gpio(pinNumber), // Initialize SysfsGpio member
        m_activeLogic(activeLogic),
        m_isInitialized(false) {}

    bool Key::initialize() {
        if (m_isInitialized) {
            return true;
        }

        if (!m_gpio.exportPin()) {
            std::cerr << "Key Error [" << m_name << "]: Failed to export pin " << m_gpio.getPinNumber() << std::endl;
            return false;
        }

        if (!m_gpio.setDirection(GpioDirection::IN)) {
            std::cerr << "Key Error [" << m_name << "]: Failed to set pin " << m_gpio.getPinNumber()
                      << " direction to IN." << std::endl;
            m_gpio.unexportPin(); // Attempt to clean up
            return false;
        }

        m_isInitialized = true;
        std::cout << "Key Info [" << m_name << "]: Initialized on pin " << m_gpio.getPinNumber()
                  << " (Active: " << (m_activeLogic == GpioValue::LOW ? "LOW" : "HIGH") << ")" << std::endl;
        return true;
    }

    KeyState Key::getState() {
        if (!m_isInitialized) {
            std::cerr << "Key Warning [" << m_name << "]: Attempted to read uninitialized key." << std::endl;
            return KeyState::READ_ERROR; // Or RELEASED, depending on desired behavior
        }

        bool readSuccess = false;
        GpioValue currentValue = m_gpio.read(readSuccess);

        if (!readSuccess) {
            std::cerr << "Key Warning [" << m_name << "]: Failed to read GPIO value for pin "
                      << m_gpio.getPinNumber() << std::endl;
            return KeyState::READ_ERROR;
        }

        // Compare current value with the defined active logic
        if (currentValue == m_activeLogic) {
            return KeyState::PRESSED;
        } else {
            return KeyState::RELEASED;
        }
    }

    bool Key::isPressed() {
        return getState() == KeyState::PRESSED;
    }

    const std::string& Key::getName() const {
        return m_name;
    }

    int Key::getPinNumber() const {
        return m_gpio.getPinNumber(); // Delegate to SysfsGpio
    }

    bool Key::isInitialized() const {
        return m_isInitialized;
    }

} // namespace Bsp