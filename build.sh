#!/bin/bash
LINK="https://github.com/openipc/firmware/releases/download/latest"

if [ "$1" = "vdec" ] || [ "$1" = "venc-goke" ] || [ "$1" = "venc-hisi" ]; then
	DL=cortex_a7_thumb2-gcc12-musl-4_9
else
	echo "Usage: $0 [vdec|venc-goke|venc-hisi]"
	exit 1
fi

if [ ! -e toolchain/$DL ]; then
	wget -c -nv --show-progress $LINK/$DL.tgz -P $PWD
	mkdir -p toolchain/$DL
	tar -xf $DL.tgz -C toolchain/$DL --strip-components=1 || exit 1
	rm -f $DL.tgz
fi

if [ ! -e firmware ]; then
	git clone https://github.com/openipc/firmware --depth=1
fi

if [ "$1" = "venc-goke" ] || [ "$1" = "venc-hisi" ]; then
	make -B CC=toolchain/$DL/bin/arm-linux-gcc TARGET=$1
	exit 0
fi

if [ ! -e hi3536dv100/osd/lvgl ]; then
	git clone https://github.com/lvgl/lvgl hi3536dv100/osd/lvgl -b release/v8.3 --depth=1
fi

rm -rf build
cmake -Bbuild -DTARGET_BUILD=Release -DTARGET_PLATFORM=hi3536dv100 \
	-DTARGET_PLATFORM_TOOLCHAIN_GCC_PATH=$PWD/toolchain/$DL/bin
cmake --build build --parallel 8
