/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __ISP_BIN_H__
#define __ISP_BIN_H__

#include "type.h"
#include "comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define MAX_BIN_REG_NUM 5

typedef struct ISP_BIN_REG_ATTR_S {
    GK_U32  u32Addr;
    GK_U8   u8StartBit;
    GK_U8   u8EndBit;
} ISP_BIN_REG_ATTR_S;


ISP_BIN_REG_ATTR_S g_astIspBinRegAttr[ISP_MAX_PIPE_NUM][MAX_BIN_REG_NUM] = {
    [0 ...(ISP_MAX_PIPE_NUM - 1)] = {0}
};


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
