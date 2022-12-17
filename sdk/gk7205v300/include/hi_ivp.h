/*
 * 
 */

#ifndef __HI_IVP_H__
#define __HI_IVP_H__

#include "hi_type.h"
#include "hi_errno.h"
#include "hi_common.h"
#include "hi_comm_video.h"
#include "ivp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_IVP_MAX_VENC_CHN_NUM IVP_MAX_VENC_CHN_NUM
#define HI_IVP_MAX_VIPIPE_NUM IVP_MAX_VIPIPE_NUM
#define HI_IVP_MAX_CLASS IVP_MAX_CLASS
#define HI_IVP_CLASS_NAME_LEN IVP_CLASS_NAME_LEN
typedef ivp_mem_info hi_ivp_mem_info;
typedef ivp_ctrl_attr hi_ivp_ctrl_attr;
typedef ivp_rect hi_ivp_rect;
typedef ivp_obj hi_ivp_obj;
typedef ivp_obj_of_one_class hi_ivp_obj_of_one_class;
typedef ivp_obj_array hi_ivp_obj_array;
typedef ivp_roi_attr hi_ivp_roi_attr;
#define HI_IVP_ROI_MB_MODE_4X4 IVP_ROI_MB_MODE_4X4
#define HI_IVP_ROI_MB_MODE_8X8 IVP_ROI_MB_MODE_8X8
#define HI_IVP_ROI_MB_MODE_16X16 IVP_ROI_MB_MODE_16X16
#define HI_IVP_ROI_MB_MODE_BUTT IVP_ROI_MB_MODE_BUTT
typedef ivp_roi_mb_mode hi_ivp_roi_mb_mode;
typedef ivp_roi_map hi_ivp_roi_map;
#define HI_ERR_IVP_NULL_PTR ERR_CODE_IVP_NULL_PTR
#define HI_ERR_IVP_ILLEGAL_PARAM ERR_CODE_IVP_ILLEGAL_PARAM
#define HI_ERR_IVP_NOT_SURPPORT ERR_CODE_IVP_NOT_SURPPORT
#define HI_ERR_IVP_INIT_FAIL ERR_CODE_IVP_INIT_FAIL
#define HI_ERR_IVP_EXIT_FAIL ERR_CODE_IVP_EXIT_FAIL
#define HI_ERR_IVP_NOMEM ERR_CODE_IVP_NOMEM
#define HI_ERR_IVP_EXIST ERR_CODE_IVP_EXIST
#define HI_ERR_IVP_UNEXIST ERR_CODE_IVP_UNEXIST
#define HI_ERR_IVP_READ_FILE ERR_CODE_IVP_READ_FILE
#define HI_ERR_IVP_OPERATE_FILE ERR_CODE_IVP_OPERATE_FILE
#define HI_ERR_IVP_PROCESS_ERR ERR_CODE_IVP_PROCESS_ERR
#define HI_ERR_IVP_LOAD_RESOURCE_FAIL ERR_CODE_IVP_LOAD_RESOURCE_FAIL
#define HI_ERR_IVP_BUSY ERR_CODE_IVP_BUSY

/*****************************************************************************
*   Prototype    : hi_ivp_init
*   Description  : Intelligent Video Process(IVP) initialization.
*   Parameters   : HI_VOID
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2018/10/26
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_init(hi_void);

/*****************************************************************************
*   Prototype    : hi_ivp_deinit
*   Description  : Intelligent Video Process(IVP) exit.
*   Parameters   : HI_VOID.
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2018/10/26
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_deinit(hi_void);

/*****************************************************************************
*   Prototype    : hi_ivp_load_resource_from_memory
*   Description  : Load resource from memory.
*   Parameters   : const hi_ivp_mem_info *ivp_file_mem_info:Input
                   hi_s32 *ivp_handle:Output
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2018/10/26
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_load_resource_from_memory(const hi_ivp_mem_info *ivp_file_mem_info, hi_s32 *ivp_handle);

/*****************************************************************************
*   Prototype    : hi_ivp_unload_resource
*   Description  : Unload resource.
*   Parameters   : hi_s32 ivp_handle:Input
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2018/10/26
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_unload_resource(hi_s32 ivp_handle);

/*****************************************************************************
*   Prototype    : hi_ivp_set_ctrl_attr
*   Description  : Set ctrl param,include SMD threshold.
*   Parameters   : hi_s32 ivp_handle:Input
                   const hi_ivp_ctrl_attr *ivp_ctrl_attr:input
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2018/10/26
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_set_ctrl_attr(hi_s32 ivp_handle, const hi_ivp_ctrl_attr *ivp_ctrl_attr);

/*****************************************************************************
*   Prototype    : hi_ivp_get_ctrl_attr
*   Description  : Get ctrl param,include SMD threshold.
*   Parameters   : hi_s32 ivp_handle:Input
                   hi_ivp_ctrl_attr *ivp_ctrl_attr:Output
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2018/10/26
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_get_ctrl_attr(hi_s32 ivp_handle, hi_ivp_ctrl_attr *ivp_ctrl_attr);

/*****************************************************************************
*   Prototype    : hi_ivp_set_fd_ctrl_attr
*   Description  : Set FD ctrl param,include FD threshold. Only Hi3518EV300 support.
*   Parameters   : hi_s32 ivp_handle:Input
                   const hi_ivp_ctrl_attr *ivp_ctrl_attr:input
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2019/05/22
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_set_fd_ctrl_attr(hi_s32 ivp_handle, const hi_ivp_ctrl_attr *ivp_ctrl_attr);

/*****************************************************************************
*   Prototype    : hi_ivp_get_fd_ctrl_attr
*   Description  : Get FD ctrl param,include FD threshold. Only Hi3518EV300 support.
*   Parameters   : hi_s32 ivp_handle:Input
                   hi_ivp_ctrl_attr *ivp_ctrl_attr:Output
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2019/05/22
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_get_fd_ctrl_attr(hi_s32 ivp_handle, hi_ivp_ctrl_attr *ivp_ctrl_attr);

/*****************************************************************************
*   Prototype    : hi_ivp_set_venc_low_bitrate
*   Description  : Enable or disable venc low bitrate.
*   Parameters   : hi_s32 ivp_handle:Input
                   hi_s32 venc_chn:Input
                   hi_bool enable:Input
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2018/10/26
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_set_venc_low_bitrate(hi_s32 ivp_handle, hi_s32 venc_chn, hi_bool enable);

/*****************************************************************************
*   Prototype    : hi_ivp_get_venc_low_bitrate
*   Description  : Get status of venc low bitrate.
*   Parameters   : hi_s32 ivp_handle:Input
                   hi_s32 venc_chn:Input
                   hi_bool *enable:Output
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2018/10/26
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_get_venc_low_bitrate(hi_s32 ivp_handle, hi_s32 venc_chn, hi_bool *enable);

/*****************************************************************************
*   Prototype    : hi_ivp_set_advance_isp
*   Description  : Enable or disable advance Isp Attr.
*   Parameters   : hi_s32 ivp_handle:Input
                   hi_s32 vi_pipe:Input
                   hi_bool enable:Input
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2018/10/26
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_set_advance_isp(hi_s32 ivp_handle, hi_s32 vi_pipe, hi_bool enable);

/*****************************************************************************
*   Prototype    : hi_ivp_get_advance_isp
*   Description  : Get status of advance ISP attr.
*   Parameters   : hi_s32 ivp_handle:Input
                   hi_s32 vi_pipe:Input
                   hi_bool *enable:Output
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2018/10/26
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_get_advance_isp(hi_s32 ivp_handle, hi_s32 vi_pipe, hi_bool *enable);

/*****************************************************************************
*   Prototype    : hi_ivp_set_roi_attr
*   Description  : Set ROI Attr.
*   Parameters   : hi_s32 ivp_handle:Input
                   const hi_ivp_roi_attr *roi_attr:Input
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2019/3/6
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_set_roi_attr(hi_s32 ivp_handle, const hi_ivp_roi_attr *roi_attr);

/*****************************************************************************
*   Prototype    : hi_ivp_get_roi_attr
*   Description  : Get ROI Attr.
*   Parameters   : hi_s32 ivp_handle:Input
                   hi_ivp_roi_attr *roi_attr:Output
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2019/3/6
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_get_roi_attr(hi_s32 ivp_handle, hi_ivp_roi_attr *roi_attr);

/*****************************************************************************
*   Prototype    : hi_ivp_set_roi_map
*   Description  : Set ROI Map.
*   Parameters   : hi_s32 ivp_handle:Input
                   const hi_ivp_roi_map *roi_map:Input
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2019/3/6
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_set_roi_map(hi_s32 ivp_handle, const hi_ivp_roi_map *roi_map);

/*****************************************************************************
*   Prototype    : hi_ivp_process
*   Description  : Process.
*   Parameters   : hi_s32 ivp_handle:Input
                   const VIDEO_FRAME_INFO_S *src_frame:Input
                   hi_bool *obj_alarm:Output,alarm info.
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2018/10/26
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_process(hi_s32 ivp_handle, const VIDEO_FRAME_INFO_S *src_frame, hi_bool *obj_alarm);

/*****************************************************************************
*   Prototype    : hi_ivp_process_ex
*   Description  : Process.
*   Parameters   : hi_s32 ivp_handle:Input
                   const VIDEO_FRAME_INFO_S *src_frame:Input
                   hi_ivp_obj_array *obj_array:Output
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   History:
*
*       1.  Date         : 2018/10/26
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
hi_s32 hi_ivp_process_ex(hi_s32 ivp_handle, const VIDEO_FRAME_INFO_S *src_frame, hi_ivp_obj_array *obj_array);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* __HI_IVP_H__ */
