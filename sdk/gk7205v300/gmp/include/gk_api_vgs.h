/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_API_VGS_H__
#define __GK_API_VGS_H__

#include "common.h"
#include "comm_video.h"
#include "comm_vgs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

GK_S32 GK_API_VGS_BeginJob(VGS_HANDLE *phHandle);

GK_S32 GK_API_VGS_EndJob(VGS_HANDLE hHandle);

GK_S32 GK_API_VGS_CancelJob(VGS_HANDLE hHandle);

GK_S32 GK_API_VGS_AddScaleTask(VGS_HANDLE hHandle, const VGS_TASK_ATTR_S *pstTask,VGS_SCLCOEF_MODE_E enScaleCoefMode);

GK_S32 GK_API_VGS_AddDrawLineTask(VGS_HANDLE hHandle, const VGS_TASK_ATTR_S *pstTask,const VGS_DRAW_LINE_S *pstVgsDrawLine);

GK_S32 GK_API_VGS_AddCoverTask(VGS_HANDLE hHandle, const VGS_TASK_ATTR_S *pstTask,const VGS_ADD_COVER_S *pstVgsAddCover);

GK_S32 GK_API_VGS_AddOsdTask(VGS_HANDLE hHandle, const VGS_TASK_ATTR_S *pstTask,const VGS_ADD_OSD_S *pstVgsAddOsd);

GK_S32 GK_API_VGS_AddDrawLineTaskArray(VGS_HANDLE hHandle, const VGS_TASK_ATTR_S *pstTask,const VGS_DRAW_LINE_S astVgsDrawLine[], GK_U32 u32ArraySize);

GK_S32 GK_API_VGS_AddCoverTaskArray(VGS_HANDLE hHandle, const VGS_TASK_ATTR_S *pstTask,const VGS_ADD_COVER_S astVgsAddCover[], GK_U32 u32ArraySize);

GK_S32 GK_API_VGS_AddOsdTaskArray(VGS_HANDLE hHandle, const VGS_TASK_ATTR_S *pstTask,const VGS_ADD_OSD_S astVgsAddOsd[], GK_U32 u32ArraySize);

GK_S32 GK_API_VGS_AddRotationTask(VGS_HANDLE hHandle, const VGS_TASK_ATTR_S *pstTask, ROTATION_E enRotationAngle);

GK_S32 GK_API_VGS_AddLumaTaskArray(VGS_HANDLE hHandle, VGS_TASK_ATTR_S *pstTask, const RECT_S astVgsLumaRect[],GK_U32 u32ArraySize, GK_U64 au64LumaData[]);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

