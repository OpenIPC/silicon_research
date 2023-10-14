#!/bin/bash
LINK="https://github.com/openipc/firmware/releases/download/latest"

if [ "$1" = "vdec" ]; then
	SDK=hi3536dv100
	ABI=glibc
	DL=cortex_a7-gcc12-glibc-4_9
elif [ "$1" = "venc" ]; then
	SDK=gk7205v300
	ABI=musl
	DL=cortex_a7_thumb2-gcc12-musl-4_9
else
	echo "Usage: $0 [venc|vdec]"
	exit 1
fi

if [ ! -e $PWD/toolchain/$DL ]; then
	wget -c -nv $LINK/$DL.tgz -P $PWD
	mkdir -p $PWD/toolchain/$DL
	tar -xf $PWD/$DL.tgz -C $PWD/toolchain/$DL --strip-components=1 || exit 1
	rm -f $PWD/$DL.tgz
fi

rm -rf build
cmake -Bbuild -DTARGET_BUILD=Release -DTARGET_PLATFORM=$SDK -DTARGET_PLATFORM_EABI=$ABI \
	-DTARGET_PLATFORM_TOOLCHAIN_GCC_PATH=$PWD/toolchain/$DL/bin
cmake --build build --parallel 8
