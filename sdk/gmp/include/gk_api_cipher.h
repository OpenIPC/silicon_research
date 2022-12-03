/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_API_CIPHER_H__
#define __GK_API_CIPHER_H__

#include "type.h"
#include "comm_cipher.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define GK_API_CIPHER_Open(GK_VOID) GK_API_CIPHER_Init(GK_VOID);
#define GK_API_CIPHER_Close(GK_VOID) GK_API_CIPHER_DeInit(GK_VOID);

GK_S32 GK_API_CIPHER_Init(GK_VOID);

GK_S32 GK_API_CIPHER_DeInit(GK_VOID);

GK_S32 GK_API_CIPHER_CreateHandle(GK_HANDLE* phCipher, const UNF_CIPHER_ATTS_S *pstCipherAttr);

GK_S32 GK_API_CIPHER_DestroyHandle(GK_HANDLE hCipher);

GK_S32 GK_API_CIPHER_ConfigHandle(GK_HANDLE hCipher, const UNF_CIPHER_CTRL_S* pstCtrl);

GK_S32 GK_API_CIPHER_ConfigHandleEx(GK_HANDLE hCipher, const UNF_CIPHER_CTRL_EX_S* pstExCtrl);

GK_S32 GK_API_CIPHER_Encrypt(GK_HANDLE hCipher, GK_SIZE_T szSrcPhyAddr, GK_SIZE_T szDestPhyAddr, GK_U32 u32ByteLength);

GK_S32 GK_API_CIPHER_Decrypt(GK_HANDLE hCipher, GK_SIZE_T szSrcPhyAddr, GK_SIZE_T szDestPhyAddr, GK_U32 u32ByteLength);

GK_S32 GK_API_CIPHER_EncryptVir(GK_HANDLE hCipher, const GK_U8 *pu8SrcData, GK_U8 *pu8DestData, GK_U32 u32ByteLength);

GK_S32 GK_API_CIPHER_DecryptVir(GK_HANDLE hCipher, const GK_U8 *pu8SrcData, GK_U8 *pu8DestData, GK_U32 u32ByteLength);

GK_S32 GK_API_CIPHER_EncryptMulti(GK_HANDLE hCipher, const UNF_CIPHER_DATA_S *pstDataPkg, GK_U32 u32DataPkgNum);

GK_S32 GK_API_CIPHER_GetHandleConfig(GK_HANDLE hCipher, UNF_CIPHER_CTRL_S* pstCtrl);

GK_S32 GK_API_CIPHER_DecryptMulti(GK_HANDLE hCipher, const UNF_CIPHER_DATA_S *pstDataPkg, GK_U32 u32DataPkgNum);

GK_S32 GK_API_CIPHER_GetTag(GK_HANDLE hCipher, GK_U8 *pu8Tag, GK_U32 *pu32TagLen);

GK_S32 GK_API_CIPHER_KladEncryptKey(UNF_CIPHER_CA_TYPE_E enRootKey,
                                    UNF_CIPHER_KLAD_TARGET_E enTarget,
                                    const GK_U8 *pu8CleanKey, GK_U8* pu8EcnryptKey, GK_U32 u32KeyLen);

GK_S32 GK_API_CIPHER_GetRandomNumber(GK_U32 *pu32RandomNumber);

GK_S32 GK_API_CIPHER_HashInit(const UNF_CIPHER_HASH_ATTS_S *pstHashAttr, GK_HANDLE *pHashHandle);

GK_S32 GK_API_CIPHER_HashUpdate(GK_HANDLE hHashHandle, const GK_U8 *pu8InputData, GK_U32 u32InputDataLen);


GK_S32 GK_API_CIPHER_HashFinal(GK_HANDLE hHashHandle, GK_U8 *pu8OutputHash);

GK_S32 GK_API_CIPHER_RsaPublicEncrypt(const UNF_CIPHER_RSA_PUB_ENC_S *pstRsaEnc,
                                  const GK_U8 *pu8Input, GK_U32 u32InLen,
                                  GK_U8 *pu8Output, GK_U32 *pu32OutLen);

GK_S32 GK_API_CIPHER_RsaPrivateDecrypt(const UNF_CIPHER_RSA_PRI_ENC_S *pstRsaDec,
                                    const GK_U8 *pu8Input, GK_U32 u32InLen,
                                    GK_U8 *pu8Output, GK_U32 *pu32OutLen);

GK_S32 GK_API_CIPHER_RsaPrivateEncrypt(const UNF_CIPHER_RSA_PRI_ENC_S *pstRsaEnc,
                                    const GK_U8 *pu8Input, GK_U32 u32InLen,
                                    GK_U8 *pu8Output, GK_U32 *pu32OutLen);

GK_S32 GK_API_CIPHER_RsaPublicDecrypt(const UNF_CIPHER_RSA_PUB_ENC_S *pstRsaDec,
                               const GK_U8 *pu8Input, GK_U32 u32InLen,
                               GK_U8 *pu8Output, GK_U32 *pu32OutLen);

GK_S32 GK_API_CIPHER_RsaSign(const UNF_CIPHER_RSA_SIGN_S *pstRsaSign,
                             const GK_U8 *pu8InData, GK_U32 u32InDataLen,
                             const GK_U8 *pu8HashData,
                             GK_U8 *pu8OutSign, GK_U32 *pu32OutSignLen);

GK_S32 GK_API_CIPHER_RsaVerify(const UNF_CIPHER_RSA_VERIFY_S *pstRsaVerify,
                               const GK_U8 *pu8InData, GK_U32 u32InDataLen,
                               const GK_U8 *pu8HashData,
                               const GK_U8 *pu8InSign, GK_U32 u32InSignLen);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

