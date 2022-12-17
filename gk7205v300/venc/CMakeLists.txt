set(NAME venc)

add_executable(${NAME}
  mipi_profiles.c
  isp_profiles.c
  vi_profiles.c
  
  main.c
  compat.c
  config.c
  common.c
)

target_include_directories(${NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC
    ${CMAKE_SOURCE_DIR}/sdk/gk7205v300/include
    ${CMAKE_SOURCE_DIR}/sdk/gk7205v300/gmp/include
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
  
  sns_imx335
  
  isp
  ae
  awb
  dehaze
  ldci
  upvqe
  dnvqe
  drc
  ir_auto
  
  voice_engine
)

target_link_directories(${NAME}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/sdk/gk7205v300/lib
    ${CMAKE_SOURCE_DIR}/sdk/gk7205v300/gmp/lib_nolog/static
    ${CMAKE_SOURCE_DIR}/sdk/gk7205v300/gmp/lib_nolog/static/base
)

set_target_properties (${NAME} PROPERTIES
  LINK_FLAGS
    -Wl,-rpath,$ORIGIN
)
