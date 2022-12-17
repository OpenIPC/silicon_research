/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef _GK_IVP_H_
#define _GK_IVP_H_

#include "type.h"
#include "errcode.h"
#include "common.h"
#include "comm_video.h"
#include "ivp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

gk_s32 gk_ivp_init(gk_void);

gk_s32 gk_ivp_deinit(gk_void);

gk_s32 gk_ivp_load_resource_from_memory(const ivp_mem_info *ivp_file_mem_info, gk_s32 *ivp_handle);

gk_s32 gk_ivp_unload_resource(gk_s32 ivp_handle);

gk_s32 gk_ivp_set_ctrl_attr(gk_s32 ivp_handle, const ivp_ctrl_attr *_ivp_ctrl_attr);

gk_s32 gk_ivp_get_ctrl_attr(gk_s32 ivp_handle, ivp_ctrl_attr *_ivp_ctrl_attr);

gk_s32 gk_ivp_set_fd_ctrl_attr(gk_s32 ivp_handle, const ivp_ctrl_attr *_ivp_ctrl_attr);

gk_s32 gk_ivp_get_fd_ctrl_attr(gk_s32 ivp_handle, ivp_ctrl_attr *_ivp_ctrl_attr);

gk_s32 gk_ivp_set_venc_low_bitrate(gk_s32 ivp_handle, gk_s32 venc_chn, gk_bool enable);

gk_s32 gk_ivp_get_venc_low_bitrate(gk_s32 ivp_handle, gk_s32 venc_chn, gk_bool *enable);

gk_s32 gk_ivp_set_advance_isp(gk_s32 ivp_handle, gk_s32 vi_pipe, gk_bool enable);

gk_s32 gk_ivp_get_advance_isp(gk_s32 ivp_handle, gk_s32 vi_pipe, gk_bool *enable);

gk_s32 gk_ivp_set_roi_attr(gk_s32 ivp_handle, const ivp_roi_attr *roi_attr);

gk_s32 gk_ivp_get_roi_attr(gk_s32 ivp_handle, ivp_roi_attr *roi_attr);

gk_s32 gk_ivp_set_roi_map(gk_s32 ivp_handle, const ivp_roi_map *roi_map);

gk_s32 gk_ivp_process(gk_s32 ivp_handle, const VIDEO_FRAME_INFO_S *src_frame, gk_bool *obj_alarm);

gk_s32 gk_ivp_process_ex(gk_s32 ivp_handle, const VIDEO_FRAME_INFO_S *src_frame, ivp_obj_array *obj_array);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

