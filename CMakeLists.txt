cmake_minimum_required(VERSION 3.15)
project(LVGLDemo C CXX) # Enable C and CXX

# --- Output Directory Configuration ---
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) # For older CMake compatibility

# --- Other Tools Configuration  ---
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)             # Enable compile command to ease indexing with e.g. clangd
add_compile_options(-fdiagnostics-color=always)     # Ninja 

# --- Toolchain Configuration ---
include(${CMAKE_SOURCE_DIR}/toolchain-arm-rockchip.cmake)
# include(${CMAKE_SOURCE_DIR}/toolchain-arm-linux-gnueabihf.cmake)

# --- Compiler Flags (Project-Wide) ---
add_compile_options(
    -O3 -g0
    # C Specific
    $<$<COMPILE_LANGUAGE:C>:-std=c99>
    # C++ Specific
    # $<$<COMPILE_LANGUAGE:CXX>:-std=c++17> # 例如

    # Common Warnings
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


# --- Include Subdirectories ---
# Subdirectories will define their own libraries/executables and sources.
add_subdirectory(lib)
add_subdirectory(bsp)
add_subdirectory(app)
add_subdirectory(images)


# --- Define the project executable ---
add_executable(${PROJECT_NAME}
    main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_SOURCE_DIR}"      
    "${CMAKE_SOURCE_DIR}/lib/lv_port_linux"
    "${CMAKE_SOURCE_DIR}/app"
    "${CMAKE_SOURCE_DIR}/bsp"
    "${CMAKE_SOURCE_DIR}/bsp/c_keypad"
)

# Link against the libraries built in other directories
target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_BINARY_DIR}/lib"  # Ensure the libraries are found in the build directory
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    # lib_custom     # From lib/CMakeLists.txt (if created)
    # bsp_custom     # From bsp/CMakeLists.txt
    # app_custom     # From app/CMakeLists.txt

    app_custom       # 这个库引用了 lib_custom 和 bsp_custom 中的函数
    lib_custom       # 定义了 app_custom 所需的函数
    bsp_custom       # 定义了 app_custom 所需的函数
    images         # From images/CMakeLists.txt
)

# # Install rules for the demo executable
# install(TARGETS ${PROJECT_NAME}
#     RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
# )

message(STATUS "CMakeLists.txt: Defined ${PROJECT_NAME} executable.")


# --- Install Rules for the main executable (defined in app/CMakeLists.txt) ---
# The executable target 'demo' will be defined in app/CMakeLists.txt
# We can set install rules here if we know the target name, or set it in app/CMakeLists.txt
set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Installation prefix")
# The actual install command for the 'demo' target should be in app/CMakeLists.txt
# or ensure the 'demo' target is globally visible if installing from here.

# --- Print configuration (optional, can be useful for debugging) ---
message(STATUS "Top-Level CMake: CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "Top-Level CMake: CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "Top-Level CMake: CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "Top-Level CMake: CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "Top-Level CMake: Output directory for executables: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")