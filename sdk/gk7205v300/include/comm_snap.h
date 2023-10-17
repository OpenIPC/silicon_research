/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __COMM_SNAP_H__
#define __COMM_SNAP_H__

#include "common.h"
#include "comm_video.h"
#include "comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum SNAP_TYPE_E {
	SNAP_TYPE_NORMAL,
	SNAP_TYPE_PRO,
	SNAP_TYPE_BUTT
} SNAP_TYPE_E;

typedef struct SNAP_PRO_MANUAL_PARAM_S {
	GK_U32 au32ManExpTime[PRO_MAX_FRAME_NUM];
	GK_U32 au32ManSysgain[PRO_MAX_FRAME_NUM];
} SNAP_PRO_MANUAL_PARAM_S;

typedef struct SNAP_PRO_AUTO_PARAM_S {
	GK_U16 au16ProExpStep[PRO_MAX_FRAME_NUM];
} SNAP_PRO_AUTO_PARAM_S;

typedef struct SNAP_PRO_PARAM_S {
	OPERATION_MODE_E enOperationMode;
	SNAP_PRO_AUTO_PARAM_S stAutoParam;
	SNAP_PRO_MANUAL_PARAM_S stManualParam;
} SNAP_PRO_PARAM_S;

typedef struct SNAP_NORMAL_ATTR_S {
	GK_U32 u32FrameCnt;
	GK_U32 u32RepeatSendTimes;

	GK_BOOL bZSL;
	GK_U32 u32FrameDepth;
	GK_U32 u32RollbackMs;
	GK_U32 u32Interval;
} SNAP_NORMAL_ATTR_S;

typedef struct SNAP_PRO_ATTR_S {
	GK_U32 u32FrameCnt;
	GK_U32 u32RepeatSendTimes;
	SNAP_PRO_PARAM_S stProParam;
} SNAP_PRO_ATTR_S;

typedef struct SNAP_USER_ATTR_S {
	GK_U32 u32FrameDepth;
} SNAP_USER_ATTR_S;

typedef struct SNAP_ATTR_S {
	SNAP_TYPE_E enSnapType;
	GK_BOOL bLoadCCM;
	union {
		SNAP_NORMAL_ATTR_S stNormalAttr;
		SNAP_PRO_ATTR_S stProAttr;
	};
} SNAP_ATTR_S;

typedef struct ISP_PRO_BNR_PARAM_S {
	GK_BOOL bEnable;
	GK_U32 u32ParamNum;
	ISP_NR_AUTO_ATTR_S *pastNrAttr;
} ISP_PRO_BNR_PARAM_S;

typedef struct ISP_PRO_SHARPEN_PARAM_S {
	GK_BOOL bEnable;
	GK_U32 u32ParamNum;
	ISP_SHARPEN_AUTO_ATTR_S *pastShpAttr;
} ISP_PRO_SHARPEN_PARAM_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
