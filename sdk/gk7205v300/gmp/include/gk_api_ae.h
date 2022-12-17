/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __GK_API_AE_H__
#define __GK_API_AE_H__

#include "comm_isp.h"
#include "comm_3a.h"
#include "ae_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


GK_S32 GK_API_AE_Register(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib);
GK_S32 GK_API_AE_UnRegister(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib);

GK_S32 GK_API_AE_SensorRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,AE_SENSOR_REGISTER_S *pstRegister);
GK_S32 GK_API_AE_SensorUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, SENSOR_ID SensorId);

GK_S32 GK_API_ISP_SetExposureAttr(VI_PIPE ViPipe, const ISP_EXPOSURE_ATTR_S *pstExpAttr);
GK_S32 GK_API_ISP_GetExposureAttr(VI_PIPE ViPipe, ISP_EXPOSURE_ATTR_S *pstExpAttr);

GK_S32 GK_API_ISP_SetWDRExposureAttr(VI_PIPE ViPipe, const ISP_WDR_EXPOSURE_ATTR_S *pstWDRExpAttr);
GK_S32 GK_API_ISP_GetWDRExposureAttr(VI_PIPE ViPipe, ISP_WDR_EXPOSURE_ATTR_S *pstWDRExpAttr);

GK_S32 GK_API_ISP_SetHDRExposureAttr(VI_PIPE ViPipe, const ISP_HDR_EXPOSURE_ATTR_S *pstHDRExpAttr);
GK_S32 GK_API_ISP_GetHDRExposureAttr(VI_PIPE ViPipe, ISP_HDR_EXPOSURE_ATTR_S *pstHDRExpAttr);

GK_S32 GK_API_ISP_SetAERouteAttr(VI_PIPE ViPipe, const ISP_AE_ROUTE_S *pstAERouteAttr);
GK_S32 GK_API_ISP_GetAERouteAttr(VI_PIPE ViPipe, ISP_AE_ROUTE_S *pstAERouteAttr);

GK_S32 GK_API_ISP_SetAERouteSFAttr(VI_PIPE ViPipe, const ISP_AE_ROUTE_S *pstAERouteSFAttr);
GK_S32 GK_API_ISP_GetAERouteSFAttr(VI_PIPE ViPipe, ISP_AE_ROUTE_S *pstAERouteSFAttr);

GK_S32 GK_API_ISP_QueryExposureInfo(VI_PIPE ViPipe, ISP_EXP_INFO_S *pstExpInfo);

GK_S32 GK_API_ISP_SetIrisAttr(VI_PIPE ViPipe, const ISP_IRIS_ATTR_S *pstIrisAttr);
GK_S32 GK_API_ISP_GetIrisAttr(VI_PIPE ViPipe, ISP_IRIS_ATTR_S *pstIrisAttr);

GK_S32 GK_API_ISP_SetDcirisAttr(VI_PIPE ViPipe, const ISP_DCIRIS_ATTR_S *pstDcirisAttr);
GK_S32 GK_API_ISP_GetDcirisAttr(VI_PIPE ViPipe, ISP_DCIRIS_ATTR_S *pstDcirisAttr);

GK_S32 GK_API_ISP_SetPirisAttr(VI_PIPE ViPipe, const ISP_PIRIS_ATTR_S *pstPirisAttr);
GK_S32 GK_API_ISP_GetPirisAttr(VI_PIPE ViPipe, ISP_PIRIS_ATTR_S *pstPirisAttr);

GK_S32 GK_API_ISP_SetAERouteAttrEx(VI_PIPE ViPipe, const ISP_AE_ROUTE_EX_S *pstAERouteAttrEx);
GK_S32 GK_API_ISP_GetAERouteAttrEx(VI_PIPE ViPipe, ISP_AE_ROUTE_EX_S *pstAERouteAttrEx);

GK_S32 GK_API_ISP_SetAERouteSFAttrEx(VI_PIPE ViPipe, const ISP_AE_ROUTE_EX_S *pstAERouteSFAttrEx);
GK_S32 GK_API_ISP_GetAERouteSFAttrEx(VI_PIPE ViPipe, ISP_AE_ROUTE_EX_S *pstAERouteSFAttrEx);

GK_S32 GK_API_ISP_SetSmartExposureAttr(VI_PIPE ViPipe, const ISP_SMART_EXPOSURE_ATTR_S *pstSmartExpAttr);
GK_S32 GK_API_ISP_GetSmartExposureAttr(VI_PIPE ViPipe, ISP_SMART_EXPOSURE_ATTR_S *pstSmartExpAttr);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
