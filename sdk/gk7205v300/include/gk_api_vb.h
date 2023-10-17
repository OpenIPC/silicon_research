/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_API_VB_H__
#define __GK_API_VB_H__

#include "comm_vb.h"
#include "comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

VB_POOL GK_API_VB_CreatePool(VB_POOL_CONFIG_S *pstVbPoolCfg);
GK_S32 GK_API_VB_DestroyPool(VB_POOL Pool);

VB_BLK GK_API_VB_GetBlock(VB_POOL Pool, GK_U64 u64BlkSize,
			  const GK_CHAR *pcMmzName);
GK_S32 GK_API_VB_ReleaseBlock(VB_BLK Block);

VB_BLK GK_API_VB_PhysAddr2Handle(GK_U64 u64PhyAddr);
GK_U64 GK_API_VB_Handle2PhysAddr(VB_BLK Block);
VB_POOL GK_API_VB_Handle2PoolId(VB_BLK Block);

GK_S32 GK_API_VB_InquireUserCnt(VB_BLK Block);

GK_S32 GK_API_VB_GetSupplementAddr(VB_BLK Block,
				   VIDEO_SUPPLEMENT_S *pstSupplement);
GK_S32 GK_API_VB_SetSupplementConfig(
	const VB_SUPPLEMENT_CONFIG_S *pstSupplementConfig);
GK_S32
GK_API_VB_GetSupplementConfig(VB_SUPPLEMENT_CONFIG_S *pstSupplementConfig);

GK_S32 GK_API_VB_Init(GK_VOID);
GK_S32 GK_API_VB_Exit(GK_VOID);
GK_S32 GK_API_VB_SetConfig(const VB_CONFIG_S *pstVbConfig);
GK_S32 GK_API_VB_GetConfig(VB_CONFIG_S *pstVbConfig);

GK_S32 GK_API_VB_MmapPool(VB_POOL Pool);
GK_S32 GK_API_VB_MunmapPool(VB_POOL Pool);

GK_S32 GK_API_VB_GetBlockVirAddr(VB_POOL Pool, GK_U64 u64PhyAddr,
				 GK_VOID **ppVirAddr);

GK_S32 GK_API_VB_InitModCommPool(VB_UID_E enVbUid);
GK_S32 GK_API_VB_ExitModCommPool(VB_UID_E enVbUid);

GK_S32 GK_API_VB_SetModPoolConfig(VB_UID_E enVbUid,
				  const VB_CONFIG_S *pstVbConfig);
GK_S32 GK_API_VB_GetModPoolConfig(VB_UID_E enVbUid, VB_CONFIG_S *pstVbConfig);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
