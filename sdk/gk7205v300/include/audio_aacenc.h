/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
/**
 * \file
 * \brief Describes the information about AACENC.
 */

#ifndef __AUDIO_AACENC_H__
#define __AUDIO_AACENC_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/********************************Macro Definition********************************/
/** \addtogroup      AACENC */
/** @{ */ /** <!-- [AACENC] */

#ifdef MONO_ONLY
#define MAX_CHANNELS 1 /**< aacenc encoder channels */
#else
#define MAX_CHANNELS 2
#endif

#define AACENC_BLOCKSIZE 1024 /**< aacenc blocksize */

#define AACENC_VERSION_MAX_BYTE 64 /**< version max byte size */

/** @} */ /** <!-- ==== Macro Definition end ==== */

/*************************** Structure Definition ****************************/
/** \addtogroup      AACENC */
/** @{ */ /** <!-- [AACENC] */

typedef enum {
	AACENC_CODE_OK = 0x0000, /* !< No error happened. All fine. */

	AACENC_CODE_INVALID_HANDLE =
		0x0020, /* !< Handle passed to function call was invalid. */
	AACENC_CODE_MEMORY_ERROR = 0x0021, /* !< Memory allocation failed. */
	AACENC_CODE_UNSUPPORTED_PARAMETER =
		0x0022, /* !< Parameter not available. */
	AACENC_CODE_INVALID_CONFIG =
		0x0023, /* !< Configuration not provided. */

	AACENC_CODE_INIT_ERROR = 0x0040, /* !< General initialization error. */
	AACENC_CODE_INIT_AAC_ERROR =
		0x0041, /* !< AAC library initialization error. */
	AACENC_CODE_INIT_SBR_ERROR =
		0x0042, /* !< SBR library initialization error. */
	AACENC_CODE_INIT_TP_ERROR =
		0x0043, /* !< Transport library initialization error. */
	AACENC_CODE_INIT_META_ERROR =
		0x0044, /* !< Meta data library initialization error. */

	AACENC_CODE_ENCODE_ERROR =
		0x0060, /* !< The encoding process was interrupted by an unexpected error. */

	AACENC_CODE_ENCODE_EOF = 0x0080 /* !< End of file reached. */

} AACENC_ERROR_E;

/** Defines AACENC quality */
typedef enum {
	AU_QualityExcellent = 0,
	AU_QualityHigh = 1,
	AU_QualityMedium = 2,
	AU_QualityLow = 3,
} AuQuality;

/** Defines AACENC format */
typedef enum {
	AACLC = 0, /**< AAC-LC format */
	EAAC = 1, /**< HEAAC or AAC+  or aacPlusV1 */
	EAACPLUS = 2, /**< AAC++ or aacPlusV2 */
	AACLD = 3, /**< AAC LD(Low Delay) */
	AACELD = 4, /**< AAC ELD(Low Delay) */
} AuEncoderFormat;

/** Defines AACENC container */
typedef enum {
	AACENC_ADTS = 0,
	AACENC_LOAS = 1,
	AACENC_LATM_MCP1 = 2,
} AACENCTransportType;

/** Defines AACENC configuration */
typedef struct {
	AuQuality quality;
	AuEncoderFormat coderFormat;
	GK_S16 bitsPerSample;
	GK_S32 sampleRate; /**< audio file sample rate */
	GK_S32 bitRate; /**< encoder bit rate in bits/sec */
	GK_S16 nChannelsIn; /**< number of channels on input (1,2) */
	GK_S16 nChannelsOut; /**< number of channels on output (1,2) */
	GK_S16 bandWidth; /**< targeted audio bandwidth in Hz */
	AACENCTransportType transtype;
} AACENC_CONFIG;

/* Defines AACENC version */
typedef struct AACENC_VERSION_S {
	GK_U8 aVersion[AACENC_VERSION_MAX_BYTE];
} AACENC_VERSION_S;

typedef GK_U32 AAC_ENCODER_S;

/** @} */ /** <!-- ==== Structure Definition End ==== */

/******************************* API declaration *****************************/
/** \addtogroup      AACENC */
/** @{ */ /** <!--  [AACENC] */

/**
\brief Get version information.
\attention \n
N/A
\param[in] pVersion       version describe struct
\retval ::GK_SUCCESS   : Success
\retval ::GK_FAILURE          : FAILURE
\see \n
N/A
*/
GK_S32 AACENC_GetVersion(AACENC_VERSION_S *pVersion);

/**
\brief get reasonable default configuration.
\attention \n
N/A
\param[in] pstConfig    pointer to an configuration information structure
\retval ::GK_SUCCESS   : Success
\retval ::GK_FAILURE          : FAILURE
\see \n
N/A
*/
GK_S32 AACInitDefaultConfig(AACENC_CONFIG *pstConfig);

/**
\brief allocate and initialize a new encoder instance.
\attention \n
N/A
\param[in] phAacPlusEnc    pointer to an configuration information structure
\param[in] pstConfig    pointer to an configuration information structure
\retval ::GK_SUCCESS   : Success
\retval ::GK_FAILURE   : FAILURE
\see \n
N/A
*/
GK_S32 AACEncoderOpen(AAC_ENCODER_S **phAacPlusEnc, AACENC_CONFIG *pstConfig);

/**
\brief allocate and initialize a new encoder instance
\attention \n
N/A
\param[in] hAacPlusEnc    pointer to an configuration information structure
\param[in] ps16PcmBuf    BLOCKSIZE*nChannels audio samples,interleaved
\param[in] pu8Outbuf    pointer to output buffer,(must be 6144/8*MAX_CHANNELS bytes large)
\param[in] ps32NumOutBytes    number of bytes in output buffer after processing
\retval ::GK_SUCCESS   : Success
\retval ::GK_FAILURE   : FAILURE
\see \n
N/A
*/
GK_S32 AACEncoderFrame(AAC_ENCODER_S *hAacPlusEnc, GK_S16 *ps16PcmBuf,
		       GK_U8 *pu8Outbuf, GK_S32 *ps32NumOutBytes);

/**
\brief close encoder device.
\attention \n
N/A
\param[in] hAacPlusEnc    pointer to an configuration information structure
\retval N/A
\see \n
N/A
*/
GK_VOID AACEncoderClose(AAC_ENCODER_S *hAacPlusEnc);

/**
\brief register sbrenc module.
\attention \n
N/A
\param[in] pModuleHandle  pointer to WorkHandle of sbrenc module
\retval ::GK_SUCCESS   : Success
\retval ::GK_FAILURE   : FAILURE
\see \n
N/A
*/
GK_S32 AACEncoderRegisterModule(GK_VOID *pModuleHandle);

/**
\brief Get WorkHandle of sbrenc module.
\attention \n
N/A
\retval ::GK_VOID *    : pointer to WorkHandle of sbrenc module
\see \n
N/A
*/
GK_VOID *AAC_SBRENC_GetHandle(GK_VOID);

/** @} */ /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __AUDIO_AACENC_H__ */
