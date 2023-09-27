/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#if !defined(__IMX335_CMOS_H_)
#define __IMX335_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"

#include "imx335_cmos_ex.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define IMX335_ID 335

#define IMX335_HIG_4BITS(x) (((x)&0xF0000) >> 16)
#define IMX335_MID_8BITS(x) (((x)&0x0FF00) >> 8)
#define IMX335_LOW_8BITS(x) ((x)&0x000ff)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastImx335[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define IMX335_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastImx335[dev])
#define IMX335_SENSOR_SET_CTX(dev, pstCtx) (g_pastImx335[dev] = pstCtx)
#define IMX335_SENSOR_RESET_CTX(dev) (g_pastImx335[dev] = GK_NULL)

static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = { { 0 } };
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = { 0 };

static GK_U32 g_u32Imx335AGain[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM -
							    1)] = 1024 };
static GK_U32 g_u32Imx335DGain[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM -
							    1)] = 1024 };

ISP_SNS_COMMBUS_U g_aunImx335BusInfo[ISP_MAX_PIPE_NUM] = {
	[0] = { .s8I2cDev = 0 },
	[1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 }
};

static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {
	[0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

typedef struct IMX335_STATE_S {
	GK_U32 u32BRL;
	GK_U32 u32RHS1_MAX;
	GK_U32 u32deltaRHS1;
} IMX335_STATE_S;

IMX335_STATE_S g_astimx335State[ISP_MAX_PIPE_NUM] = { { 0 } };

static GK_U32 gu32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = { 0 };

static GK_U32 gu32STimeFps = 30;
static GK_U32 gu32LGain = 0;

/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/
extern unsigned char imx335_i2c_addr;
extern unsigned int imx335_addr_byte;
extern unsigned int imx335_data_byte;

extern void IMX335_init(VI_PIPE ViPipe);
extern void IMX335_exit(VI_PIPE ViPipe);
extern void IMX335_standby(VI_PIPE ViPipe);
extern void IMX335_restart(VI_PIPE ViPipe);
extern int IMX335_write_register(VI_PIPE ViPipe, int addr, int data);
extern int IMX335_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define IMX335_FULL_LINES_MAX (0xFFFF)

#define IMX335_VMAX_ADDR_L (0x3030)
#define IMX335_VMAX_ADDR_M (0x3031)
#define IMX335_VMAX_ADDR_H (0x3032)
#define IMX335_Y_OUT_SIZE_LOW (0x3056)
#define IMX335_Y_OUT_SIZE_HIGH (0x3057)
#define IMX335_SHR0_LOW (0x3058)
#define IMX335_SHR0_MIDDLE (0x3059)
#define IMX335_SHR0_HIGH (0x305A)
#define IMX335_SHR1_LOW (0x305C)
#define IMX335_SHR1_MIDDLE (0x305D)
#define IMX335_SHR1_HIGH (0x305E)
#define IMX335_RHS1_LOW (0x3068)
#define IMX335_RHS1_MIDDLE (0x3069)
#define IMX335_RHS1_HIGH (0x306A)
#define IMX335_GAIN_LONG_LOW (0x30E8)
#define IMX335_GAIN_LONG_HIGH (0x30E9)
#define IMX335_GAIN_SHORT_LOW (0x30EA)
#define IMX335_GAIN_SHORT_HIGH (0x30EB)

#define IMX335_INCREASE_LINES \
	(0) /* make real fps less than stand fps because NVR require */

#define IMX335_VMAX_5M_30FPS_12BIT_LINEAR (0x1194 + IMX335_INCREASE_LINES)
#define IMX335_VMAX_5M_30FPS_10BIT_WDR (0x1194 + IMX335_INCREASE_LINES)
#define IMX335_VMAX_4M_30FPS_10BIT_WDR (3300 + IMX335_INCREASE_LINES)
#define IMX335_VMAX_4M_25FPS_10BIT_WDR (0xBB8 + IMX335_INCREASE_LINES)

// sensor fps mode

#define IMX335_5M_30FPS_12BIT_LINEAR_MODE (0) //2592x1944
#define IMX335_5M_30FPS_10BIT_WDR_MODE (1) //2592x1944
#define IMX335_4M_25FPS_10BIT_WDR_MODE (2) //2560x1440
#define IMX335_4M_30FPS_10BIT_WDR_MODE (3) //2592x1520

#define IMX335_RES_IS_5M_12BIT_LINEAR(w, h) (((w) == 2592) && ((h) == 1944))
#define IMX335_RES_IS_5M_10BIT_WDR(w, h) (((w) == 2592) && ((h) == 1944))
#define IMX335_RES_IS_4M_12BIT_LINEAR(w, h) (((w) == 2592) && ((h) == 1520))
#define IMX335_RES_IS_4M_10BIT_WDR(w, h) (((w) == 2592) && ((h) == 1520))
#define IMX335_RES_IS_4M_10BIT_WDR_EX(w, h) (((w) == 2560) && ((h) == 1440))

// sensor gain
#define IMX335_AGAIN_MIN (1024)
#define IMX335_AGAIN_MAX (32381) // the max again is 32381

#define IMX335_DGAIN_MIN (1024)
#define IMX335_DGAIN_MAX (128914)

#define IMX335_AD_GAIN_TBL_RANGE 241
#define IMX335_AGAIN_TBL_RANGE 100
#define IMX335_DGAIN_TBL_RANGE 140

#define IMX335_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState)                    \
	do {                                                                      \
		ISP_TRACE(                                                        \
			MODULE_DBG_ERR,                                           \
			"Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n", \
			pstSensorImageMode->u16Width,                             \
			pstSensorImageMode->u16Height,                            \
			pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);      \
	} while (0)

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,
				  AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	GK_U32 u32Fll = 0;
	GK_U32 U32MaxFps = 0;

	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAeSnsDft);
	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if (IMX335_5M_30FPS_12BIT_LINEAR_MODE == pstSnsState->u8ImgMode) {
		u32Fll = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;
		U32MaxFps = 30;
		pstSnsState->u32FLStd =
			u32Fll * U32MaxFps / DIV_0_TO_1_FLOAT(gu32STimeFps);
	} else if (IMX335_5M_30FPS_10BIT_WDR_MODE == pstSnsState->u8ImgMode) {
		u32Fll = IMX335_VMAX_5M_30FPS_10BIT_WDR;
		U32MaxFps = 30;

		pstSnsState->u32FLStd =
			u32Fll * U32MaxFps / DIV_0_TO_1_FLOAT(gu32STimeFps);

		if (0 != (pstSnsState->u32FLStd % 4)) {
			pstSnsState->u32FLStd =
				pstSnsState->u32FLStd -
				(pstSnsState->u32FLStd % 4) +
				4; //Because FSC value an integer multiple of 8
		}
		pstSnsState->u32FLStd = pstSnsState->u32FLStd * 2;

	} else if (IMX335_4M_30FPS_10BIT_WDR_MODE == pstSnsState->u8ImgMode) {
		u32Fll = IMX335_VMAX_4M_30FPS_10BIT_WDR;
		U32MaxFps = 30;
		pstSnsState->u32FLStd =
			u32Fll * U32MaxFps / DIV_0_TO_1_FLOAT(gu32STimeFps);

		if (0 != (pstSnsState->u32FLStd % 4)) {
			pstSnsState->u32FLStd =
				pstSnsState->u32FLStd -
				(pstSnsState->u32FLStd % 4) +
				4; //Because FSC value an integer multiple of 8
		}
		pstSnsState->u32FLStd = pstSnsState->u32FLStd * 2;
	}

	else if (IMX335_4M_25FPS_10BIT_WDR_MODE == pstSnsState->u8ImgMode) {
		u32Fll = IMX335_VMAX_4M_25FPS_10BIT_WDR;
		U32MaxFps = 25;
		pstSnsState->u32FLStd =
			u32Fll * U32MaxFps / DIV_0_TO_1_FLOAT(gu32STimeFps);
		if (0 != (pstSnsState->u32FLStd % 4)) {
			pstSnsState->u32FLStd =
				pstSnsState->u32FLStd -
				(pstSnsState->u32FLStd % 4) +
				4; //Because FSC value an integer multiple of 8
		}
		pstSnsState->u32FLStd = pstSnsState->u32FLStd * 2;

	} else {
		u32Fll = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;
		U32MaxFps = 30;
		pstSnsState->u32FLStd =
			u32Fll * U32MaxFps / DIV_0_TO_1_FLOAT(gu32STimeFps);
	}

	//pstSnsState->u32FLStd = u32Fll;

	pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
	pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;

	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FlickerFreq = 0;

	pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;

	pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
	pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

	pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
	pstAeSnsDft->stDgainAccu.f32Accuracy = 1;

	pstAeSnsDft->u32ISPDgainShift = 8;
	pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
	pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift;

	memcpy(&pstAeSnsDft->stPirisAttr, &gstPirisAttr,
	       sizeof(ISP_PIRIS_ATTR_S));
	pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_4;
	pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_5_6;

	pstAeSnsDft->bAERouteExValid = GK_FALSE;
	pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
	pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

	if (g_au32InitExposure[ViPipe] == 0) {
		pstAeSnsDft->u32InitExposure = 130000;
	} else {
		pstAeSnsDft->u32InitExposure = g_au32InitExposure[ViPipe];
	}

	if (g_au32LinesPer500ms[ViPipe] == 0) {
		pstAeSnsDft->u32LinesPer500ms = (u32Fll * U32MaxFps) >> 1;
	} else {
		pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
	}

	switch (pstSnsState->enWDRMode) {
	default:
	case WDR_MODE_NONE: {
		pstAeSnsDft->au8HistThresh[0] = 0xd;
		pstAeSnsDft->au8HistThresh[1] = 0x28;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;

		pstAeSnsDft->u8AeCompensation = 0x38;
		pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

		pstAeSnsDft->u32MinIntTime = 2;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;
		pstAeSnsDft->stIntTimeAccu.f32Offset = -0.198;

		pstAeSnsDft->u32MaxAgain = IMX335_AGAIN_MAX;
		pstAeSnsDft->u32MinAgain = IMX335_AGAIN_MIN;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain =
			IMX335_DGAIN_MAX; /* if Dgain enable,please set ispdgain bigger than 1 */
		pstAeSnsDft->u32MinDgain = IMX335_DGAIN_MIN;
		pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		break;
	}

	case WDR_MODE_2To1_LINE: {
		/* FS WDR mode */
		pstAeSnsDft->au8HistThresh[0] = 0xc;
		pstAeSnsDft->au8HistThresh[1] = 0x18;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;

		pstAeSnsDft->u8AeCompensation = 0x38;

		pstAeSnsDft->stIntTimeAccu.f32Accuracy = 4;
		pstAeSnsDft->u32MinIntTime = 4; // 2;//192;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;
		;
		pstAeSnsDft->stIntTimeAccu.f32Offset = -0.396;

		pstAeSnsDft->u32MaxIntTimeStep = 1000;
		pstAeSnsDft->u32LFMinExposure = 15000000;
		pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;

		pstAeSnsDft->u32MaxAgain = IMX335_AGAIN_MAX;
		pstAeSnsDft->u32MinAgain = IMX335_AGAIN_MIN;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain =
			IMX335_DGAIN_MAX; /* if Dgain enable,please set ispdgain bigger than 1 */
		pstAeSnsDft->u32MinDgain = IMX335_DGAIN_MIN;
		pstAeSnsDft->u32MaxDgainTarget = 1024;
		pstAeSnsDft->u32MinDgainTarget = 1024;
		pstAeSnsDft->u32MaxISPDgainTarget =
			16 << pstAeSnsDft->u32ISPDgainShift;
		pstAeSnsDft->bDiffGainSupport = GK_TRUE;

		pstAeSnsDft->u32ExpRatioMin =
			0x80; // The exposure ration from 2x to convergence

		if (ISP_FSWDR_LONG_FRAME_MODE != genFSWDRMode[ViPipe]) {
			pstAeSnsDft->u16ManRatioEnable = GK_FALSE;
			pstAeSnsDft->au32Ratio[0] = 0x400;
			pstAeSnsDft->au32Ratio[1] = 0x40;
			pstAeSnsDft->au32Ratio[2] = 0x40;
		}
		break;
	}
	}

	return GK_SUCCESS;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps,
			    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	GK_U32 u32MaxFps;
	GK_U32 u32Lines;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	switch (pstSnsState->u8ImgMode) {
	case IMX335_5M_30FPS_12BIT_LINEAR_MODE:
		if ((f32Fps <= 30.0) && (f32Fps >= 2.0)) {
			u32MaxFps = 30;
			u32Lines = IMX335_VMAX_5M_30FPS_12BIT_LINEAR *
				   u32MaxFps / DIV_0_TO_1_FLOAT(f32Fps);
			pstAeSnsDft->u32LinesPer500ms =
				IMX335_VMAX_5M_30FPS_12BIT_LINEAR * 15;
			pstSnsState->u32FLStd = u32Lines;
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		break;

	case IMX335_5M_30FPS_10BIT_WDR_MODE:
		if ((f32Fps <= 30.0) && (f32Fps >= 15.0)) {
			u32MaxFps = 30;
			u32Lines = IMX335_VMAX_5M_30FPS_10BIT_WDR * u32MaxFps /
				   DIV_0_TO_1_FLOAT(f32Fps);
			pstAeSnsDft->u32LinesPer500ms =
				IMX335_VMAX_5M_30FPS_10BIT_WDR * 30;
			if (0 != (u32Lines % 4)) {
				u32Lines =
					u32Lines - (u32Lines % 4) +
					4; //Because FSC value an integer multiple of 8
			}
			pstSnsState->u32FLStd = u32Lines;
		}

		else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		break;
	case IMX335_4M_30FPS_10BIT_WDR_MODE:
		if ((f32Fps <= 30.0) && (f32Fps >= 15.0)) {
			u32MaxFps = 30;
			u32Lines = IMX335_VMAX_4M_30FPS_10BIT_WDR * u32MaxFps /
				   DIV_0_TO_1_FLOAT(f32Fps);
			pstAeSnsDft->u32LinesPer500ms =
				IMX335_VMAX_4M_30FPS_10BIT_WDR * 30;
			if (0 != (u32Lines % 4)) {
				u32Lines =
					u32Lines - (u32Lines % 4) +
					4; //Because FSC value an integer multiple of 8
			}
			pstSnsState->u32FLStd = u32Lines;
		}

		else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		break;
	case IMX335_4M_25FPS_10BIT_WDR_MODE:
		if ((f32Fps <= 30) && (f32Fps >= 15)) {
			u32MaxFps = 25;
			u32Lines = IMX335_VMAX_4M_25FPS_10BIT_WDR * u32MaxFps /
				   DIV_0_TO_1_FLOAT(f32Fps);
			u32Lines = u32Lines +
				   (u32Lines % 2); //VMAX is 2n(n-0,1,2.....)
			pstSnsState->u32FLStd = u32Lines;
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		break;

	default:
		ISP_TRACE(MODULE_DBG_ERR, "Not support this Mode!!!\n");
		return;
		break;
	}

	/* SHR 16bit, So limit full_lines as 0xFFFF */
	if (f32Fps > u32MaxFps) {
		ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n", f32Fps);
		return;
	}

	if (u32Lines > IMX335_FULL_LINES_MAX) {
		u32Lines = IMX335_FULL_LINES_MAX;
	}

	pstAeSnsDft->f32Fps = f32Fps;
	pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * f32Fps / 2;
	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;

	pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

	if (WDR_MODE_NONE == pstSnsState->enWDRMode) {
		pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
			IMX335_LOW_8BITS(pstSnsState->au32FL[0]);
		pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
			IMX335_MID_8BITS(pstSnsState->au32FL[0]);
		pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
			IMX335_HIG_4BITS(pstSnsState->au32FL[0]);

	} else {
		pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
			IMX335_LOW_8BITS(pstSnsState->au32FL[0]);
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data =
			IMX335_MID_8BITS(pstSnsState->au32FL[0]);
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
			IMX335_HIG_4BITS(pstSnsState->au32FL[0]);
	}

	if (WDR_MODE_2To1_LINE == pstSnsState->enWDRMode) {
		pstSnsState->u32FLStd = u32Lines * 2;
		/*
            RHS1 limitation:
            8n + 2
            RHS1<BRL*2
            RHS1(N+1)>= RHS1(N)+ 2*BRL - 2*VMAX + 2 ===> RHS1(N)-RHS1(N+1)<= 2*VMAX - 2*BRL -2)
        */
		g_astimx335State[ViPipe].u32RHS1_MAX =
			g_astimx335State[ViPipe].u32BRL * 2;
		g_astimx335State[ViPipe].u32deltaRHS1 =
			(u32Lines * 2) - (g_astimx335State[ViPipe].u32BRL * 2) -
			2;
	} else {
		pstSnsState->u32FLStd = u32Lines;
	}

	pstAeSnsDft->f32Fps = f32Fps;
	gu32STimeFps = (GK_U32)f32Fps;
	pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * f32Fps / 2;
	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;

	pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
	pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
	pstAeSnsDft->u32HmaxTimes =
		(1000000) / (pstSnsState->u32FLStd * DIV_0_TO_1_FLOAT(f32Fps));

	return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines,
				       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	GK_U32 u32Fll;
	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (WDR_MODE_2To1_LINE == pstSnsState->enWDRMode) {
		if (IMX335_4M_30FPS_10BIT_WDR_MODE == pstSnsState->u8ImgMode) {
			u32Fll = IMX335_VMAX_4M_30FPS_10BIT_WDR;
		} else {
			u32Fll = IMX335_VMAX_5M_30FPS_10BIT_WDR;
		}

		u32FullLines = (u32FullLines > (2 * 2 * u32Fll)) ?
				       (2 * 2 * u32Fll) :
				       u32FullLines;

		if (0 != (u32FullLines % 4)) {
			u32FullLines =
				u32FullLines +
				(4 -
				 (u32FullLines %
				  4)); // Because FSC value an integer multiple of 8
		}
		pstSnsState->au32FL[0] = u32FullLines;

		g_astimx335State[ViPipe].u32RHS1_MAX =
			g_astimx335State[ViPipe].u32BRL * 2;
		g_astimx335State[ViPipe].u32deltaRHS1 =
			u32FullLines - (g_astimx335State[ViPipe].u32BRL * 2) -
			2;
	} else {
		u32FullLines = (u32FullLines > IMX335_FULL_LINES_MAX) ?
				       IMX335_FULL_LINES_MAX :
				       u32FullLines;
		pstSnsState->au32FL[0] = u32FullLines;
	}

	if (WDR_MODE_NONE == pstSnsState->enWDRMode) {
		pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
			IMX335_LOW_8BITS(pstSnsState->au32FL[0]); // VMAX
		pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
			IMX335_MID_8BITS(pstSnsState->au32FL[0]);
		pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
			IMX335_HIG_4BITS(pstSnsState->au32FL[0]);
	} else {
		pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
			IMX335_LOW_8BITS(pstSnsState->au32FL[0] >> 1); // VMAX
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data =
			IMX335_MID_8BITS(pstSnsState->au32FL[0] >> 1);
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
			IMX335_HIG_4BITS(pstSnsState->au32FL[0] >> 1);
	}

	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
	pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 8;

	return;
}

/* Again and Dgain use the same table, Step is 0.3db */
static GK_U32 ad_gain_table[IMX335_AD_GAIN_TBL_RANGE] = {
	1024,	 1059,	  1097,	   1135,    1175,    1217,    1259,
	1304,	 1349,	  1397,	   1446,    1497,    1549,    1604, // 3.9dB
	1660,	 1719,	  1779,	   1842,    1906,    1973,    2043,
	2114,	 2189,	  2266,	   2345,    2428,    2513,    2601, // 8.1dB
	2693,	 2788,	  2886,	   2987,    3092,    3201,    3313,
	3430,	 3550,	  3675,	   3804,    3938,    4076,    4219, // 12.3dB
	4368,	 4521,	  4680,	   4845,    5015,    5191,    5374,
	5562,	 5758,	  5960,	   6170,    6387,    6611,    6843, // 16.5dB
	7084,	 7333,	  7591,	   7857,    8133,    8419,    8715,
	9021,	 9338,	  9667,	   10006,   10358,   10722,   11099, // 20.7dB
	11489,	 11893,	  12311,   12743,   13191,   13655,   14135,
	14631,	 15146,	  15678,   16229,   16799,   17390,   18001, // 24.9dB
	18633,	 19288,	  19966,   20668,   21394,   22146,   22924,
	23730,	 24564,	  25427,   26320,   27245,   28203,   29194, // 29.1dB
	30220,	 31282,	  32381,   33519,   34697,   35917,   37179,
	38485,	 39838,	  41238,   42687,   44187,   45740,   47347, // 33.3dB
	49011,	 50734,	  52517,   54362,   56273,   58250,   60297,
	62416,	 64610,	  66880,   69230,   71663,   74182,   76789, // 37.5dB
	79487,	 82281,	  85172,   88165,   91264,   94471,   97791,
	101228,	 104785,  108468,  112279,  116225,  120310,  124537, // 41.7dB
	128914,	 133444,  138134,  142988,  148013,  153215,  158599,
	164172,	 169942,  175914,  182096,  188495,  195119,  201976, // 45.9dB
	209074,	 216421,  224027,  231900,  240049,  248485,  257217,
	266256,	 275613,  285299,  295325,  305703,  316446,  327567, // 50.1dB
	339078,	 350994,  363329,  376097,  389314,  402995,  417157,
	431817,	 446992,  462700,  478961,  495793,  513216,  531251, // 54.3dB
	549921,	 569246,  589250,  609958,  631393,  653582,  676550,
	700326,	 724936,  750412,  776783,  804081,  832338,  861589, // 58.5dB
	891867,	 923209,  955652,  989236,  1024000, 1059985, 1097236,
	1135795, 1175709, 1217026, 1259795, 1304067, 1349895, 1397333, // 62.7dB
	1446438, 1497269, 1549887, 1604353, 1660734, 1719095, 1779508,
	1842044, 1906777, 1973786, 2043149, 2114949, 2189273, 2266209, // 66.9dB
	2345848, 2428287, 2513622, 2601956, 2693394, 2788046, 2886024,
	2987445, 3092431, 3201105, 3313599, 3430046, 3550585, 3675361, // 71.1dB
	3804521, 3938220, 4076617 // 72.0dB
};

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
	static GK_BOOL bFirst[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM -
							   1)] = 1 };
	static GK_U32 u32LastShortIntTime = 0;
	static GK_U32 u32ShortIntTime = 0;
	static GK_U32 u32LongIntTime = 0;

	GK_U32 u32RHS1 = 0;
	GK_U32 u32SHR0 = 8872;
	GK_U32 u32SHR1 = 18;

	GK_U32 u32delta = 0;
	GK_U32 u32Value = 0;
	GK_U16 u16ShortExpMod4 = 0;
	GK_U16 u16ShortExpMod8 = 0;
	GK_U32 u32RHS1Limit = 0;

	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (WDR_MODE_2To1_LINE == pstSnsState->enWDRMode) {
		if (bFirst[ViPipe]) { /* short exposure */
			pstSnsState->au32WDRIntTime[0] = u32IntTime;
			u32ShortIntTime = u32IntTime;

			bFirst[ViPipe] = GK_FALSE;
		} else { /* long exposure */
			pstSnsState->au32WDRIntTime[1] = u32IntTime;
			u32LongIntTime = u32IntTime;

			u32SHR0 = pstSnsState->au32FL[1] - u32LongIntTime +
				  (u32LongIntTime % 4);

			if (u32LastShortIntTime > u32ShortIntTime) {
				u32delta =
					u32LastShortIntTime - u32ShortIntTime;
				u32RHS1Limit =
					(pstSnsState->au32FL[1] -
					 (g_astimx335State[ViPipe].u32BRL * 2) -
					 2);
				if (u32delta > u32RHS1Limit) {
					if (u32ShortIntTime >= 30) {
						u32ShortIntTime =
							u32LastShortIntTime -
							(u32RHS1Limit -
							 (u32RHS1Limit % 8) +
							 8 + 8);
					} else {
						u32ShortIntTime =
							u32LastShortIntTime -
							(u32RHS1Limit -
							 (u32RHS1Limit % 8) +
							 8);
					}
				}
			}

			u16ShortExpMod4 = u32ShortIntTime % 4;
			u16ShortExpMod8 = u32ShortIntTime % 8;

			if (0 == u16ShortExpMod8) {
				u32SHR1 = 18;
				u32RHS1 = 18 + u32ShortIntTime;
			} else {
				if (u16ShortExpMod4 == u16ShortExpMod8) {
					u32SHR1 = 18;
					u32RHS1 = 18 + u32ShortIntTime -
						  u16ShortExpMod8;
				} else {
					u32SHR1 = 22;
					u32RHS1 = 22 + u32ShortIntTime -
						  u16ShortExpMod4;
				}
			}
			if (u32SHR0 < (u32RHS1 + u32SHR1)) {
				u32SHR0 = u32RHS1 + u32SHR1;
			} else {
			}

			pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
				IMX335_LOW_8BITS(u32SHR0); // SHR0
			pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
				IMX335_MID_8BITS(u32SHR0);
			pstSnsState->astRegsInfo[0].astI2cData[2].u32Data =
				IMX335_HIG_4BITS(u32SHR0);

			pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
				IMX335_LOW_8BITS(u32SHR1); // SHR1
			pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
				IMX335_MID_8BITS(u32SHR1);
			pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
				IMX335_HIG_4BITS(u32SHR1);

			pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
				IMX335_LOW_8BITS(u32RHS1); // RHS1
			pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =
				IMX335_MID_8BITS(u32RHS1);
			pstSnsState->astRegsInfo[0].astI2cData[13].u32Data =
				IMX335_HIG_4BITS(u32RHS1);
			bFirst[ViPipe] = GK_TRUE;
			u32LastShortIntTime = u32ShortIntTime;
		}
	} else { // Linear
		u32Value = pstSnsState->au32FL[0] - u32IntTime;
		u32Value = MIN(u32Value, IMX335_FULL_LINES_MAX);
		u32Value = MIN(MAX(u32Value, 9), (pstSnsState->au32FL[0] - 1));

		pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
			IMX335_LOW_8BITS(u32Value); // SHR0
		pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
			IMX335_MID_8BITS(u32Value);
		pstSnsState->astRegsInfo[0].astI2cData[2].u32Data =
			IMX335_HIG_4BITS(u32Value);
	}

	return;
}

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin,
				     GK_U32 *pu32AgainDb)
{
	int i;

	CMOS_CHECK_POINTER_VOID(pu32AgainLin);
	CMOS_CHECK_POINTER_VOID(pu32AgainDb);

	if (*pu32AgainLin >= ad_gain_table[IMX335_AGAIN_TBL_RANGE]) {
		*pu32AgainLin = ad_gain_table[IMX335_AGAIN_TBL_RANGE];
		*pu32AgainDb = IMX335_AGAIN_TBL_RANGE;
	} else {
		for (i = 1; i < (IMX335_AGAIN_TBL_RANGE + 1); i++) {
			if (*pu32AgainLin < ad_gain_table[i]) {
				*pu32AgainLin = ad_gain_table[i - 1];
				*pu32AgainDb = i - 1;
				break;
			}
		}
	}

	g_u32Imx335AGain[ViPipe] = *pu32AgainLin;
	return;
}

static GK_VOID cmos_dgain_calc_table(VI_PIPE ViPipe, GK_U32 *pu32DgainLin,
				     GK_U32 *pu32DgainDb)
{
	int i;

	CMOS_CHECK_POINTER_VOID(pu32DgainLin);
	CMOS_CHECK_POINTER_VOID(pu32DgainDb);

	if (*pu32DgainLin >= ad_gain_table[IMX335_DGAIN_TBL_RANGE]) {
		*pu32DgainLin = ad_gain_table[IMX335_DGAIN_TBL_RANGE];
		*pu32DgainDb = IMX335_DGAIN_TBL_RANGE;
	} else {
		for (i = 1; i < (IMX335_DGAIN_TBL_RANGE + 1); i++) {
			if (*pu32DgainLin < ad_gain_table[i]) {
				*pu32DgainLin = ad_gain_table[i - 1];
				*pu32DgainDb = i - 1;
				break;
			}
		}
	}

	g_u32Imx335DGain[ViPipe] = *pu32DgainLin;
	return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again,
				 GK_U32 u32Dgain)
{
	static GK_BOOL bFirstGain[ISP_MAX_PIPE_NUM] = {
		[0 ...(ISP_MAX_PIPE_NUM - 1)] = 1
	};
	GK_U32 u32Tmp;

	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		if (bFirstGain[ViPipe]) {
			u32Tmp = u32Again + u32Dgain;
			gu32LGain = u32Tmp;

			pstSnsState->astRegsInfo[0].astI2cData[3].u32Data =
				IMX335_LOW_8BITS(u32Tmp);
			pstSnsState->astRegsInfo[0].astI2cData[4].u32Data =
				IMX335_MID_8BITS(u32Tmp);
			bFirstGain[ViPipe] = GK_FALSE;
		} else {
			u32Tmp = u32Again + u32Dgain;

			pstSnsState->astRegsInfo[0].astI2cData[14].u32Data =
				IMX335_LOW_8BITS(u32Tmp);
			pstSnsState->astRegsInfo[0].astI2cData[15].u32Data =
				IMX335_MID_8BITS(u32Tmp);
			bFirstGain[ViPipe] = GK_TRUE;
		}
	} else {
		u32Tmp = u32Again + u32Dgain;
		gu32LGain = u32Tmp;

		pstSnsState->astRegsInfo[0].astI2cData[3].u32Data =
			IMX335_LOW_8BITS(u32Tmp);
		pstSnsState->astRegsInfo[0].astI2cData[4].u32Data =
			IMX335_MID_8BITS(u32Tmp);

		pstSnsState->astRegsInfo[0].astI2cData[14].u32Data =
			IMX335_LOW_8BITS(u32Tmp);
		pstSnsState->astRegsInfo[0].astI2cData[15].u32Data =
			IMX335_MID_8BITS(u32Tmp);
	}

	return;
}

/*
    Linear:    SHR0 [9, VMAX-1];    Initime = VMAX - SHR0  Initime [1,VMAX -9]
    WDR2t1:    SHS1 4n+2 (n=0,1,2....)  and 18 <= SHS1 <= RHS1-4
               RHS1 8n+2 (n=0,1,2....)  and (SHR1+5)<= RHS1<=(SHR0-18) and RHS1<(BRL*2)
               SHR0 4n(n=0,1,2.....) and (RHS1+18) <= SHR0<=(2*VMAX-4)  VMAX :2n (n=0,1,2.....)
*/
/* Only used in WDR_MODE_2To1_LINE and WDR_MODE_2To1_FRAME mode */
static GK_VOID cmos_get_inttime_max(VI_PIPE ViPipe, GK_U16 u16ManRatioEnable,
				    GK_U32 *au32Ratio, GK_U32 *au32IntTimeMax,
				    GK_U32 *au32IntTimeMin,
				    GK_U32 *pu32LFMaxIntTime)
{
	GK_U32 u32IntTimeMaxTmp0 = 0;
	GK_U32 u32IntTimeMaxTmp = 0;
	GK_U32 u32RatioTmp = 0x40;
	GK_U32 u32ShortTimeMinLimit = 4;
	GK_U32 u32delta;
	static GK_U32 u32LastIntTimeMaxTmp = 4;
	GK_U16 u16LimitValue = 0;

	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(au32Ratio);
	CMOS_CHECK_POINTER_VOID(au32IntTimeMax);
	CMOS_CHECK_POINTER_VOID(au32IntTimeMin);
	CMOS_CHECK_POINTER_VOID(pu32LFMaxIntTime);
	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if ((WDR_MODE_2To1_LINE == pstSnsState->enWDRMode)) {
		/*
            limitation for line base WDR
            SHR1 limitation:
                18 or more
                and (RHS1 - 4) or less
                and 4n+2(n=0,1,2....)

            SHR0 limitation:
                4n(n=0,1,2....)
                (RHS1 + 18) or more
                and (FSC - 4) or less

            RHS1 Limitation:
                8n+2 (n=0,1,2....)
                (SHR1+4)<=RHS1 <= (SHR0-18)
            |RHS1(n+1) - RHS1(n)| < (VMAX-BRL)*2 - 2

            RHS1 = 18 + st - (st % 8) + 8;
            SHR0 = FSC - st * ratio + (lt % 4) - 4

            RHS1 <= SHR0 -18 = FSC - st * ratio + (lt % 4) - 22
            st * (1 + ratio) <= FSC - 48 + (lt % 4) +(st % 8)
            st * (1 + ratio) < = FSC - 48

            short exposure time = RHS1 - SHR1 <= RHS1 - 18
            long exposure time = FSC -  SHR0  <= FSC - (RHS1 + 18)
            ExposureShort + ExposureLong <= FSC - 36
            short exposure time <= (FSC - 36) / (ratio + 1)

           when using withing FSC mode, please set to satisfy theste restrictions below
           (VMAX*2 - BRL*2) / 4 -2 > (RHS1/2 - 3) / 2
           ==> RHS < ((((VMAX*2 - BRL*2) / 4 -2)* 2) + 3) * 2
        */
		//RHS1(N+1)>=RHS1(N)+BRL*2-VMAX*2+2

		u16LimitValue = (IMX335_VMAX_4M_30FPS_10BIT_WDR * 2 -
				 g_astimx335State[ViPipe].u32BRL * 2 - 2) -
				8;
		u16LimitValue = 300;

		if (ISP_FSWDR_LONG_FRAME_MODE ==
		    genFSWDRMode[ViPipe]) { /* long frame mode enable */
			u32IntTimeMaxTmp0 = pstSnsState->au32FL[1] - 280 -
					    pstSnsState->au32WDRIntTime[0];
			u32IntTimeMaxTmp = pstSnsState->au32FL[0] - 280;
			u32IntTimeMaxTmp =
				(u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ?
					u32IntTimeMaxTmp0 :
					u32IntTimeMaxTmp;
			au32IntTimeMax[0] = u32IntTimeMaxTmp;
			au32IntTimeMin[0] = u32ShortTimeMinLimit;
			u32LastIntTimeMaxTmp = u32ShortTimeMinLimit;
			return;
		} else if (ISP_FSWDR_AUTO_LONG_FRAME_MODE ==
			   genFSWDRMode[ViPipe]) {
			if (pstSnsState->au32WDRIntTime[0] ==
				    u32ShortTimeMinLimit &&
			    au32Ratio[0] == 0x40) {
				u32IntTimeMaxTmp0 =
					pstSnsState->au32FL[1] - 280 -
					pstSnsState->au32WDRIntTime[0];
				u32IntTimeMaxTmp = pstSnsState->au32FL[0] - 280;
				u32IntTimeMaxTmp =
					(u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ?
						u32IntTimeMaxTmp0 :
						u32IntTimeMaxTmp;
				au32IntTimeMax[0] = u32IntTimeMaxTmp;
				au32IntTimeMin[0] = u32ShortTimeMinLimit;
				u32LastIntTimeMaxTmp = u32ShortTimeMinLimit;
			} else {
				u32IntTimeMaxTmp0 =
					((pstSnsState->au32FL[1] - 280 -
					  pstSnsState->au32WDRIntTime[0]) *
					 0x40) /
					DIV_0_TO_1(au32Ratio[0]);
				u32IntTimeMaxTmp =
					((pstSnsState->au32FL[0] - 280) *
					 0x40) /
					DIV_0_TO_1(au32Ratio[0] + 0x40);
				u32IntTimeMaxTmp =
					(u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ?
						u32IntTimeMaxTmp0 :
						u32IntTimeMaxTmp;
				u32IntTimeMaxTmp =
					(u32IntTimeMaxTmp >
					 (g_astimx335State[ViPipe].u32RHS1_MAX -
					  18)) ?
						(g_astimx335State[ViPipe]
							 .u32RHS1_MAX -
						 18) :
						u32IntTimeMaxTmp;
				if (u32IntTimeMaxTmp > u32LastIntTimeMaxTmp) {
					u32delta = u32IntTimeMaxTmp -
						   u32LastIntTimeMaxTmp;
					if (u32delta > g_astimx335State[ViPipe]
							       .u32deltaRHS1) {
						u32IntTimeMaxTmp =
							u32LastIntTimeMaxTmp +
							(g_astimx335State[ViPipe]
								 .u32deltaRHS1 -
							 (g_astimx335State[ViPipe]
								  .u32deltaRHS1 %
							  8));
					} else {
					}
				} else if (u32LastIntTimeMaxTmp >
					   u32IntTimeMaxTmp) {
					u32delta = u32LastIntTimeMaxTmp -
						   u32IntTimeMaxTmp;
					if (u32delta > g_astimx335State[ViPipe]
							       .u32deltaRHS1) {
						u32IntTimeMaxTmp =
							u32LastIntTimeMaxTmp -
							(g_astimx335State[ViPipe]
								 .u32deltaRHS1 -
							 (g_astimx335State[ViPipe]
								  .u32deltaRHS1 %
							  8));
					} else {
					}
				}
				u32IntTimeMaxTmp = (0 == u32IntTimeMaxTmp) ?
							   1 :
							   u32IntTimeMaxTmp;
				u32LastIntTimeMaxTmp = u32IntTimeMaxTmp;
			}
		} else {
			u32IntTimeMaxTmp0 = ((pstSnsState->au32FL[1] - 280 -
					      pstSnsState->au32WDRIntTime[0]) *
					     0x40) /
					    DIV_0_TO_1(au32Ratio[0]);
			u32IntTimeMaxTmp =
				((pstSnsState->au32FL[0] - 280) * 0x40) /
				DIV_0_TO_1(au32Ratio[0] + 0x40);
			u32IntTimeMaxTmp =
				(u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ?
					u32IntTimeMaxTmp0 :
					u32IntTimeMaxTmp;
			u32IntTimeMaxTmp =
				(u32IntTimeMaxTmp >
				 (g_astimx335State[ViPipe].u32RHS1_MAX - 18)) ?
					(g_astimx335State[ViPipe].u32RHS1_MAX -
					 18) :
					u32IntTimeMaxTmp;

			if (u32IntTimeMaxTmp >= u32LastIntTimeMaxTmp) {
				u32delta =
					u32IntTimeMaxTmp - u32LastIntTimeMaxTmp;
				if (u32delta > u16LimitValue) {
					u32IntTimeMaxTmp =
						u32LastIntTimeMaxTmp +
						u16LimitValue;
				}
			} else if (u32LastIntTimeMaxTmp > u32IntTimeMaxTmp) {
				u32delta =
					u32LastIntTimeMaxTmp - u32IntTimeMaxTmp;
				if (u32delta > u16LimitValue) {
					if (pstSnsState->au32WDRIntTime[0] >
					    (u16LimitValue + 4)) {
						u32ShortTimeMinLimit =
							pstSnsState
								->au32WDRIntTime
									[0] -
							u16LimitValue;
					}

					u32IntTimeMaxTmp =
						pstSnsState->au32WDRIntTime[0];

					u32LastIntTimeMaxTmp =
						u32ShortTimeMinLimit;
				}
			}
			u32IntTimeMaxTmp =
				(0 == u32IntTimeMaxTmp) ? 1 : u32IntTimeMaxTmp;
			u32LastIntTimeMaxTmp = u32IntTimeMaxTmp;
			*pu32LFMaxIntTime =
				g_astimx335State[ViPipe].u32RHS1_MAX - 18;
		}
	} else {
	}

	if (u32IntTimeMaxTmp >= u32ShortTimeMinLimit) {
		if (IS_LINE_WDR_MODE(pstSnsState->enWDRMode)) {
			au32IntTimeMax[0] = u32IntTimeMaxTmp;
			au32IntTimeMax[1] = au32IntTimeMax[0] * au32Ratio[0] >>
					    6;
			au32IntTimeMax[2] = au32IntTimeMax[1] * au32Ratio[1] >>
					    6;
			au32IntTimeMax[3] = au32IntTimeMax[2] * au32Ratio[2] >>
					    6;
			au32IntTimeMin[0] = u32ShortTimeMinLimit;
			au32IntTimeMin[1] = au32IntTimeMin[0] * au32Ratio[0] >>
					    6;
			au32IntTimeMin[2] = au32IntTimeMin[1] * au32Ratio[1] >>
					    6;
			au32IntTimeMin[3] = au32IntTimeMin[2] * au32Ratio[2] >>
					    6;
		} else {
		}
	} else {
		if (1 == u16ManRatioEnable) {
			printf("Manaul ExpRatio is too large!\n");
			return;
		} else {
			u32IntTimeMaxTmp = u32ShortTimeMinLimit;

			if (WDR_MODE_2To1_LINE == pstSnsState->enWDRMode) {
				u32RatioTmp = 0xFFF;
				au32IntTimeMax[0] = u32IntTimeMaxTmp;
				au32IntTimeMax[1] =
					au32IntTimeMax[0] * u32RatioTmp >> 6;
			} else {
			}
			au32IntTimeMin[0] = au32IntTimeMax[0];
			au32IntTimeMin[1] = au32IntTimeMax[1];
			au32IntTimeMin[2] = au32IntTimeMax[2];
			au32IntTimeMin[3] = au32IntTimeMax[3];
		}
	}
	return;
}

/* Only used in FSWDR mode */
static GK_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe,
				      AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
	CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

	genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
	gu32MaxTimeGetCnt[ViPipe] = 0;
}

static GK_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
	CMOS_CHECK_POINTER(pstExpFuncs);

	memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

	pstExpFuncs->pfn_cmos_get_ae_default = cmos_get_ae_default;
	pstExpFuncs->pfn_cmos_fps_set = cmos_fps_set;
	pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
	pstExpFuncs->pfn_cmos_inttime_update = cmos_inttime_update;
	pstExpFuncs->pfn_cmos_gains_update = cmos_gains_update;
	pstExpFuncs->pfn_cmos_again_calc_table = cmos_again_calc_table;
	pstExpFuncs->pfn_cmos_dgain_calc_table = cmos_dgain_calc_table;
	pstExpFuncs->pfn_cmos_get_inttime_max = cmos_get_inttime_max;
	pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

	return GK_SUCCESS;
}

/* AWB default parameter and function */
#define CALIBRATE_STATIC_WB_R_GAIN 0x1E3
#define CALIBRATE_STATIC_WB_GR_GAIN 0x100
#define CALIBRATE_STATIC_WB_GB_GAIN 0x100
#define CALIBRATE_STATIC_WB_B_GAIN 0x1d1

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 -0x0012
#define CALIBRATE_AWB_P2 0x010b
#define CALIBRATE_AWB_Q1 -0x0007
#define CALIBRATE_AWB_A1 0x2711F
#define CALIBRATE_AWB_B1 0x0080
#define CALIBRATE_AWB_C1 -0x1A5C1

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe,
				   AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAwbSnsDft);
	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

	pstAwbSnsDft->u16WbRefTemp = 4900;

	pstAwbSnsDft->au16GainOffset[0] = CALIBRATE_STATIC_WB_R_GAIN;
	pstAwbSnsDft->au16GainOffset[1] = CALIBRATE_STATIC_WB_GR_GAIN;
	pstAwbSnsDft->au16GainOffset[2] = CALIBRATE_STATIC_WB_GB_GAIN;
	pstAwbSnsDft->au16GainOffset[3] = CALIBRATE_STATIC_WB_B_GAIN;

	pstAwbSnsDft->as32WbPara[0] = CALIBRATE_AWB_P1;
	pstAwbSnsDft->as32WbPara[1] = CALIBRATE_AWB_P2;
	pstAwbSnsDft->as32WbPara[2] = CALIBRATE_AWB_Q1;
	pstAwbSnsDft->as32WbPara[3] = CALIBRATE_AWB_A1;
	pstAwbSnsDft->as32WbPara[4] = CALIBRATE_AWB_B1;
	pstAwbSnsDft->as32WbPara[5] = CALIBRATE_AWB_C1;

	pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
	pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

	switch (pstSnsState->enWDRMode) {
	default:
	case WDR_MODE_NONE:
		memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
		memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable,
		       sizeof(AWB_AGC_TABLE_S));
		break;

	case WDR_MODE_2To1_LINE:
		memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcmFsWdr,
		       sizeof(AWB_CCM_S));
		memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableFSWDR,
		       sizeof(AWB_AGC_TABLE_S));
		break;
	}

	pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0];
	pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1];
	pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2];
	pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
	pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];

	return GK_SUCCESS;
}

static GK_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
	CMOS_CHECK_POINTER(pstExpFuncs);

	memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

	pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

	return GK_SUCCESS;
}

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = { { 378, 256, 430 },
						       { 439, 256, 439 } };

static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstDef);
	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));
#ifdef CONFIG_ISP_CA_SUPPORT
	pstDef->unKey.bit1Ca = 1;
	pstDef->pstCa = &g_stIspCA;
#endif

	pstDef->unKey.bit1Dpc = 1;
	pstDef->pstDpc = &g_stCmosDpc;

	pstDef->unKey.bit1Wdr = 1;
	pstDef->pstWdr = &g_stIspWDR;
	pstDef->unKey.bit1Lsc = 0;
	pstDef->pstLsc = &g_stCmosLsc;
	pstDef->unKey.bit1Ge = 1;
	pstDef->pstGe = &g_stIspGe;

	switch (pstSnsState->enWDRMode) {
	default:
	case WDR_MODE_NONE:
		pstDef->unKey.bit1Demosaic = 1;
		pstDef->pstDemosaic = &g_stIspDemosaic;
		pstDef->unKey.bit1Sharpen = 1;
		pstDef->pstSharpen = &g_stIspYuvSharpen;
		pstDef->unKey.bit1Drc = 1;
		pstDef->pstDrc = &g_stIspDRC;
		pstDef->unKey.bit1Gamma = 1;
		pstDef->pstGamma = &g_stIspGamma;
		pstDef->unKey.bit1BayerNr = 1;
		pstDef->pstBayerNr = &g_stIspBayerNr;
		pstDef->unKey.bit1AntiFalseColor = 1;
		pstDef->pstAntiFalseColor = &g_stIspAntiFalseColor;
		pstDef->unKey.bit1Ldci = 1;
		pstDef->pstLdci = &g_stIspLdci;
		pstDef->unKey.bit1Dehaze = 1;
		pstDef->pstDehaze = &g_stIspDehaze;
		pstDef->unKey.bit1Lcac = 0;
		pstDef->pstLcac = &g_stIspLCac;
		memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibration,
		       sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
		break;

	case WDR_MODE_2To1_LINE:
		pstDef->unKey.bit1Demosaic = 1;
		pstDef->pstDemosaic = &g_stIspDemosaicWdr;
		pstDef->unKey.bit1Sharpen = 1;
		pstDef->pstSharpen = &g_stIspYuvSharpenWdr;
		pstDef->unKey.bit1Drc = 1;
		pstDef->pstDrc = &g_stIspDRCWDR;
		pstDef->unKey.bit1Gamma = 1;
		pstDef->pstGamma = &g_stIspGammaFSWDR;
		pstDef->unKey.bit1PreGamma = 0;
		pstDef->pstPreGamma = &g_stPreGamma;
		pstDef->unKey.bit1BayerNr = 1;
		pstDef->pstBayerNr = &g_stIspBayerNrWdr2To1;
		pstDef->unKey.bit1Ge = 1;
		pstDef->pstGe = &g_stIspWdrGe;
		pstDef->unKey.bit1AntiFalseColor = 1;
		pstDef->pstAntiFalseColor = &g_stIspWdrAntiFalseColor;
		pstDef->unKey.bit1Ldci = 1;
		pstDef->pstLdci = &g_stIspWdrLdci;
		pstDef->unKey.bit1Dehaze = 1;
		pstDef->pstDehaze = &g_stIspDehazeWDR;
		pstDef->unKey.bit1Lcac = 0;
		pstDef->pstLcac = &g_stIspLCacWdr;
		pstDef->unKey.bit1Rgbir = 0;
		pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x40;

		memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibration,
		       sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
		break;
	}

	pstDef->stSensorMode.u32SensorID = IMX335_ID;
	pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

	memcpy(&pstDef->stDngColorParam, &g_stDngColorParam,
	       sizeof(ISP_CMOS_DNG_COLORPARAM_S));

	switch (pstSnsState->u8ImgMode) {
	default:
	case IMX335_5M_30FPS_12BIT_LINEAR_MODE:
		pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12;
		pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 2592;
		break;

	case IMX335_5M_30FPS_10BIT_WDR_MODE:
		pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;
		pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 2592;
		break;
	case IMX335_4M_30FPS_10BIT_WDR_MODE:
		pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;
		pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 2592;
		break;

	case IMX335_4M_25FPS_10BIT_WDR_MODE:
		pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;
		pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 2560;
		break;
	}

	pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH
		.u32Denominator = 1;
	pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH
		.u32Numerator = 1;
	pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV
		.u32Denominator = 1;
	pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV
		.u32Numerator = 1;
	pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim
		.u16RepeatPatternDimRows = 2;
	pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim
		.u16RepeatPatternDimCols = 2;
	pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatRows = 2;
	pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatCols = 2;
	pstDef->stSensorMode.stDngRawFormat.enCfaLayout =
		CFALAYOUT_TYPE_RECTANGULAR;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[0] = 0;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[1] = 1;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[2] = 2;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 0;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[2] = 1;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[3] = 2;
	pstDef->stSensorMode.bValidDngRawFormat = GK_TRUE;

	return GK_SUCCESS;
}

static GK_S32 cmos_get_isp_black_level(VI_PIPE ViPipe,
				       ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
	GK_S32 i;

	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if (GK_NULL == pstBlackLevel) {
		printf("null pointer when get isp black level value!\n");
		return -1;
	}

	/* Don't need to update black level when iso change */
	pstBlackLevel->bUpdate = GK_TRUE;

	if (GK_TRUE == pstBlackLevel->bUpdate) {
		if (g_u32Imx335AGain[ViPipe] >= 32381) {
			if (g_u32Imx335DGain[ViPipe] < 12288) {
				for (i = 0; i < 4; i++) {
					pstBlackLevel->au16BlackLevel[i] =
						200; /* 12bit,0xC8 */
				}
			} else if ((g_u32Imx335DGain[ViPipe] >= 12288) &&
				   (g_u32Imx335DGain[ViPipe] < 16384)) {
				pstBlackLevel->au16BlackLevel[0] = 217;
				pstBlackLevel->au16BlackLevel[1] = 211;
				pstBlackLevel->au16BlackLevel[2] = 208;
				pstBlackLevel->au16BlackLevel[3] = 216;
			} else if ((g_u32Imx335DGain[ViPipe] >= 16384) &&
				   (g_u32Imx335DGain[ViPipe] < 20480)) {
				pstBlackLevel->au16BlackLevel[0] = 223;
				pstBlackLevel->au16BlackLevel[1] = 228;
				pstBlackLevel->au16BlackLevel[2] = 229;
				pstBlackLevel->au16BlackLevel[3] = 219;
			} else if ((g_u32Imx335DGain[ViPipe] >= 20480) &&
				   (g_u32Imx335DGain[ViPipe] < 24576)) {
				pstBlackLevel->au16BlackLevel[0] = 248;
				pstBlackLevel->au16BlackLevel[1] = 255;
				pstBlackLevel->au16BlackLevel[2] = 256;
				pstBlackLevel->au16BlackLevel[3] = 244;
			} else if ((g_u32Imx335DGain[ViPipe] >= 24576) &&
				   (g_u32Imx335DGain[ViPipe] < 28672)) {
				pstBlackLevel->au16BlackLevel[0] = 277;
				pstBlackLevel->au16BlackLevel[1] = 283;
				pstBlackLevel->au16BlackLevel[2] = 285;
				pstBlackLevel->au16BlackLevel[3] = 272;
			} else if ((g_u32Imx335DGain[ViPipe] >= 28672) &&
				   (g_u32Imx335DGain[ViPipe] < 31768)) {
				pstBlackLevel->au16BlackLevel[0] = 303;
				pstBlackLevel->au16BlackLevel[1] = 308;
				pstBlackLevel->au16BlackLevel[2] = 310;
				pstBlackLevel->au16BlackLevel[3] = 295;
			} else {
				pstBlackLevel->au16BlackLevel[0] = 330;
				pstBlackLevel->au16BlackLevel[1] = 338;
				pstBlackLevel->au16BlackLevel[2] = 339;
				pstBlackLevel->au16BlackLevel[3] = 323;
			}
		} else {
			for (i = 0; i < 4; i++) {
				pstBlackLevel->au16BlackLevel[i] =
					200; /* 12bit,0xC8 */
			}
		}
	} else {
		if (WDR_MODE_NONE == pstSnsState->enWDRMode) {
			for (i = 0; i < 4; i++) {
				pstBlackLevel->au16BlackLevel[i] =
					200; /* 12bit,0xC8 */
			}
		} else if (WDR_MODE_2To1_LINE == pstSnsState->enWDRMode) {
			for (i = 0; i < 4; i++) {
				pstBlackLevel->au16BlackLevel[i] =
					200; /* 10bit 50, 12bit 200 */
			}
		} else {
			for (i = 0; i < 4; i++) {
				pstBlackLevel->au16BlackLevel[i] =
					200; /* 10bit 50, 12bit 200 */
			}
		}
	}

	return 0;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
	GK_U32 u32FullLines_5Fps = 0;
	GK_U32 u32MaxIntTime_5Fps = 0;

	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (WDR_MODE_2To1_LINE == pstSnsState->enWDRMode) {
		return;
	} else {
		if (IMX335_5M_30FPS_12BIT_LINEAR_MODE ==
		    pstSnsState->u8ImgMode) {
			u32FullLines_5Fps =
				(IMX335_VMAX_5M_30FPS_12BIT_LINEAR * 30) / 5;
		} else {
			return;
		}
	}

	u32MaxIntTime_5Fps = u32FullLines_5Fps - 8;

	if (bEnable) { /* setup for ISP pixel calibration mode */
		IMX335_write_register(ViPipe, IMX335_GAIN_LONG_LOW,
				      0x00); // gain
		IMX335_write_register(ViPipe, IMX335_GAIN_LONG_HIGH, 0x00);

		IMX335_write_register(
			ViPipe, IMX335_VMAX_ADDR_L,
			IMX335_LOW_8BITS(u32FullLines_5Fps)); // VMAX
		IMX335_write_register(ViPipe, IMX335_VMAX_ADDR_M,
				      IMX335_MID_8BITS(u32FullLines_5Fps));
		IMX335_write_register(ViPipe, IMX335_VMAX_ADDR_H,
				      IMX335_HIG_4BITS(u32FullLines_5Fps));

		IMX335_write_register(ViPipe, IMX335_SHR0_LOW,
				      IMX335_LOW_8BITS(u32MaxIntTime_5Fps));
		IMX335_write_register(ViPipe, IMX335_SHR0_MIDDLE,
				      IMX335_MID_8BITS(u32MaxIntTime_5Fps));
		IMX335_write_register(ViPipe, IMX335_SHR0_MIDDLE,
				      IMX335_HIG_4BITS(u32MaxIntTime_5Fps));
	} else { /* setup for ISP 'normal mode' */
		pstSnsState->u32FLStd =
			(pstSnsState->u32FLStd > IMX335_FULL_LINES_MAX) ?
				IMX335_FULL_LINES_MAX :
				pstSnsState->u32FLStd;
		IMX335_write_register(ViPipe, IMX335_VMAX_ADDR_L,
				      IMX335_LOW_8BITS(pstSnsState->u32FLStd));
		IMX335_write_register(ViPipe, IMX335_VMAX_ADDR_M,
				      IMX335_MID_8BITS(pstSnsState->u32FLStd));
		IMX335_write_register(ViPipe, IMX335_VMAX_ADDR_H,
				      IMX335_HIG_4BITS(pstSnsState->u32FLStd));
		pstSnsState->bSyncInit = GK_FALSE;
	}

	return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	pstSnsState->bSyncInit = GK_FALSE;

	switch (u8Mode & 0x3F) {
	case WDR_MODE_NONE:
		pstSnsState->enWDRMode = WDR_MODE_NONE;
		printf("linear mode\n");
		break;

	case WDR_MODE_2To1_LINE:
		pstSnsState->enWDRMode = WDR_MODE_2To1_LINE;
		printf("2to1 line WDR mode\n");
		break;

	default:
		ISP_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
		return GK_FAILURE;
	}

	memset(pstSnsState->au32WDRIntTime, 0,
	       sizeof(pstSnsState->au32WDRIntTime));

	return GK_SUCCESS;
}

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe,
				     ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
	GK_S32 i;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstSnsRegsInfo);
	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if ((GK_FALSE == pstSnsState->bSyncInit) ||
	    (GK_FALSE == pstSnsRegsInfo->bConfig)) {
		pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
		pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
			g_aunImx335BusInfo[ViPipe].s8I2cDev;
		pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
		pstSnsState->astRegsInfo[0].u32RegNum = 8;

		if (WDR_MODE_2To1_LINE == pstSnsState->enWDRMode) {
			pstSnsState->astRegsInfo[0].u32RegNum += 8;
		}

		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr =
				imx335_i2c_addr;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32AddrByteNum = imx335_addr_byte;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32DataByteNum = imx335_data_byte;
		}

		// shutter related
		pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr =
			IMX335_SHR0_LOW; // SHR0
		pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr =
			IMX335_SHR0_MIDDLE;
		pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr =
			IMX335_SHR0_HIGH;

		pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum =
			0; // Long Gain
		pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr =
			IMX335_GAIN_LONG_LOW;
		pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr =
			IMX335_GAIN_LONG_HIGH;

		// Vmax
		pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
			IMX335_VMAX_ADDR_L;
		pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr =
			IMX335_VMAX_ADDR_M;
		pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr =
			IMX335_VMAX_ADDR_H;

		if (WDR_MODE_2To1_LINE == pstSnsState->enWDRMode) {
			pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum =
				1;

			pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
				IMX335_SHR1_LOW; // SHR1
			pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr =
				IMX335_SHR1_MIDDLE;
			pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr =
				IMX335_SHR1_HIGH;

			pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum =
				1;
			pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr =
				IMX335_VMAX_ADDR_L; // VMAX wdr
			pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum =
				1;
			pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr =
				IMX335_VMAX_ADDR_M;
			pstSnsState->astRegsInfo[0]
				.astI2cData[10]
				.u8DelayFrmNum = 1;
			pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr =
				IMX335_VMAX_ADDR_H;

			pstSnsState->astRegsInfo[0]
				.astI2cData[11]
				.u8DelayFrmNum = 0;
			pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr =
				IMX335_RHS1_LOW; // RHS1
			pstSnsState->astRegsInfo[0]
				.astI2cData[12]
				.u8DelayFrmNum = 0;
			pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr =
				IMX335_RHS1_MIDDLE;
			pstSnsState->astRegsInfo[0]
				.astI2cData[13]
				.u8DelayFrmNum = 0;
			pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr =
				IMX335_RHS1_HIGH;

			pstSnsState->astRegsInfo[0]
				.astI2cData[14]
				.u8DelayFrmNum = 0;
			pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr =
				IMX335_GAIN_SHORT_LOW; // Short Gain
			pstSnsState->astRegsInfo[0]
				.astI2cData[15]
				.u8DelayFrmNum = 1;
			pstSnsState->astRegsInfo[0].astI2cData[15].u32RegAddr =
				IMX335_GAIN_SHORT_HIGH;
		}

		pstSnsState->bSyncInit = GK_TRUE;
	} else {
		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data ==
			    pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
				pstSnsState->astRegsInfo[0]
					.astI2cData[i]
					.bUpdate = GK_FALSE;
			}

			else {
				pstSnsState->astRegsInfo[0]
					.astI2cData[i]
					.bUpdate = GK_TRUE;
			}
		}
	}
	pstSnsRegsInfo->bConfig = GK_FALSE;

	memcpy(pstSnsRegsInfo, &pstSnsState->astRegsInfo[0],
	       sizeof(ISP_SNS_REGS_INFO_S));
	memcpy(&pstSnsState->astRegsInfo[1], &pstSnsState->astRegsInfo[0],
	       sizeof(ISP_SNS_REGS_INFO_S));

	pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

	return GK_SUCCESS;
}

static GK_S32
cmos_set_image_mode(VI_PIPE ViPipe,
		    ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	GK_U8 u8SensorImageMode = 0;

	CMOS_CHECK_POINTER(pstSensorImageMode);
	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	pstSnsState->bSyncInit = GK_FALSE;

	//printf("Lation@ cmos_set_image_mode width: %d, height: %d\n", pstSensorImageMode->u16Width, pstSensorImageMode->u16Height);
	if (WDR_MODE_2To1_LINE == pstSnsState->enWDRMode) {
		if (IMX335_RES_IS_4M_10BIT_WDR_EX(
			    pstSensorImageMode->u16Width,
			    pstSensorImageMode->u16Height)) {
			u8SensorImageMode = IMX335_4M_25FPS_10BIT_WDR_MODE;
			g_astimx335State[ViPipe].u32BRL = 1480 * 2;
			pstSnsState->u32FLStd = IMX335_VMAX_5M_30FPS_10BIT_WDR;
		} else if (IMX335_RES_IS_5M_10BIT_WDR(
				   pstSensorImageMode->u16Width,
				   pstSensorImageMode->u16Height)) {
			u8SensorImageMode = IMX335_5M_30FPS_10BIT_WDR_MODE;
			g_astimx335State[ViPipe].u32BRL = 1984 * 2;
			pstSnsState->u32FLStd = IMX335_VMAX_5M_30FPS_10BIT_WDR;
		} else if (IMX335_RES_IS_4M_10BIT_WDR(
				   pstSensorImageMode->u16Width,
				   pstSensorImageMode->u16Height)) {
			u8SensorImageMode = IMX335_4M_30FPS_10BIT_WDR_MODE;
			g_astimx335State[ViPipe].u32BRL = 1548 * 2;
			pstSnsState->u32FLStd = IMX335_VMAX_4M_30FPS_10BIT_WDR;
		} else {
			IMX335_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
			return GK_FAILURE;
		}
	} else if (WDR_MODE_NONE == pstSnsState->enWDRMode) {
		if (IMX335_RES_IS_5M_12BIT_LINEAR(
			    pstSensorImageMode->u16Width,
			    pstSensorImageMode->u16Height) ||
		    IMX335_RES_IS_4M_12BIT_LINEAR(
			    pstSensorImageMode->u16Width,
			    pstSensorImageMode->u16Height)) {
			u8SensorImageMode = IMX335_5M_30FPS_12BIT_LINEAR_MODE;
			g_astimx335State[ViPipe].u32BRL = 1984 * 2;
			pstSnsState->u32FLStd =
				IMX335_VMAX_5M_30FPS_12BIT_LINEAR;
		} else {
			IMX335_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
			return GK_FAILURE;
		}
	} else {
		IMX335_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
		return GK_FAILURE;
	}

	if ((GK_TRUE == pstSnsState->bInit) &&
	    (u8SensorImageMode == pstSnsState->u8ImgMode)) {
		/* Don't need to switch SensorImageMode */
		return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
	}

	pstSnsState->u8ImgMode = u8SensorImageMode;
	pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
	pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

	return GK_SUCCESS;
}

static GK_VOID sensor_global_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	IMX335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->bInit = GK_FALSE;
	pstSnsState->bSyncInit = GK_FALSE;
	pstSnsState->u8ImgMode = IMX335_5M_30FPS_12BIT_LINEAR_MODE;
	pstSnsState->enWDRMode = WDR_MODE_NONE;
	pstSnsState->u32FLStd = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;
	pstSnsState->au32FL[0] = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;
	pstSnsState->au32FL[1] = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;

	memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
	memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static GK_S32
cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	CMOS_CHECK_POINTER(pstSensorExpFunc);

	memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

	pstSensorExpFunc->pfn_cmos_sensor_init = IMX335_init;
	pstSensorExpFunc->pfn_cmos_sensor_exit = IMX335_exit;
	pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
	pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
	pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;
	pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
	pstSensorExpFunc->pfn_cmos_get_isp_black_level =
		cmos_get_isp_black_level;
	pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
	pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

	return GK_SUCCESS;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

static GK_S32 IMX335_set_bus_info(VI_PIPE ViPipe,
				  ISP_SNS_COMMBUS_U unSNSBusInfo)
{
	g_aunImx335BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

	return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	IMX335_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

	if (GK_NULL == pastSnsStateCtx) {
		pastSnsStateCtx =
			(ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
		if (GK_NULL == pastSnsStateCtx) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Isp[%d] SnsCtx malloc memory failed!\n",
				  ViPipe);
			return ERR_CODE_ISP_NOMEM;
		}
	}

	memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

	IMX335_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

	return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	IMX335_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
	SENSOR_FREE(pastSnsStateCtx);
	IMX335_SENSOR_RESET_CTX(ViPipe);
}

static GK_S32 sensor_register_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
				       ALG_LIB_S *pstAwbLib)
{
	GK_S32 s32Ret;

	ISP_SENSOR_REGISTER_S stIspRegister;
	AE_SENSOR_REGISTER_S stAeRegister;
	AWB_SENSOR_REGISTER_S stAwbRegister;
	ISP_SNS_ATTR_INFO_S stSnsAttrInfo;

	CMOS_CHECK_POINTER(pstAeLib);
	CMOS_CHECK_POINTER(pstAwbLib);

	s32Ret = sensor_ctx_init(ViPipe);

	if (GK_SUCCESS != s32Ret) {
		return GK_FAILURE;
	}

	stSnsAttrInfo.eSensorId = IMX335_ID;

	s32Ret = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
	s32Ret |= GK_API_ISP_SensorRegCallBack(ViPipe, &stSnsAttrInfo,
					       &stIspRegister);

	if (GK_SUCCESS != s32Ret) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor register callback function failed!\n");
		return s32Ret;
	}

	s32Ret = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
	s32Ret |= GK_API_AE_SensorRegCallBack(ViPipe, pstAeLib, &stSnsAttrInfo,
					      &stAeRegister);

	if (GK_SUCCESS != s32Ret) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor register callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
	s32Ret |= GK_API_AWB_SensorRegCallBack(ViPipe, pstAwbLib,
					       &stSnsAttrInfo, &stAwbRegister);

	if (GK_SUCCESS != s32Ret) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor register callback function to awb lib failed!\n");
		return s32Ret;
	}

	return GK_SUCCESS;
}

static GK_S32 sensor_unregister_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
					 ALG_LIB_S *pstAwbLib)
{
	GK_S32 s32Ret;

	CMOS_CHECK_POINTER(pstAeLib);
	CMOS_CHECK_POINTER(pstAwbLib);

	s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, IMX335_ID);

	if (GK_SUCCESS != s32Ret) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor unregister callback function failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, IMX335_ID);

	if (GK_SUCCESS != s32Ret) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, IMX335_ID);

	if (GK_SUCCESS != s32Ret) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to awb lib failed!\n");
		return s32Ret;
	}

	sensor_ctx_exit(ViPipe);

	return GK_SUCCESS;
}

static GK_S32 sensor_set_init(VI_PIPE ViPipe, ISP_INIT_ATTR_S *pstInitAttr)
{
	CMOS_CHECK_POINTER(pstInitAttr);

	g_au32InitExposure[ViPipe] = pstInitAttr->u32Exposure;
	g_au32LinesPer500ms[ViPipe] = pstInitAttr->u32LinesPer500ms;
	g_au16InitWBGain[ViPipe][0] = pstInitAttr->u16WBRgain;
	g_au16InitWBGain[ViPipe][1] = pstInitAttr->u16WBGgain;
	g_au16InitWBGain[ViPipe][2] = pstInitAttr->u16WBBgain;
	g_au16SampleRgain[ViPipe] = pstInitAttr->u16SampleRgain;
	g_au16SampleBgain[ViPipe] = pstInitAttr->u16SampleBgain;

	return GK_SUCCESS;
}

ISP_SNS_OBJ_S stSnsImx335Obj = {
	.pfnRegisterCallback = sensor_register_callback,
	.pfnUnRegisterCallback = sensor_unregister_callback,
	.pfnStandby = IMX335_standby,
	.pfnRestart = IMX335_restart,
	.pfnMirrorFlip = GK_NULL,
	.pfnWriteReg = IMX335_write_register,
	.pfnReadReg = IMX335_read_register,
	.pfnSetBusInfo = IMX335_set_bus_info,
	.pfnSetInit = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __IMX335_CMOS_H_ */
