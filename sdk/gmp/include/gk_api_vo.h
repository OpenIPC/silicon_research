/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __GK_API_VO_H__
#define __GK_API_VO_H__

#include "comm_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

GK_S32 GK_API_VO_SetPubAttr(VO_DEV VoDev, const VO_PUB_ATTR_S *pstPubAttr);
GK_S32 GK_API_VO_GetPubAttr(VO_DEV VoDev, VO_PUB_ATTR_S *pstPubAttr);

GK_S32 GK_API_VO_Enable(VO_DEV VoDev);
GK_S32 GK_API_VO_Disable(VO_DEV VoDev);

GK_S32 GK_API_VO_CloseFd(GK_VOID);
GK_S32 GK_API_VO_SetUserIntfSyncInfo(VO_DEV VoDev, VO_USER_INTFSYNC_INFO_S *pstUserInfo);

GK_S32 GK_API_VO_SetVideoLayerAttr(VO_LAYER VoLayer, const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);
GK_S32 GK_API_VO_GetVideoLayerAttr(VO_LAYER VoLayer, VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);

GK_S32 GK_API_VO_EnableVideoLayer(VO_LAYER VoLayer);
GK_S32 GK_API_VO_DisableVideoLayer(VO_LAYER VoLayer);

GK_S32 GK_API_VO_BindVideoLayer(VO_LAYER VoLayer, VO_DEV VoDev);
GK_S32 GK_API_VO_UnBindVideoLayer(VO_LAYER VoLayer, VO_DEV VoDev);

GK_S32 GK_API_VO_SetVideoLayerPriority(VO_LAYER VoLayer, GK_U32 u32Priority);
GK_S32 GK_API_VO_GetVideoLayerPriority(VO_LAYER VoLayer, GK_U32 *pu32Priority);

GK_S32 GK_API_VO_SetVideoLayerCSC(VO_LAYER VoLayer, const VO_CSC_S *pstVideoCSC);
GK_S32 GK_API_VO_GetVideoLayerCSC(VO_LAYER VoLayer, VO_CSC_S *pstVideoCSC);

GK_S32 GK_API_VO_SetVideoLayerPartitionMode(VO_LAYER VoLayer, VO_PART_MODE_E enPartMode);
GK_S32 GK_API_VO_GetVideoLayerPartitionMode(VO_LAYER VoLayer, VO_PART_MODE_E *penPartMode);

GK_S32 GK_API_VO_BatchBegin(VO_LAYER VoLayer);
GK_S32 GK_API_VO_BatchEnd(VO_LAYER VoLayer);

GK_S32 GK_API_VO_SetVideoLayerBoundary(VO_LAYER VoLayer, const VO_LAYER_BOUNDARY_S *pstLayerBoundary);
GK_S32 GK_API_VO_GetVideoLayerBoundary(VO_LAYER VoLayer, VO_LAYER_BOUNDARY_S *pstLayerBoundary);

GK_S32 GK_API_VO_SetVideoLayerParam(VO_LAYER VoLayer, const VO_LAYER_PARAM_S *pstLayerParam);
GK_S32 GK_API_VO_GetVideoLayerParam(VO_LAYER VoLayer, VO_LAYER_PARAM_S *pstLayerParam);

GK_S32 GK_API_VO_SetVideoLayerDecompress(VO_LAYER VoLayer, GK_BOOL bSupportDecompress);
GK_S32 GK_API_VO_GetVideoLayerDecompress(VO_LAYER VoLayer, GK_BOOL *pbSupportDecompress);

GK_S32 GK_API_VO_SetVideoLayerCrop(VO_LAYER VoLayer, const CROP_INFO_S *pstCropInfo);
GK_S32 GK_API_VO_GetVideoLayerCrop(VO_LAYER VoLayer, CROP_INFO_S *pstCropInfo);

GK_S32 GK_API_VO_SetPlayToleration(VO_LAYER VoLayer, GK_U32 u32Toleration);
GK_S32 GK_API_VO_GetPlayToleration(VO_LAYER VoLayer, GK_U32 *pu32Toleration);

GK_S32 GK_API_VO_GetScreenFrame(VO_LAYER VoLayer, VIDEO_FRAME_INFO_S *pstVFrame, GK_S32 s32MilliSec);
GK_S32 GK_API_VO_ReleaseScreenFrame(VO_LAYER VoLayer, const VIDEO_FRAME_INFO_S *pstVFrame);

GK_S32 GK_API_VO_SetDisplayBufLen(VO_LAYER VoLayer, GK_U32 u32BufLen);
GK_S32 GK_API_VO_GetDisplayBufLen(VO_LAYER VoLayer, GK_U32 *pu32BufLen);

GK_S32 GK_API_VO_SetChnAttr(VO_LAYER VoLayer, VO_CHN VoChn, const VO_CHN_ATTR_S *pstChnAttr);
GK_S32 GK_API_VO_GetChnAttr(VO_LAYER VoLayer, VO_CHN VoChn, VO_CHN_ATTR_S *pstChnAttr);

GK_S32 GK_API_VO_EnableChn(VO_LAYER VoLayer, VO_CHN VoChn);
GK_S32 GK_API_VO_DisableChn(VO_LAYER VoLayer, VO_CHN VoChn);

GK_S32 GK_API_VO_SetChnParam(VO_LAYER VoLayer, VO_CHN VoChn, const VO_CHN_PARAM_S *pstChnParam);
GK_S32 GK_API_VO_GetChnParam(VO_LAYER VoLayer, VO_CHN VoChn, VO_CHN_PARAM_S *pstChnParam);

GK_S32 GK_API_VO_SetChnDisplayPosition(VO_LAYER VoLayer, VO_CHN VoChn, const POINT_S *pstDispPos);
GK_S32 GK_API_VO_GetChnDisplayPosition(VO_LAYER VoLayer, VO_CHN VoChn, POINT_S *pstDispPos);

GK_S32 GK_API_VO_SetChnFrameRate(VO_LAYER VoLayer, VO_CHN VoChn, GK_S32 s32ChnFrmRate);
GK_S32 GK_API_VO_GetChnFrameRate(VO_LAYER VoLayer, VO_CHN VoChn, GK_S32 *ps32ChnFrmRate);

GK_S32 GK_API_VO_GetChnFrame(VO_LAYER VoLayer, VO_CHN VoChn, VIDEO_FRAME_INFO_S *pstFrame, GK_S32 s32MilliSec);
GK_S32 GK_API_VO_ReleaseChnFrame(VO_LAYER VoLayer, VO_CHN VoChn, const VIDEO_FRAME_INFO_S *pstFrame);

GK_S32 GK_API_VO_PauseChn(VO_LAYER VoLayer, VO_CHN VoChn);
GK_S32 GK_API_VO_ResumeChn(VO_LAYER VoLayer, VO_CHN VoChn);
GK_S32 GK_API_VO_StepChn(VO_LAYER VoLayer, VO_CHN VoChn);

GK_S32 GK_API_VO_RefreshChn(VO_LAYER VoLayer, VO_CHN VoChn);

GK_S32 GK_API_VO_ShowChn(VO_LAYER VoLayer, VO_CHN VoChn);
GK_S32 GK_API_VO_HideChn(VO_LAYER VoLayer, VO_CHN VoChn);

GK_S32 GK_API_VO_SetZoomInWindow(VO_LAYER VoLayer, VO_CHN VoChn, const VO_ZOOM_ATTR_S *pstZoomAttr);
GK_S32 GK_API_VO_GetZoomInWindow(VO_LAYER VoLayer, VO_CHN VoChn, VO_ZOOM_ATTR_S *pstZoomAttr);

GK_S32 GK_API_VO_GetChnPTS(VO_LAYER VoLayer, VO_CHN VoChn, GK_U64 *pu64ChnPTS);
GK_S32 GK_API_VO_QueryChnStatus(VO_LAYER VoLayer, VO_CHN VoChn, VO_QUERY_STATUS_S *pstStatus);

GK_S32 GK_API_VO_SendFrame(VO_LAYER VoLayer, VO_CHN VoChn, VIDEO_FRAME_INFO_S *pstVFrame, GK_S32 s32MilliSec);

GK_S32 GK_API_VO_ClearChnBuf(VO_LAYER VoLayer, VO_CHN VoChn, GK_BOOL bClrAll);

GK_S32 GK_API_VO_SetChnBorder(VO_LAYER VoLayer, VO_CHN VoChn, const VO_BORDER_S *pstBorder);
GK_S32 GK_API_VO_GetChnBorder(VO_LAYER VoLayer, VO_CHN VoChn, VO_BORDER_S *pstBorder);

GK_S32 GK_API_VO_SetChnBoundary(VO_LAYER VoLayer, VO_CHN VoChn, const VO_CHN_BOUNDARY_S *pstChnBoundary);
GK_S32 GK_API_VO_GetChnBoundary(VO_LAYER VoLayer, VO_CHN VoChn, VO_CHN_BOUNDARY_S *pstChnBoundary);

GK_S32 GK_API_VO_SetChnRecvThreshold(VO_LAYER VoLayer, VO_CHN VoChn, GK_U32 u32Threshold);
GK_S32 GK_API_VO_GetChnRecvThreshold(VO_LAYER VoLayer, VO_CHN VoChn, GK_U32 *pu32Threshold);

GK_S32 GK_API_VO_SetChnRotation(VO_LAYER VoLayer, VO_CHN VoChn, ROTATION_E enRotation);
GK_S32 GK_API_VO_GetChnRotation(VO_LAYER VoLayer, VO_CHN VoChn, ROTATION_E *penRotation);

GK_S32 GK_API_VO_GetChnRegionLuma(VO_LAYER VoLayer, VO_CHN VoChn, VO_REGION_INFO_S *pstRegionInfo,GK_U64 *pu64LumaData, GK_S32 s32MilliSec);

GK_S32 GK_API_VO_SetWBCSource(VO_WBC VoWBC, const VO_WBC_SOURCE_S *pstWBCSource);
GK_S32 GK_API_VO_GetWBCSource(VO_WBC VoWBC, VO_WBC_SOURCE_S *pstWBCSources);

GK_S32 GK_API_VO_SetWBCAttr(VO_WBC VoWBC, const VO_WBC_ATTR_S *pstWBCAttr);
GK_S32 GK_API_VO_GetWBCAttr(VO_WBC VoWBC, VO_WBC_ATTR_S *pstWBCAttr);

GK_S32 GK_API_VO_EnableWBC(VO_WBC VoWBC);
GK_S32 GK_API_VO_DisableWBC(VO_WBC VoWBC);

GK_S32 GK_API_VO_SetWBCMode(VO_WBC VoWBC, VO_WBC_MODE_E enWBCMode);
GK_S32 GK_API_VO_GetWBCMode(VO_WBC VoWBC, VO_WBC_MODE_E *penWBCMode);

GK_S32 GK_API_VO_SetWBCDepth(VO_WBC VoWBC, GK_U32 u32Depth);
GK_S32 GK_API_VO_GetWBCDepth(VO_WBC VoWBC, GK_U32 *pu32Depth);

GK_S32 GK_API_VO_GetWBCFrame(VO_WBC VoWBC, VIDEO_FRAME_INFO_S *pstVFrame, GK_S32 s32MilliSec);
GK_S32 GK_API_VO_ReleaseWBCFrame(VO_WBC VoWBC, const VIDEO_FRAME_INFO_S *pstVFrame);

GK_S32 GK_API_VO_BindGraphicLayer(GRAPHIC_LAYER GraphicLayer, VO_DEV VoDev);
GK_S32 GK_API_VO_UnBindGraphicLayer(GRAPHIC_LAYER GraphicLayer, VO_DEV VoDev);

GK_S32 GK_API_VO_SetGraphicLayerCSC(GRAPHIC_LAYER GraphicLayer, const VO_CSC_S *pstCSC);
GK_S32 GK_API_VO_GetGraphicLayerCSC(GRAPHIC_LAYER GraphicLayer, VO_CSC_S *pstCSC);

GK_S32 GK_API_VO_SetDevFrameRate(VO_DEV VoDev, GK_U32 u32FrameRate);
GK_S32 GK_API_VO_GetDevFrameRate(VO_DEV VoDev, GK_U32 *pu32FrameRate);

GK_S32 GK_API_VO_SetModParam(const VO_MOD_PARAM_S *pstModParam);
GK_S32 GK_API_VO_GetModParam(VO_MOD_PARAM_S *pstModParam);
GK_S32 GK_API_VO_SetVtth(VO_DEV VoDev, GK_U32 u32Vtth);
GK_S32 GK_API_VO_GetVtth(VO_DEV VoDev, GK_U32 *pu32Vtth);
GK_S32 GK_API_VO_SetVtth2(VO_DEV VoDev, GK_U32 u32Vtth);
GK_S32 GK_API_VO_GetVtth2(VO_DEV VoDev, GK_U32 *pu32Vtth);
GK_S32 GK_API_VO_SetChnMirror(VO_LAYER VoLayer, VO_CHN VoChn, VO_MIRROR_MODE_E enMirrorMode);
GK_S32 GK_API_VO_GetChnMirror(VO_LAYER VoLayer, VO_CHN VoChn, VO_MIRROR_MODE_E *penMirrorMode);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


