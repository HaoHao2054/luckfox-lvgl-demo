# --- Librarys ---
# Sources for LVGL core, widgets, themes, and extras
file(GLOB_RECURSE LIB_CUSTOM_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(lib_custom STATIC
    ${LIB_CUSTOM_SOURCES}
)

target_include_directories(lib_custom PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/lv_port_linux" 
    "${CMAKE_CURRENT_SOURCE_DIR}/lv_port_linux/lvgl"    
    "${CMAKE_CURRENT_SOURCE_DIR}/lv_port_linux/lv_drivers" 

)
message(STATUS "lib/CMakeLists.txt: Defined lvgl_core library.")