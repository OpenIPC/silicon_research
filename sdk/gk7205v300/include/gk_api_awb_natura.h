/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __GK_API_AWB_NATRUA_H__
#define __GK_API_AWB_NATRUA_H__

#include "comm_isp.h"
#include "comm_3a.h"
#include "awb_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* The interface of awb lib register to isp. */
GK_S32 GK_API_AWB_Register(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib);
GK_S32 GK_API_AWB_UnRegister(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib);

/* The callback function of sensor register to awb lib. */
GK_S32 GK_API_AWB_SensorRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib,
				    ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
				    AWB_SENSOR_REGISTER_S *pstRegister);
GK_S32 GK_API_AWB_SensorUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib,
				      SENSOR_ID SensorId);

GK_S32 GK_API_ISP_SetWBAttr(VI_PIPE ViPipe, const ISP_WB_ATTR_S *pstWBAttr);
GK_S32 GK_API_ISP_GetWBAttr(VI_PIPE ViPipe, ISP_WB_ATTR_S *pstWBAttr);

GK_S32 GK_API_ISP_SetAWBAttrEx(VI_PIPE ViPipe, ISP_AWB_ATTR_EX_S *pstAWBAttrEx);
GK_S32 GK_API_ISP_GetAWBAttrEx(VI_PIPE ViPipe, ISP_AWB_ATTR_EX_S *pstAWBAttrEx);

GK_S32 GK_API_ISP_SetCCMAttr(VI_PIPE ViPipe,
			     const ISP_COLORMATRIX_ATTR_S *pstCCMAttr);
GK_S32 GK_API_ISP_GetCCMAttr(VI_PIPE ViPipe,
			     ISP_COLORMATRIX_ATTR_S *pstCCMAttr);

GK_S32 GK_API_ISP_SetSaturationAttr(VI_PIPE ViPipe,
				    const ISP_SATURATION_ATTR_S *pstSatAttr);
GK_S32 GK_API_ISP_GetSaturationAttr(VI_PIPE ViPipe,
				    ISP_SATURATION_ATTR_S *pstSatAttr);

GK_S32 GK_API_ISP_QueryWBInfo(VI_PIPE ViPipe, ISP_WB_INFO_S *pstWBInfo);
GK_S32 GK_API_ISP_CalGainByTemp(VI_PIPE ViPipe, const ISP_WB_ATTR_S *pstWBAttr,
				GK_U16 u16ColorTemp, GK_S16 s16Shift,
				GK_U16 *pu16AWBGain);
GK_S32 GK_API_ISP_SetSpecAwbAttr(VI_PIPE ViPipe,
				 const ISP_SPECAWB_ATTR_S *pstSpecAwbAttr);
GK_S32 GK_API_ISP_SetCaaControlAttr(
	VI_PIPE ViPipe, const ISP_SPECAWB_CAA_CONTROl_S *pstSpecAwbCaaAttr);
GK_S32
GK_API_ISP_GetCaaControlAttr(VI_PIPE ViPipe,
			     ISP_SPECAWB_CAA_CONTROl_S *pstSpecAwbCaaAttr);
GK_S32 GK_API_ISP_GetSpecAwbAttr(VI_PIPE ViPipe,
				 ISP_SPECAWB_ATTR_S *pstSpecAwbAttr);
GK_S32 GK_API_ISP_SetSpecAwbControlAttr(
	VI_PIPE ViPipe, ISP_SPECAWB_CONTROL_ATTR_S *pstSpecAwbControlAttr);
GK_S32 GK_API_ISP_GetSpecAwbControlAttr(
	VI_PIPE ViPipe, ISP_SPECAWB_CONTROL_ATTR_S *pstSpecAwbControlAttr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
