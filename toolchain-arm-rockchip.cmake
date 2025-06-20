# CMake toolchain file for arm-rockchip830-linux-uclibcgnueabihf

# Target system
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER_TARGET "arm-linux-uclibcgnueabihf")
set(CMAKE_CXX_COMPILER_TARGET "arm-linux-uclibcgnueabihf")

# Cross compiler paths
set(CROSS_COMPILER_PREFIX /home/hao/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-)

set(CMAKE_C_COMPILER ${CROSS_COMPILER_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILER_PREFIX}g++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the sysroot if necessary (often needed for includes and libraries)
# Example: set(CMAKE_SYSROOT /home/hao/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/arm-rockchip830-linux-uclibcgnueabihf/sysroot)
# If your toolchain handles sysroot implicitly, you might not need this.
# Adjust the path above if your sysroot is located elsewhere within the toolchain directory.
set(CMAKE_SYSROOT /home/hao/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/arm-rockchip830-linux-uclibcgnueabihf/sysroot)

# Configure find commands
# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)