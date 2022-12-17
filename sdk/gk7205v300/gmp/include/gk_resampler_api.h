/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_RESAMPLER_API_H__
#define __GK_RESAMPLER_API_H__

#include "type.h"
#include "common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

GK_VOID *GK_AUDIO_Resampler_Create(GK_S32 s32Inrate, GK_S32 s32Outrate, GK_S32 s32Chans);

GK_S32 GK_AUDIO_Resampler_Process(GK_VOID *inst, GK_S16 *s16Inbuf, GK_S32 s32Insamps, GK_S16 *s16Outbuf);

GK_VOID GK_AUDIO_Resampler_Destroy(GK_VOID *inst);

GK_S32 GK_AUDIO_Resampler_GetMaxOutputNum(GK_VOID *inst, GK_S32 s32Insamps);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

