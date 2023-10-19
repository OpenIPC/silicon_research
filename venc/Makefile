VENC := main.c common.c compat.c isp_profiles.c mipi_profiles.c vi_profiles.c
SENSOR = $(SDK)/sensor/imx307_2l_cmos.c $(SDK)/sensor/imx307_2l_sensor_ctl.c \
	$(SDK)/sensor/imx335_cmos.c $(SDK)/sensor/imx335_sensor_ctl.c
BUILD = $(CC) $(VENC) $(SENSOR) -I $(SDK)/include -L $(DRV) $(LIB) -Os -s -o venc

venc-goke:
	$(eval SDK = ../sdk/gk7205v300)
	$(eval LIB = -lhi_mpi -lhi_isp -lhi_ae -lhi_awb -lgk_api -lgk_isp -lgk_ae -lgk_awb \
		-ldehaze -ldrc -lldci -lir_auto -ldnvqe -lupvqe -lvoice_engine -lsecurec)
	$(BUILD)

venc-hisi:
	$(eval SDK = ../sdk/hi3516ev300)
	$(eval LIB = -lisp -lmpi -ldnvqe -lupvqe -l_hiae -l_hiawb \
		-l_hildci -l_hidrc -l_hidehaze -lVoiceEngine -lsecurec)
	$(BUILD)
