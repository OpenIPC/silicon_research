include_directories(
  ${CMAKE_CURRENT_LIST_DIR}
)

add_executable(osd
  osd.c
  display_fbdev.c
  fonts/aero_14.c
)

target_include_directories(osd
  PUBLIC
    ${LVGL_ROOT_DIR}
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/sdk/hi3536dv100/include
)

target_link_libraries(osd 
  PUBLIC
    lvgl
    m
    osd-item-horizon
)


add_subdirectory(lvgl)
add_subdirectory(items)