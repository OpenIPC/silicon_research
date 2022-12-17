/*
 * 
 */

#ifndef __HI_COMM_VPSS_H__
#define __HI_COMM_VPSS_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hi_errno.h"
#include "hi_comm_video.h"
#include "comm_vpss.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_ERR_VPSS_NULL_PTR ERR_CODE_VPSS_NULL_PTR
#define HI_ERR_VPSS_NOTREADY ERR_CODE_VPSS_NOTREADY
#define HI_ERR_VPSS_INVALID_DEVID ERR_CODE_VPSS_INVALID_DEVID
#define HI_ERR_VPSS_INVALID_CHNID ERR_CODE_VPSS_INVALID_CHNID
#define HI_ERR_VPSS_EXIST ERR_CODE_VPSS_EXIST
#define HI_ERR_VPSS_UNEXIST ERR_CODE_VPSS_UNEXIST
#define HI_ERR_VPSS_NOT_SUPPORT ERR_CODE_VPSS_NOT_SUPPORT
#define HI_ERR_VPSS_NOT_PERM ERR_CODE_VPSS_NOT_PERM
#define HI_ERR_VPSS_NOMEM ERR_CODE_VPSS_NOMEM
#define HI_ERR_VPSS_NOBUF ERR_CODE_VPSS_NOBUF
#define HI_ERR_VPSS_SIZE_NOT_ENOUGH ERR_CODE_VPSS_SIZE_NOT_ENOUGH
#define HI_ERR_VPSS_ILLEGAL_PARAM ERR_CODE_VPSS_ILLEGAL_PARAM
#define HI_ERR_VPSS_BUSY ERR_CODE_VPSS_BUSY
#define HI_ERR_VPSS_BUF_EMPTY ERR_CODE_VPSS_BUF_EMPTY

typedef tV1IEy  tV56aIEy;
typedef tV1SFy tV56aSFy;
typedef tV1MDy tV56aMDy;
typedef tV1TFy tV56aTFy;
typedef tV1NRc tV56aNRc;

typedef tV2_VPSS_IEy  tV500_VPSS_IEy;
typedef tV2_VPSS_SFy  tV500_VPSS_SFy;
typedef tV2_VPSS_MDy  tV500_VPSS_MDy;
typedef tV2_VPSS_RFs  tV500_VPSS_RFs;
typedef tV2_VPSS_TFy  tV500_VPSS_TFy;
typedef tV2_VPSS_pNRc tV500_VPSS_pNRc;
typedef tV2_VPSS_NRc  tV500_VPSS_NRc;

typedef tV3_VPSS_IEy tV200_VPSS_IEy;
typedef tV3_VPSS_SFy tV200_VPSS_SFy;
typedef tV3_VPSS_MDy tV200_VPSS_MDy;
typedef tV3_VPSS_TFy tV200_VPSS_TFy;
typedef tV3_VPSS_NRc tV200_VPSS_NRc;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* __HI_COMM_VPSS_H__ */
