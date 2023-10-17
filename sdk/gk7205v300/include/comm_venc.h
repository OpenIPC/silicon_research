/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __COMM_VENC_H__
#define __COMM_VENC_H__

#include "type.h"
#include "common.h"
#include "errcode.h"
#include "comm_video.h"
#include "comm_rc.h"
#include "comm_vb.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ERR_CODE_VENC_INVALID_CHNID \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define ERR_CODE_VENC_ILLEGAL_PARAM \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_VENC_EXIST \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define ERR_CODE_VENC_UNEXIST \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define ERR_CODE_VENC_NULL_PTR \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_VENC_NOT_CONFIG \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
#define ERR_CODE_VENC_NOT_SUPPORT \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define ERR_CODE_VENC_NOT_PERM \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define ERR_CODE_VENC_NOMEM \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define ERR_CODE_VENC_NOBUF \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define ERR_CODE_VENC_BUF_EMPTY \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define ERR_CODE_VENC_BUF_FULL \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define ERR_CODE_VENC_SYS_NOTREADY \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define ERR_CODE_VENC_BUSY \
	DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)

typedef enum H264E_NALU_TYPE_E {
	H264E_NALU_BSLICE = 0,
	H264E_NALU_PSLICE = 1,
	H264E_NALU_ISLICE = 2,
	H264E_NALU_IDRSLICE = 5,
	H264E_NALU_SEI = 6,
	H264E_NALU_SPS = 7,
	H264E_NALU_PPS = 8,
	H264E_NALU_BUTT
} H264E_NALU_TYPE_E;

typedef enum H265E_NALU_TYPE_E {
	H265E_NALU_BSLICE = 0,
	H265E_NALU_PSLICE = 1,
	H265E_NALU_ISLICE = 2,
	H265E_NALU_IDRSLICE = 19,
	H265E_NALU_VPS = 32,
	H265E_NALU_SPS = 33,
	H265E_NALU_PPS = 34,
	H265E_NALU_SEI = 39,

	H265E_NALU_BUTT
} H265E_NALU_TYPE_E;

typedef enum H264E_REFSLICE_TYPE_E {
	H264E_REFSLICE_FOR_1X = 1,
	H264E_REFSLICE_FOR_2X = 2,
	H264E_REFSLICE_FOR_4X = 5,
	H264E_REFSLICE_FOR_BUTT
} H264E_REFSLICE_TYPE_E;

typedef enum JPEGE_PACK_TYPE_E {
	JPEGE_PACK_ECS = 5,
	JPEGE_PACK_APP = 6,
	JPEGE_PACK_VDO = 7,
	JPEGE_PACK_PIC = 8,
	JPEGE_PACK_DCF = 9,
	JPEGE_PACK_DCF_PIC = 10,
	JPEGE_PACK_BUTT
} JPEGE_PACK_TYPE_E;

typedef enum PRORES_PACK_TYPE_E {
	PRORES_PACK_PIC = 1,
	PRORES_PACK_BUTT
} PRORES_PACK_TYPE_E;

typedef union VENC_DATA_TYPE_U {
	H264E_NALU_TYPE_E enH264EType;
	JPEGE_PACK_TYPE_E enJPEGEType;
	H265E_NALU_TYPE_E enH265EType;
	PRORES_PACK_TYPE_E enPRORESType;
} VENC_DATA_TYPE_U;

typedef struct VENC_PACK_INFO_S {
	VENC_DATA_TYPE_U u32PackType;
	GK_U32 u32PackOffset;
	GK_U32 u32PackLength;
} VENC_PACK_INFO_S;

typedef struct VENC_PACK_S {
	GK_U64 u64PhyAddr;
	GK_U8 ATTRIBUTE *pu8Addr;
	GK_U32 ATTRIBUTE u32Len;

	GK_U64 u64PTS;
	GK_BOOL bFrameEnd;

	VENC_DATA_TYPE_U DataType;
	GK_U32 u32Offset;
	GK_U32 u32DataNum;
	VENC_PACK_INFO_S stPackInfo[8];
} VENC_PACK_S;

typedef enum H264E_REF_TYPE_E {
	BASE_IDRSLICE = 0,
	BASE_PSLICE_REFTOIDR,
	BASE_PSLICE_REFBYBASE,
	BASE_PSLICE_REFBYENHANCE,
	ENHANCE_PSLICE_REFBYENHANCE,
	ENHANCE_PSLICE_NOTFORREF,
	ENHANCE_PSLICE_BUTT
} H264E_REF_TYPE_E;

typedef enum H264E_REF_TYPE_E H265E_REF_TYPE_E;

typedef struct VENC_STREAM_INFO_H264_S {
	GK_U32 u32PicBytesNum;
	GK_U32 u32Inter16x16MbNum;
	GK_U32 u32Inter8x8MbNum;
	GK_U32 u32Intra16MbNum;
	GK_U32 u32Intra8MbNum;
	GK_U32 u32Intra4MbNum;

	H264E_REF_TYPE_E enRefType;
	GK_U32 u32UpdateAttrCnt;
	GK_U32 u32StartQp;
	GK_U32 u32MeanQp;
	GK_BOOL bPSkip;
} VENC_STREAM_INFO_H264_S;

typedef struct VENC_STREAM_INFO_H265_S {
	GK_U32 u32PicBytesNum;
	GK_U32 u32Inter64x64CuNum;
	GK_U32 u32Inter32x32CuNum;
	GK_U32 u32Inter16x16CuNum;
	GK_U32 u32Inter8x8CuNum;
	GK_U32 u32Intra32x32CuNum;
	GK_U32 u32Intra16x16CuNum;
	GK_U32 u32Intra8x8CuNum;
	GK_U32 u32Intra4x4CuNum;

	H265E_REF_TYPE_E enRefType;
	GK_U32 u32UpdateAttrCnt;
	GK_U32 u32StartQp;
	GK_U32 u32MeanQp;
	GK_BOOL bPSkip;
} VENC_STREAM_INFO_H265_S;

typedef struct VENC_SSE_INFO_S {
	GK_BOOL bSSEEn;
	GK_U32 u32SSEVal;
} VENC_SSE_INFO_S;

typedef struct VENC_STREAM_ADVANCE_INFO_H264_S {
	GK_U32 u32ResidualBitNum;
	GK_U32 u32HeadBitNum;
	GK_U32 u32MadiVal;
	GK_U32 u32MadpVal;
	GK_DOUBLE dPSNRVal;
	GK_U32 u32MseLcuCnt;
	GK_U32 u32MseSum;
	VENC_SSE_INFO_S stSSEInfo[8];
	GK_U32 u32QpHstgrm[VENC_QP_HISGRM_NUM];
	GK_U32 u32MoveScene16x16Num;
	GK_U32 u32MoveSceneBits;
} VENC_STREAM_ADVANCE_INFO_H264_S;

typedef struct VENC_STREAM_ADVANCE_INFO_JPEG_S {
} VENC_STREAM_ADVANCE_INFO_JPEG_S;

typedef struct VENC_STREAM_ADVANCE_INFO_PRORES_S {
} VENC_STREAM_ADVANCE_INFO_PRORES_S;

typedef struct VENC_STREAM_ADVANCE_INFO_H265_S {
	GK_U32 u32ResidualBitNum;
	GK_U32 u32HeadBitNum;
	GK_U32 u32MadiVal;
	GK_U32 u32MadpVal;
	GK_DOUBLE dPSNRVal;
	GK_U32 u32MseLcuCnt;
	GK_U32 u32MseSum;
	VENC_SSE_INFO_S stSSEInfo[8];
	GK_U32 u32QpHstgrm[VENC_QP_HISGRM_NUM];
	GK_U32 u32MoveScene32x32Num;
	GK_U32 u32MoveSceneBits;
} VENC_STREAM_ADVANCE_INFO_H265_S;

typedef struct VENC_STREAM_INFO_PRORES_S {
	GK_U32 u32PicBytesNum;
	GK_U32 u32UpdateAttrCnt;
} VENC_STREAM_INFO_PRORES_S;

typedef struct VENC_STREAM_INFO_JPEG_S {
	GK_U32 u32PicBytesNum;
	GK_U32 u32UpdateAttrCnt;
	GK_U32 u32Qfactor;
} VENC_STREAM_INFO_JPEG_S;

typedef struct VENC_STREAM_S {
	VENC_PACK_S ATTRIBUTE *pstPack;
	GK_U32 ATTRIBUTE u32PackCount;
	GK_U32 u32Seq;

	union {
		VENC_STREAM_INFO_H264_S stH264Info;
		VENC_STREAM_INFO_JPEG_S stJpegInfo;
		VENC_STREAM_INFO_H265_S stH265Info;
		VENC_STREAM_INFO_PRORES_S stProresInfo;
	};

	union {
		VENC_STREAM_ADVANCE_INFO_H264_S stAdvanceH264Info;
		VENC_STREAM_ADVANCE_INFO_JPEG_S stAdvanceJpegInfo;
		VENC_STREAM_ADVANCE_INFO_H265_S stAdvanceH265Info;
		VENC_STREAM_ADVANCE_INFO_PRORES_S stAdvanceProresInfo;
	};
} VENC_STREAM_S;

typedef struct VENC_STREAM_INFO_S {
	H265E_REF_TYPE_E enRefType;

	GK_U32 u32PicBytesNum;
	GK_U32 u32PicCnt;
	GK_U32 u32StartQp;
	GK_U32 u32MeanQp;
	GK_BOOL bPSkip;

	GK_U32 u32ResidualBitNum;
	GK_U32 u32HeadBitNum;
	GK_U32 u32MadiVal;
	GK_U32 u32MadpVal;
	GK_U32 u32MseSum;
	GK_U32 u32MseLcuCnt;
	GK_DOUBLE dPSNRVal;
} VENC_STREAM_INFO_S;

typedef struct VENC_MPF_CFG_S {
	GK_U8 u8LargeThumbNailNum;
	SIZE_S astLargeThumbNailSize[2];
} VENC_MPF_CFG_S;

typedef enum VENC_PIC_RECEIVE_MODE_E {
	VENC_PIC_RECEIVE_SINGLE = 0,
	VENC_PIC_RECEIVE_MULTI,

	VENC_PIC_RECEIVE_BUTT
} VENC_PIC_RECEIVE_MODE_E;

typedef struct VENC_ATTR_JPEG_S {
	GK_BOOL bSupportDCF;
	VENC_MPF_CFG_S stMPFCfg;
	VENC_PIC_RECEIVE_MODE_E enReceiveMode;
} VENC_ATTR_JPEG_S;

typedef struct VENC_ATTR_MJPEG_S {
} VENC_ATTR_MJPEG_S;

typedef struct VENC_ATTR_H264_S {
	GK_BOOL bRcnRefShareBuf;
} VENC_ATTR_H264_S;

typedef struct VENC_ATTR_H265_S {
	GK_BOOL bRcnRefShareBuf;
} VENC_ATTR_H265_S;

typedef enum PRORES_FRAMERATE {
	PRORES_FR_UNKNOWN = 0,
	PRORES_FR_23_976,
	PRORES_FR_24,
	PRORES_FR_25,
	PRORES_FR_29_97,
	PRORES_FR_30,
	PRORES_FR_50,
	PRORES_FR_59_94,
	PRORES_FR_60,
	PRORES_FR_100,
	PRORES_FR_119_88,
	PRORES_FR_120,
	PRORES_FR_BUTT
} PRORES_FRAMERATE;

typedef enum PRORES_ASPECT_RATIO {
	PRORES_ASPECT_RATIO_UNKNOWN = 0,
	PRORES_ASPECT_RATIO_SQUARE,
	PRORES_ASPECT_RATIO_4_3,
	PRORES_ASPECT_RATIO_16_9,
	PRORES_ASPECT_RATIO_BUTT
} PRORES_ASPECT_RATIO;

typedef struct VENC_ATTR_PRORES_S {
	GK_CHAR cIdentifier[4];
	PRORES_FRAMERATE enFrameRateCode;
	PRORES_ASPECT_RATIO enAspectRatio;
} VENC_ATTR_PRORES_S;

typedef struct VENC_ATTR_S {
	PAYLOAD_TYPE_E enType;

	GK_U32 u32MaxPicWidth;
	GK_U32 u32MaxPicHeight;

	GK_U32 u32BufSize;
	GK_U32 u32Profile;
	GK_BOOL bByFrame;
	GK_U32 u32PicWidth;
	GK_U32 u32PicHeight;
	union {
		VENC_ATTR_H264_S stAttrH264e;
		VENC_ATTR_H265_S stAttrH265e;
		VENC_ATTR_MJPEG_S stAttrMjpege;
		VENC_ATTR_JPEG_S stAttrJpege;
		VENC_ATTR_PRORES_S stAttrProres;
	};
} VENC_ATTR_S;

typedef enum VENC_GOP_MODE_E {
	VENC_GOPMODE_NORMALP = 0,
	VENC_GOPMODE_DUALP = 1,
	VENC_GOPMODE_SMARTP = 2,
	VENC_GOPMODE_ADVSMARTP = 3,
	VENC_GOPMODE_BIPREDB = 4,
	VENC_GOPMODE_LOWDELAYB = 5,

	VENC_GOPMODE_BUTT,
} VENC_GOP_MODE_E;

typedef struct VENC_GOP_NORMALP_S {
	GK_S32 s32IPQpDelta;
} VENC_GOP_NORMALP_S;

typedef struct VENC_GOP_DUALP_S {
	GK_U32 u32SPInterval;
	GK_S32 s32SPQpDelta;
	GK_S32 s32IPQpDelta;
} VENC_GOP_DUALP_S;

typedef struct VENC_GOP_SMARTP_S {
	GK_U32 u32BgInterval;
	GK_S32 s32BgQpDelta;
	GK_S32 s32ViQpDelta;
} VENC_GOP_SMARTP_S;

typedef struct VENC_GOP_ADVSMARTP_S {
	GK_U32 u32BgInterval;
	GK_S32 s32BgQpDelta;
	GK_S32 s32ViQpDelta;
} VENC_GOP_ADVSMARTP_S;

typedef struct VENC_GOP_BIPREDB_S {
	GK_U32 u32BFrmNum;
	GK_S32 s32BQpDelta;
	GK_S32 s32IPQpDelta;
} VENC_GOP_BIPREDB_S;

typedef struct VENC_GOP_ATTR_S {
	VENC_GOP_MODE_E enGopMode;
	union {
		VENC_GOP_NORMALP_S stNormalP;
		VENC_GOP_DUALP_S stDualP;
		VENC_GOP_SMARTP_S stSmartP;
		VENC_GOP_ADVSMARTP_S stAdvSmartP;
		VENC_GOP_BIPREDB_S stBipredB;
	};

} VENC_GOP_ATTR_S;

typedef struct VENC_CHN_ATTR_S {
	VENC_ATTR_S stVencAttr;
	VENC_RC_ATTR_S stRcAttr;
	VENC_GOP_ATTR_S stGopAttr;
} VENC_CHN_ATTR_S;

typedef struct VENC_RECV_PIC_PARAM_S {
	GK_S32 s32RecvPicNum;
} VENC_RECV_PIC_PARAM_S;

typedef struct VENC_CHN_STATUS_S {
	GK_U32 u32LeftPics;
	GK_U32 u32LeftStreamBytes;
	GK_U32 u32LeftStreamFrames;
	GK_U32 u32CurPacks;
	GK_U32 u32LeftRecvPics;
	GK_U32 u32LeftEncPics;
	GK_BOOL bJpegSnapEnd;
	VENC_STREAM_INFO_S stVencStrmInfo;
} VENC_CHN_STATUS_S;

typedef struct VENC_H264_SLICE_SPLIT_S {
	GK_BOOL bSplitEnable;
	GK_U32 u32MbLineNum;
} VENC_H264_SLICE_SPLIT_S;

typedef struct VENC_H264_INTRA_PRED_S {
	GK_U32 constrained_intra_pred_flag;
} VENC_H264_INTRA_PRED_S;

typedef struct VENC_H264_TRANS_S {
	GK_U32 u32IntraTransMode;
	GK_U32 u32InterTransMode;

	GK_BOOL bScalingListValid;
	GK_U8 InterScalingList8X8[64];
	GK_U8 IntraScalingList8X8[64];

	GK_S32 chroma_qp_index_offset;
} VENC_H264_TRANS_S;

typedef struct VENC_H264_ENTROPY_S {
	GK_U32 u32EntropyEncModeI;
	GK_U32 u32EntropyEncModeP;
	GK_U32 u32EntropyEncModeB;
	GK_U32 cabac_init_idc;
} VENC_H264_ENTROPY_S;

typedef struct VENC_H264_POC_S {
	GK_U32 pic_order_cnt_type;

} VENC_H264_POC_S;

typedef struct VENC_H264_DBLK_S {
	GK_U32 disable_deblocking_filter_idc;
	GK_S32 slice_alpha_c0_offset_div2;
	GK_S32 slice_beta_offset_div2;
} VENC_H264_DBLK_S;

typedef struct VENC_H264_VUI_TIME_INFO_S {
	GK_U8 timing_info_present_flag;
	GK_U8 fixed_frame_rate_flag;
	GK_U32 num_units_in_tick;
	GK_U32 time_scale;
} VENC_VUI_H264_TIME_INFO_S;

typedef struct VENC_VUI_ASPECT_RATIO_S {
	GK_U8 aspect_ratio_info_present_flag;
	GK_U8 aspect_ratio_idc;
	GK_U8 overscan_info_present_flag;
	GK_U8 overscan_appropriate_flag;
	GK_U16 sar_width;
	GK_U16 sar_height;
} VENC_VUI_ASPECT_RATIO_S;

typedef struct VENC_VUI_VIDEO_SIGNAL_S {
	GK_U8 video_signal_type_present_flag;
	GK_U8 video_format;
	GK_U8 video_full_range_flag;
	GK_U8 colour_description_present_flag;
	GK_U8 colour_primaries;
	GK_U8 transfer_characteristics;
	GK_U8 matrix_coefficients;
} VENC_VUI_VIDEO_SIGNAL_S;

typedef struct VENC_VUI_BITSTREAM_RESTRIC_S {
	GK_U8 bitstream_restriction_flag;
} VENC_VUI_BITSTREAM_RESTRIC_S;

typedef struct VENC_H264_VUI_S {
	VENC_VUI_ASPECT_RATIO_S stVuiAspectRatio;
	VENC_VUI_H264_TIME_INFO_S stVuiTimeInfo;
	VENC_VUI_VIDEO_SIGNAL_S stVuiVideoSignal;
	VENC_VUI_BITSTREAM_RESTRIC_S stVuiBitstreamRestric;
} VENC_H264_VUI_S;

typedef struct VENC_VUI_H265_TIME_INFO_S {
	GK_U32 timing_info_present_flag;
	GK_U32 num_units_in_tick;
	GK_U32 time_scale;
	GK_U32 num_ticks_poc_diff_one_minus1;
} VENC_VUI_H265_TIME_INFO_S;

typedef struct VENC_H265_VUI_S {
	VENC_VUI_ASPECT_RATIO_S stVuiAspectRatio;
	VENC_VUI_H265_TIME_INFO_S stVuiTimeInfo;
	VENC_VUI_VIDEO_SIGNAL_S stVuiVideoSignal;
	VENC_VUI_BITSTREAM_RESTRIC_S stVuiBitstreamRestric;
} VENC_H265_VUI_S;

typedef struct VENC_JPEG_PARAM_S {
	GK_U32 u32Qfactor;
	GK_U8 u8YQt[64];
	GK_U8 u8CbQt[64];
	GK_U8 u8CrQt[64];
	GK_U32 u32MCUPerECS;
} VENC_JPEG_PARAM_S;

typedef struct VENC_MJPEG_PARAM_S {
	GK_U8 u8YQt[64];
	GK_U8 u8CbQt[64];
	GK_U8 u8CrQt[64];
	GK_U32 u32MCUPerECS;
} VENC_MJPEG_PARAM_S;

typedef struct VENC_PRORES_PARAM_S {
	GK_U8 u8LumaQt[64];
	GK_U8 u8ChromaQt[64];
	GK_CHAR encoder_identifier[4];
} VENC_PRORES_PARAM_S;

typedef struct VENC_ROI_ATTR_S {
	GK_U32 u32Index;
	GK_BOOL bEnable;
	GK_BOOL bAbsQp;
	GK_S32 s32Qp;
	RECT_S stRect;
} VENC_ROI_ATTR_S;

typedef struct VENC_ROI_ATTR_EX_S {
	GK_U32 u32Index;
	GK_BOOL bEnable[3];
	GK_BOOL bAbsQp[3];
	GK_S32 s32Qp[3];
	RECT_S stRect[3];
} VENC_ROI_ATTR_EX_S;

typedef struct VENC_ROIBG_FRAME_RATE_S {
	GK_S32 s32SrcFrmRate;
	GK_S32 s32DstFrmRate;
} VENC_ROIBG_FRAME_RATE_S;

typedef struct VENC_REF_PARAM_S {
	GK_U32 u32Base;
	GK_U32 u32Enhance;
	GK_BOOL bEnablePred;
} VENC_REF_PARAM_S;

typedef enum VENC_JPEG_ENCODE_MODE_E {
	JPEG_ENCODE_ALL = 0,
	JPEG_ENCODE_SNAP = 1,
	JPEG_ENCODE_BUTT,
} VENC_JPEG_ENCODE_MODE_E;

typedef struct VENC_STREAM_BUF_INFO_S {
	GK_U64 u64PhyAddr[MAX_TILE_NUM];
	GK_VOID ATTRIBUTE *pUserAddr[MAX_TILE_NUM];
	GK_U64 ATTRIBUTE u64BufSize[MAX_TILE_NUM];
} VENC_STREAM_BUF_INFO_S;

typedef struct VENC_H265_SLICE_SPLIT_S {
	GK_BOOL bSplitEnable;
	GK_U32 u32LcuLineNum;
} VENC_H265_SLICE_SPLIT_S;

typedef struct VENC_H265_PU_S {
	GK_U32 constrained_intra_pred_flag;
	GK_U32 strong_intra_smoothing_enabled_flag;
} VENC_H265_PU_S;

typedef struct VENC_H265_TRANS_S {
	GK_S32 cb_qp_offset;
	GK_S32 cr_qp_offset;

	GK_BOOL bScalingListEnabled;

	GK_BOOL bScalingListTu4Valid;
	GK_U8 InterScalingList4X4[2][16];
	GK_U8 IntraScalingList4X4[2][16];

	GK_BOOL bScalingListTu8Valid;
	GK_U8 InterScalingList8X8[2][64];
	GK_U8 IntraScalingList8X8[2][64];

	GK_BOOL bScalingListTu16Valid;
	GK_U8 InterScalingList16X16[2][64];
	GK_U8 IntraScalingList16X16[2][64];

	GK_BOOL bScalingListTu32Valid;
	GK_U8 InterScalingList32X32[64];
	GK_U8 IntraScalingList32X32[64];

} VENC_H265_TRANS_S;

typedef struct VENC_H265_ENTROPY_S {
	GK_U32 cabac_init_flag;
} VENC_H265_ENTROPY_S;

typedef struct VENC_H265_DBLK_S {
	GK_U32 slice_deblocking_filter_disabled_flag;
	GK_S32 slice_beta_offset_div2;
	GK_S32 slice_tc_offset_div2;
} VENC_H265_DBLK_S;

typedef struct VENC_H265_SAO_S {
	GK_U32 slice_sao_luma_flag;
	GK_U32 slice_sao_chroma_flag;
} VENC_H265_SAO_S;

typedef enum VENC_INTRA_REFRESH_MODE_E {
	INTRA_REFRESH_ROW = 0,
	INTRA_REFRESH_COLUMN,
	INTRA_REFRESH_BUTT
} VENC_INTRA_REFRESH_MODE_E;

typedef struct VENC_INTRA_REFRESH_S {
	GK_BOOL bRefreshEnable;
	VENC_INTRA_REFRESH_MODE_E enIntraRefreshMode;
	GK_U32 u32RefreshNum;
	GK_U32 u32ReqIQp;
} VENC_INTRA_REFRESH_S;

typedef enum VENC_MODTYPE_E {
	MODTYPE_VENC = 1,
	MODTYPE_H264E,
	MODTYPE_H265E,
	MODTYPE_JPEGE,
	MODTYPE_RC,
	MODTYPE_BUTT
} VENC_MODTYPE_E;

typedef struct VENC_MOD_H264E_S {
	GK_U32 u32OneStreamBuffer;
	GK_U32 u32H264eMiniBufMode;
	GK_U32 u32H264ePowerSaveEn;
	VB_SOURCE_E enH264eVBSource;
	GK_BOOL bQpHstgrmEn;
} VENC_MOD_H264E_S;

typedef struct VENC_MOD_H265E_S {
	GK_U32 u32OneStreamBuffer;
	GK_U32 u32H265eMiniBufMode;
	GK_U32 u32H265ePowerSaveEn;
	VB_SOURCE_E enH265eVBSource;
	GK_BOOL bQpHstgrmEn;
} VENC_MOD_H265E_S;

typedef struct VENC_MOD_JPEGE_S {
	GK_U32 u32OneStreamBuffer;
	GK_U32 u32JpegeMiniBufMode;
	GK_U32 u32JpegClearStreamBuf;
	GK_U32 u32JpegeDeringMode;
} VENC_MOD_JPEGE_S;

typedef struct VENC_MOD_RC_S {
	GK_U32 u32ClrStatAfterSetBr;
} VENC_MOD_RC_S;
typedef struct VENC_MOD_VENC_S {
	GK_U32 u32VencBufferCache;
	GK_U32 u32FrameBufRecycle;
} VENC_MOD_VENC_S;

typedef struct VENC_MODPARAM_S {
	VENC_MODTYPE_E enVencModType;
	union {
		VENC_MOD_VENC_S stVencModParam;
		VENC_MOD_H264E_S stH264eModParam;
		VENC_MOD_H265E_S stH265eModParam;
		VENC_MOD_JPEGE_S stJpegeModParam;
		VENC_MOD_RC_S stRcModParam;
	};
} VENC_PARAM_MOD_S;

typedef enum VENC_FRAME_TYPE_E {
	VENC_FRAME_TYPE_NONE = 1,
	VENC_FRAME_TYPE_IDR,
	VENC_FRAME_TYPE_BUTT
} VENC_FRAME_TYPE_E;

typedef struct USER_RC_INFO_S {
	GK_BOOL bQpMapValid;
	GK_BOOL bSkipWeightValid;
	GK_U32 u32BlkStartQp;
	GK_U64 u64QpMapPhyAddr;
	GK_U64 u64SkipWeightPhyAddr;
	VENC_FRAME_TYPE_E enFrameType;
} USER_RC_INFO_S;

typedef struct USER_FRAME_INFO_S {
	VIDEO_FRAME_INFO_S stUserFrame;
	USER_RC_INFO_S stUserRcInfo;
} USER_FRAME_INFO_S;

typedef struct VENC_SSE_CFG_S {
	GK_U32 u32Index;
	GK_BOOL bEnable;
	RECT_S stRect;
} VENC_SSE_CFG_S;

typedef struct VENC_CROP_INFO_S {
	GK_BOOL bEnable;
	RECT_S stRect;
} VENC_CROP_INFO_S;

typedef struct VENC_FRAME_RATE_S {
	GK_S32 s32SrcFrmRate;
	GK_S32 s32DstFrmRate;
} VENC_FRAME_RATE_S;

typedef struct VENC_CHN_PARAM_S {
	GK_BOOL bColor2Grey;
	GK_U32 u32Priority;
	GK_U32 u32MaxStrmCnt;
	GK_U32 u32PollWakeUpFrmCnt;
	VENC_CROP_INFO_S stCropCfg;
	VENC_FRAME_RATE_S stFrameRate;
} VENC_CHN_PARAM_S;

typedef struct VENC_FOREGROUND_PROTECT_S {
	GK_BOOL bForegroundCuRcEn;
	GK_U32 u32ForegroundDirectionThresh;
	GK_U32 u32ForegroundThreshGain;
	GK_U32 u32ForegroundThreshOffset;
	GK_U32 u32ForegroundThreshP[RC_TEXTURE_THR_SIZE];
	GK_U32 u32ForegroundThreshB[RC_TEXTURE_THR_SIZE];
} VENC_FOREGROUND_PROTECT_S;

typedef enum VENC_SCENE_MODE_E {
	SCENE_0 = 0,
	SCENE_1 = 1,
	SCENE_2 = 2,
	SCENE_BUTT
} VENC_SCENE_MODE_E;

typedef struct VENC_DEBREATHEFFECT_S {
	GK_BOOL bEnable;
	GK_S32 s32Strength0;
	GK_S32 s32Strength1;
} VENC_DEBREATHEFFECT_S;

typedef struct VENC_CU_PREDICTION_S {
	OPERATION_MODE_E enPredMode;

	GK_U32 u32Intra32Cost;
	GK_U32 u32Intra16Cost;
	GK_U32 u32Intra8Cost;
	GK_U32 u32Intra4Cost;

	GK_U32 u32Inter64Cost;
	GK_U32 u32Inter32Cost;
	GK_U32 u32Inter16Cost;
	GK_U32 u32Inter8Cost;
} VENC_CU_PREDICTION_S;

typedef struct VENC_SKIP_BIAS_S {
	GK_BOOL bSkipBiasEn;
	GK_U32 u32SkipThreshGain;
	GK_U32 u32SkipThreshOffset;
	GK_U32 u32SkipBackgroundCost;
	GK_U32 u32SkipForegroundCost;
} VENC_SKIP_BIAS_S;

typedef struct VENC_HIERARCHICAL_QP_S {
	GK_BOOL bHierarchicalQpEn;
	GK_S32 s32HierarchicalQpDelta[4];
	GK_S32 s32HierarchicalFrameNum[4];
} VENC_HIERARCHICAL_QP_S;

typedef struct VENC_CHN_POOL_S {
	VB_POOL hPicVbPool;
	VB_POOL hPicInfoVbPool;
} VENC_CHN_POOL_S;

typedef struct VENC_RC_ADVPARAM_S {
	GK_U32 u32ClearStatAfterSetAttr;
} VENC_RC_ADVPARAM_S;

typedef enum FG_REGION_TYPE_E {
	FG_REGION_PEOPLE = 0,
	FG_REGION_FACE,
	FG_REGION_BUTT
} FG_REGION_TYPE_E;

typedef struct {
	GK_U8 u8QpmapValueI;
	GK_U8 u8QpmapValueP;
	GK_U8 u8SkipmapValue;
} VENC_SVC_MAP_PARAM_S;

typedef struct {
	VENC_SVC_MAP_PARAM_S stFgRegion[FG_REGION_BUTT];
	VENC_SVC_MAP_PARAM_S stActivityRegion;
	VENC_SVC_MAP_PARAM_S stBgRegion;
	GK_BOOL bFgProtectAdaptiveEn;
} VENC_SVC_PARAM_S;

#define VENC_MAX_SVC_RECT_NUM 32

typedef enum VENC_SVC_RECT_TYPE_E {
	SVC_RECT_TYPE_FACE,
	SVC_RECT_TYPE_PEOPLE,
	SVC_RECT_TYPE_BUTT
} VENC_SVC_RECT_TYPE_E;

typedef struct VENC_SVC_RECT_INFO_S {
	GK_U32 u32RectNum;
	SIZE_S stBaseResolution;
	RECT_S stRectAttr[VENC_MAX_SVC_RECT_NUM];
	VENC_SVC_RECT_TYPE_E enDetectType[VENC_MAX_SVC_RECT_NUM];
	GK_U64 u64Pts;
} VENC_SVC_RECT_INFO_S;

typedef struct {
	GK_U16 u16SceneComplexity;
	GK_U8 u8ObjectLevel;
} VENC_SVC_DETECT_RESULT_S;

typedef struct VENC_SEARCH_WINDOW_S {
	OPERATION_MODE_E enMode;
	GK_U32 u32Hor;
	GK_U32 u32Ver;
} VENC_SEARCH_WINDOW_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
