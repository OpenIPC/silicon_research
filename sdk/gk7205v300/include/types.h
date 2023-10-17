/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __TYPES_H__
#define __TYPES_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define GK_HANDLE_MAKEHANDLE(mod, privatedata, chnid)                        \
	(gk_handle)((((mod)&0xffff) << 16) | ((((privatedata)&0xff) << 8)) | \
		    (((chnid)&0xff)))

#define GK_HANDLE_GET_MODID(handle) (((handle) >> 16) & 0xffff)
#define GK_HANDLE_GET_PriDATA(handle) (((handle) >> 8) & 0xff)
#define GK_HANDLE_GET_CHNID(handle) (((handle)) & 0xff)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
