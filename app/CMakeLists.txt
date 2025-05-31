file(GLOB_RECURSE APP_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(app_custom STATIC
    ${APP_SOURCES}
)

target_include_directories(app_custom PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/" 
    "${CMAKE_SOURCE_DIR}/lib/lv_port_linux"
    "${CMAKE_SOURCE_DIR}/bsp"
)

message(STATUS "app/CMakeLists.txt: Defined lvgl_core library.")

