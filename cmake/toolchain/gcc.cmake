# -- Standard GCC toolchain definition

# - Path to compiler
if (NOT TOOLCHAIN_GCC_PATH)
  if (NOT TARGET_PLATFORM_TOOLCHAIN_GCC_PATH)
    set (TOOLCHAIN_GCC_PATH "/usr/bin")
  else ()
    set (TOOLCHAIN_GCC_PATH ${TARGET_PLATFORM_TOOLCHAIN_GCC_PATH})
  endif ()
endif ()

# - GCC Type
if (NOT TOOLCHAIN_GCC_TYPE)
  if (NOT TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE)
    set (TOOLCHAIN_GCC_TYPE "")
  else ()
    set (TOOLCHAIN_GCC_TYPE ${TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE})
  endif ()
endif ()

# - GCC Version
if (NOT TOOLCHAIN_GCC_VERSION)
  if (NOT TARGET_PLATFORM_TOOLCHAIN_GCC_VERSION)
    set (TOOLCHAIN_GCC_VERSION "")
  else ()
    set (TOOLCHAIN_GCC_VERSION ${TARGET_PLATFORM_TOOLCHAIN_GCC_VERSION})
  endif ()
endif ()

# -----------------------------------------------------
# - Create compiler name
# ----------------------------------------------------
set (CMAKE_C_COMPILER   "")
set (CMAKE_CXX_COMPILER "")

# - Write path
#if (NOT TOOLCHAIN_GCC_PATH STREQUAL "")
#  set (CMAKE_C_COMPILER   "")
#  set (CMAKE_CXX_COMPILER "")
#else ()
  set (CMAKE_C_COMPILER   "${TOOLCHAIN_GCC_PATH}/${CMAKE_C_COMPILER}")
  set (CMAKE_CXX_COMPILER "${TOOLCHAIN_GCC_PATH}/${CMAKE_CXX_COMPILER}")
#endif ()

# - Write type (prefix)
if (NOT TOOLCHAIN_GCC_TYPE STREQUAL "")
  set (CMAKE_C_COMPILER   "${CMAKE_C_COMPILER}${TOOLCHAIN_GCC_TYPE}-")
  set (CMAKE_CXX_COMPILER "${CMAKE_CXX_COMPILER}${TOOLCHAIN_GCC_TYPE}-")
endif ()

# - Store compiler prefix
set (TOOLCHAIN_GCC_PREFIX "${CMAKE_C_COMPILER}" CACHE STRING "" FORCE)

# - Write compiler name
set (CMAKE_C_COMPILER     "${TOOLCHAIN_GCC_PREFIX}gcc"      )
set (CMAKE_CXX_COMPILER   "${TOOLCHAIN_GCC_PREFIX}g++"      )

# - Write version
if (NOT TOOLCHAIN_GCC_VERSION STREQUAL "")
  set (CMAKE_C_COMPILER   "${CMAKE_C_COMPILER}-${TOOLCHAIN_GCC_VERSION}"  )
  set (CMAKE_CXX_COMPILER "${CMAKE_CXX_COMPILER}-${TOOLCHAIN_GCC_VERSION}")
endif ()

# - Store in cache
set (CMAKE_C_COMPILER                       "${CMAKE_C_COMPILER}"                       CACHE STRING "" FORCE)
set (CMAKE_CXX_COMPILER                     "${CMAKE_CXX_COMPILER}"                     CACHE STRING "" FORCE)
set (CMAKE_SIZE                             "${TOOLCHAIN_GCC_PREFIX}size"               CACHE STRING "" FORCE)
set (CMAKE_LINKER                           "${TOOLCHAIN_GCC_PREFIX}ld"                 CACHE STRING "" FORCE)
set (CMAKE_LD                               "${TOOLCHAIN_GCC_PREFIX}ld"                 CACHE STRING "" FORCE)
set (CMAKE_AR                               "${TOOLCHAIN_GCC_PREFIX}ar"                 CACHE STRING "" FORCE)
set (CMAKE_STRIP                            "${TOOLCHAIN_GCC_PREFIX}strip"              CACHE STRING "" FORCE)
set (CMAKE_OBJCOPY                          "${TOOLCHAIN_GCC_PREFIX}objcopy"            CACHE STRING "" FORCE)
set (CMAKE_OBJDUMP                          "${TOOLCHAIN_GCC_PREFIX}objdump"            CACHE STRING "" FORCE)
set (CMAKE_READELF                          "${TOOLCHAIN_GCC_PREFIX}readelf"            CACHE STRING "" FORCE)
set (CMAKE_CPP                              "${TOOLCHAIN_GCC_PREFIX}cpp"                CACHE STRING "" FORCE)

set (CMAKE_RANLIB                           "${TOOLCHAIN_GCC_PREFIX}ranlib"             CACHE STRING "" FORCE)
set (CMAKE_NM                               "${TOOLCHAIN_GCC_PREFIX}nm"                 CACHE STRING "" FORCE)

set (TARGET_PLATFORM_TOOLCHAIN_GCC_PATH     "${TARGET_PLATFORM_TOOLCHAIN_GCC_PATH}"     CACHE STRING "" FORCE)
set (TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE     "${TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE}"     CACHE STRING "" FORCE)
set (TARGET_PLATFORM_TOOLCHAIN_GCC_VERSION  "${TARGET_PLATFORM_TOOLCHAIN_GCC_VERSION}"  CACHE STRING "" FORCE)


set (TOOLCHAIN_TARGET                       "${TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE}"     CACHE STRING "" FORCE)
set (TOOLCHAIN_VERSION                      "${TARGET_PLATFORM_TOOLCHAIN_GCC_VERSION}"  CACHE STRING "" FORCE)

set (TOOLCHAIN_LINKER_OPTIONS_SHARED_BASE         "-Wl,--unresolved-symbols=report-all" CACHE STRING "" FORCE)
set (TOOLCHAIN_LINKER_OPTIONS_STATIC_BASE         ""                                    CACHE STRING "" FORCE)
set (TOOLCHAIN_COMPILER_OPTIONS_DEBUG_BASE        "-ggdb"                               CACHE STRING "" FORCE)
set (TOOLCHAIN_COMPILER_OPTIONS_RELEASE_BASE      "-DNDEBUG"                            CACHE STRING "" FORCE)

set (TOOLCHAIN_COMPILER_DEFINITIONS_RELEASE_BASE  "NDEBUG"                              CACHE STRING "" FORCE)