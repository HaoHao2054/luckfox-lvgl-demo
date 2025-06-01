#ifndef KEYPAD_MANAGER_HPP
#define KEYPAD_MANAGER_HPP

#include "Key.hpp" // Manages Key objects
#include <string>
#include <map>
#include <memory> // For std::unique_ptr
#include <cstdint> // For uint32_t

namespace Bsp {

    // KeyPins namespace and KeyId enum are removed as keys are now dynamically added.
    // KeyState is now defined in Key.h or can be moved to a common types file.

    class KeypadManager {
    public:
        KeypadManager() = default; // Default constructor
        ~KeypadManager();          // Custom destructor to log, map handles Key cleanup

        // Disable copy and move operations
        KeypadManager(const KeypadManager &) = delete;
        KeypadManager &operator=(const KeypadManager &) = delete;
        KeypadManager(KeypadManager &&) = delete;
        KeypadManager &operator=(KeypadManager &&) = delete;

        // Adds and initializes a new key to the keypad
        // Returns true on success, false if key with same name exists or initialization fails
        bool addKey(const std::string& name, int pinNumber, GpioValue activeLogic);

        // Reads the state of a specific key by its name
        KeyState getKeyState(const std::string& name);

        // Checks if a specific key (by name) is currently pressed
        bool isKeyPressed(const std::string& name);

        // Static delay utility function (can be moved to a general utility if needed)
        static void delayMs(uint32_t milliseconds);

    private:
        // Maps a string key name to a unique pointer managing a Key object
        std::map<std::string, std::unique_ptr<Key>> m_keys;
    };

} // namespace Bsp
#endif // KEYPAD_MANAGER_HPP