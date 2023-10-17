/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_VQE_REGISTER_API_H__
#define __GK_VQE_REGISTER_API_H__

#include "type.h"
#include "common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

GK_VOID *GK_VQE_RECORD_GetHandle(GK_VOID);
GK_VOID *GK_VQE_HPF_GetHandle(GK_VOID);
GK_VOID *GK_VQE_AEC_GetHandle(GK_VOID);
GK_VOID *GK_VQE_ANR_GetHandle(GK_VOID);
GK_VOID *GK_VQE_RNR_GetHandle(GK_VOID);
GK_VOID *GK_VQE_HDR_GetHandle(GK_VOID);
GK_VOID *GK_VQE_DRC_GetHandle(GK_VOID);
GK_VOID *GK_VQE_PEQ_GetHandle(GK_VOID);
GK_VOID *GK_VQE_AGC_GetHandle(GK_VOID);
GK_VOID *GK_VQE_EQ_GetHandle(GK_VOID);
GK_VOID *GK_VQE_RESAMPLE_GetHandle(GK_VOID);
GK_VOID *GK_VQE_GAIN_GetHandle(GK_VOID);
GK_VOID *GK_VQE_TALKV2_GetHandle(GK_VOID);
GK_VOID *GK_VQE_WNR_GetHandle(GK_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
