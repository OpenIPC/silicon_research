/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __COMM_VB_H__
#define __COMM_VB_H__

#include "type.h"
#include "errcode.h"
#include "mpp_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define VB_INVALID_POOLID (-1U)
#define VB_INVALID_HANDLE (-1U)

#define VB_MAX_COMM_POOLS 16
#define VB_MAX_MOD_COMM_POOLS 16

#define VB_MAX_USER VB_UID_BUTT

typedef enum VB_UID_E {
	VB_UID_VI = 0,
	VB_UID_VO = 1,
	VB_UID_VGS = 2,
	VB_UID_VENC = 3,
	VB_UID_VDEC = 4,
	VB_UID_H265E = 5,
	VB_UID_H264E = 6,
	VB_UID_JPEGE = 7,
	VB_UID_H264D = 8,
	VB_UID_JPEGD = 9,
	VB_UID_VPSS = 10,
	VB_UID_DIS = 11,
	VB_UID_USER = 12,
	VB_UID_PCIV = 13,
	VB_UID_AI = 14,
	VB_UID_AENC = 15,
	VB_UID_RC = 16,
	VB_UID_VFMW = 17,
	VB_UID_GDC = 18,
	VB_UID_AVS = 19,
	VB_UID_DPU_RECT = 20,
	VB_UID_DPU_MATCH = 21,
	VB_UID_MCF = 22,
	VB_UID_BUTT = 23,
} VB_UID_E;

#define POOL_OWNER_COMMON -1

#define POOL_OWNER_PRIVATE -2

typedef GK_U32 VB_POOL;
typedef GK_U32 VB_BLK;

#define RESERVE_MMZ_NAME "window"

typedef enum VB_REMAP_MODE_E {
	VB_REMAP_MODE_NONE = 0,
	VB_REMAP_MODE_NOCACHE = 1,
	VB_REMAP_MODE_CACHED = 2,
	VB_REMAP_MODE_BUTT
} VB_REMAP_MODE_E;

typedef struct VB_POOL_CONFIG_S {
	GK_U64 u64BlkSize;
	GK_U32 u32BlkCnt;
	VB_REMAP_MODE_E enRemapMode;
	GK_CHAR acMmzName[MAX_MMZ_NAME_LEN];
} VB_POOL_CONFIG_S;

typedef struct VB_CONFIG_S {
	GK_U32 u32MaxPoolCnt;
	VB_POOL_CONFIG_S astCommPool[VB_MAX_COMM_POOLS];
} VB_CONFIG_S;

typedef struct VB_POOL_STATUS_S {
	GK_U32 bIsCommPool;
	GK_U32 u32BlkCnt;
	GK_U32 u32FreeBlkCnt;
} VB_POOL_STATUS_S;

#define VB_SUPPLEMENT_JPEG_MASK 0x1
#define VB_SUPPLEMENT_ISPINFO_MASK 0x2
#define VB_SUPPLEMENT_MOTION_DATA_MASK 0x4
#define VB_SUPPLEMENT_DNG_MASK 0x8

typedef struct VB_SUPPLEMENT_CONFIG_S {
	GK_U32 u32SupplementConfig;
} VB_SUPPLEMENT_CONFIG_S;

#define ERR_CODE_VB_NULL_PTR \
	DEFINE_ERR_CODE(MOD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_VB_NOMEM \
	DEFINE_ERR_CODE(MOD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define ERR_CODE_VB_NOBUF \
	DEFINE_ERR_CODE(MOD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define ERR_CODE_VB_UNEXIST \
	DEFINE_ERR_CODE(MOD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define ERR_CODE_VB_ILLEGAL_PARAM \
	DEFINE_ERR_CODE(MOD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_VB_NOTREADY \
	DEFINE_ERR_CODE(MOD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define ERR_CODE_VB_BUSY \
	DEFINE_ERR_CODE(MOD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define ERR_CODE_VB_NOT_PERM \
	DEFINE_ERR_CODE(MOD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define ERR_CODE_VB_SIZE_NOT_ENOUGH \
	DEFINE_ERR_CODE(MOD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH)

#define ERR_CODE_VB_2MPOOLS \
	DEFINE_ERR_CODE(MOD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_BUTT + 1)

#define MODULE_TRACE_VB(level, fmt, ...)                                      \
	do {                                                                  \
		MODULE_TRACE(level, MOD_ID_VB,                                \
			     "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, \
			     __LINE__, ##__VA_ARGS__);                        \
	} while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
