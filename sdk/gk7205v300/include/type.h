/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __TYPE_H__
#define __TYPE_H__

#ifdef __KERNEL__

#include <linux/types.h>
#else

#include <stdint.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef unsigned char GK_UCHAR;
typedef unsigned char GK_U8;
typedef unsigned short GK_U16;
typedef unsigned int GK_U32;
typedef unsigned long GK_UL;
typedef uintptr_t GK_UINTPTR_T;

typedef char GK_CHAR;
typedef signed char GK_S8;
typedef short GK_S16;
typedef int GK_S32;
typedef long GK_SL;

typedef float GK_FLOAT;
typedef double GK_DOUBLE;

#ifndef _M_IX86
typedef unsigned long long GK_U64;
typedef long long GK_S64;
#else
typedef unsigned __int64 GK_U64;
typedef __int64 GK_S64;
#endif

typedef unsigned long GK_SIZE_T;
typedef unsigned long GK_LENGTH_T;
typedef long int GK_PHYS_ADDR_T;

typedef unsigned int GK_HANDLE;

typedef enum {
	GK_FALSE = 0,
	GK_TRUE = 1,
} GK_BOOL;

#ifndef NULL
#define NULL 0L
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define GK_NULL 0L
#define GK_SUCCESS 0
#define GK_FAILURE (-1)

#define GK_VOID void

typedef unsigned char gk_uchar;
typedef unsigned char gk_u8;
typedef unsigned short gk_u16;
typedef unsigned int gk_u32;
typedef unsigned long long gk_u64;
typedef unsigned long gk_ulong;

typedef char gk_char;
typedef signed char gk_s8;
typedef short gk_s16;
typedef int gk_s32;
typedef long long gk_s64;
typedef long gk_slong;

typedef float gk_float;
typedef double gk_double;

typedef void gk_void;

typedef unsigned long gk_size_t;
typedef unsigned long gk_length_t;

typedef gk_u32 gk_handle;

typedef GK_BOOL gk_bool;
typedef GK_UINTPTR_T gk_uintptr_t;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
