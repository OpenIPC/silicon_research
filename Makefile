ifeq ($(TARGET),venc-hisi)
	SDK := hi3516ev300
	LIB := -lisp -lmpi -ldnvqe -lupvqe -l_hiae -l_hiawb \
		-l_hildci -l_hidrc -l_hidehaze -lVoiceEngine -lsecurec
	DRV ?= firmware/general/package/hisilicon-osdrv-hi3516ev200/files/lib
else ifeq ($(TARGET),venc-goke)
	SDK := gk7205v300
	LIB := -lhi_mpi -lhi_isp -lhi_ae -lhi_awb -lgk_api -lgk_isp -lgk_ae -lgk_awb \
		-ldehaze -ldrc -lldci -lir_auto -ldnvqe -lupvqe -lvoice_engine -lsecurec
	DRV ?= firmware/general/package/goke-osdrv-gk7205v200/files/lib
endif

VENC := venc/main.c venc/common.c venc/compat.c \
	venc/isp_profiles.c venc/mipi_profiles.c venc/vi_profiles.c
SENSOR := sdk/$(SDK)/sensor/imx307_2l_cmos.c sdk/$(SDK)/sensor/imx307_2l_sensor_ctl.c \
	sdk/$(SDK)/sensor/imx335_cmos.c sdk/$(SDK)/sensor/imx335_sensor_ctl.c
FILE ?= $(TARGET)

$(TARGET):
	@mkdir -p output
	$(CC) $(VENC) $(SENSOR) -I sdk/$(SDK)/include -L $(DRV) $(LIB) -Os -s -o output/$(FILE)
