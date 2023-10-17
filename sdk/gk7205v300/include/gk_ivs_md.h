/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef _GK_IVS_MD_H_
#define _GK_IVS_MD_H_

#include "md.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

GK_S32 GK_IVS_MD_Init(GK_VOID);

GK_S32 GK_IVS_MD_Exit(GK_VOID);

GK_S32 GK_IVS_MD_CreateChn(MD_CHN MdChn, MD_ATTR_S *pstMdAttr);

GK_S32 GK_IVS_MD_DestroyChn(MD_CHN MdChn);

GK_S32 GK_IVS_MD_SetChnAttr(MD_CHN MdChn, MD_ATTR_S *pstMdAttr);

GK_S32 GK_IVS_MD_GetChnAttr(MD_CHN MdChn, MD_ATTR_S *pstMdAttr);

GK_S32 GK_IVS_MD_GetBg(MD_CHN MdChn, IVE_DST_IMAGE_S *pstBg);

GK_S32 GK_IVS_MD_Process(MD_CHN MdChn, IVE_SRC_IMAGE_S *pstCur,
			 IVE_SRC_IMAGE_S *pstRef, IVE_DST_IMAGE_S *pstSad,
			 IVE_DST_MEM_INFO_S *pstBlob);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
