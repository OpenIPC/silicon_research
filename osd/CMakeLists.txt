cmake_minimum_required(VERSION 3.12.4)
project(osd)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -s -Wno-address-of-packed-member")

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}
)

FILE(GLOB_RECURSE UI_Sources CONFIGURE_DEPENDS ui/*.c)

add_executable(osd
  osd.c
  display_fbdev.c
  queue.c
  mavlink_parser.c
  util.c
  ${UI_Sources}
)

target_include_directories(osd
  PUBLIC
    ${LVGL_ROOT_DIR}
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/ui
    ${CMAKE_CURRENT_LIST_DIR}/components
    ${CMAKE_SOURCE_DIR}/../sdk/hi3536dv100/include
    ${CMAKE_SOURCE_DIR}/../vdec
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(osd
  PUBLIC
    lvgl
    m
    osd-item-horizon
    pthread
)

add_subdirectory(items)

include(FetchContent)
FetchContent_Declare(
  lvgl
  URL https://github.com/lvgl/lvgl/archive/refs/tags/v8.3.10.tar.gz
)

FetchContent_MakeAvailable(lvgl)
target_include_directories(lvgl PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
