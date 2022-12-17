/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_AUDIO_BCD_H__
#define __GK_AUDIO_BCD_H__

#include "type.h"
#include "common.h"

#include "audio_bcd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

gk_s32 gk_audio_baby_crying_detection_init(bcd_handle *bcd, gk_s32 sample_rate, const audio_bcd_config *bcd_config);

gk_s32 gk_audio_baby_crying_detection_process(bcd_handle bcd, audio_bcd_process_data *input_data,
    audio_bcd_process_data *output_data);

gk_s32 gk_audio_baby_crying_detection_deinit(bcd_handle bcd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

