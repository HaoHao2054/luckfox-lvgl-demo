file(GLOB_RECURSE IMAGES_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
)


add_library(images STATIC
    ${IMAGES_SOURCES}
)
target_include_directories(images PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_SOURCE_DIR}/lib/lv_port_linux"
)

message(STATUS "images/CMakeLists.txt: Defined images library.")