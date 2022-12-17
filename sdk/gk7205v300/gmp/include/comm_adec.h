/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef  __COMM_ADEC_H__
#define  __COMM_ADEC_H__


#include "type.h"
#include "common.h"
#include "comm_aio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct ADEC_ATTR_G711_S {
    GK_U32 resv;
} ADEC_ATTR_G711_S;

typedef struct ADEC_ATTR_G726_S {
    G726_BPS_E enG726bps;
} ADEC_ATTR_G726_S;

typedef struct ADEC_ATTR_ADPCM_S {
    ADPCM_TYPE_E enADPCMType;
} ADEC_ATTR_ADPCM_S;

typedef struct ADEC_ATTR_LPCM_S {
    GK_U32 resv;
} ADEC_ATTR_LPCM_S;

typedef enum ADEC_MODE_E {
    ADEC_MODE_PACK = 0,
    ADEC_MODE_STREAM,
    ADEC_MODE_BUTT
}ADEC_MODE_E;

typedef struct ADEC_CH_ATTR_S {
    PAYLOAD_TYPE_E enType;
    GK_U32         u32BufSize;
    ADEC_MODE_E    enMode;
    GK_VOID ATTRIBUTE      *pValue;
}ADEC_CHN_ATTR_S;

typedef struct ADEC_CHN_STATE_S {
    GK_BOOL bEndOfStream;
    GK_U32 u32BufferFrmNum;
    GK_U32 u32BufferFreeNum;
    GK_U32 u32BufferBusyNum;
} ADEC_CHN_STATE_S;

typedef struct ADEC_DECODER_S {
    PAYLOAD_TYPE_E  enType;
    GK_CHAR         aszName[17];
    GK_S32          (*pfnOpenDecoder)(GK_VOID *pDecoderAttr, GK_VOID **ppDecoder);
    GK_S32          (*pfnDecodeFrm)(GK_VOID *pDecoder, GK_U8 **pu8Inbuf, GK_S32 *ps32LeftByte,
                        GK_U16 *pu16Outbuf, GK_U32 *pu32OutLen, GK_U32 *pu32Chns);
    GK_S32          (*pfnGetFrmInfo)(GK_VOID *pDecoder, GK_VOID *pInfo);
    GK_S32          (*pfnCloseDecoder)(GK_VOID *pDecoder);
    GK_S32          (*pfnResetDecoder)(GK_VOID *pDecoder);
} ADEC_DECODER_S;

typedef enum EN_ADEC_ERR_CODE_E {
    ADEC_ERR_DECODER_ERR     = 64,
    ADEC_ERR_BUF_LACK,
} EN_ADEC_ERR_CODE_E;


#define ERR_CODE_ADEC_INVALID_DEVID     DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
#define ERR_CODE_ADEC_INVALID_CHNID     DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define ERR_CODE_ADEC_ILLEGAL_PARAM     DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_ADEC_EXIST             DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define ERR_CODE_ADEC_UNEXIST           DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define ERR_CODE_ADEC_NULL_PTR          DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_ADEC_NOT_CONFIG        DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
#define ERR_CODE_ADEC_NOT_SUPPORT       DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define ERR_CODE_ADEC_NOT_PERM          DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define ERR_CODE_ADEC_NOMEM             DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define ERR_CODE_ADEC_NOBUF             DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define ERR_CODE_ADEC_BUF_EMPTY         DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define ERR_CODE_ADEC_BUF_FULL          DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define ERR_CODE_ADEC_SYS_NOTREADY      DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define ERR_CODE_ADEC_DECODER_ERR       DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_DECODER_ERR)
#define ERR_CODE_ADEC_BUF_LACK          DEFINE_ERR_CODE(MOD_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_BUF_LACK)




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

