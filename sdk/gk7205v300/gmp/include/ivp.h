/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef _IVP_H_
#define _IVP_H_

#include "type.h"
#include "errcode.h"
#include "common.h"
#include "comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define IVP_MAX_VENC_CHN_NUM 16
#define IVP_MAX_VIPIPE_NUM 2
#define IVP_MAX_CLASS 2
#define IVP_CLASS_NAME_LEN 32

typedef struct{
    gk_u64  physical_addr;
    gk_u64  virtual_addr;
    gk_u32  memory_size;
}ivp_mem_info;

typedef struct{
    gk_float threshold;
}ivp_ctrl_attr;

typedef struct{
    gk_s32 x;
    gk_s32 y;
    gk_u32 width;
    gk_u32 height;
}ivp_rect;

typedef struct{
	ivp_rect rect;
	gk_float quality;
}ivp_obj;

typedef struct{
    gk_char class_name[IVP_CLASS_NAME_LEN];
    gk_u32 rect_num;
    gk_u32 rect_capcity;
    ivp_obj *objs;
}ivp_obj_of_one_class;

typedef struct{
    gk_s32 class_num;
    ivp_obj_of_one_class obj_class[IVP_MAX_CLASS];
}ivp_obj_array;

typedef struct{
    gk_bool enable;
    gk_u32 threshold;
}ivp_roi_attr;

typedef enum{
    IVP_ROI_MB_MODE_4X4,
    IVP_ROI_MB_MODE_8X8,
    IVP_ROI_MB_MODE_16X16,
    IVP_ROI_MB_MODE_BUTT
}ivp_roi_mb_mode;

typedef struct{
    ivp_roi_mb_mode roi_mb_mode;
    gk_u32 img_width;
    gk_u32 img_height;
    gk_u8 *mb_map;
}ivp_roi_map;

gk_s32 ivp_init(gk_void);

gk_s32 ivp_deinit(gk_void);

gk_s32 ivp_load_resource_from_memory(const ivp_mem_info *ivp_file_mem_info, gk_s32 *ivp_handle);

gk_s32 ivp_unload_resource(gk_s32 ivp_handle);

gk_s32 ivp_set_ctrl_attr(gk_s32 ivp_handle, const ivp_ctrl_attr *_ivp_ctrl_attr);

gk_s32 ivp_get_ctrl_attr(gk_s32 ivp_handle, ivp_ctrl_attr *_ivp_ctrl_attr);

gk_s32 ivp_set_fd_ctrl_attr(gk_s32 ivp_handle, const ivp_ctrl_attr *_ivp_ctrl_attr);

gk_s32 ivp_get_fd_ctrl_attr(gk_s32 ivp_handle, ivp_ctrl_attr *_ivp_ctrl_attr);

gk_s32 ivp_set_venc_low_bitrate(gk_s32 ivp_handle, gk_s32 venc_chn, gk_bool enable);

gk_s32 ivp_get_venc_low_bitrate(gk_s32 ivp_handle, gk_s32 venc_chn, gk_bool *enable);

gk_s32 ivp_set_advance_isp(gk_s32 ivp_handle, gk_s32 vi_pipe, gk_bool enable);

gk_s32 ivp_get_advance_isp(gk_s32 ivp_handle, gk_s32 vi_pipe, gk_bool *enable);

gk_s32 ivp_set_roi_attr(gk_s32 ivp_handle, const ivp_roi_attr *roi_attr);

gk_s32 ivp_get_roi_attr(gk_s32 ivp_handle, ivp_roi_attr *roi_attr);

gk_s32 ivp_set_roi_map(gk_s32 ivp_handle, const ivp_roi_map *roi_map);

gk_s32 ivp_process(gk_s32 ivp_handle, const VIDEO_FRAME_INFO_S *src_frame, gk_bool *obj_alarm);

gk_s32 ivp_process_ex(gk_s32 ivp_handle, const VIDEO_FRAME_INFO_S *src_frame, ivp_obj_array *obj_array);

typedef enum EN_IVP_ERR_CODE_E {
    ERR_IVP_READ_FILE      = 0x41,
    ERR_IVP_OPERATE_FILE   = 0x42,
    ERR_IVP_PROCESS_ERR    = 0x43,
    ERR_IVP_INIT_FAIL      = 0x44,
    ERR_IVP_EXIT_FAIL      = 0x45,
    ERR_IVP_LOAD_RESOURCE_FAIL = 0x46,

    ERR_IVP_BUTT
}EN_IVP_ERR_CODE_E;

#define ERR_CODE_IVP_NULL_PTR          DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_IVP_ILLEGAL_PARAM     DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_IVP_NOT_SURPPORT      DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define ERR_CODE_IVP_INIT_FAIL         DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_INIT_FAIL)
#define ERR_CODE_IVP_EXIT_FAIL         DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_EXIT_FAIL)
#define ERR_CODE_IVP_NOMEM             DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define ERR_CODE_IVP_EXIST             DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define ERR_CODE_IVP_UNEXIST           DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define ERR_CODE_IVP_READ_FILE         DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_READ_FILE)
#define ERR_CODE_IVP_OPERATE_FILE      DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_OPERATE_FILE)
#define ERR_CODE_IVP_PROCESS_ERR       DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_PROCESS_ERR)
#define ERR_CODE_IVP_LOAD_RESOURCE_FAIL       DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_LOAD_RESOURCE_FAIL)
#define ERR_CODE_IVP_BUSY              DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

