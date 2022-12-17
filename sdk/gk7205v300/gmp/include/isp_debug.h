/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __ISP_DEBUG_H__
#define __ISP_DEBUG_H__

#include "mpp_debug.h"
#define ISP_TRACE(level, fmt, ...)\
    do{ \
        MODULE_TRACE(level, MOD_ID_ISP,"[Func]:%s [Line]:%d [Info]:"fmt,__FUNCTION__, __LINE__,##__VA_ARGS__);\
    }while(0)

#define DIV_0_TO_1(a)   ( ((a) == 0) ? 1 : (a) )
#define DIV_0_TO_1_FLOAT(a) ((((a) < 1E-10) && ((a) > -1E-10)) ? 1 : (a))

#endif
