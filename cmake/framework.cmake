include(ProcessorCount)
#
# Options:
#   TARGET_PLATFORM   = native, intel, amd, ...
#   TARGET_BUILD      = Debug, Release
#   TARGET_DEBUG_INFO = (NO) / YES
#
# Defined:
#   TARGET_CPU_VENDOR = intel / amd
#   TARGET_CPU_MODEL  = coffeelake
#   TARGET_CPU_ARCH   = x86_64, MIPSEL, etc

include_guard(GLOBAL)
include ("${CMAKE_CURRENT_LIST_DIR}/external_project.cmake")
include ("${CMAKE_CURRENT_LIST_DIR}/set_option.cmake")

# - Set framework root paths
get_filename_component(DATAFLOW_FRAMEWORK_DIR "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)

# - Load build type
if (NOT TARGET_BUILD)
  message (FATAL_ERROR "TARGET_BUILD not set")
else ()
  set (TARGET_BUILD     "${TARGET_BUILD}" CACHE STRING "" FORCE)
  set (CMAKE_BUILD_TYPE "${TARGET_BUILD}" CACHE STRING "" FORCE)
endif ()

# - Load platform
if (NOT TARGET_PLATFORM)
  message (FATAL_ERROR "TARGET_PLATFORM not set")
else ()
  # - Find platform configuration in root directory
  if (EXISTS "${CMAKE_SOURCE_DIR}/cmake/platform/${TARGET_PLATFORM}.cmake")
    include ("${CMAKE_SOURCE_DIR}/cmake/platform/${TARGET_PLATFORM}.cmake")
  else ()
    # - Check is platform configuration exists
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/platform/${TARGET_PLATFORM}.cmake")
      include ("${CMAKE_CURRENT_LIST_DIR}/platform/${TARGET_PLATFORM}.cmake")
    else ()
      message (FATAL_ERROR "Platform [${TARGET_PLATFORM}] not supported")
    endif ()
  endif ()
endif ()

# - Check platform
if (NOT TARGET_CPU_VENDOR)
  message (FATAL_ERROR "Unknown CPU vendor")
endif ()
if (NOT TARGET_CPU_MODEL)
  message (FATAL_ERROR "Unknown CPU model")
endif ()
if (NOT TARGET_CPU_ARCH)
  message (FATAL_ERROR "Unknown CPU architecture")
endif ()

# - Store target information
set (TARGET_CPU_ARCH    ${TARGET_CPU_ARCH}    CACHE STRING "" FORCE)
set (TARGET_CPU_MODEL   ${TARGET_CPU_MODEL}   CACHE STRING "" FORCE)
set (TARGET_CPU_VENDOR  ${TARGET_CPU_VENDOR}  CACHE STRING "" FORCE)
set (TARGET_PLATFORM_TOOLCHAIN  ${TARGET_PLATFORM_TOOLCHAIN}  CACHE STRING "" FORCE)

# - Load toolchain
if (NOT TOOLCHAIN)
  # - Check is toolchain defined in platform
  if (NOT TARGET_PLATFORM_TOOLCHAIN)
    # - Use default toolchain
    set (TOOLCHAIN "gcc"  CACHE STRING "" FORCE)
  else ()
    # - Use toolchain from platform
    set (TOOLCHAIN ${TARGET_PLATFORM_TOOLCHAIN} CACHE STRING "" FORCE)
  endif ()
endif ()

if (EXISTS "${CMAKE_SOURCE_DIR}/toolchain/${TOOLCHAIN}.cmake")
  include ("${CMAKE_SOURCE_DIR}/toolchain/${TOOLCHAIN}.cmake")
else ()
  if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/toolchain/${TOOLCHAIN}.cmake")
    include ("${CMAKE_CURRENT_LIST_DIR}/toolchain/${TOOLCHAIN}.cmake")
  else ()
    message (FATAL_ERROR "Toolchain [${TOOLCHAIN}] definition not found")
  endif ()
endif ()

# - Store toolchain information
set (TOOLCHAIN_CPU_OPTIONS        "${TOOLCHAIN_CPU_OPTIONS}"      CACHE STRING "Extra options for toolchain compiler"     FORCE)
set (TOOLCHAIN_LINKER_OPTIONS     "${TOOLCHAIN_LINKER_OPTIONS}"   CACHE STRING "Extra options for toolchain linker"       FORCE)
set (TOOLCHAIN_CPU_C_OPTIONS      "${TOOLCHAIN_CPU_C_OPTIONS}"    CACHE STRING "Extra options for toolchain C compiler"   FORCE)
set (TOOLCHAIN_CPU_CXX_OPTIONS    "${TOOLCHAIN_CPU_CXX_OPTIONS}"  CACHE STRING "Extra options for toolchain C++ compiler" FORCE)

set (TOOLCHAIN_COMPILER_OPTIONS_DEBUG   ${TOOLCHAIN_COMPILER_OPTIONS_DEBUG}   CACHE STRING "Extra options for Debug build")
set (TOOLCHAIN_COMPILER_OPTIONS_RELEASE ${TOOLCHAIN_COMPILER_OPTIONS_RELEASE} CACHE STRING "Extra options for Release build")

# - Setup toolchain build threads count
if (NOT TOOLCHAIN_BUILD_THREAD_COUNT)
  # - Detect processors count
  ProcessorCount(TOOLCHAIN_BUILD_THREAD_COUNT)
  if (TOOLCHAIN_BUILD_THREAD_COUNT EQUAL 0)
    # - By default use 1 thread
    set(TOOLCHAIN_BUILD_THREAD_COUNT 1)
  endif()

endif ()
set(TOOLCHAIN_BUILD_THREAD_COUNT ${TOOLCHAIN_BUILD_THREAD_COUNT} CACHE STRING "Build thread count" FORCE)

# - Set output binary dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin"          CACHE STRING "Binaries"         FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin/modules"  CACHE STRING "Modules"          FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib"          CACHE STRING "Static libraries" FORCE)
#set(CMAKE_HOST_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/host"  CACHE STRING "Host output" FORCE)

# --- Generic compile configuration
set (CMAKE_C_FLAGS    "${TOOLCHAIN_CPU_OPTIONS} ${TOOLCHAIN_CPU_C_OPTIONS}"   CACHE STRING "" FORCE)
set (CMAKE_CXX_FLAGS  "${TOOLCHAIN_CPU_OPTIONS} ${TOOLCHAIN_CPU_CXX_OPTIONS}" CACHE STRING "" FORCE)

# - Force reporting of unresolved symbols in shared libraries
set (CMAKE_SHARED_LINKER_FLAGS "${TOOLCHAIN_LINKER_OPTIONS_SHARED_BASE} ${TOOLCHAIN_LINKER_OPTIONS}"  CACHE STRING "" FORCE)
set (CMAKE_STATIC_LINKER_FLAGS "${TOOLCHAIN_LINKER_OPTIONS_STATIC_BASE}"                              CACHE STRING "" FORCE)

# - Force debug information in debug mode
if (TARGET_BUILD STREQUAL "Debug")
  set (TARGET_DEBUG_INFO  YES)
endif ()

# - Add debug information
if (TARGET_DEBUG_INFO)
  set (CMAKE_C_FLAGS    "${CMAKE_C_FLAGS} ${TOOLCHAIN_COMPILER_OPTIONS_DEBUG_BASE}"   CACHE STRING "" FORCE)
  set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${TOOLCHAIN_COMPILER_OPTIONS_DEBUG_BASE}" CACHE STRING "" FORCE)
endif ()

# - Disable automatic RPATH generation
set (CMAKE_SKIP_RPATH TRUE)

# ---------------------------------------
# --- Debug configuration
# ---------------------------------------
set (CMAKE_C_FLAGS_DEBUG    "${TOOLCHAIN_COMPILER_OPTIONS_DEBUG_BASE} ${TOOLCHAIN_COMPILER_OPTIONS_DEBUG}"  CACHE STRING "" FORCE)
set (CMAKE_CXX_FLAGS_DEBUG  "${TOOLCHAIN_COMPILER_OPTIONS_DEBUG_BASE} ${TOOLCHAIN_COMPILER_OPTIONS_DEBUG}"  CACHE STRING "" FORCE)

# ---------------------------------------
# --- Release configuration
# ---------------------------------------
set (CMAKE_C_FLAGS_RELEASE        "${TOOLCHAIN_COMPILER_OPTIONS_RELEASE_BASE} ${TOOLCHAIN_COMPILER_OPTIONS_RELEASE}"  CACHE STRING "" FORCE)
set (CMAKE_CXX_FLAGS_RELEASE      "${TOOLCHAIN_COMPILER_OPTIONS_RELEASE_BASE} ${TOOLCHAIN_COMPILER_OPTIONS_RELEASE}"  CACHE STRING "" FORCE)
set (COMPILE_DEFINITIONS_RELEASE  "${TOOLCHAIN_COMPILER_DEFINITIONS_RELEASE_BASE};${COMPILE_DEFINITIONS_RELEASE}"     CACHE STRING "" FORCE)

# Make possible to pass paths to program code.
#add_definitions(-DFRAMEWORK_OUTPUT_BINARTY_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
#add_definitions(-DFRAMEWORK_OUTPUT_LIBRARY_DIR="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
#add_definitions(-DFRAMEWORK_OUTPUT_ARCHIVE_DIR="${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

# - Disable prefixes
set (CMAKE_SHARED_LIBRARY_PREFIX "")
set (CMAKE_STATIC_LIBRARY_PREFIX "")
set (CMAKE_SHARED_LIBRARY_PREFIX "${CMAKE_SHARED_LIBRARY_PREFIX}" CACHE STRING "" FORCE)
set (CMAKE_STATIC_LIBRARY_PREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}" CACHE STRING "" FORCE)


message ("----------------------------------------"                 )
message ("---        DataFlow Framework        ---"                 )
message ("----------------------------------------"                 )
message ("  Build type           : ${TARGET_BUILD}"                 )
message ("  Platform             : ${TARGET_PLATFORM}"              )
message ("  CPU Vendor           : ${TARGET_CPU_VENDOR}"            )
message ("  CPU Model            : ${TARGET_CPU_MODEL}"             )
message ("  CPU Arch             : ${TARGET_CPU_ARCH}"              )
message ("  Compiler (C)         : ${CMAKE_C_COMPILER}"             )
message ("  Compiler (C++)       : ${CMAKE_CXX_COMPILER}"           )
message ("  Linker               : ${CMAKE_LINKER}"                 )
message ("  Strip tool           : ${CMAKE_STRIP}"                  )
message ("  Archiving tool       : ${CMAKE_AR}"                     )
message ("  Object copy tool     : ${CMAKE_OBJCOPY}"                )
message ("  Object dump tool     : ${CMAKE_OBJDUMP}"                )
message ("  Compiler Flags (C)   : ${CMAKE_C_FLAGS}"                )
message ("  Compiler Flags (C++) : ${CMAKE_CXX_FLAGS}"              )
message ("  Shared Linker flags  : ${CMAKE_SHARED_LINKER_FLAGS}"    )
message ("  Static Linker flags  : ${CMAKE_STATIC_LINKER_FLAGS}"    )
message ("  CMAKE_BINARY_DIR     : ${CMAKE_BINARY_DIR}"             )
message ("  Build thread count   : ${TOOLCHAIN_BUILD_THREAD_COUNT}" )
message ("----------------------------------------"                 )

function (DF_AddTargetSources target_name )
  file(GLOB _sources
    CONFIGURE_DEPENDS
      "*.cc"
      "*.h"
  )
  target_sources(${target_name} PRIVATE ${_sources})
  message(${_sources})
  unset(_sources)
endfunction ()
