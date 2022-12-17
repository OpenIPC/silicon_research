/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __COMM_VGS_H__
#define __COMM_VGS_H__

#include "type.h"
#include "common.h"
#include "errcode.h"
#include "comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ERR_CODE_VGS_NOBUF         DEFINE_ERR_CODE(MOD_ID_VGS, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define ERR_CODE_VGS_BUF_EMPTY     DEFINE_ERR_CODE(MOD_ID_VGS, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define ERR_CODE_VGS_NULL_PTR      DEFINE_ERR_CODE(MOD_ID_VGS, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_VGS_ILLEGAL_PARAM DEFINE_ERR_CODE(MOD_ID_VGS, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_VGS_BUF_FULL      DEFINE_ERR_CODE(MOD_ID_VGS, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define ERR_CODE_VGS_SYS_NOTREADY  DEFINE_ERR_CODE(MOD_ID_VGS, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define ERR_CODE_VGS_NOT_SUPPORT   DEFINE_ERR_CODE(MOD_ID_VGS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define ERR_CODE_VGS_NOT_PERMITTED DEFINE_ERR_CODE(MOD_ID_VGS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)

#define VGS_PRIVATE_DATA_LEN 6

typedef GK_S32 VGS_HANDLE;

typedef enum VGS_COLOR_REVERT_MODE_E {
    VGS_COLOR_REVERT_NONE = 0,
    VGS_COLOR_REVERT_RGB,
    VGS_COLOR_REVERT_ALPHA,
    VGS_COLOR_REVERT_BOTH,
    VGS_COLOR_REVERT_BUTT
} VGS_COLOR_REVERT_MODE_E;

typedef struct VGS_OSD_REVERT_S {
    RECT_S stSrcRect;
    VGS_COLOR_REVERT_MODE_E enColorRevertMode;
} VGS_OSD_REVERT_S;

typedef struct VGS_TASK_ATTR_S {
    VIDEO_FRAME_INFO_S stImgIn;
    VIDEO_FRAME_INFO_S stImgOut;
    GK_U64 au64PrivateData[4];
    GK_U32 reserved;
} VGS_TASK_ATTR_S;

typedef struct VGS_DRAW_LINE_S {
    POINT_S stStartPoint;
    POINT_S stEndPoint;
    GK_U32 u32Thick;
    GK_U32 u32Color;
} VGS_DRAW_LINE_S;

typedef enum VGS_COVER_TYPE_E {
    COVER_RECT = 0,
    COVER_QUAD_RANGLE,
    COVER_BUTT
} VGS_COVER_TYPE_E;

typedef struct VGS_QUADRANGLE_COVER_S {
    GK_BOOL bSolid;
    GK_U32 u32Thick;
    POINT_S stPoint[4];
} VGS_QUADRANGLE_COVER_S;

typedef struct VGS_ADD_COVER_S {
    VGS_COVER_TYPE_E enCoverType;
    union {
        RECT_S stDstRect;
        VGS_QUADRANGLE_COVER_S stQuadRangle;
    };

    GK_U32 u32Color;
} VGS_ADD_COVER_S;

typedef struct VGS_ADD_OSD_S {
    RECT_S stRect;
    GK_U32 u32BgColor;
    PIXEL_FORMAT_E enPixelFmt;
    GK_U64 u64PhyAddr;
    GK_U32 u32Stride;
    GK_U32 u32BgAlpha;
    GK_U32 u32FgAlpha;
    GK_BOOL bOsdRevert;
    VGS_OSD_REVERT_S stOsdRevert;
    GK_U16 u16ColorLUT[2];
} VGS_ADD_OSD_S;

typedef enum VGS_SCLCOEF_MODE_E {
    VGS_SCLCOEF_NORMAL = 0,
    VGS_SCLCOEF_TAP2 = 1,
    VGS_SCLCOEF_TAP4 = 2,
    VGS_SCLCOEF_TAP6 = 3,
    VGS_SCLCOEF_TAP8 = 4,
    VGS_SCLCOEF_BUTT
} VGS_SCLCOEF_MODE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
