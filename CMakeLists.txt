cmake_minimum_required(VERSION 3.15)
project(LVGLDemo C CXX) # Enable C and CXX

# --- Output Directory Configuration ---
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) # For older CMake compatibility

# --- Compiler Flags from Makefile ---
# Note: Some of these warnings might be GCC-specific.
set(COMMON_WARNINGS
    -Wall -Wshadow -Wundef -Wmissing-prototypes -Wno-discarded-qualifiers -Wextra
    -Wno-unused-function -Wno-error=strict-prototypes -Wpointer-arith -fno-strict-aliasing
    -Wno-error=cpp -Wuninitialized -Wmaybe-uninitialized -Wno-unused-parameter
    -Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess
    -Wno-format-nonliteral -Wno-cast-qual -Wunreachable-code -Wno-switch-default
    -Wreturn-type -Wmultichar -Wformat-security -Wno-ignored-qualifiers
    -Wno-error=pedantic -Wno-sign-compare -Wno-error=missing-prototypes
    -Wdouble-promotion -Wclobbered -Wdeprecated -Wempty-body -Wtype-limits
    -Wshift-negative-value -Wstack-usage=2048 -Wno-unused-value
)
# For C files
set(CMAKE_C_FLAGS_INIT "-O3 -g0 ${COMMON_WARNINGS} -std=c99")
# For C++ files (if any, e.g., from bsp/keypad)
set(CMAKE_CXX_FLAGS_INIT "-O3 -g0 ${COMMON_WARNINGS}") # Add -std=c++11 or newer if needed for C++ code

# --- Add Executable Target ---
add_executable(demo)

# --- Source File Collection ---
# Application sources
file(GLOB_RECURSE APP_SOURCES
    "app/*.c"
    "app/*.cpp" # If you have C++ files in app
)

# BSP sources (Board Support Package)
file(GLOB_RECURSE BSP_SOURCES
    "bsp/c_keypad/*.c"
    "bsp/keypad/*.cpp" # For C++ keypad implementation
)

# LVGL Core Sources
file(GLOB_RECURSE LVGL_CORE_SOURCES
    "lib/lvgl/src/core/*.c"
    "lib/lvgl/src/hal/*.c"
    "lib/lvgl/src/misc/*.c"
    "lib/lvgl/src/draw/*.c"
    "lib/lvgl/src/font/*.c"
)

# LVGL Widgets
file(GLOB_RECURSE LVGL_WIDGETS_SOURCES
    "lib/lvgl/src/widgets/*.c"
)

# LVGL Extra Components (Themes are common)
file(GLOB_RECURSE LVGL_EXTRA_THEMES_SOURCES
    "lib/lvgl/src/extra/themes/*/*.c" # All themes
)
# Add other LVGL 'extra' components if you use them and they are enabled in lv_conf.h
# For example, if you use specific extra widgets:
# file(GLOB_RECURSE LVGL_EXTRA_WIDGETS_CALENDAR "lib/lvgl/src/extra/widgets/calendar/*.c")
# list(APPEND LVGL_EXTRA_SOURCES ${LVGL_EXTRA_WIDGETS_CALENDAR})
# Example for layouts:
# file(GLOB_RECURSE LVGL_EXTRA_LAYOUTS_FLEX "lib/lvgl/src/extra/layouts/flex/*.c")
# list(APPEND LVGL_EXTRA_SOURCES ${LVGL_EXTRA_LAYOUTS_FLEX})
# Example for libs:
# file(GLOB_RECURSE LVGL_EXTRA_LIBS_QRCODE "lib/lvgl/src/extra/libs/qrcode/*.c")
# list(APPEND LVGL_EXTRA_SOURCES ${LVGL_EXTRA_LIBS_QRCODE})

set(LVGL_EXTRA_SOURCES "") # Initialize empty list for extras
# Add specific extra widgets/layouts/libs sources as needed based on lv_conf.h
# This is where you would replicate the logic from lvgl.mk for conditional compilation of extras.
# Example: If LV_USE_CHART is 1 in lv_conf.h
# list(APPEND LVGL_EXTRA_SOURCES "lib/lvgl/src/extra/widgets/chart/lv_chart.c")
# Example: If LV_USE_FS_POSIX is 1
# list(APPEND LVGL_EXTRA_SOURCES "lib/lvgl/src/extra/libs/fsdrv/lv_fs_posix.c")


# LVGL Drivers
# This section is HIGHLY dependent on your `lv_conf.h` and the specific drivers you use.
# The Makefile's `lv_drivers.mk` would have handled this selection.
# Based on your `tree` output, you have `lib/lv_drivers/display` and `lib/lv_drivers/indev`.
# You need to add the specific .c files for your chosen display and input device drivers.
set(LVGL_DRIVER_SOURCES "")

# Example: If you are using a framebuffer device (common on embedded Linux)
# Ensure `LV_USE_FBDEV` (or similar) is 1 in `lv_conf.h`
# and you have `fbdev.c` (or your driver file) in `lib/lv_drivers/display/`
file(GLOB FBDEV_SOURCES "lib/lv_drivers/display/*.c") # This might grab more than just fbdev if other .c files are there
list(APPEND LVGL_DRIVER_SOURCES ${FBDEV_SOURCES})


# Example: If you are using evdev for input
# Ensure `LV_USE_EVDEV` (or similar) is 1 in `lv_conf.h`
# and you have `evdev.c` in `lib/lv_drivers/indev/`
file(GLOB EVDEV_SOURCES "lib/lv_drivers/indev/*.c") # This might grab more than just evdev
list(APPEND LVGL_DRIVER_SOURCES ${EVDEV_SOURCES})

# If you are using GTK for simulation (as suggested by build/obj/lv_drivers/gtkdrv):
# You would enable LV_USE_GTK in lv_conf.h and add:
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
# list(APPEND LVGL_DRIVER_SOURCES "lib/lv_drivers/gtkdrv/lv_gtk_drv.c") # Add other necessary gtkdrv files
# include_directories(${GTK3_INCLUDE_DIRS})
# For cross-compilation to an embedded target, GTK is usually not used.

# --- Aggregate all sources for the target ---
target_sources(demo PRIVATE
    ${APP_SOURCES}
    ${BSP_SOURCES}
    ${LVGL_CORE_SOURCES}
    ${LVGL_WIDGETS_SOURCES}
    ${LVGL_EXTRA_THEMES_SOURCES}
    ${LVGL_EXTRA_SOURCES} # Add other specific extra components here
    ${LVGL_DRIVER_SOURCES}
)

# --- Include Directories ---
# The Makefile had -I$(LVGL_DIR)/ which implies the project root.
# Assuming lv_conf.h might be in the project root or app directory.
target_include_directories(demo PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"        # For lv_conf.h if in root
    "${CMAKE_CURRENT_SOURCE_DIR}/app"    # For app headers and potentially lv_conf.h
    "${CMAKE_CURRENT_SOURCE_DIR}/bsp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/lv_drivers"
)

# --- Link Libraries ---
# From LDFLAGS ?= -lm
target_link_libraries(demo PRIVATE m)

# If using GTK driver, you would link GTK libraries here:
# target_link_libraries(demo PRIVATE ${GTK3_LIBRARIES})

# --- Install Rules (Optional, matches Makefile) ---
# Corresponds to `prefix ?= /usr` and `bindir ?= $(prefix)/bin`
set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Installation prefix") # Default /usr/local
install(TARGETS demo
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" # Matches 'bindir'
)

# --- Print configuration ---
message(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS_INIT} ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS_INIT} ${CMAKE_CXX_FLAGS}")
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")