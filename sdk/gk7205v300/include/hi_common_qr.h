/*
 * 
 */

#ifndef __HI_COMMON_QR_H__
#define __HI_COMMON_QR_H__

#include "hi_type.h"
#include "hi_errno.h"
#include "common_qr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_QR_MAX_PAYLOAD_LEN QR_MAX_PAYLOAD_LEN
#define HI_QR_STATE_DECODE_SUCCESS QR_STATE_DECODE_SUCCESS
#define HI_QR_STATE_NON_EXISTENT QR_STATE_NON_EXISTENT
#define HI_QR_STATE_DECODE_FAIL QR_STATE_DECODE_FAIL
#define HI_QR_STATE_DATA_OVERFLOW QR_STATE_DATA_OVERFLOW
#define HI_QR_STATE_BUTT QR_STATE_BUTT
typedef qr_decode_state hi_qr_decode_state;
#define HI_ERR_QR_ILLEGAL_PARAM ERR_CODE_QR_ILLEGAL_PARAM
#define HI_ERR_QR_EXIST ERR_CODE_QR_EXIST
#define HI_ERR_QR_NULL_PTR ERR_CODE_QR_NULL_PTR
#define HI_ERR_QR_NO_MEM ERR_CODE_QR_NO_MEM
#define HI_ERR_QR_NOT_READY ERR_CODE_QR_NOT_READY
typedef qr_info hi_qr_info;
typedef qr_mem_info hi_qr_mem_info;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* __HI_COMMON_QR_H__ */
