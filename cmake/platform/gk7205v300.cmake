# - Platform CPU configuration
set (TARGET_CPU_VENDOR  "Goke"        )
set (TARGET_CPU_MODEL   "GK7205v300"  )
set (TARGET_CPU_ARCH    "ARM"         )

set (TOOLCHAIN_CPU_OPTIONS    "-fPIC -fno-plt -ffunction-sections -fdata-sections -mno-unaligned-access")
set (TOOLCHAIN_LINKER_OPTIONS "-Wl,--gc-sections -Wl,-fuse-ld=gold")

set (TOOLCHAIN_COMPILER_OPTIONS_DEBUG   "-O0")
set (TOOLCHAIN_COMPILER_OPTIONS_RELEASE "-Os -s")

set (TARGET_PLATFORM_TOOLCHAIN          "gcc")
#set (TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE "arm-openipc-linux-gnueabi")
set (TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE "arm-openipc-linux-musleabi")
#set (TARGET_PLATFORM_TOOLCHAIN_GCC_PATH "/system/hdd/projects/openipc/output/host/bin")

if (TARGET_PLATFORM_EABI STREQUAL "musl")
  set (TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE "arm-openipc-linux-musleabi")
  set (TARGET_PLATFORM_TOOLCHAIN_GCC_PATH "/system/dev/arm-openipc-linux-musleabi/bin")
elseif (TARGET_PLATFORM_EABI STREQUAL "glibc")
  set (TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE "arm-openipc-linux-gnueabi")
  set (TARGET_PLATFORM_TOOLCHAIN_GCC_PATH "/system/hdd/projects/fpv_camera/openipc/output/host/bin")
  
endif()