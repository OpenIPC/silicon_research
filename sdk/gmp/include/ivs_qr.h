/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef _IVS_QR_H_
#define _IVS_QR_H_

#include "common_qr.h"
#include "comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

gk_s32 ivs_qr_init(gk_void);

gk_s32 ivs_qr_get_buf_size(gk_u32 max_img_width, gk_u32 max_img_height, gk_u32 *size);

gk_s32 ivs_qr_proc(VIDEO_FRAME_INFO_S *src_frame, qr_mem_info *buf, qr_info *qr_info);

gk_s32 ivs_qr_deinit(gk_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
