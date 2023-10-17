#ifndef HICOMPAT_H
#define HICOMPAT_H

#ifdef SDK_CODE
#if SDK_CODE == 0x3516E200
#define GK_API_ISP_SensorRegCallBack HI_MPI_ISP_SensorRegCallBack
#define GK_API_ISP_SensorUnRegCallBack HI_MPI_ISP_SensorUnRegCallBack
#define GK_API_AE_SensorRegCallBack HI_MPI_AE_SensorRegCallBack
#define GK_API_AE_SensorUnRegCallBack HI_MPI_AE_SensorUnRegCallBack
#define GK_API_AWB_SensorRegCallBack HI_MPI_AWB_SensorRegCallBack
#define GK_API_AWB_SensorUnRegCallBack HI_MPI_AWB_SensorUnRegCallBack
#endif

GK_S32 GK_API_ISP_SensorRegCallBack(VI_PIPE ViPipe,
				    ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
				    ISP_SENSOR_REGISTER_S *pstRegister);
GK_S32 GK_API_ISP_SensorUnRegCallBack(VI_PIPE ViPipe, SENSOR_ID SensorId);

GK_S32 GK_API_AE_SensorRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
				   ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
				   AE_SENSOR_REGISTER_S *pstRegister);
GK_S32 GK_API_AE_SensorUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
				     SENSOR_ID SensorId);

GK_S32 GK_API_AWB_SensorRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib,
				    ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
				    AWB_SENSOR_REGISTER_S *pstRegister);
GK_S32 GK_API_AWB_SensorUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib,
				      SENSOR_ID SensorId);
#endif

#endif /* HICOMPAT_H */
