/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __AUDIO_BCD_H__
#define __AUDIO_BCD_H__

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ERR_CODE_BCD_INVALID_HANDLE (gk_u32)(0xA0148100)
#define ERR_CODE_BCD_MEMORY_ERROR (gk_u32)(0xA0148101)
#define ERR_CODE_BCD_ILLEGAL_PARAM (gk_u32)(0xA0148102)
#define ERR_CODE_BCD_NULL_PTR (gk_u32)(0xA0148103)
#define ERR_CODE_BCD_NOT_PERM (gk_u32)(0xA0148104)

typedef gk_void *bcd_handle;
typedef gk_s32 (*fn_bcd_callback)(gk_void *);

typedef struct {
	gk_bool usr_mode;
	gk_bool bypass;
	gk_u32 alarm_threshold;
	gk_u32 time_limit;
	gk_u32 time_limit_threshold_count;
	gk_u32 interval_time;
	fn_bcd_callback callback;
} audio_bcd_config;

typedef struct {
	gk_s16 *data;
	gk_s32 data_size;
} audio_bcd_process_data;

gk_s32 audio_baby_crying_detection_init(bcd_handle *bcd, gk_s32 sample_rate,
					const audio_bcd_config *bcd_config);

gk_s32 audio_baby_crying_detection_process(bcd_handle bcd,
					   audio_bcd_process_data *input_data,
					   audio_bcd_process_data *output_data);

gk_s32 audio_baby_crying_detection_deinit(bcd_handle bcd);

#ifdef __cplusplus
}
#endif
#endif