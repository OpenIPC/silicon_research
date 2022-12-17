/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include "type.h"
#include "common.h"
#include "errcode.h"
#include "comm_video.h"

#ifndef __COMM_DIS_H__
#define __COMM_DIS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ERR_CODE_DIS_NOBUF         DEFINE_ERR_CODE(MOD_ID_DIS, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define ERR_CODE_DIS_BUF_EMPTY     DEFINE_ERR_CODE(MOD_ID_DIS, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define ERR_CODE_DIS_NULL_PTR      DEFINE_ERR_CODE(MOD_ID_DIS, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_DIS_ILLEGAL_PARAM DEFINE_ERR_CODE(MOD_ID_DIS, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_DIS_BUF_FULL      DEFINE_ERR_CODE(MOD_ID_DIS, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define ERR_CODE_DIS_SYS_NOTREADY  DEFINE_ERR_CODE(MOD_ID_DIS, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define ERR_CODE_DIS_NOT_SUPPORT   DEFINE_ERR_CODE(MOD_ID_DIS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define ERR_CODE_DIS_NOT_PERMITTED DEFINE_ERR_CODE(MOD_ID_DIS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define ERR_CODE_DIS_BUSY          DEFINE_ERR_CODE(MOD_ID_DIS, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define ERR_CODE_DIS_INVALID_CHNID DEFINE_ERR_CODE(MOD_ID_DIS, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define ERR_CODE_DIS_CHN_UNEXIST   DEFINE_ERR_CODE(MOD_ID_DIS, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)

typedef enum DIS_MODE_E {
    DIS_MODE_4_DOF_GME = 0,
    DIS_MODE_6_DOF_GME,
    DIS_MODE_GYRO,
    DIS_MODE_DOF_BUTT,
} DIS_MODE_E;

typedef enum DIS_MOTION_LEVEL_E {
    DIS_MOTION_LEVEL_LOW = 0,
    DIS_MOTION_LEVEL_NORMAL,
    DIS_MOTION_LEVEL_HIGH,
    DIS_MOTION_LEVEL_BUTT
} DIS_MOTION_LEVEL_E;

typedef enum DIS_PDT_TYPE_E {
    DIS_PDT_TYPE_IPC = 0,
    DIS_PDT_TYPE_DV,
    DIS_PDT_TYPE_DRONE,
    DIS_PDT_TYPE_BUTT
} DIS_PDT_TYPE_E;

typedef struct DIS_ATTR_S {
    GK_BOOL bEnable;
    GK_BOOL bGdcBypass;
    GK_U32 u32MovingSubjectLevel;
    GK_S32 s32RollingShutterCoef;
    GK_S32 s32Timelag;
    GK_U32 u32ViewAngle;
    GK_U32 u32HorizontalLimit;
    GK_U32 u32VerticalLimit;
    GK_BOOL bStillCrop;
} DIS_ATTR_S;

typedef struct DIS_CONFIG_S {
    DIS_MODE_E enMode;
    DIS_MOTION_LEVEL_E enMotionLevel;
    DIS_PDT_TYPE_E enPdtType;
    GK_U32 u32BufNum;
    GK_U32 u32CropRatio;
    GK_U32 u32FrameRate;
    GK_U32 u32GyroOutputRange;
    GK_U32 u32GyroDataBitWidth;
    GK_BOOL bCameraSteady;
    GK_BOOL bScale;
} DIS_CONFIG_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
