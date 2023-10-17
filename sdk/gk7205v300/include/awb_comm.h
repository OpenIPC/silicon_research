/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __ISP_AWB_COMM_H__
#define __ISP_AWB_COMM_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ISP_AWB_LIB_NAME "awb_lib"

typedef struct AWB_DBG_ATTR_S {
	GK_U16 u16WhiteLevel;
	GK_U16 u16BlackLevel;
	GK_U16 u16CrMax;
	GK_U16 u16CrMin;
	GK_U16 u16CbMax;
	GK_U16 u16CbMin;
	GK_U16 u16CrHigh;
	GK_U16 u16CrLow;
	GK_U16 u16CbHigh;
	GK_U16 u16CbLow;
	GK_U16 u16RawWhiteLevel;
	GK_U16 u16RawBlackLevel;
	GK_U16 u16RawCrMax;
	GK_U16 u16RawCrMin;
	GK_U16 u16RawCbMax;
	GK_U16 u16RawCbMin;
	GK_U16 u16RawCrHigh;
	GK_U16 u16RawCrLow;
	GK_U16 u16RawCbHigh;
	GK_U16 u16RawCbLow;

	GK_U16 u16WDRMode;
	GK_U16 u16Enable;
	GK_U16 u16ManualEnable;
	GK_U16 u16Zone;
	GK_U16 u16HighTemp;
	GK_U16 u16LowTemp;
	GK_U16 u16RefTemp;
	GK_U16 u16RgainBase;
	GK_U16 u16GgainBase;
	GK_U16 u16BgainBase;
	GK_S32 s32p1;
	GK_S32 s32p2;
	GK_S32 s32q;
	GK_S32 s32a;
	GK_S32 s32c;

	GK_U16 u16ManSatEnable;
	GK_U16 u16SatTarget;
} AWB_DBG_ATTR_S;

typedef struct AWB_ZONE_DBG_S {
	GK_U16 u16Sum;
	GK_U16 u16Rg;
	GK_U16 u16Bg;
	GK_U16 u16CountAll;
	GK_U16 u16CountMin;
	GK_U16 u16CountMax;
	GK_U16 u16RawRAvg;
	GK_U16 u16RawGAvg;
	GK_U16 u16RawBAvg;
	GK_U16 u16TK;
	GK_U16 u16Weight;
	GK_S16 s16Shift;
} AWB_ZONE_DBG_S;

typedef struct AWB_DBG_STATUS_S {
	GK_U32 u32FrmNumBgn;
	GK_U32 u32GlobalSum;
	GK_U16 u16GlobalRgSta;
	GK_U16 u16GlobalBgSta;
	GK_U16 u16GlobalCountAll;
	GK_U16 u16GlobalCountMin;
	GK_U16 u16GlobalCountMax;
	GK_U16 u16GlobalRAvg;
	GK_U16 u16GlobalGAvg;
	GK_U16 u16GlobalBAvg;
	GK_U16 u16TK;
	GK_U16 u16Rgain;
	GK_U16 u16Ggain;
	GK_U16 u16Bgain;
	GK_U16 au16CCM[CCM_MATRIX_SIZE];

	GK_U32 au32HistInfo[256];
	AWB_ZONE_DBG_S astZoneDebug[AWB_ZONE_NUM];

	GK_U32 u32FrmNumEnd;
} AWB_DBG_STATUS_S;

typedef struct AWB_CCM_TAB_S {
	GK_U16 u16ColorTemp;
	GK_U16 au16CCM[CCM_MATRIX_SIZE];
} AWB_CCM_TAB_S;

typedef struct AWB_CCM_S {
	GK_U16 u16CCMTabNum;
	AWB_CCM_TAB_S astCCMTab[CCM_MATRIX_NUM];
} AWB_CCM_S;

typedef struct AWB_AGC_TABLE_S {
	GK_BOOL bValid;

	GK_U8 au8Saturation[ISP_AUTO_ISO_STRENGTH_NUM];
} AWB_AGC_TABLE_S;

typedef struct AWB_SENSOR_DEFAULT_S {
	GK_U16 u16WbRefTemp;
	GK_U16 au16GainOffset[ISP_BAYER_CHN_NUM];
	GK_S32 as32WbPara[AWB_CURVE_PARA_NUM];

	GK_U16 u16GoldenRgain;
	GK_U16 u16GoldenBgain;
	GK_U16 u16SampleRgain;
	GK_U16 u16SampleBgain;
	AWB_AGC_TABLE_S stAgcTbl;
	AWB_CCM_S stCcm;
	GK_U16 u16InitRgain;
	GK_U16 u16InitGgain;
	GK_U16 u16InitBgain;
	GK_U8 u8AWBRunInterval;
	GK_U16 au16InitCCM[CCM_MATRIX_SIZE];
} AWB_SENSOR_DEFAULT_S;

typedef struct AWB_SPEC_SENSOR_DEFAULT_S {
	ISP_SPECAWB_ATTR_S stSpecAwbAttrs;
	ISP_SPECAWB_CAA_CONTROl_S stCaaControl;
} AWB_SPEC_SENSOR_DEFAULT_S;

typedef struct AWB_SENSOR_EXP_FUNC_S {
	GK_S32 (*pfn_cmos_get_awb_default)
	(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft);
	GK_S32 (*pfn_cmos_get_awb_spec_default)
	(VI_PIPE ViPipe, AWB_SPEC_SENSOR_DEFAULT_S *pstAwbSpecSnsDft);
} AWB_SENSOR_EXP_FUNC_S;

typedef struct AWB_SENSOR_REGISTER_S {
	AWB_SENSOR_EXP_FUNC_S stSnsExp;
} AWB_SENSOR_REGISTER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
