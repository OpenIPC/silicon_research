/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef _MD_H_
#define _MD_H_

#include "ive.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
typedef enum MD_ALG_MODE_E {
    MD_ALG_MODE_BG = 0x0,
    MD_ALG_MODE_REF = 0x1,

    MD_ALG_MODE_BUTT
} MD_ALG_MODE_E;
typedef struct MD_ATTR_S {
    MD_ALG_MODE_E enAlgMode;
    IVE_SAD_MODE_E enSadMode;
    IVE_SAD_OUT_CTRL_E enSadOutCtrl;
    GK_U32 u32Width;
    GK_U32 u32Height;
    GK_U16 u16SadThr;
    IVE_CCL_CTRL_S stCclCtrl;
    IVE_ADD_CTRL_S stAddCtrl;
} MD_ATTR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
