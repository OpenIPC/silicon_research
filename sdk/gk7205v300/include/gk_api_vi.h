/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_API_VI_H__
#define __GK_API_VI_H__

#include "comm_vi.h"
#include "comm_dis.h"
#include "comm_gdc.h"
#include "comm_vb.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

GK_S32 GK_API_VI_SetDevAttr(VI_DEV ViDev, const VI_DEV_ATTR_S *pstDevAttr);
GK_S32 GK_API_VI_GetDevAttr(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr);

GK_S32 GK_API_VI_SetDevAttrEx(VI_DEV ViDev,
			      const VI_DEV_ATTR_EX_S *pstDevAttrEx);
GK_S32 GK_API_VI_GetDevAttrEx(VI_DEV ViDev, VI_DEV_ATTR_EX_S *pstDevAttrEx);

GK_S32 GK_API_VI_SetVSSignalAttr(VI_DEV ViDev,
				 const VI_VS_SIGNAL_ATTR_S *pstVSSignalAttr);
GK_S32 GK_API_VI_GetVSSignalAttr(VI_DEV ViDev,
				 VI_VS_SIGNAL_ATTR_S *pstVSSignalAttr);
GK_S32 GK_API_VI_TriggerVSSignal(VI_DEV ViDev, GK_BOOL bEnable);

GK_S32 GK_API_VI_EnableDev(VI_DEV ViDev);
GK_S32 GK_API_VI_DisableDev(VI_DEV ViDev);

GK_S32 GK_API_VI_SetMipiBindDev(VI_DEV ViDev, MIPI_DEV MipiDev);
GK_S32 GK_API_VI_GetMipiBindDev(VI_DEV ViDev, MIPI_DEV *pMipiDev);

GK_S32 GK_API_VI_SetDevBindPipe(VI_DEV ViDev,
				const VI_DEV_BIND_PIPE_S *pstDevBindPipe);
GK_S32 GK_API_VI_GetDevBindPipe(VI_DEV ViDev,
				VI_DEV_BIND_PIPE_S *pstDevBindPipe);

GK_S32 GK_API_VI_SetDevTimingAttr(VI_DEV ViDev,
				  const VI_DEV_TIMING_ATTR_S *pstTimingAttr);
GK_S32 GK_API_VI_GetDevTimingAttr(VI_DEV ViDev,
				  VI_DEV_TIMING_ATTR_S *pstTimingAttr);

GK_S32 GK_API_VI_GetPipeCmpParam(VI_PIPE ViPipe, VI_CMP_PARAM_S *pCmpParam);

GK_S32 GK_API_VI_SetUserPic(VI_PIPE ViPipe, const VI_USERPIC_ATTR_S *pstUsrPic);
GK_S32 GK_API_VI_EnableUserPic(VI_PIPE ViPipe);
GK_S32 GK_API_VI_DisableUserPic(VI_PIPE ViPipe);

GK_S32 GK_API_VI_CreatePipe(VI_PIPE ViPipe, const VI_PIPE_ATTR_S *pstPipeAttr);
GK_S32 GK_API_VI_DestroyPipe(VI_PIPE ViPipe);

GK_S32 GK_API_VI_SetPipeAttr(VI_PIPE ViPipe, const VI_PIPE_ATTR_S *pstPipeAttr);
GK_S32 GK_API_VI_GetPipeAttr(VI_PIPE ViPipe, VI_PIPE_ATTR_S *pstPipeAttr);

GK_S32 GK_API_VI_StartPipe(VI_PIPE ViPipe);
GK_S32 GK_API_VI_StopPipe(VI_PIPE ViPipe);

GK_S32 GK_API_VI_SetPipeCrop(VI_PIPE ViPipe, const CROP_INFO_S *pstCropInfo);
GK_S32 GK_API_VI_GetPipeCrop(VI_PIPE ViPipe, CROP_INFO_S *pstCropInfo);

GK_S32 GK_API_VI_SetPipeFisheyeConfig(VI_PIPE ViPipe,
				      const FISHEYE_CONFIG_S *pstFishEyeConfig);
GK_S32 GK_API_VI_GetPipeFisheyeConfig(VI_PIPE ViPipe,
				      FISHEYE_CONFIG_S *pstFishEyeConfig);

GK_S32 GK_API_VI_FisheyePosQueryDst2Src(VI_PIPE ViPipe, VI_CHN ViChn,
					GK_U32 u32RegionIndex,
					const POINT_S *pstDstPointIn,
					POINT_S *pstSrcPointOut);

GK_S32 GK_API_VI_SetPipeDumpAttr(VI_PIPE ViPipe,
				 const VI_DUMP_ATTR_S *pstDumpAttr);
GK_S32 GK_API_VI_GetPipeDumpAttr(VI_PIPE ViPipe, VI_DUMP_ATTR_S *pstDumpAttr);

GK_S32 GK_API_VI_SetPipeFrameSource(VI_PIPE ViPipe,
				    const VI_PIPE_FRAME_SOURCE_E enSource);
GK_S32 GK_API_VI_GetPipeFrameSource(VI_PIPE ViPipe,
				    VI_PIPE_FRAME_SOURCE_E *penSource);

GK_S32 GK_API_VI_GetPipeFrame(VI_PIPE ViPipe, VIDEO_FRAME_INFO_S *pstVideoFrame,
			      GK_S32 s32MilliSec);
GK_S32 GK_API_VI_ReleasePipeFrame(VI_PIPE ViPipe,
				  const VIDEO_FRAME_INFO_S *pstVideoFrame);

GK_S32 GK_API_VI_SendPipeYUV(VI_PIPE ViPipe,
			     const VIDEO_FRAME_INFO_S *pstVideoFrame,
			     GK_S32 s32MilliSec);
GK_S32 GK_API_VI_SendPipeRaw(GK_U32 u32PipeNum, VI_PIPE PipeId[],
			     const VIDEO_FRAME_INFO_S *pstVideoFrame[],
			     GK_S32 s32MilliSec);

GK_S32 GK_API_VI_SetPipeNRXParam(VI_PIPE ViPipe,
				 const VI_PIPE_NRX_PARAM_S *pstNrXParam);
GK_S32 GK_API_VI_GetPipeNRXParam(VI_PIPE ViPipe,
				 VI_PIPE_NRX_PARAM_S *pstNrXParam);

GK_S32 GK_API_VI_SetPipeRepeatMode(VI_PIPE ViPipe,
				   const VI_PIPE_REPEAT_MODE_E enPepeatMode);
GK_S32 GK_API_VI_GetPipeRepeatMode(VI_PIPE ViPipe,
				   VI_PIPE_REPEAT_MODE_E *penPepeatMode);

GK_S32 GK_API_VI_QueryPipeStatus(VI_PIPE ViPipe, VI_PIPE_STATUS_S *pstStatus);

GK_S32 GK_API_VI_EnablePipeInterrupt(VI_PIPE ViPipe);
GK_S32 GK_API_VI_DisablePipeInterrupt(VI_PIPE ViPipe);

GK_S32 GK_API_VI_SetPipeVCNumber(VI_PIPE ViPipe, GK_U32 u32VCNumber);
GK_S32 GK_API_VI_GetPipeVCNumber(VI_PIPE ViPipe, GK_U32 *pu32VCNumber);

GK_S32 GK_API_VI_SetPipeFrameInterruptAttr(
	VI_PIPE ViPipe, const FRAME_INTERRUPT_ATTR_S *pstFrameIntAttr);
GK_S32
GK_API_VI_GetPipeFrameInterruptAttr(VI_PIPE ViPipe,
				    FRAME_INTERRUPT_ATTR_S *pstFrameIntAttr);

GK_S32 GK_API_VI_SetPipeBNRRawDumpAttr(VI_PIPE ViPipe,
				       const BNR_DUMP_ATTR_S *pstBnrDumpAttr);
GK_S32 GK_API_VI_GetPipeBNRRawDumpAttr(VI_PIPE ViPipe,
				       BNR_DUMP_ATTR_S *pstBnrDumpAttr);

GK_S32 GK_API_VI_GetPipeBNRRaw(VI_PIPE ViPipe,
			       VIDEO_FRAME_INFO_S *pstVideoFrame,
			       GK_S32 s32MilliSec);
GK_S32 GK_API_VI_ReleasePipeBNRRaw(VI_PIPE ViPipe,
				   const VIDEO_FRAME_INFO_S *pstVideoFrame);

GK_S32 GK_API_VI_PipeAttachVbPool(VI_PIPE ViPipe, VB_POOL VbPool);
GK_S32 GK_API_VI_PipeDetachVbPool(VI_PIPE ViPipe);

GK_S32 GK_API_VI_GetPipeFd(VI_PIPE ViPipe);

GK_S32 GK_API_VI_SetChnAttr(VI_PIPE ViPipe, VI_CHN ViChn,
			    const VI_CHN_ATTR_S *pstChnAttr);
GK_S32 GK_API_VI_GetChnAttr(VI_PIPE ViPipe, VI_CHN ViChn,
			    VI_CHN_ATTR_S *pstChnAttr);

GK_S32 GK_API_VI_EnableChn(VI_PIPE ViPipe, VI_CHN ViChn);
GK_S32 GK_API_VI_DisableChn(VI_PIPE ViPipe, VI_CHN ViChn);

GK_S32 GK_API_VI_SetChnCrop(VI_PIPE ViPipe, VI_CHN ViChn,
			    const VI_CROP_INFO_S *pstCropInfo);
GK_S32 GK_API_VI_GetChnCrop(VI_PIPE ViPipe, VI_CHN ViChn,
			    VI_CROP_INFO_S *pstCropInfo);

GK_S32 GK_API_VI_SetChnRotation(VI_PIPE ViPipe, VI_CHN ViChn,
				const ROTATION_E enRotation);
GK_S32 GK_API_VI_GetChnRotation(VI_PIPE ViPipe, VI_CHN ViChn,
				ROTATION_E *penRotation);

GK_S32
GK_API_VI_SetChnRotationEx(VI_PIPE ViPipe, VI_CHN ViChn,
			   const VI_ROTATION_EX_ATTR_S *pstViRotationExAttr);
GK_S32 GK_API_VI_GetChnRotationEx(VI_PIPE ViPipe, VI_CHN ViChn,
				  VI_ROTATION_EX_ATTR_S *pstViRotationExAttr);

GK_S32 GK_API_VI_SetChnLDCAttr(VI_PIPE ViPipe, VI_CHN ViChn,
			       const VI_LDC_ATTR_S *pstLDCAttr);
GK_S32 GK_API_VI_GetChnLDCAttr(VI_PIPE ViPipe, VI_CHN ViChn,
			       VI_LDC_ATTR_S *pstLDCAttr);

GK_S32 GK_API_VI_SetChnLDCV2Attr(VI_PIPE ViPipe, VI_CHN ViChn,
				 const VI_LDCV2_ATTR_S *pstLDCV2Attr);
GK_S32 GK_API_VI_GetChnLDCV2Attr(VI_PIPE ViPipe, VI_CHN ViChn,
				 VI_LDCV2_ATTR_S *pstLDCV2Attr);

GK_S32 GK_API_VI_SetChnLDCV3Attr(VI_PIPE ViPipe, VI_CHN ViChn,
				 const VI_LDCV3_ATTR_S *pstLDCV3Attr);
GK_S32 GK_API_VI_GetChnLDCV3Attr(VI_PIPE ViPipe, VI_CHN ViChn,
				 VI_LDCV3_ATTR_S *pstLDCV3Attr);

GK_S32 GK_API_VI_SetChnSpreadAttr(VI_PIPE ViPipe, VI_CHN ViChn,
				  const SPREAD_ATTR_S *pstSpreadAttr);
GK_S32 GK_API_VI_GetChnSpreadAttr(VI_PIPE ViPipe, VI_CHN ViChn,
				  SPREAD_ATTR_S *pstSpreadAttr);

GK_S32 GK_API_VI_SetChnLowDelayAttr(VI_PIPE ViPipe, VI_CHN ViChn,
				    const VI_LOW_DELAY_INFO_S *pstLowDelayInfo);
GK_S32 GK_API_VI_GetChnLowDelayAttr(VI_PIPE ViPipe, VI_CHN ViChn,
				    VI_LOW_DELAY_INFO_S *pstLowDelayInfo);

GK_S32 GK_API_VI_GetChnRegionLuma(VI_PIPE ViPipe, VI_CHN ViChn,
				  const VIDEO_REGION_INFO_S *pstRegionInfo,
				  GK_U64 *pu64LumaData, GK_S32 s32MilliSec);
GK_S32 GK_API_VI_SetChnDISConfig(VI_PIPE ViPipe, VI_CHN ViChn,
				 const DIS_CONFIG_S *pstDISConfig);
GK_S32 GK_API_VI_GetChnDISConfig(VI_PIPE ViPipe, VI_CHN ViChn,
				 DIS_CONFIG_S *pstDISConfig);
GK_S32 GK_API_VI_SetChnDISAttr(VI_PIPE ViPipe, VI_CHN ViChn,
			       const DIS_ATTR_S *pstDISAttr);
GK_S32 GK_API_VI_GetChnDISAttr(VI_PIPE ViPipe, VI_CHN ViChn,
			       DIS_ATTR_S *pstDISAttr);

GK_S32 GK_API_VI_SetExtChnFisheye(VI_PIPE ViPipe, VI_CHN ViChn,
				  const FISHEYE_ATTR_S *pstFishEyeAttr);
GK_S32 GK_API_VI_GetExtChnFisheye(VI_PIPE ViPipe, VI_CHN ViChn,
				  FISHEYE_ATTR_S *pstFishEyeAttr);

GK_S32 GK_API_VI_SetExtChnAttr(VI_PIPE ViPipe, VI_CHN ViChn,
			       const VI_EXT_CHN_ATTR_S *pstExtChnAttr);
GK_S32 GK_API_VI_GetExtChnAttr(VI_PIPE ViPipe, VI_CHN ViChn,
			       VI_EXT_CHN_ATTR_S *pstExtChnAttr);

GK_S32 GK_API_VI_GetChnFrame(VI_PIPE ViPipe, VI_CHN ViChn,
			     VIDEO_FRAME_INFO_S *pstFrameInfo,
			     GK_S32 s32MilliSec);
GK_S32 GK_API_VI_ReleaseChnFrame(VI_PIPE ViPipe, VI_CHN ViChn,
				 const VIDEO_FRAME_INFO_S *pstFrameInfo);

GK_S32
GK_API_VI_SetChnEarlyInterrupt(VI_PIPE ViPipe, VI_CHN ViChn,
			       const VI_EARLY_INTERRUPT_S *pstEarlyInterrupt);
GK_S32 GK_API_VI_GetChnEarlyInterrupt(VI_PIPE ViPipe, VI_CHN ViChn,
				      VI_EARLY_INTERRUPT_S *pstEarlyInterrupt);

GK_S32 GK_API_VI_SetChnAlign(VI_PIPE ViPipe, VI_CHN ViChn, GK_U32 u32Align);
GK_S32 GK_API_VI_GetChnAlign(VI_PIPE ViPipe, VI_CHN ViChn, GK_U32 *pu32Align);

GK_S32 GK_API_VI_ChnAttachVbPool(VI_PIPE ViPipe, VI_CHN ViChn, VB_POOL VbPool);
GK_S32 GK_API_VI_ChnDetachVbPool(VI_PIPE ViPipe, VI_CHN ViChn);

GK_S32 GK_API_VI_QueryChnStatus(VI_PIPE ViPipe, VI_CHN ViChn,
				VI_CHN_STATUS_S *pstChnStatus);

GK_S32 GK_API_VI_GetChnFd(VI_PIPE ViPipe, VI_CHN ViChn);

GK_S32 GK_API_VI_SetStitchGrpAttr(VI_STITCH_GRP StitchGrp,
				  const VI_STITCH_GRP_ATTR_S *pstStitchGrpAttr);
GK_S32 GK_API_VI_GetStitchGrpAttr(VI_STITCH_GRP StitchGrp,
				  VI_STITCH_GRP_ATTR_S *pstStitchGrpAttr);

GK_S32 GK_API_VI_SetModParam(const VI_MOD_PARAM_S *pstModParam);
GK_S32 GK_API_VI_GetModParam(VI_MOD_PARAM_S *pstModParam);

GK_S32 GK_API_VI_CloseFd(GK_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
