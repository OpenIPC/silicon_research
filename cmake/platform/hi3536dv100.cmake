# - Platform CPU configuration
set_option (TARGET_CPU_VENDOR  "Hisilicon"   )
set_option (TARGET_CPU_MODEL   "HI3536Dv100" )
set_option (TARGET_CPU_ARCH    "ARM"         )

set_option (TOOLCHAIN_CPU_OPTIONS    "-fPIC -mcpu=cortex-a7 -mfloat-abi=softfp -mfpu=neon-vfpv4 -mno-unaligned-access -fno-aggressive-loop-optimizations -ffunction-sections -fstack-protector ")
set_option (TOOLCHAIN_LINKER_OPTIONS "-Wl,--gc-sections -Wl,-fuse-ld=gold")

set_option (TOOLCHAIN_COMPILER_OPTIONS_DEBUG   "-O0")
set_option (TOOLCHAIN_COMPILER_OPTIONS_RELEASE "-Os -s")

set_option (TARGET_PLATFORM_TOOLCHAIN          "gcc")
set_option (TARGET_PLATFORM_TOOLCHAIN_GCC_TYPE "arm-openipc-linux-gnueabi")
