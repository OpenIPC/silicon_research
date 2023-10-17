/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __COMM_SYS_H__
#define __COMM_SYS_H__

#include "type.h"
#include "errcode.h"
#include "mpp_debug.h"
#include "comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define BIND_DEST_MAXNUM 64

#define UNIQUE_ID_NUM 6

#define MODULE_TRACE_SYS(level, fmt, ...)                                     \
	do {                                                                  \
		MODULE_TRACE(level, MOD_ID_SYS,                               \
			     "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, \
			     __LINE__, ##__VA_ARGS__);                        \
	} while (0)

typedef struct VPSS_VENC_WRAP_PARAM_S {
	GK_BOOL bAllOnline;

	GK_U32 u32FrameRate;
	GK_U32 u32FullLinesStd;

	SIZE_S stLargeStreamSize;
	SIZE_S stSmallStreamSize;

} VPSS_VENC_WRAP_PARAM_S;

typedef struct MPP_SYS_CONFIG_S {
	GK_U32 u32Align;

} MPP_SYS_CONFIG_S;

typedef struct SYS_VIRMEM_INFO_S {
	GK_U64 u64PhyAddr;
	GK_BOOL bCached;
} SYS_VIRMEM_INFO_S;

typedef struct MPP_BIND_DEST_S {
	GK_U32 u32Num;
	MPP_CHN_S astMppChn[BIND_DEST_MAXNUM];
} MPP_BIND_DEST_S;

typedef enum SCALE_RANGE_E {
	SCALE_RANGE_0 = 0,
	SCALE_RANGE_1,
	SCALE_RANGE_2,
	SCALE_RANGE_3,
	SCALE_RANGE_4,
	SCALE_RANGE_5,
	SCALE_RANGE_6,
	SCALE_RANGE_7,
	SCALE_RANGE_8,
	SCALE_RANGE_9,
	SCALE_RANGE_10,
	SCALE_RANGE_11,
	SCALE_RANGE_12,
	SCALE_RANGE_13,
	SCALE_RANGE_14,
	SCALE_RANGE_15,
	SCALE_RANGE_16,
	SCALE_RANGE_17,
	SCALE_RANGE_18,
	SCALE_RANGE_BUTT,
} SCALE_RANGE_E;

typedef enum COEFF_LEVEL_E {
	COEFF_LEVEL_0 = 0,
	COEFF_LEVEL_1,
	COEFF_LEVEL_2,
	COEFF_LEVEL_3,
	COEFF_LEVEL_4,
	COEFF_LEVEL_5,
	COEFF_LEVEL_6,
	COEFF_LEVEL_7,
	COEFF_LEVEL_8,
	COEFF_LEVEL_9,
	COEFF_LEVEL_10,
	COEFF_LEVEL_11,
	COEFF_LEVEL_12,
	COEFF_LEVEL_13,
	COEFF_LEVEL_14,
	COEFF_LEVEL_15,
	COEFF_LEVEL_16,
	COEFF_LEVEL_17,
	COEFF_LEVEL_18,
	COEFF_LEVEL_BUTT,
} COEFF_LEVEL_E;

typedef struct SCALE_COEFF_LEVEL_S {
	COEFF_LEVEL_E enHorLum;
	COEFF_LEVEL_E enHorChr;
	COEFF_LEVEL_E enVerLum;
	COEFF_LEVEL_E enVerChr;
} SCALE_COEFF_LEVEL_S;

typedef struct SCALE_RANGE_S {
	SCALE_RANGE_E enHorizontal;
	SCALE_RANGE_E enVertical;
} SCALE_RANGE_S;

typedef struct SCALE_COEFF_INFO_S {
	SCALE_RANGE_S stScaleRange;
	SCALE_COEFF_LEVEL_S stScaleCoeffLevel;
} SCALE_COEFF_INFO_S;

typedef struct GPS_INFO_S {
	GK_CHAR chGPSLatitudeRef;
	GK_U32 au32GPSLatitude[3][2];
	GK_CHAR chGPSLongitudeRef;
	GK_U32 au32GPSLongitude[3][2];
	GK_U8 u8GPSAltitudeRef;
	GK_U32 au32GPSAltitude[2];
} GPS_INFO_S;

typedef enum VI_VPSS_MODE_E {
	VI_OFFLINE_VPSS_OFFLINE = 0,
	VI_OFFLINE_VPSS_ONLINE,
	VI_ONLINE_VPSS_OFFLINE,
	VI_ONLINE_VPSS_ONLINE,
	VI_PARALLEL_VPSS_OFFLINE,
	VI_PARALLEL_VPSS_PARALLEL,
	VI_VPSS_MODE_BUTT
} VI_VPSS_MODE_E;

typedef struct VI_VPSS_MODE_S {
	VI_VPSS_MODE_E aenMode[VI_MAX_PIPE_NUM];
} VI_VPSS_MODE_S;

typedef struct RAW_FRAME_COMPRESS_PARAM_S {
	GK_U32 u32CompRatio_10Bit;
	GK_U32 u32CompRatio_12Bit;
	GK_U32 u32CompRatio_14Bit;
} RAW_FRAME_COMPRESS_PARAM_S;

typedef struct UNIQUE_ID_S {
	GK_U32 u32Id[UNIQUE_ID_NUM];
} UNIQUE_ID_S;

#define ERR_CODE_SYS_NULL_PTR \
	DEFINE_ERR_CODE(MOD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_SYS_NOTREADY \
	DEFINE_ERR_CODE(MOD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define ERR_CODE_SYS_NOT_PERM \
	DEFINE_ERR_CODE(MOD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define ERR_CODE_SYS_NOMEM \
	DEFINE_ERR_CODE(MOD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define ERR_CODE_SYS_ILLEGAL_PARAM \
	DEFINE_ERR_CODE(MOD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_SYS_BUSY \
	DEFINE_ERR_CODE(MOD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define ERR_CODE_SYS_NOT_SUPPORT \
	DEFINE_ERR_CODE(MOD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
