set(NAME venc)

add_executable(${NAME}
  mipi_profiles.c
  isp_profiles.c
  vi_profiles.c
  
  main.c
  compat.c
  common.c
  
  sensor/imx307_2l_cmos.c
  sensor/imx307_2l_sensor_ctl.c
  
  sensor/imx335_cmos.c
  sensor/imx335_sensor_ctl.c
)

target_include_directories(${NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/sdk/gk7205v300/include
    ${CMAKE_SOURCE_DIR}/sdk/hi3516ev300/include
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${NAME}
  hi_mpi
  hi_isp
  hi_awb
  hi_ae
  gk_isp
  gk_api
  gk_ae
  gk_awb

  dehaze
  drc
  ir_auto
  ldci
  dnvqe
  upvqe
  voice_engine
  securec
)

target_link_directories(${NAME}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/firmware/general/package/goke-osdrv-gk7205v200/files/lib
)

set_target_properties (${NAME} PROPERTIES
  LINK_FLAGS
    -Wl,-rpath,$ORIGIN
)

if (TARGET_PLATFORM_EABI STREQUAL "musl")
  target_compile_definitions(${NAME}
    PUBLIC
      TARGET_PLATFORM_EABI_MUSL
  )
endif()
