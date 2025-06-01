#include "KeypadManager.hpp"
#include <chrono>   // For std::chrono::milliseconds
#include <thread>   // For std::this_thread::sleep_for
#include <iostream> // For std::cout, std::cerr

namespace Bsp {

    KeypadManager::~KeypadManager() {
        if (!m_keys.empty()) {
            std::cout << "KeypadManager: Shutting down and releasing keys." << std::endl;
        }
        // m_keys unique_ptrs will automatically be destroyed,
        // calling the destructor of each Key object,
        // which in turn ensures their SysfsGpio members are properly destructed (unexported).
        m_keys.clear();
    }

    bool KeypadManager::addKey(const std::string& name, int pinNumber, GpioValue activeLogic) {
        if (m_keys.count(name)) {
            std::cerr << "KeypadManager Error: Key with name '" << name << "' already exists." << std::endl;
            return false;
        }

        auto newKey = std::make_unique<Key>(name, pinNumber, activeLogic);
        if (!newKey->initialize()) {
            std::cerr << "KeypadManager Error: Failed to initialize key '" << name << "' on pin " << pinNumber << "." << std::endl;
            // newKey will be destroyed here if initialization fails, SysfsGpio unexport handled by Key's SysfsGpio destructor
            return false;
        }

        m_keys[name] = std::move(newKey);
        std::cout << "KeypadManager Info: Successfully added and initialized key '" << name << "'." << std::endl;
        return true;
    }

    KeyState KeypadManager::getKeyState(const std::string& name) {
        auto it = m_keys.find(name);
        if (it == m_keys.end()) {
            std::cerr << "KeypadManager Warning: Attempted to read unknown key '" << name << "'." << std::endl;
            return KeyState::READ_ERROR; // Or a default like RELEASED
        }
        return it->second->getState();
    }

    bool KeypadManager::isKeyPressed(const std::string& name) {
        return getKeyState(name) == KeyState::PRESSED;
    }

    void KeypadManager::delayMs(uint32_t milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

} // namespace Bsp