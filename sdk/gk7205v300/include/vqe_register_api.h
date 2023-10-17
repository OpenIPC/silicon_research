/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __VQE_REGISTER_API_H__
#define __VQE_REGISTER_API_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

GK_VOID *VQE_RECORD_GetHandle(GK_VOID);
GK_VOID *VQE_HPF_GetHandle(GK_VOID);
GK_VOID *VQE_AEC_GetHandle(GK_VOID);
GK_VOID *VQE_ANR_GetHandle(GK_VOID);
GK_VOID *VQE_RNR_GetHandle(GK_VOID);
GK_VOID *VQE_HDR_GetHandle(GK_VOID);
GK_VOID *VQE_DRC_GetHandle(GK_VOID);
GK_VOID *VQE_PEQ_GetHandle(GK_VOID);
GK_VOID *VQE_AGC_GetHandle(GK_VOID);
GK_VOID *VQE_EQ_GetHandle(GK_VOID);
GK_VOID *VQE_RESAMPLE_GetHandle(GK_VOID);
GK_VOID *VQE_GAIN_GetHandle(GK_VOID);
GK_VOID *VQE_TALKV2_GetHandle(GK_VOID);
GK_VOID *VQE_WNR_GetHandle(GK_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
