set(NAME "osd-item-horizon")

add_library(${NAME}
  horizon.c
)

target_include_directories(${NAME}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${NAME}
  PUBLIC
    lvgl
)

unset(NAME)