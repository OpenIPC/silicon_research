/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include "common.h"

#ifndef __VOU_EXP_H__
#define __VOU_EXP_H__

typedef struct VOU_EXPORT_CALLBACK_S {
    void (*pfnVoNotify)(int module_id, int vodev);
} VOU_EXPORT_CALLBACK_S;

typedef GK_S32 FN_VO_RegisterExternCallback(VOU_EXPORT_CALLBACK_S *pstExpCallback);

typedef struct VOU_EXPORT_SYMBOL_S {
    FN_VO_RegisterExternCallback *pfnVoRegisterExpCallback;
} VOU_EXPORT_SYMBOL_S;

#endif


