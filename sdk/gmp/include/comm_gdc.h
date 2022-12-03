/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "type.h"
#include "common.h"
#include "errcode.h"
#include "comm_video.h"

#ifndef __COMM_GDC_H__
#define __COMM_GDC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ERR_CODE_GDC_NOBUF         DEFINE_ERR_CODE(MOD_ID_GDC, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define ERR_CODE_GDC_BUF_EMPTY     DEFINE_ERR_CODE(MOD_ID_GDC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define ERR_CODE_GDC_NULL_PTR      DEFINE_ERR_CODE(MOD_ID_GDC, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_GDC_ILLEGAL_PARAM DEFINE_ERR_CODE(MOD_ID_GDC, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_GDC_BUF_FULL      DEFINE_ERR_CODE(MOD_ID_GDC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define ERR_CODE_GDC_SYS_NOTREADY  DEFINE_ERR_CODE(MOD_ID_GDC, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define ERR_CODE_GDC_NOT_SUPPORT   DEFINE_ERR_CODE(MOD_ID_GDC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define ERR_CODE_GDC_NOT_PERMITTED DEFINE_ERR_CODE(MOD_ID_GDC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define ERR_CODE_GDC_BUSY          DEFINE_ERR_CODE(MOD_ID_GDC, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define ERR_CODE_GDC_INVALID_CHNID DEFINE_ERR_CODE(MOD_ID_GDC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define ERR_CODE_GDC_CHN_UNEXIST   DEFINE_ERR_CODE(MOD_ID_GDC, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)

#define FISHEYE_MAX_REGION_NUM   4
#define FISHEYE_LMFCOEF_NUM      128
#define GDC_PMFCOEF_NUM          9

typedef GK_S32 GDC_HANDLE;

typedef struct GDC_TASK_ATTR_S {
    VIDEO_FRAME_INFO_S stImgIn;
    VIDEO_FRAME_INFO_S stImgOut;
    GK_U64 au64privateData[4];
    GK_U64 reserved;
} GDC_TASK_ATTR_S;

typedef enum FISHEYE_MOUNT_MODE_E {
    FISHEYE_DESKTOP_MOUNT = 0,
    FISHEYE_CEILING_MOUNT = 1,
    FISHEYE_WALL_MOUNT = 2,

    FISHEYE_MOUNT_MODE_BUTT
} FISHEYE_MOUNT_MODE_E;

typedef enum FISHEYE_VIEW_MODE_E {
    FISHEYE_VIEW_360_PANORAMA = 0,
    FISHEYE_VIEW_180_PANORAMA = 1,
    FISHEYE_VIEW_NORMAL = 2,
    FISHEYE_NO_TRANSFORMATION = 3,

    FISHEYE_VIEW_MODE_BUTT
} FISHEYE_VIEW_MODE_E;

typedef struct FISHEYE_REGION_ATTR_S {
    FISHEYE_VIEW_MODE_E enViewMode;
    GK_U32 u32InRadius;
    GK_U32 u32OutRadius;
    GK_U32 u32Pan;
    GK_U32 u32Tilt;
    GK_U32 u32HorZoom;
    GK_U32 u32VerZoom;
    RECT_S stOutRect;
} FISHEYE_REGION_ATTR_S;

typedef struct FISHEYE_REGION_ATTR_EX_S {
    FISHEYE_VIEW_MODE_E enViewMode;
    GK_U32 u32InRadius;
    GK_U32 u32OutRadius;
    GK_U32 u32X;
    GK_U32 u32Y;
    GK_U32 u32HorZoom;
    GK_U32 u32VerZoom;
    RECT_S stOutRect;
} FISHEYE_REGION_ATTR_EX_S;

typedef struct FISHEYE_ATTR_S {
    GK_BOOL bEnable;
    GK_BOOL bLMF;
    GK_BOOL bBgColor;
    GK_U32 u32BgColor;

    GK_S32 s32HorOffset;
    GK_S32 s32VerOffset;

    GK_U32 u32TrapezoidCoef;
    GK_S32 s32FanStrength;

    FISHEYE_MOUNT_MODE_E enMountMode;

    GK_U32 u32RegionNum;
    FISHEYE_REGION_ATTR_S astFishEyeRegionAttr[FISHEYE_MAX_REGION_NUM];
} FISHEYE_ATTR_S;

typedef struct FISHEYE_ATTR_EX_S {
    GK_BOOL bEnable;
    GK_BOOL bLMF;
    GK_BOOL bBgColor;
    GK_U32 u32BgColor;

    GK_S32 s32HorOffset;
    GK_S32 s32VerOffset;

    GK_U32 u32TrapezoidCoef;
    GK_S32 s32FanStrength;

    FISHEYE_MOUNT_MODE_E enMountMode;

    GK_U32 u32RegionNum;
    FISHEYE_REGION_ATTR_EX_S astFishEyeRegionAttr[FISHEYE_MAX_REGION_NUM];
} FISHEYE_ATTR_EX_S;

typedef struct SPREAD_ATTR_S {
    GK_BOOL bEnable;
    GK_U32 u32SpreadCoef;
    SIZE_S stDestSize;
} SPREAD_ATTR_S;

typedef struct FISHEYE_JOB_CONFIG_S {
    GK_U64 u64LenMapPhyAddr;
} FISHEYE_JOB_CONFIG_S;

typedef struct FISHEYE_CONFIG_S {
    GK_U16 au16LMFCoef[FISHEYE_LMFCOEF_NUM];
} FISHEYE_CONFIG_S;

typedef struct GDC_PMF_ATTR_S {
    GK_S64 as64PMFCoef[GDC_PMFCOEF_NUM];
} GDC_PMF_ATTR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
