# - Platform CPU configuration
set (TARGET_CPU_VENDOR  "Hisilicon"   )
set (TARGET_CPU_MODEL   "HI3536Dv100" )
set (TARGET_CPU_ARCH    "ARM"         )

set (TOOLCHAIN_CPU_OPTIONS    "-fPIC -mcpu=cortex-a7 -mfloat-abi=softfp -mfpu=neon-vfpv4 -mno-unaligned-access -fno-aggressive-loop-optimizations -ffunction-sections -fstack-protector ")
set (TOOLCHAIN_LINKER_OPTIONS "-Wl,--gc-sections -Wl,-fuse-ld=gold")

set (TOOLCHAIN_COMPILER_OPTIONS_DEBUG   "-O0")
set (TOOLCHAIN_COMPILER_OPTIONS_RELEASE "-Os -s")

set (TARGET_PLATFORM_TOOLCHAIN          "gcc")
set (TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE "arm-openipc-linux-gnueabi")
#set (TARGET_PLATFORM_TOOLCHAIN_GCC_PATH "/system/hdd/projects/openipc-nvr/output/host/bin")
