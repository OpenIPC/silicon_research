/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __TDE_ERRCODE_H__
#define __TDE_ERRCODE_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ERR_CODE_TDE_BASE  ((GK_S32)( ((0x80UL + 0x20UL)<<24) | (100 << 16 ) | (4 << 13) | 1 ))

enum
{
    ERR_CODE_TDE_DEV_NOT_OPEN = ERR_CODE_TDE_BASE,
    ERR_CODE_TDE_DEV_OPEN_FAILED,
    ERR_CODE_TDE_NULL_PTR,
    ERR_CODE_TDE_NO_MEM,
    ERR_CODE_TDE_INVALID_HANDLE,
    ERR_CODE_TDE_INVALID_PARA,
    ERR_CODE_TDE_NOT_ALIGNED,
    ERR_CODE_TDE_MINIFICATION,
    ERR_CODE_TDE_CLIP_AREA,
    ERR_CODE_TDE_JOB_TIMEOUT,
    ERR_CODE_TDE_UNSUPPORTED_OPERATION,
    ERR_CODE_TDE_QUERY_TIMEOUT,
    ERR_CODE_TDE_INTERRUPT
};


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


