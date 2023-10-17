/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef _API_TDE2_H_
#define _API_TDE2_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "type.h"
#include "tde_type.h"

#define TDE_Open TDE2_Open
#define TDE_Close TDE2_Close
#define TDE_BeginJob TDE2_BeginJob

GK_S32 GK_TDE2_Open(GK_VOID);

GK_VOID GK_TDE2_Close(GK_VOID);

TDE_HANDLE GK_TDE2_BeginJob(GK_VOID);

GK_S32 GK_TDE2_EndJob(TDE_HANDLE s32Handle, GK_BOOL bSync, GK_BOOL bBlock,
		      GK_U32 u32TimeOut);

GK_S32 GK_TDE2_CancelJob(TDE_HANDLE s32Handle);

GK_S32 GK_TDE2_WaitForDone(TDE_HANDLE s32Handle);

GK_S32 GK_TDE2_WaitAllDone(GK_VOID);

GK_S32 GK_TDE2_Reset(GK_VOID);

GK_S32 GK_TDE2_QuickCopy(TDE_HANDLE s32Handle, const TDE2_SURFACE_S *pstSrc,
			 const TDE2_RECT_S *pstSrcRect,
			 const TDE2_SURFACE_S *pstDst,
			 const TDE2_RECT_S *pstDstRect);

GK_S32 GK_TDE2_QuickFill(TDE_HANDLE s32Handle, const TDE2_SURFACE_S *pstDst,
			 const TDE2_RECT_S *pstDstRect, GK_U32 u32FillData);

GK_S32 GK_TDE2_QuickResize(TDE_HANDLE s32Handle, const TDE2_SURFACE_S *pstSrc,
			   const TDE2_RECT_S *pstSrcRect,
			   const TDE2_SURFACE_S *pstDst,
			   const TDE2_RECT_S *pstDstRect);

GK_S32 GK_TDE2_QuickDeflicker(TDE_HANDLE s32Handle,
			      const TDE2_SURFACE_S *pstSrc,
			      const TDE2_RECT_S *pstSrcRect,
			      const TDE2_SURFACE_S *pstDst,
			      const TDE2_RECT_S *pstDstRect);

GK_S32 GK_TDE2_Bitblit(TDE_HANDLE s32Handle,
		       const TDE2_SURFACE_S *pstBackGround,
		       const TDE2_RECT_S *pstBackGroundRect,
		       const TDE2_SURFACE_S *pstForeGround,
		       const TDE2_RECT_S *pstForeGroundRect,
		       const TDE2_SURFACE_S *pstDst,
		       const TDE2_RECT_S *pstDstRect, const TDE2_OPT_S *pstOpt);

GK_S32 GK_TDE2_SolidDraw(TDE_HANDLE s32Handle,
			 const TDE2_SURFACE_S *pstForeGround,
			 const TDE2_RECT_S *pstForeGroundRect,
			 const TDE2_SURFACE_S *pstDst,
			 const TDE2_RECT_S *pstDstRect,
			 const TDE2_FILLCOLOR_S *pstFillColor,
			 const TDE2_OPT_S *pstOpt);

GK_S32 GK_TDE2_MbBlit(TDE_HANDLE s32Handle, const TDE2_MB_S *pstMB,
		      const TDE2_RECT_S *pstMbRect,
		      const TDE2_SURFACE_S *pstDst,
		      const TDE2_RECT_S *pstDstRect,
		      const TDE2_MBOPT_S *pstMbOpt);

GK_S32 GK_TDE2_BitmapMaskRop(
	TDE_HANDLE s32Handle, const TDE2_SURFACE_S *pstBackGround,
	const TDE2_RECT_S *pstBackGroundRect,
	const TDE2_SURFACE_S *pstForeGround,
	const TDE2_RECT_S *pstForeGroundRect, const TDE2_SURFACE_S *pstMask,
	const TDE2_RECT_S *pstMaskRect, const TDE2_SURFACE_S *pstDst,
	const TDE2_RECT_S *pstDstRect, TDE2_ROP_CODE_E enRopCode_Color,
	TDE2_ROP_CODE_E enRopCode_Alpha);

GK_S32 GK_TDE2_BitmapMaskBlend(TDE_HANDLE s32Handle,
			       const TDE2_SURFACE_S *pstBackGround,
			       const TDE2_RECT_S *pstBackGroundRect,
			       const TDE2_SURFACE_S *pstForeGround,
			       const TDE2_RECT_S *pstForeGroundRect,
			       const TDE2_SURFACE_S *pstMask,
			       const TDE2_RECT_S *pstMaskRect,
			       const TDE2_SURFACE_S *pstDst,
			       const TDE2_RECT_S *pstDstRect, GK_U8 u8Alpha,
			       TDE2_ALUCMD_E enBlendMode);

GK_S32 GK_TDE2_Rotate(TDE_HANDLE s32Handle, const TDE2_SURFACE_S *pstSrc,
		      const TDE2_RECT_S *pstSrcRect,
		      const TDE2_SURFACE_S *pstDst,
		      const TDE2_RECT_S *pstDstRect,
		      TDE_ROTATE_ANGLE_E enRotateAngle);

GK_S32 GK_TDE2_SetDeflickerLevel(TDE_DEFLICKER_LEVEL_E enDeflickerLevel);

GK_S32 GK_TDE2_GetDeflickerLevel(TDE_DEFLICKER_LEVEL_E *pDeflickerLevel);

GK_S32 GK_TDE2_SetAlphaThresholdValue(GK_U8 u8ThresholdValue);

GK_S32 GK_TDE2_GetAlphaThresholdValue(GK_U8 *pu8ThresholdValue);

GK_S32 GK_TDE2_SetAlphaThresholdState(GK_BOOL bEnAlphaThreshold);

GK_S32 GK_TDE2_GetAlphaThresholdState(GK_BOOL *p_bEnAlphaThreshold);

GK_S32 GK_TDE2_PatternFill(TDE_HANDLE s32Handle,
			   const TDE2_SURFACE_S *pstBackGround,
			   const TDE2_RECT_S *pstBackGroundRect,
			   const TDE2_SURFACE_S *pstForeGround,
			   const TDE2_RECT_S *pstForeGroundRect,
			   const TDE2_SURFACE_S *pstDst,
			   const TDE2_RECT_S *pstDstRect,
			   const TDE2_PATTERN_FILL_OPT_S *pstOpt);

GK_S32 GK_TDE2_EnableRegionDeflicker(GK_BOOL bRegionDeflicker);

GK_S32 GK_TDE2_MultiBlending(TDE_HANDLE s32Handle,
			     const TDE_SURFACE_LIST_S *pstSurfaceList);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
