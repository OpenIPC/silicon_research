/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __ISP_AE_COMM_H__
#define __ISP_AE_COMM_H__

#include "type.h"
#include "comm_isp.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ISP_AE_LIB_NAME "ae_lib"

typedef enum AE_CTRL_CMD_E {
    AE_DEBUG_ATTR_SET,
    AE_DEBUG_ATTR_GET,

    AE_CTRL_BUTT,
} AE_CTRL_CMD_E;

typedef struct AE_DBG_ATTR_S {
    GK_U32  u32MaxIntTime;
    GK_U32  u32MinIntTime;
    GK_U32  u32MaxAgain;
    GK_U32  u32MinAgain;
    GK_U32  u32MaxDgain;
    GK_U32  u32MinDgain;
    GK_U32  u32MaxIspDgain;
    GK_U32  u32MinIspDgain;
    GK_U32  u32MaxSysGain;
    GK_U32  u32MinSysGain;
    GK_U32  u32Compensation;
    GK_U32  u32EVBias;
    GK_BOOL bManualExposureEn;
    GK_BOOL bManualTimeEn;
    GK_BOOL bManualAgainEn;
    GK_BOOL bManualDgainEn;
    GK_BOOL bManualIspDgainEn;
    GK_U32  u32ManualExposureLines;
    GK_U32  u32ManualAgain;
    GK_U32  u32ManualDgain;
    GK_U32  u32ManualIspDgain;
    GK_U32  au32AeWeights[AE_ZONE_ROW *AE_ZONE_COLUMN];
} AE_DBG_ATTR_S;

typedef struct AE_DBG_STATUS_S {
    GK_U32  u32FrmNumBgn;
    GK_U32  u32FullLines;
    GK_U32  u32IntTime;
    GK_U32  u32ShortIntTime;
    GK_U32  u32MedIntTime;
    GK_U32  u32LongIntTime;
    GK_U32  u32Again;
    GK_U32  u32Dgain;
    GK_U32  u32IspDgain;
    GK_U32  u32IrisFNOLin;
    GK_U64  u64Exposure;
    GK_U32  u32Increment;
    GK_S32  s32HistError;
    GK_S32  s32HistOriAverage;
    GK_S32  s32LumaOffset;
    GK_U32  u32Iso;
    GK_U32  u32ExpRatio;
    GK_U32  u32OverExpRatio;
    GK_U32  u32OverExpRatioFilter;
} AE_DBG_STATUS_S;

typedef enum AE_ACCURACY_E {
    AE_ACCURACY_DB = 0,
    AE_ACCURACY_LINEAR,
    AE_ACCURACY_TABLE,

    AE_ACCURACY_BUTT,
} AE_ACCURACY_E;

typedef struct AE_ACCURACY_S {
    AE_ACCURACY_E enAccuType;
    float   f32Accuracy;
    float   f32Offset;
} AE_ACCURACY_S;

typedef struct AE_SENSOR_DEFAULT_S {
    GK_U8   au8HistThresh[HIST_THRESH_NUM];
    GK_U8   u8AeCompensation;

    GK_U32  u32LinesPer500ms;
    GK_U32  u32FlickerFreq;
    GK_FLOAT f32Fps;
    GK_U32  u32HmaxTimes;
    GK_U32  u32InitExposure;
    GK_U32  u32InitAESpeed;
    GK_U32  u32InitAETolerance;

    GK_U32  u32FullLinesStd;
    GK_U32  u32FullLinesMax;
    GK_U32  u32FullLines;
    GK_U32  u32MaxIntTime;
    GK_U32  u32MinIntTime;
    GK_U32  u32MaxIntTimeTarget;
    GK_U32  u32MinIntTimeTarget;
    AE_ACCURACY_S stIntTimeAccu;

    GK_U32  u32MaxAgain;
    GK_U32  u32MinAgain;
    GK_U32  u32MaxAgainTarget;
    GK_U32  u32MinAgainTarget;
    AE_ACCURACY_S stAgainAccu;

    GK_U32  u32MaxDgain;
    GK_U32  u32MinDgain;
    GK_U32  u32MaxDgainTarget;
    GK_U32  u32MinDgainTarget;
    AE_ACCURACY_S stDgainAccu;

    GK_U32  u32MaxISPDgainTarget;
    GK_U32  u32MinISPDgainTarget;
    GK_U32  u32ISPDgainShift;

    GK_U32  u32MaxIntTimeStep;
    GK_U32  u32LFMaxShortTime;
    GK_U32  u32LFMinExposure;

    ISP_AE_ROUTE_S stAERouteAttr;
    GK_BOOL bAERouteExValid;
    ISP_AE_ROUTE_EX_S stAERouteAttrEx;

    ISP_AE_ROUTE_S stAERouteSFAttr;
    ISP_AE_ROUTE_EX_S stAERouteSFAttrEx;

    GK_U16 u16ManRatioEnable;
    GK_U32 au32Ratio[EXP_RATIO_NUM];

    ISP_IRIS_TYPE_E  enIrisType;
    ISP_PIRIS_ATTR_S stPirisAttr;
    ISP_IRIS_F_NO_E  enMaxIrisFNO;
    ISP_IRIS_F_NO_E  enMinIrisFNO;

    ISP_AE_STRATEGY_E enAeExpMode;

    GK_U16 u16ISOCalCoef;
    GK_U8  u8AERunInterval;
    GK_U32 u32ExpRatioMax;
    GK_U32 u32ExpRatioMin;
    GK_BOOL bDiffGainSupport;
} AE_SENSOR_DEFAULT_S;

typedef struct AE_FSWDR_ATTR_S {
    ISP_FSWDR_MODE_E enFSWDRMode;
} AE_FSWDR_ATTR_S;

typedef struct AE_SENSOR_EXP_FUNC_S {
    GK_S32 (*pfn_cmos_get_ae_default)(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft);

    GK_VOID (*pfn_cmos_fps_set)(VI_PIPE ViPipe, GK_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft);
    GK_VOID (*pfn_cmos_slow_framerate_set)(VI_PIPE ViPipe, GK_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft);

    GK_VOID (*pfn_cmos_inttime_update)(VI_PIPE ViPipe, GK_U32 u32IntTime);
    GK_VOID (*pfn_cmos_gains_update)(VI_PIPE ViPipe, GK_U32 u32Again, GK_U32 u32Dgain);

    GK_VOID (*pfn_cmos_again_calc_table)(VI_PIPE ViPipe, GK_U32 *pu32AgainLin, GK_U32 *pu32AgainDb);
    GK_VOID (*pfn_cmos_dgain_calc_table)(VI_PIPE ViPipe, GK_U32 *pu32DgainLin, GK_U32 *pu32DgainDb);

    GK_VOID (*pfn_cmos_get_inttime_max)(VI_PIPE ViPipe, GK_U16 u16ManRatioEnable, GK_U32 *au32Ratio, GK_U32 *au32IntTimeMax, GK_U32 *au32IntTimeMin, GK_U32 *pu32LFMaxIntTime);

    GK_VOID (*pfn_cmos_ae_fswdr_attr_set)(VI_PIPE ViPipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr);

} AE_SENSOR_EXP_FUNC_S;

typedef struct AE_SENSOR_REGISTER_S {
    AE_SENSOR_EXP_FUNC_S stSnsExp;
} AE_SENSOR_REGISTER_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
