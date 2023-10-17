/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __GK_API_REGION_H__
#define __GK_API_REGION_H__

#include "comm_region.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

GK_S32 GK_API_RGN_Create(RGN_HANDLE Handle, const RGN_ATTR_S *pstRegion);
GK_S32 GK_API_RGN_Destroy(RGN_HANDLE Handle);

GK_S32 GK_API_RGN_GetAttr(RGN_HANDLE Handle, RGN_ATTR_S *pstRegion);
GK_S32 GK_API_RGN_SetAttr(RGN_HANDLE Handle, const RGN_ATTR_S *pstRegion);

GK_S32 GK_API_RGN_SetBitMap(RGN_HANDLE Handle, const BITMAP_S *pstBitmap);

GK_S32 GK_API_RGN_AttachToChn(RGN_HANDLE Handle, const MPP_CHN_S *pstChn,
			      const RGN_CHN_ATTR_S *pstChnAttr);
GK_S32 GK_API_RGN_DetachFromChn(RGN_HANDLE Handle, const MPP_CHN_S *pstChn);

GK_S32 GK_API_RGN_SetDisplayAttr(RGN_HANDLE Handle, const MPP_CHN_S *pstChn,
				 const RGN_CHN_ATTR_S *pstChnAttr);
GK_S32 GK_API_RGN_GetDisplayAttr(RGN_HANDLE Handle, const MPP_CHN_S *pstChn,
				 RGN_CHN_ATTR_S *pstChnAttr);

GK_S32 GK_API_RGN_GetCanvasInfo(RGN_HANDLE Handle,
				RGN_CANVAS_INFO_S *pstCanvasInfo);
GK_S32 GK_API_RGN_UpdateCanvas(RGN_HANDLE Handle);

GK_S32 GK_API_RGN_BatchBegin(RGN_HANDLEGROUP *pu32Group, GK_U32 u32Num,
			     const RGN_HANDLE handle[]);
GK_S32 GK_API_RGN_BatchEnd(RGN_HANDLEGROUP u32Group);

GK_S32 GK_API_RGN_GetFd(GK_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
