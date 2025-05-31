file(GLOB_RECURSE BSP_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/c_keypad/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/c_keypad/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/c_keypad/*.cc"
)


add_library(bsp_custom STATIC
    ${BSP_SOURCES}
)
target_include_directories(bsp_custom PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}" 
    "${CMAKE_CURRENT_SOURCE_DIR}/c_keypad"
    "${CMAKE_CURRENT_SOURCE_DIR}/keypad"
)

message(STATUS "bsp/CMakeLists.txt: Defined bsp_custom library.")