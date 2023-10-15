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
    ${CMAKE_SOURCE_DIR}/sdk/hi3536dv100/include
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(osd 
  PUBLIC
    lvgl
    m
    osd-item-horizon
    pthread
)


add_subdirectory(lvgl)
add_subdirectory(items)
