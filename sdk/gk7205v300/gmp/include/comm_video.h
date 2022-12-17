/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __COMM_VIDEO_H__
#define __COMM_VIDEO_H__

#include "type.h"
#include "common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SRC_LENS_COEF_SEG  2
#define DST_LENS_COEF_SEG  3
#define SRC_LENS_COEF_NUM  4
#define DST_LENS_COEF_NUM  4
#define DST_LENS_COEF_SEG_POINT  (DST_LENS_COEF_SEG - 1)

#define ISP_BAYER_CHN   (4)


typedef enum OPERATION_MODE_E
{
    OPERATION_MODE_AUTO   = 0,
    OPERATION_MODE_MANUAL = 1,
    OPERATION_MODE_BUTT
} OPERATION_MODE_E;


typedef enum ROTATION_E
{
    ROTATION_0   = 0,
    ROTATION_90  = 1,
    ROTATION_180 = 2,
    ROTATION_270 = 3,
    ROTATION_BUTT
} ROTATION_E;

typedef enum VB_SOURCE_E
{
    VB_SOURCE_COMMON  = 0,
    VB_SOURCE_MODULE  = 1,
    VB_SOURCE_PRIVATE = 2,
    VB_SOURCE_USER    = 3,
    VB_SOURCE_BUTT
} VB_SOURCE_E;

typedef enum DATA_RATE_E
{
    DATA_RATE_X1 = 0,
    DATA_RATE_X2 = 1,

    DATA_RATE_BUTT
} DATA_RATE_E;

typedef struct BORDER_S
{
    GK_U32 u32TopWidth;
    GK_U32 u32BottomWidth;
    GK_U32 u32LeftWidth;
    GK_U32 u32RightWidth;
    GK_U32 u32Color;
} BORDER_S;


typedef struct POINT_S
{
    GK_S32 s32X;
    GK_S32 s32Y;
} POINT_S;

typedef struct SIZE_S
{
    GK_U32 u32Width;
    GK_U32 u32Height;
} SIZE_S;

typedef struct RECT_S
{
    GK_S32 s32X;
    GK_S32 s32Y;
    GK_U32 u32Width;
    GK_U32 u32Height;
} RECT_S;

typedef struct VIDEO_REGION_INFO_S
{
    GK_U32           u32RegionNum;
    RECT_S ATTRIBUTE* pstRegion;
} VIDEO_REGION_INFO_S;


typedef struct CROP_INFO_S
{
    GK_BOOL bEnable;
    RECT_S  stRect;
} CROP_INFO_S;

typedef struct FRAME_RATE_CTRL_S
{
    GK_S32  s32SrcFrameRate;
    GK_S32  s32DstFrameRate;
} FRAME_RATE_CTRL_S;

typedef enum ASPECT_RATIO_E
{
    ASPECT_RATIO_NONE   = 0,
    ASPECT_RATIO_AUTO   = 1,
    ASPECT_RATIO_MANUAL = 2,
    ASPECT_RATIO_BUTT

}ASPECT_RATIO_E;

typedef struct ASPECT_RATIO_S
{
    ASPECT_RATIO_E enMode;
    GK_U32         u32BgColor;
    RECT_S         stVideoRect;

} ASPECT_RATIO_S;



typedef enum PIXEL_FORMAT_E
{
    PIXEL_FORMAT_RGB_444 = 0,
    PIXEL_FORMAT_RGB_555,
    PIXEL_FORMAT_RGB_565,
    PIXEL_FORMAT_RGB_888,

    PIXEL_FORMAT_BGR_444,
    PIXEL_FORMAT_BGR_555,
    PIXEL_FORMAT_BGR_565,
    PIXEL_FORMAT_BGR_888,

    PIXEL_FORMAT_ARGB_1555,
    PIXEL_FORMAT_ARGB_4444,
    PIXEL_FORMAT_ARGB_8565,
    PIXEL_FORMAT_ARGB_8888,
    PIXEL_FORMAT_ARGB_2BPP,

    PIXEL_FORMAT_ABGR_1555,
    PIXEL_FORMAT_ABGR_4444,
    PIXEL_FORMAT_ABGR_8565,
    PIXEL_FORMAT_ABGR_8888,

    PIXEL_FORMAT_RGB_BAYER_8BPP,
    PIXEL_FORMAT_RGB_BAYER_10BPP,
    PIXEL_FORMAT_RGB_BAYER_12BPP,
    PIXEL_FORMAT_RGB_BAYER_14BPP,
    PIXEL_FORMAT_RGB_BAYER_16BPP,


    PIXEL_FORMAT_YVU_PLANAR_422,
    PIXEL_FORMAT_YVU_PLANAR_420,
    PIXEL_FORMAT_YVU_PLANAR_444,

    PIXEL_FORMAT_YVU_SEMIPLANAR_422,
    PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    PIXEL_FORMAT_YVU_SEMIPLANAR_444,

    PIXEL_FORMAT_YUV_SEMIPLANAR_422,
    PIXEL_FORMAT_YUV_SEMIPLANAR_420,
    PIXEL_FORMAT_YUV_SEMIPLANAR_444,

    PIXEL_FORMAT_YUYV_PACKAGE_422,
    PIXEL_FORMAT_YVYU_PACKAGE_422,
    PIXEL_FORMAT_UYVY_PACKAGE_422,
    PIXEL_FORMAT_VYUY_PACKAGE_422,
    PIXEL_FORMAT_YYUV_PACKAGE_422,
    PIXEL_FORMAT_YYVU_PACKAGE_422,
    PIXEL_FORMAT_UVYY_PACKAGE_422,
    PIXEL_FORMAT_VUYY_PACKAGE_422,
    PIXEL_FORMAT_VY1UY0_PACKAGE_422,

    PIXEL_FORMAT_YUV_400,
    PIXEL_FORMAT_UV_420,


    PIXEL_FORMAT_BGR_888_PLANAR,
    PIXEL_FORMAT_HSV_888_PACKAGE,
    PIXEL_FORMAT_HSV_888_PLANAR,
    PIXEL_FORMAT_LAB_888_PACKAGE,
    PIXEL_FORMAT_LAB_888_PLANAR,
    PIXEL_FORMAT_S8C1,
    PIXEL_FORMAT_S8C2_PACKAGE,
    PIXEL_FORMAT_S8C2_PLANAR,
    PIXEL_FORMAT_S8C3_PLANAR,
    PIXEL_FORMAT_S16C1,
    PIXEL_FORMAT_U8C1,
    PIXEL_FORMAT_U16C1,
    PIXEL_FORMAT_S32C1,
    PIXEL_FORMAT_U32C1,
    PIXEL_FORMAT_U64C1,
    PIXEL_FORMAT_S64C1,

    PIXEL_FORMAT_BUTT
} PIXEL_FORMAT_E;


typedef enum VIDEO_FIELD_E
{
    VIDEO_FIELD_TOP         = 0x1,
    VIDEO_FIELD_BOTTOM      = 0x2,
    VIDEO_FIELD_INTERLACED  = 0x3,
    VIDEO_FIELD_FRAME       = 0x4,

    VIDEO_FIELD_BUTT
} VIDEO_FIELD_E;

typedef enum VIDEO_FORMAT_E
{
    VIDEO_FORMAT_LINEAR = 0,
    VIDEO_FORMAT_TILE_64x16,
    VIDEO_FORMAT_TILE_16x8,
    VIDEO_FORMAT_LINEAR_DISCRETE,
    VIDEO_FORMAT_BUTT
} VIDEO_FORMAT_E;



typedef enum COMPRESS_MODE_E
{
    COMPRESS_MODE_NONE = 0,
    COMPRESS_MODE_SEG,
    COMPRESS_MODE_TILE,
    COMPRESS_MODE_LINE,
    COMPRESS_MODE_FRAME,

    COMPRESS_MODE_BUTT
} COMPRESS_MODE_E;


typedef enum VIDEO_DISPLAY_MODE_E
{
    VIDEO_DISPLAY_MODE_PREVIEW  = 0x0,
    VIDEO_DISPLAY_MODE_PLAYBACK = 0x1,

    VIDEO_DISPLAY_MODE_BUTT
} VIDEO_DISPLAY_MODE_E;

typedef struct LUMA_INFO_S
{
    GK_U64 u64LumaPixSum;
    GK_U32 u32LumaPixAverage;
    GK_U64 u64PTS;
} LUMA_INFO_S;

#define LDCI_STAT_WND_X             24
#define LDCI_STAT_WND_Y             16

#define DCF_DRSCRIPTION_LENGTH      32
#define DCF_CAPTURE_TIME_LENGTH     20

typedef struct ISP_DCF_CONST_INFO_S
{
    GK_U8       au8ImageDescription[DCF_DRSCRIPTION_LENGTH];
    GK_U8       au8Make[DCF_DRSCRIPTION_LENGTH];
    GK_U8       au8Model[DCF_DRSCRIPTION_LENGTH];
    GK_U8       au8Software[DCF_DRSCRIPTION_LENGTH];

    GK_U8       u8LightSource;
    GK_U32      u32FocalLength;
    GK_U8       u8SceneType;
    GK_U8       u8CustomRendered;
    GK_U8       u8FocalLengthIn35mmFilm;
    GK_U8       u8SceneCaptureType;
    GK_U8       u8GainControl;
    GK_U8       u8Contrast;
    GK_U8       u8Saturation;
    GK_U8       u8Sharpness;
    GK_U8       u8MeteringMode;
} ISP_DCF_CONST_INFO_S;

typedef struct ISP_DCF_UPDATE_INFO_S
{
    GK_U32      u32ISOSpeedRatings;
    GK_U32      u32ExposureTime;
    GK_U32      u32ExposureBiasValue;
    GK_U8       u8ExposureProgram;
    GK_U32      u32FNumber;
    GK_U32      u32MaxApertureValue;
    GK_U8       u8ExposureMode;
    GK_U8       u8WhiteBalance;
} ISP_DCF_UPDATE_INFO_S;

typedef struct ISP_DCF_INFO_S
{
    ISP_DCF_CONST_INFO_S  stIspDCFConstInfo;
    ISP_DCF_UPDATE_INFO_S stIspDCFUpdateInfo;
} ISP_DCF_INFO_S;

typedef struct JPEG_DCF_S
{
    GK_U8           au8CaptureTime[DCF_CAPTURE_TIME_LENGTH];
    GK_BOOL         bFlash;
    GK_U32          u32DigitalZoomRatio;
    ISP_DCF_INFO_S  stIspDCFInfo;

} JPEG_DCF_S;


typedef struct ISP_FRAME_INFO_S
{
    GK_U32      u32ISO;
    GK_U32      u32ExposureTime;
    GK_U32      u32IspDgain;
    GK_U32      u32Again;
    GK_U32      u32Dgain;
    GK_U32      au32Ratio[3];
    GK_U32      u32IspNrStrength;
    GK_U32      u32FNumber;
    GK_U32      u32SensorID;
    GK_U32      u32SensorMode;
    GK_U32      u32HmaxTimes;
    GK_U32      u32Vmax;
    GK_U32      u32VcNum;
} ISP_FRAME_INFO_S;


#define CFACOLORPLANE     (3)
#define DNG_NP_SIZE       (6)


typedef struct DNG_RATIONAL_S
{
    GK_U32 u32Numerator;
    GK_U32 u32Denominator;
} DNG_RATIONAL_S;

typedef struct DNG_IMAGE_DYNAMIC_INFO_S
{
    GK_U32 au32BlackLevel[ISP_BAYER_CHN];
    DNG_RATIONAL_S astAsShotNeutral[CFACOLORPLANE];
    GK_DOUBLE adNoiseProfile[DNG_NP_SIZE];
} DNG_IMAGE_DYNAMIC_INFO_S;


typedef struct ISP_HDR_INFO_S
{
    GK_U32 u32ColorTemp;
    GK_U16 au16CCM[9];
    GK_U8  u8Saturation;
} ISP_HDR_INFO_S;

typedef struct ISP_ATTACH_INFO_S
{
    ISP_HDR_INFO_S     stIspHdr;
    GK_U32 u32ISO;
    GK_U32 u32InitISO;
    GK_U8 u8SnsWDRMode;
} ISP_ATTACH_INFO_S;

typedef enum FRAME_FLAG_E
{
    FRAME_FLAG_SNAP_FLASH  = 0x1 << 0,
    FRAME_FLAG_SNAP_CUR      = 0x1 << 1,
    FRAME_FLAG_SNAP_REF       = 0x1 << 2,
    FRAME_FLAG_SNAP_END      = 0x1 << 31,
    FRAME_FLAG_BUTT
} FRAME_FLAG_E;


#define ISP_WB_GAIN_NUM  4
#define ISP_CAP_CCM_NUM   9
typedef struct ISP_CONFIG_INFO_S
{
    GK_U32  u32ISO;
    GK_U32  u32IspDgain;
    GK_U32  u32ExposureTime;
    GK_U32  au32WhiteBalanceGain[ISP_WB_GAIN_NUM];
    GK_U32  u32ColorTemperature;
    GK_U16  au16CapCCM[ISP_CAP_CCM_NUM];
} ISP_CONFIG_INFO_S;

typedef struct VIDEO_SUPPLEMENT_S
{
    GK_U64   u64JpegDCFPhyAddr;
    GK_U64   u64IspInfoPhyAddr;
    GK_U64   u64LowDelayPhyAddr;
    GK_U64   u64MotionDataPhyAddr;
    GK_U64   u64FrameDNGPhyAddr;

    GK_VOID* ATTRIBUTE pJpegDCFVirAddr;
    GK_VOID* ATTRIBUTE pIspInfoVirAddr;
    GK_VOID* ATTRIBUTE pLowDelayVirAddr;
    GK_VOID* ATTRIBUTE pMotionDataVirAddr;
    GK_VOID* ATTRIBUTE pFrameDNGVirAddr;
} VIDEO_SUPPLEMENT_S;

typedef enum COLOR_GAMUT_E
{
    COLOR_GAMUT_BT601 = 0,
    COLOR_GAMUT_BT709,
    COLOR_GAMUT_BT2020,
    COLOR_GAMUT_USER,
    COLOR_GAMUT_BUTT
} COLOR_GAMUT_E;

typedef struct ISP_COLORGAMMUT_INFO_S
{
        COLOR_GAMUT_E enColorGamut;
} ISP_COLORGAMMUT_INFO_S;

typedef enum DYNAMIC_RANGE_E
{
    DYNAMIC_RANGE_SDR8 = 0,
    DYNAMIC_RANGE_SDR10,
    DYNAMIC_RANGE_HDR10,
    DYNAMIC_RANGE_HLG,
    DYNAMIC_RANGE_SLF,
    DYNAMIC_RANGE_XDR,
    DYNAMIC_RANGE_BUTT
} DYNAMIC_RANGE_E;


typedef enum DATA_BITWIDTH_E
{
    DATA_BITWIDTH_8 = 0,
    DATA_BITWIDTH_10,
    DATA_BITWIDTH_12,
    DATA_BITWIDTH_14,
    DATA_BITWIDTH_16,
    DATA_BITWIDTH_BUTT
} DATA_BITWIDTH_E;

typedef struct VIDEO_FRAME_S
{
    GK_U32              u32Width;
    GK_U32              u32Height;
    VIDEO_FIELD_E       enField;
    PIXEL_FORMAT_E      enPixelFormat;
    VIDEO_FORMAT_E      enVideoFormat;
    COMPRESS_MODE_E     enCompressMode;
    DYNAMIC_RANGE_E     enDynamicRange;
    COLOR_GAMUT_E       enColorGamut;
    GK_U32              u32HeaderStride[3];
    GK_U32              u32Stride[3];
    GK_U32              u32ExtStride[3];

    GK_U64              u64HeaderPhyAddr[3];
    GK_U64              u64HeaderVirAddr[3];
    GK_U64              u64PhyAddr[3];
    GK_U64              u64VirAddr[3];
    GK_U64              u64ExtPhyAddr[3];
    GK_U64              u64ExtVirAddr[3];

    GK_S16              s16OffsetTop;
    GK_S16              s16OffsetBottom;
    GK_S16              s16OffsetLeft;
    GK_S16              s16OffsetRight;

    GK_U32              u32MaxLuminance;
    GK_U32              u32MinLuminance;

    GK_U32              u32TimeRef;
    GK_U64              u64PTS;

    GK_U64              u64PrivateData;
    GK_U32              u32FrameFlag;
    VIDEO_SUPPLEMENT_S  stSupplement;
} VIDEO_FRAME_S;

typedef struct VIDEO_FRAME_INFO_S
{
    VIDEO_FRAME_S stVFrame;
    GK_U32        u32PoolId;
    MOD_ID_E      enModId;
} VIDEO_FRAME_INFO_S;



typedef struct VB_CAL_CONFIG_S
{
    GK_U32 u32VBSize;

    GK_U32 u32HeadStride;
    GK_U32 u32HeadSize;
    GK_U32 u32HeadYSize;

    GK_U32 u32MainStride;
    GK_U32 u32MainSize;
    GK_U32 u32MainYSize;

    GK_U32 u32ExtStride;
    GK_U32 u32ExtYSize;
}VB_CAL_CONFIG_S;


typedef struct BITMAP_S
{
    PIXEL_FORMAT_E enPixelFormat;
    GK_U32 u32Width;
    GK_U32 u32Height;
    GK_VOID* ATTRIBUTE pData;
} BITMAP_S;

typedef struct LDC_ATTR_S
{
    GK_BOOL    bAspect;
    GK_S32     s32XRatio;
    GK_S32     s32YRatio;
    GK_S32     s32XYRatio;
    GK_S32 s32CenterXOffset;
    GK_S32 s32CenterYOffset;
    GK_S32 s32DistortionRatio;
} LDC_ATTR_S;


typedef struct LDCV2_ATTR_S
{
    GK_S32 s32FocalLenX;
    GK_S32 s32FocalLenY;
    GK_S32 s32CoorShiftX;
    GK_S32 s32CoorShiftY;
    GK_S32 as32SrcCaliRatio[SRC_LENS_COEF_SEG][SRC_LENS_COEF_NUM];
    GK_S32 s32SrcJunPt;
    GK_S32 as32DstCaliRatio[DST_LENS_COEF_SEG][DST_LENS_COEF_NUM];
    GK_S32 as32DstJunPt[DST_LENS_COEF_SEG_POINT];
    GK_S32 s32MaxDu;
} LDCV2_ATTR_S;

typedef enum LDC_VIEW_TYPE_E
{
    LDC_VIEW_TYPE_ALL  = 0,
    LDC_VIEW_TYPE_CROP = 1,

    LDC_VIEW_TYPE_BUTT,
} LDC_VIEW_TYPE_E;

typedef struct LDCV3_ATTR_S
{
    LDC_VIEW_TYPE_E enViewType;
    GK_S32 s32CenterXOffset;
    GK_S32 s32CenterYOffset;
    GK_S32 s32DistortionRatio;
    GK_S32 s32MinRatio;
} LDCV3_ATTR_S;

typedef enum ROTATION_VIEW_TYPE_E
{
    ROTATION_VIEW_TYPE_ALL      = 0,
    ROTATION_VIEW_TYPE_TYPICAL  = 1,
    ROTATION_VIEW_TYPE_INSIDE   = 2,
    ROTATION_VIEW_TYPE_BUTT,
} ROTATION_VIEW_TYPE_E;

typedef struct ROTATION_EX_S
{
    ROTATION_VIEW_TYPE_E enViewType;
    GK_U32               u32Angle;
    GK_S32               s32CenterXOffset;
    GK_S32               s32CenterYOffset;
    SIZE_S               stDestSize;
} ROTATION_EX_S;


typedef enum WDR_MODE_E
{
    WDR_MODE_NONE = 0,
    WDR_MODE_BUILT_IN,
    WDR_MODE_QUDRA,

    WDR_MODE_2To1_LINE,
    WDR_MODE_2To1_FRAME,
    WDR_MODE_2To1_FRAME_FULL_RATE,

    WDR_MODE_3To1_LINE,
    WDR_MODE_3To1_FRAME,
    WDR_MODE_3To1_FRAME_FULL_RATE,

    WDR_MODE_4To1_LINE,
    WDR_MODE_4To1_FRAME,
    WDR_MODE_4To1_FRAME_FULL_RATE,

    WDR_MODE_BUTT,
} WDR_MODE_E;

typedef enum FRAME_INTERRUPT_TYPE_E
{
    FRAME_INTERRUPT_START,
    FRAME_INTERRUPT_EARLY,
    FRAME_INTERRUPT_EARLY_END,
    FRAME_INTERRUPT_EARLY_ONE_BUF,
    FRAME_INTERRUPT_EARLY_END_ONE_BUF,
    FRAME_INTERRUPT_BUTT,
} FRAME_INTERRUPT_TYPE_E;

typedef struct FRAME_INTERRUPT_ATTR_S
{
    FRAME_INTERRUPT_TYPE_E enIntType;
    GK_U32 u32EarlyLine;
} FRAME_INTERRUPT_ATTR_S;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

