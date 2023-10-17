#!/bin/bash
SRC=gk7205v300/venc
INC=sdk/hi3516ev300
SNS=sdk/hi3516ev300/sensor
LIB="-lisp -lmpi -ldnvqe -lupvqe -l_hiae -l_hiawb -l_hildci -l_hidrc -l_hidehaze -lVoiceEngine -lsecurec"

echo Building venc-hisi
toolchain/$1/bin/arm-linux-gcc -o venc-hisi -Os -s -I $INC/include -L $INC/lib $LIB \
	$SRC/main.c $SRC/common.c $SRC/compat.c $SRC/isp_profiles.c $SRC/mipi_profiles.c $SRC/vi_profiles.c \
	$SNS/imx307_2l_cmos.c $SNS/imx307_2l_sensor_ctl.c $SNS/imx335_cmos.c $SNS/imx335_sensor_ctl.c \
	-D TARGET_PLATFORM_EABI_MUSL
