#include "main.h"

int VO_init(VO_DEV device_id, VO_INTF_TYPE_E interface_type,
	VO_INTF_SYNC_E interface_mode, uint32_t framerate, uint32_t background_color) {
	VO_PUB_ATTR_S vo_config;

	memset(&vo_config, 0x00, sizeof(VO_PUB_ATTR_S));
	vo_config.u32BgColor = background_color;
	vo_config.enIntfType = interface_type;
	vo_config.enIntfSync = interface_mode;

	// Configure device
	int ret = HI_MPI_VO_SetPubAttr(device_id, &vo_config);
	if (ret != HI_SUCCESS) {
		printf("ERROR: Unable to set VO timing = 0x%x\n");
		return ret;
	}

	// Enable device
	return HI_MPI_VO_Enable(device_id);
}

HI_HDMI_VIDEO_FMT_E VO_interfaceModeToHDMIMode(VO_INTF_SYNC_E enIntfSync) {
	switch (enIntfSync) {
		case VO_OUTPUT_PAL:
			return HI_HDMI_VIDEO_FMT_PAL;

		case VO_OUTPUT_NTSC:
			return HI_HDMI_VIDEO_FMT_NTSC;

		case VO_OUTPUT_1080P24:
			return HI_HDMI_VIDEO_FMT_1080P_24;

		case VO_OUTPUT_1080P25:
			return HI_HDMI_VIDEO_FMT_1080P_25;

		case VO_OUTPUT_1080P30:
			return HI_HDMI_VIDEO_FMT_1080P_30;

		case VO_OUTPUT_720P50:
			return HI_HDMI_VIDEO_FMT_720P_50;

		case VO_OUTPUT_720P60:
			return HI_HDMI_VIDEO_FMT_720P_60;

		case VO_OUTPUT_1080I50:
			return HI_HDMI_VIDEO_FMT_1080i_50;

		case VO_OUTPUT_1080I60:
			return HI_HDMI_VIDEO_FMT_1080i_60;

		case VO_OUTPUT_1080P50:
			return HI_HDMI_VIDEO_FMT_1080P_50;

		case VO_OUTPUT_1080P60:
			return HI_HDMI_VIDEO_FMT_1080P_60;

		case VO_OUTPUT_576P50:
			return HI_HDMI_VIDEO_FMT_576P_50;

		case VO_OUTPUT_480P60:
			return HI_HDMI_VIDEO_FMT_480P_60;

		case VO_OUTPUT_800x600_60:
			return HI_HDMI_VIDEO_FMT_VESA_800X600_60;

		case VO_OUTPUT_1024x768_60:
			return HI_HDMI_VIDEO_FMT_VESA_1024X768_60;

		case VO_OUTPUT_1280x1024_60:
			return HI_HDMI_VIDEO_FMT_VESA_1280X1024_60;

		case VO_OUTPUT_1366x768_60:
			return HI_HDMI_VIDEO_FMT_VESA_1366X768_60;

		case VO_OUTPUT_1440x900_60:
			return HI_HDMI_VIDEO_FMT_VESA_1440X900_60;

		case VO_OUTPUT_1280x800_60:
			return HI_HDMI_VIDEO_FMT_VESA_1280X800_60;

		case VO_OUTPUT_1600x1200_60:
			return HI_HDMI_VIDEO_FMT_VESA_1600X1200_60;

		case VO_OUTPUT_2560x1440_30:
			return HI_HDMI_VIDEO_FMT_2560x1440_30;

		case VO_OUTPUT_2560x1440_60:
			return HI_HDMI_VIDEO_FMT_2560x1440_60;

		case VO_OUTPUT_2560x1600_60:
			return HI_HDMI_VIDEO_FMT_2560x1600_60;

		case VO_OUTPUT_3840x2160_30:
			return HI_HDMI_VIDEO_FMT_3840X2160P_30;

		default:
			return 0;
	}
}

int VO_HDMI_init(HI_HDMI_ID_E device_id, VO_INTF_SYNC_E interface_mode) {
	// Initialize HDMI
	int ret = HI_MPI_HDMI_Init();
	if (ret != HI_SUCCESS) {
		printf("HDMI init failed\n");
		return ret;
	}

	// Enable HDMI
	ret = HI_MPI_HDMI_Open(device_id);
	if (ret != HI_SUCCESS) {
		printf("HDMI open failed\n");
		return ret;
	}

	// Obtain HDMI configuration
	HI_HDMI_ATTR_S hdmi_config;
	ret = HI_MPI_HDMI_GetAttr(device_id, &hdmi_config);
	if (ret != HI_SUCCESS) {
		printf("HDMI get config failed\n");
		return ret;
	}

	// Configure HDMI
	hdmi_config.bEnableHdmi = HI_TRUE;
	hdmi_config.bEnableVideo = HI_TRUE;
	hdmi_config.enVideoFmt = VO_interfaceModeToHDMIMode(interface_mode);

	hdmi_config.enVidOutMode = HI_HDMI_VIDEO_MODE_YCBCR444;
	hdmi_config.enDeepColorMode = HI_HDMI_DEEP_COLOR_OFF;
	hdmi_config.bxvYCCMode = HI_FALSE;

	hdmi_config.bEnableAudio = HI_FALSE;
	hdmi_config.enSoundIntf = HI_HDMI_SND_INTERFACE_I2S;
	hdmi_config.bIsMultiChannel = HI_FALSE;
	hdmi_config.enSampleRate = HI_HDMI_SAMPLE_RATE_48K;
	hdmi_config.u8DownSampleParm = 0;

	hdmi_config.enBitDepth = HI_HDMI_BIT_DEPTH_16;
	hdmi_config.u8I2SCtlVbit = 0;

	hdmi_config.bEnableAviInfoFrame = HI_TRUE;
	hdmi_config.bEnableAudInfoFrame = HI_TRUE;
	hdmi_config.bEnableSpdInfoFrame = HI_FALSE;
	hdmi_config.bEnableMpegInfoFrame = HI_FALSE;

	hdmi_config.bDebugFlag = HI_FALSE;
	hdmi_config.bHDCPEnable = HI_FALSE;
	hdmi_config.b3DEnable = HI_FALSE;
	hdmi_config.u83DParam = 9;
	hdmi_config.enDefaultMode = HI_HDMI_FORCE_HDMI;

	ret = HI_MPI_HDMI_SetAttr(device_id, &hdmi_config);
	if (ret != HI_SUCCESS) {
		printf("HDMI set config failed\n");
		return ret;
	}

	// Start HDMI
	ret = HI_MPI_HDMI_Start(device_id);
	if (ret != HI_SUCCESS) {
		printf("HDMI start failed\n");
		return ret;
	}

	printf("HDMI start success\n");

	return HI_SUCCESS;
}
