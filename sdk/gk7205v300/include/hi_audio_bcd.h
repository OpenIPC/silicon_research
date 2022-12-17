/*
 * 
 */

#ifndef __HI_AUDIO_BCD_H__
#define __HI_AUDIO_BCD_H__

#include "hi_type.h"
#include "audio_bcd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_ERR_BCD_INVALID_HANDLE ERR_CODE_BCD_INVALID_HANDLE
#define HI_ERR_BCD_MEMORY_ERROR ERR_CODE_BCD_MEMORY_ERROR
#define HI_ERR_BCD_ILLEGAL_PARAM ERR_CODE_BCD_ILLEGAL_PARAM
#define HI_ERR_BCD_NULL_PTR ERR_CODE_BCD_NULL_PTR
#define HI_ERR_BCD_NOT_PERM ERR_CODE_BCD_NOT_PERM

typedef audio_bcd_config hi_bcd_config;
typedef audio_bcd_process_data hi_bcd_process_data;

hi_s32 hi_baby_crying_detection_init(bcd_handle *bcd, hi_s32 sample_rate, const hi_bcd_config *bcd_config);

hi_s32 hi_baby_crying_detection_process(bcd_handle bcd, hi_bcd_process_data *input_data,
    hi_bcd_process_data *output_data);

hi_s32 hi_baby_crying_detection_deinit(bcd_handle bcd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* __HI_AUDIO_BCD_H__ */
