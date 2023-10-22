#!/bin/bash
DL="https://github.com/openipc/firmware/releases/download/latest"

if [ "$1" = "vdec" ] || [ "$1" = "osd" ]; then
	CC=cortex_a7-gcc12-glibc-4_9
else
	CC=cortex_a7_thumb2-gcc12-musl-4_9
fi

GCC=$PWD/toolchain/$CC/bin/arm-linux-gcc

if [ ! -e toolchain/$CC ]; then
	wget -c -nv --show-progress $DL/$CC.tgz -P $PWD
	mkdir -p toolchain/$CC
	tar -xf $CC.tgz -C toolchain/$CC --strip-components=1 || exit 1
	rm -f $CC.tgz
fi

if [ ! -e firmware ]; then
	git clone https://github.com/openipc/firmware --depth=1
fi

if [ "$1" = "vdec" ]; then
	DRV=$PWD/firmware/general/package/hisilicon-osdrv-hi3536dv100/files/lib
	make -C vdec -B CC=$GCC DRV=$DRV
elif [ "$1" = "venc-goke" ]; then
	DRV=$PWD/firmware/general/package/goke-osdrv-gk7205v200/files/lib
	make -C venc -B CC=$GCC DRV=$DRV $1
elif [ "$1" = "venc-hisi" ]; then
	DRV=$PWD/firmware/general/package/hisilicon-osdrv-hi3516ev200/files/lib
	make -C venc -B CC=$GCC DRV=$DRV $1
elif [ "$1" = "osd" ]; then
	cd osd
	cmake -Bbuild -DCMAKE_C_COMPILER=$GCC -DCMAKE_BUILD_TYPE=Release
	cmake --build build --parallel 8
else
	echo "Usage: $0 [vdec|venc-goke|venc-hisi|osd]"
	exit 1
fi
