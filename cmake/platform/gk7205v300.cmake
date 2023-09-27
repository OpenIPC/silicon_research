# - Platform CPU configuration
set_option (TARGET_CPU_VENDOR  "Goke"        )
set_option (TARGET_CPU_MODEL   "GK7205v300"  )
set_option (TARGET_CPU_ARCH    "ARM"         )

set_option (TOOLCHAIN_CPU_OPTIONS    "-fPIC -fno-plt -ffunction-sections -fdata-sections -mno-unaligned-access")
set_option (TOOLCHAIN_LINKER_OPTIONS "-Wl,--gc-sections -Wl,-fuse-ld=gold")

set_option (TOOLCHAIN_COMPILER_OPTIONS_DEBUG    "-O0")
set_option (TOOLCHAIN_COMPILER_OPTIONS_RELEASE  "-Os -s")

set_option (TARGET_PLATFORM_TOOLCHAIN           "gcc")

if (TARGET_PLATFORM_EABI STREQUAL "musl")
  set_option (TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE "arm-openipc-linux-musleabi")
  set_option (TARGET_PLATFORM_TOOLCHAIN_GCC_PATH "/system/dev/arm-openipc-linux-musleabi/bin")
  
elseif (TARGET_PLATFORM_EABI STREQUAL "glibc")
  set_option (TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE "arm-openipc-linux-gnueabi")
  set_option (TARGET_PLATFORM_TOOLCHAIN_GCC_PATH "/system/hdd/projects/fpv_camera/openipc/output/host/bin")
  
endif()