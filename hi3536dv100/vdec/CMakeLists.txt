set(NAME vdec)

add_executable(${NAME}
  main.c
  vo.c
  udp_stream.c
)

target_include_directories(${NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/sdk/hi3536dv100/include
)

target_link_libraries(${NAME}
  mpi
  upvqe
  dnvqe
  VoiceEngine
  jpeg
  hdmi
  pthread
  dl
)

target_link_directories(${NAME}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/sdk/hi3536dv100/lib
)

set_target_properties (${NAME} PROPERTIES
  LINK_FLAGS
    -Wl,-rpath,$ORIGIN
)
