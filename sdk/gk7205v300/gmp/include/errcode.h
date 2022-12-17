/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __ERRNO_H__
#define __ERRNO_H__

#include "mpp_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


#define ERR_CODE_APPID  (0x80000000L + 0x20000000L)

typedef enum ERR_LEVEL_E
{
    EN_ERR_LEVEL_DEBUG = 0,
    EN_ERR_LEVEL_INFO,
    EN_ERR_LEVEL_NOTICE,
    EN_ERR_LEVEL_WARNING,
    EN_ERR_LEVEL_ERROR,
    EN_ERR_LEVEL_CRIT,
    EN_ERR_LEVEL_ALERT,
    EN_ERR_LEVEL_FATAL,
    EN_ERR_LEVEL_BUTT
}ERR_LEVEL_E;



#define DEFINE_ERR_CODE( module, level, errid) \
    ((GK_S32)( (ERR_CODE_APPID) | ((module) << 16 ) | ((level)<<13) | (errid) ))

typedef enum EN_ERR_CODE_E
{
    EN_ERR_INVALID_DEVID = 1,
    EN_ERR_INVALID_CHNID = 2,
    EN_ERR_ILLEGAL_PARAM = 3,
    EN_ERR_EXIST         = 4,
    EN_ERR_UNEXIST       = 5,

    EN_ERR_NULL_PTR      = 6,

    EN_ERR_NOT_CONFIG    = 7,

    EN_ERR_NOT_SUPPORT   = 8,
    EN_ERR_NOT_PERM      = 9,
    EN_ERR_INVALID_PIPEID = 10,
    EN_ERR_INVALID_STITCHGRPID  = 11,

    EN_ERR_NOMEM         = 12,
    EN_ERR_NOBUF         = 13,

    EN_ERR_BUF_EMPTY     = 14,
    EN_ERR_BUF_FULL      = 15,

    EN_ERR_SYS_NOTREADY  = 16,

    EN_ERR_BADADDR       = 17,

    EN_ERR_BUSY          = 18,
    EN_ERR_SIZE_NOT_ENOUGH = 19,

    EN_ERR_BUTT          = 63,
}EN_ERR_CODE_E;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

