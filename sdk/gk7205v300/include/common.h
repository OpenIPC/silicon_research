/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#include "autoconf.h"
#include "type.h"

#include "defines.h"

#ifndef __IGNORE_HWSEC__

#ifndef __KERNEL__
#include "securec.h"
#endif

#ifdef __LITEOS__
#include "securec.h"
#endif

#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef VER_X
#define VER_X 1
#endif

#ifndef VER_Y
#define VER_Y 0
#endif

#ifndef VER_Z
#define VER_Z 0
#endif

#ifndef VER_P
#define VER_P 0
#endif

#ifndef VER_B
#define VER_B 0
#endif

#ifdef MODE_DEBUG
#define VER_D " Debug"
#else
#define VER_D " Release"
#endif

#ifdef CONFIG_LOG_TRACE_SUPPORT
#define VER_E ""
#else
#define VER_E " NoLog"
#endif

#define ATTRIBUTE __attribute__((aligned(ALIGN_NUM)))

#define COMPAT_POINTER(ptr, type)                \
	do {                                     \
		GK_UL ulAddr = (GK_UL)ptr;       \
		GK_U32 u32Addr = (GK_U32)ulAddr; \
		ptr = (type)(GK_UL)u32Addr;      \
	} while (0)

#define __MK_VERSION(x, y, z, p, b) #x "." #y "." #z "." #p " B0" #b
#define MK_VERSION(x, y, z, p, b) __MK_VERSION(x, y, z, p, b)
#define MPP_VERSION                                                \
	MPP_VER_PRIX MK_VERSION(VER_X, VER_Y, VER_Z, VER_P, VER_B) \
	VER_D VER_E

#define VERSION_NAME_MAXLEN 64
typedef struct MPP_VERSION_S {
	GK_CHAR aVersion[VERSION_NAME_MAXLEN];
} MPP_VERSION_S;

typedef GK_S32 AI_CHN;
typedef GK_S32 AO_CHN;
typedef GK_S32 AENC_CHN;
typedef GK_S32 ADEC_CHN;
typedef GK_S32 AUDIO_DEV;
typedef GK_S32 VI_DEV;
typedef GK_S32 VI_PIPE;
typedef GK_S32 VI_CHN;
typedef GK_S32 VI_STITCH_GRP;
typedef GK_S32 VO_DEV;
typedef GK_S32 VO_LAYER;
typedef GK_S32 VO_CHN;
typedef GK_S32 VO_WBC;
typedef GK_S32 GRAPHIC_LAYER;
typedef GK_S32 VENC_CHN;
typedef GK_S32 VDEC_CHN;
typedef GK_S32 IVE_HANDLE;
typedef GK_S32 CLS_HANDLE;
typedef GK_S32 FD_CHN;
typedef GK_S32 MD_CHN;
typedef GK_S32 ISP_DEV;
typedef GK_S32 BLK_DEV;
typedef GK_S32 SENSOR_ID;
typedef GK_S32 MIPI_DEV;
typedef GK_S32 SLAVE_DEV;
typedef GK_S32 SVP_NNIE_HANDLE;
typedef GK_S32 SVP_DSP_HANDLE;
typedef GK_S32 SVP_ALG_CHN;
typedef GK_S32 VPSS_GRP;
typedef GK_S32 VPSS_GRP_PIPE;
typedef GK_S32 VPSS_CHN;
typedef GK_S32 AVS_GRP;
typedef GK_S32 AVS_PIPE;
typedef GK_S32 AVS_CHN;
typedef GK_S32 MCF_GRP;
typedef GK_S32 MCF_PIPE;
typedef GK_S32 MCF_CHN;

#define GK_INVALID_CHN (-1)
#define GK_INVALID_WAY (-1)
#define GK_INVALID_LAYER (-1)
#define GK_INVALID_DEV (-1)
#define GK_INVALID_HANDLE (-1)
#define GK_INVALID_VALUE (-1)
#define GK_INVALID_TYPE (-1)

#define CCM_MATRIX_SIZE (9)
#define CCM_MATRIX_NUM (7)

#define CLUT_R_NUM (17)
#define CLUT_B_NUM (17)
#define CLUT_G_NUM (17)
#define ISP_CLUT_COEFACMCNT (4850)
#define CUBIC_POINT_MAX (5)

typedef enum MOD_ID_E {
	MOD_ID_CMPI = 0,
	MOD_ID_VB = 1,
	MOD_ID_SYS = 2,
	MOD_ID_RGN = 3,
	MOD_ID_CHNL = 4,
	MOD_ID_VDEC = 5,
	MOD_ID_AVS = 6,
	MOD_ID_VPSS = 7,
	MOD_ID_VENC = 8,
	MOD_ID_SVP = 9,
	MOD_ID_H264E = 10,
	MOD_ID_JPEGE = 11,
	MOD_ID_MPEG4E = 12,
	MOD_ID_H265E = 13,
	MOD_ID_JPEGD = 14,
	MOD_ID_VO = 15,
	MOD_ID_VI = 16,
	MOD_ID_DIS = 17,
	MOD_ID_VALG = 18,
	MOD_ID_RC = 19,
	MOD_ID_AIO = 20,
	MOD_ID_AI = 21,
	MOD_ID_AO = 22,
	MOD_ID_AENC = 23,
	MOD_ID_ADEC = 24,
	MOD_ID_VPU = 25,
	MOD_ID_PCIV = 26,
	MOD_ID_PCIVFMW = 27,
	MOD_ID_ISP = 28,
	MOD_ID_IVE = 29,
	MOD_ID_USER = 30,
	MOD_ID_DCCM = 31,
	MOD_ID_DCCS = 32,
	MOD_ID_PROC = 33,
	MOD_ID_LOG = 34,
	MOD_ID_VFMW = 35,
	MOD_ID_H264D = 36,
	MOD_ID_GDC = 37,
	MOD_ID_PHOTO = 38,
	MOD_ID_FB = 39,
	MOD_ID_HDMI = 40,
	MOD_ID_VOIE = 41,
	MOD_ID_TDE = 42,
	MOD_ID_HDR = 43,
	MOD_ID_PRORES = 44,
	MOD_ID_VGS = 45,

	MOD_ID_FD = 47,
	MOD_ID_ODT = 48,
	MOD_ID_VQA = 49,
	MOD_ID_LPR = 50,
	MOD_ID_SVP_NNIE = 51,
	MOD_ID_SVP_DSP = 52,
	MOD_ID_DPU_RECT = 53,
	MOD_ID_DPU_MATCH = 54,

	MOD_ID_MOTIONSENSOR = 55,
	MOD_ID_MOTIONFUSION = 56,

	MOD_ID_GYRODIS = 57,
	MOD_ID_PM = 58,
	MOD_ID_SVP_ALG = 59,
	MOD_ID_IVP = 60,
	MOD_ID_MCF = 61,
	MOD_ID_QR = 62,
	MOD_ID_FR = 63,

	MOD_ID_BUTT,
} MOD_ID_E;

typedef struct MPP_CHN_S {
	MOD_ID_E enModId;
	GK_S32 s32DevId;
	GK_S32 s32ChnId;
} MPP_CHN_S;

#define MPP_MOD_VI "vi"
#define MPP_MOD_VO "vo"
#define MPP_MOD_AVS "avs"
#define MPP_MOD_HDMI "hdmi"
#define MPP_MOD_VGS "vgs"
#define MPP_MOD_GDC "gdc"
#define MPP_MOD_DIS "dis"
#define MPP_MOD_GYRODIS "gyrodis"

#define MPP_MOD_CHNL "chnl"
#define MPP_MOD_VENC "venc"
#define MPP_MOD_VPSS "vpss"
#define MPP_MOD_RGN "rgn"
#define MPP_MOD_IVE "ive"
#define MPP_MOD_FD "fd"
#define MPP_MOD_MD "md"
#define MPP_MOD_IVP "ivp"
#define MPP_MOD_SVP "svp"
#define MPP_MOD_SVP_NNIE "nnie"
#define MPP_MOD_SVP_DSP "dsp"
#define MPP_MOD_SVP_ALG "svp_alg"
#define MPP_MOD_DPU_RECT "rect"
#define MPP_MOD_DPU_MATCH "match"

#define MPP_MOD_H264E "h264e"
#define MPP_MOD_H265E "h265e"
#define MPP_MOD_JPEGE "jpege"
#define MPP_MOD_MPEG4E "mpeg4e"
#define MPP_MOD_VEDU "vedu"
#define MPP_MOD_PRORES "prores"

#define MPP_MOD_VDEC "vdec"
#define MPP_MOD_H264D "h264d"
#define MPP_MOD_JPEGD "jpegd"

#define MPP_MOD_AI "ai"
#define MPP_MOD_AO "ao"
#define MPP_MOD_AENC "aenc"
#define MPP_MOD_ADEC "adec"
#define MPP_MOD_AIO "aio"
#define MPP_MOD_ACODEC "acodec"

#define MPP_MOD_VB "vb"
#define MPP_MOD_SYS "sys"
#define MPP_MOD_CMPI "cmpi"

#define MPP_MOD_PCIV "pciv"
#define MPP_MOD_PCIVFMW "pcivfmw"

#define MPP_MOD_PROC "proc"
#define MPP_MOD_LOG "logmpp"

#define MPP_MOD_DCCM "dccm"
#define MPP_MOD_DCCS "dccs"

#define MPP_MOD_FB "fb"

#define MPP_MOD_RC "rc"

#define MPP_MOD_VOIE "voie"

#define MPP_MOD_TDE "tde"
#define MPP_MOD_ISP "isp"
#define MPP_MOD_USR "usr"
#define MPP_MOD_MCF "mcf"
#define MPP_MOD_PM "pm"
#define MPP_MOD_MFUSION "motionfusion"

typedef enum {
	PT_PCMU = 0,
	PT_1016 = 1,
	PT_G721 = 2,
	PT_GSM = 3,
	PT_G723 = 4,
	PT_DVI4_8K = 5,
	PT_DVI4_16K = 6,
	PT_LPC = 7,
	PT_PCMA = 8,
	PT_G722 = 9,
	PT_S16BE_STEREO = 10,
	PT_S16BE_MONO = 11,
	PT_QCELP = 12,
	PT_CN = 13,
	PT_MPEGAUDIO = 14,
	PT_G728 = 15,
	PT_DVI4_3 = 16,
	PT_DVI4_4 = 17,
	PT_G729 = 18,
	PT_G711A = 19,
	PT_G711U = 20,
	PT_G726 = 21,
	PT_G729A = 22,
	PT_LPCM = 23,
	PT_CelB = 25,
	PT_JPEG = 26,
	PT_CUSM = 27,
	PT_NV = 28,
	PT_PICW = 29,
	PT_CPV = 30,
	PT_H261 = 31,
	PT_MPEGVIDEO = 32,
	PT_MPEG2TS = 33,
	PT_H263 = 34,
	PT_SPEG = 35,
	PT_MPEG2VIDEO = 36,
	PT_AAC = 37,
	PT_WMA9STD = 38,
	PT_HEAAC = 39,
	PT_PCM_VOICE = 40,
	PT_PCM_AUDIO = 41,
	PT_MP3 = 43,
	PT_ADPCMA = 49,
	PT_AEC = 50,
	PT_X_LD = 95,
	PT_H264 = 96,
	PT_D_GSM_HR = 200,
	PT_D_GSM_EFR = 201,
	PT_D_L8 = 202,
	PT_D_RED = 203,
	PT_D_VDVI = 204,
	PT_D_BT656 = 220,
	PT_D_H263_1998 = 221,
	PT_D_MP1S = 222,
	PT_D_MP2P = 223,
	PT_D_BMPEG = 224,
	PT_MP4VIDEO = 230,
	PT_MP4AUDIO = 237,
	PT_VC1 = 238,
	PT_JVC_ASF = 255,
	PT_D_AVI = 256,
	PT_DIVX3 = 257,
	PT_AVS = 258,
	PT_REAL8 = 259,
	PT_REAL9 = 260,
	PT_VP6 = 261,
	PT_VP6F = 262,
	PT_VP6A = 263,
	PT_SORENSON = 264,
	PT_H265 = 265,
	PT_VP8 = 266,
	PT_MVC = 267,
	PT_PNG = 268,
	PT_AMR = 1001,
	PT_MJPEG = 1002,
	PT_AMRWB = 1003,
	PT_PRORES = 1006,
	PT_OPUS = 1007,
	PT_BUTT
} PAYLOAD_TYPE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
