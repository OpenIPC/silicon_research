cmake_minimum_required(VERSION 3.18...3.21)

# - Declare best project ever
project(SDWAN
  LANGUAGES C CXX
)

# - Enable compile commands export for 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# - Disable prefixes
set(CMAKE_SHARED_LIBRARY_PREFIX "" CACHE STRING "" FORCE)
set(CMAKE_STATIC_LIBRARY_PREFIX "" CACHE STRING "" FORCE)

# - Include framework
include("${CMAKE_SOURCE_DIR}/cmake/framework.cmake")

# - Detect platform
if (${TARGET_CPU_MODEL} STREQUAL "GK7205v300") 
  add_subdirectory(gk7205v300)
  
elseif(${TARGET_CPU_MODEL} STREQUAL "HI3536Dv100")
  add_subdirectory(hi3536dv100)
  
endif ()

# - Create compile command symlink in workspace directory
execute_process(
  COMMAND 
    ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
)