/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef _IVE_H_
#define _IVE_H_

#include "comm_ive.h"
#include "common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define IVE_HIST_NUM          256
#define IVE_MAP_NUM           256
#define IVE_MAX_REGION_NUM    254
#define IVE_ST_MAX_CORNER_NUM 500


typedef enum IVE_DMA_MODE_E {
    IVE_DMA_MODE_DIRECT_COPY = 0x0,
    IVE_DMA_MODE_INTERVAL_COPY = 0x1,
    IVE_DMA_MODE_SET_3BYTE = 0x2,
    IVE_DMA_MODE_SET_8BYTE = 0x3,
    IVE_DMA_MODE_BUTT
} IVE_DMA_MODE_E;


typedef struct IVE_DMA_CTRL_S {
    IVE_DMA_MODE_E enMode;
    GK_U64 u64Val;
    GK_U8 u8HorSegSize;
    GK_U8 u8ElemSize;
    GK_U8 u8VerSegRows;
} IVE_DMA_CTRL_S;


typedef struct IVE_FILTER_CTRL_S {
    GK_S8 as8Mask[25];
    GK_U8 u8Norm;
} IVE_FILTER_CTRL_S;


typedef enum IVE_CSC_MODE_E {
    IVE_CSC_MODE_VIDEO_BT601_YUV2RGB = 0x0,
    IVE_CSC_MODE_VIDEO_BT709_YUV2RGB = 0x1,
    IVE_CSC_MODE_PIC_BT601_YUV2RGB = 0x2,
    IVE_CSC_MODE_PIC_BT709_YUV2RGB = 0x3,

    IVE_CSC_MODE_PIC_BT601_YUV2HSV = 0x4,
    IVE_CSC_MODE_PIC_BT709_YUV2HSV = 0x5,

    IVE_CSC_MODE_PIC_BT601_YUV2LAB = 0x6,
    IVE_CSC_MODE_PIC_BT709_YUV2LAB = 0x7,

    IVE_CSC_MODE_VIDEO_BT601_RGB2YUV = 0x8,
    IVE_CSC_MODE_VIDEO_BT709_RGB2YUV = 0x9,
    IVE_CSC_MODE_PIC_BT601_RGB2YUV = 0xa,
    IVE_CSC_MODE_PIC_BT709_RGB2YUV = 0xb,

    IVE_CSC_MODE_BUTT
} IVE_CSC_MODE_E;


typedef struct IVE_CSC_CTRL_S {
    IVE_CSC_MODE_E enMode;
} IVE_CSC_CTRL_S;


typedef struct IVE_FILTER_AND_CSC_CTRL_S {
    IVE_CSC_MODE_E enMode;
    GK_S8 as8Mask[25];
    GK_U8 u8Norm;
} IVE_FILTER_AND_CSC_CTRL_S;

typedef enum IVE_SOBEL_OUT_CTRL_E {
    IVE_SOBEL_OUT_CTRL_BOTH = 0x0,
    IVE_SOBEL_OUT_CTRL_HOR = 0x1,
    IVE_SOBEL_OUT_CTRL_VER = 0x2,
    IVE_SOBEL_OUT_CTRL_BUTT
} IVE_SOBEL_OUT_CTRL_E;


typedef struct IVE_SOBEL_CTRL_S {
    IVE_SOBEL_OUT_CTRL_E enOutCtrl;
    GK_S8 as8Mask[25];
} IVE_SOBEL_CTRL_S;


typedef enum IVE_MAG_AND_ANG_OUT_CTRL_E {
    IVE_MAG_AND_ANG_OUT_CTRL_MAG = 0x0,
    IVE_MAG_AND_ANG_OUT_CTRL_MAG_AND_ANG = 0x1,
    IVE_MAG_AND_ANG_OUT_CTRL_BUTT
} IVE_MAG_AND_ANG_OUT_CTRL_E;

typedef struct IVE_MAG_AND_ANG_CTRL_S {
    IVE_MAG_AND_ANG_OUT_CTRL_E enOutCtrl;
    GK_U16 u16Thr;
    GK_S8 as8Mask[25];
} IVE_MAG_AND_ANG_CTRL_S;

typedef struct IVE_DILATE_CTRL_S {
    GK_U8 au8Mask[25];
} IVE_DILATE_CTRL_S;

typedef struct IVE_ERODE_CTRL_S {
    GK_U8 au8Mask[25];
} IVE_ERODE_CTRL_S;


typedef enum IVE_THRESH_MODE_E {
    IVE_THRESH_MODE_BINARY = 0x0,
    IVE_THRESH_MODE_TRUNC = 0x1,
    IVE_THRESH_MODE_TO_MINVAL = 0x2,

    IVE_THRESH_MODE_MIN_MID_MAX = 0x3,
    IVE_THRESH_MODE_ORI_MID_MAX = 0x4,
    IVE_THRESH_MODE_MIN_MID_ORI = 0x5,
    IVE_THRESH_MODE_MIN_ORI_MAX = 0x6,
    IVE_THRESH_MODE_ORI_MID_ORI = 0x7,

    IVE_THRESH_MODE_BUTT
} IVE_THRESH_MODE_E;


typedef struct IVE_THRESH_CTRL_S {
    IVE_THRESH_MODE_E enMode;
    GK_U8 u8LowThr;
    GK_U8 u8HighThr;
    GK_U8 u8MinVal;
    GK_U8 u8MidVal;
    GK_U8 u8MaxVal;
} IVE_THRESH_CTRL_S;

typedef enum IVE_SUB_MODE_E {
    IVE_SUB_MODE_ABS = 0x0,
    IVE_SUB_MODE_SHIFT = 0x1,
    IVE_SUB_MODE_BUTT
} IVE_SUB_MODE_E;

typedef struct IVE_SUB_CTRL_S {
    IVE_SUB_MODE_E enMode;
} IVE_SUB_CTRL_S;


typedef enum IVE_INTEG_OUT_CTRL_E {
    IVE_INTEG_OUT_CTRL_COMBINE = 0x0,
    IVE_INTEG_OUT_CTRL_SUM = 0x1,
    IVE_INTEG_OUT_CTRL_SQSUM = 0x2,
    IVE_INTEG_OUT_CTRL_BUTT
} IVE_INTEG_OUT_CTRL_E;


typedef struct IVE_INTEG_CTRL_S {
    IVE_INTEG_OUT_CTRL_E enOutCtrl;
} IVE_INTEG_CTRL_S;

typedef enum IVE_THRESH_S16_MODE_E {
    IVE_THRESH_S16_MODE_S16_TO_S8_MIN_MID_MAX = 0x0,
    IVE_THRESH_S16_MODE_S16_TO_S8_MIN_ORI_MAX = 0x1,
    IVE_THRESH_S16_MODE_S16_TO_U8_MIN_MID_MAX = 0x2,
    IVE_THRESH_S16_MODE_S16_TO_U8_MIN_ORI_MAX = 0x3,

    IVE_THRESH_S16_MODE_BUTT
} IVE_THRESH_S16_MODE_E;

typedef struct IVE_THRESH_S16_CTRL_S {
    IVE_THRESH_S16_MODE_E enMode;
    GK_S16 s16LowThr;
    GK_S16 s16HighThr;
    IVE_8BIT_U un8MinVal;
    IVE_8BIT_U un8MidVal;
    IVE_8BIT_U un8MaxVal;
} IVE_THRESH_S16_CTRL_S;

typedef enum IVE_THRESH_U16_MODE_E {
    IVE_THRESH_U16_MODE_U16_TO_U8_MIN_MID_MAX = 0x0,
    IVE_THRESH_U16_MODE_U16_TO_U8_MIN_ORI_MAX = 0x1,

    IVE_THRESH_U16_MODE_BUTT
} IVE_THRESH_U16_MODE_E;

typedef struct IVE_THRESH_U16_CTRL_S {
    IVE_THRESH_U16_MODE_E enMode;
    GK_U16 u16LowThr;
    GK_U16 u16HighThr;
    GK_U8 u8MinVal;
    GK_U8 u8MidVal;
    GK_U8 u8MaxVal;
} IVE_THRESH_U16_CTRL_S;

typedef enum IVE_16BIT_TO_8BIT_MODE_E {
    IVE_16BIT_TO_8BIT_MODE_S16_TO_S8 = 0x0,
    IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_ABS = 0x1,
    IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_BIAS = 0x2,
    IVE_16BIT_TO_8BIT_MODE_U16_TO_U8 = 0x3,

    IVE_16BIT_TO_8BIT_MODE_BUTT
} IVE_16BIT_TO_8BIT_MODE_E;

typedef struct IVE_16BIT_TO_8BIT_CTRL_S {
    IVE_16BIT_TO_8BIT_MODE_E enMode;
    GK_U16 u16Denominator;
    GK_U8 u8Numerator;
    GK_S8 s8Bias;
} IVE_16BIT_TO_8BIT_CTRL_S;

typedef enum IVE_ORD_STAT_FILTER_MODE_E {
    IVE_ORD_STAT_FILTER_MODE_MEDIAN = 0x0,
    IVE_ORD_STAT_FILTER_MODE_MAX = 0x1,
    IVE_ORD_STAT_FILTER_MODE_MIN = 0x2,

    IVE_ORD_STAT_FILTER_MODE_BUTT
} IVE_ORD_STAT_FILTER_MODE_E;

typedef struct IVE_ORD_STAT_FILTER_CTRL_S {
    IVE_ORD_STAT_FILTER_MODE_E enMode;

} IVE_ORD_STAT_FILTER_CTRL_S;

typedef enum IVE_MAP_MODE_E {
    IVE_MAP_MODE_U8 = 0x0,
    IVE_MAP_MODE_S16 = 0x1,
    IVE_MAP_MODE_U16 = 0x2,

    IVE_MAP_MODE_BUTT
} IVE_MAP_MODE_E;
typedef struct IVE_MAP_CTRL_S {
    IVE_MAP_MODE_E enMode;
} IVE_MAP_CTRL_S;
typedef struct IVE_MAP_U8BIT_LUT_MEM_S {
    GK_U8 au8Map[IVE_MAP_NUM];
} IVE_MAP_U8BIT_LUT_MEM_S;
typedef struct IVE_MAP_U16BIT_LUT_MEM_S {
    GK_U16 au16Map[IVE_MAP_NUM];
} IVE_MAP_U16BIT_LUT_MEM_S;
typedef struct IVE_MAP_S16BIT_LUT_MEM_S {
    GK_S16 as16Map[IVE_MAP_NUM];
} IVE_MAP_S16BIT_LUT_MEM_S;
typedef struct IVE_EQUALIZE_HIST_CTRL_MEM_S {
    GK_U32 au32Hist[IVE_HIST_NUM];
    GK_U8 au8Map[IVE_MAP_NUM];
} IVE_EQUALIZE_HIST_CTRL_MEM_S;


typedef struct IVE_EQUALIZE_HIST_CTRL_S {
    IVE_MEM_INFO_S stMem;
} IVE_EQUALIZE_HIST_CTRL_S;


typedef struct IVE_ADD_CTRL_S {
    GK_U0Q16 u0q16X;
    GK_U0Q16 u0q16Y;
} IVE_ADD_CTRL_S;

typedef struct IVE_NCC_DST_MEM_S {
    GK_U64 u64Numerator;
    GK_U64 u64QuadSum1;
    GK_U64 u64QuadSum2;
    GK_U8 u8Reserved[8];
} IVE_NCC_DST_MEM_S;

typedef struct IVE_REGION_S {
    GK_U32 u32Area;
    GK_U16 u16Left;
    GK_U16 u16Right;
    GK_U16 u16Top;
    GK_U16 u16Bottom;
} IVE_REGION_S;

typedef struct IVE_CCBLOB_S {
    GK_U16 u16CurAreaThr;
    GK_S8 s8LabelStatus;
    GK_U8 u8RegionNum;
    IVE_REGION_S astRegion[IVE_MAX_REGION_NUM];
} IVE_CCBLOB_S;

typedef enum IVE_CCL_MODE_E {
    IVE_CCL_MODE_4C = 0x0,
    IVE_CCL_MODE_8C = 0x1,

    IVE_CCL_MODE_BUTT
} IVE_CCL_MODE_E;
typedef struct IVE_CCL_CTRL_S {
    IVE_CCL_MODE_E enMode;
    GK_U16 u16InitAreaThr;
    GK_U16 u16Step;
} IVE_CCL_CTRL_S;

typedef struct IVE_GMM_CTRL_S {
    GK_U22Q10 u22q10NoiseVar;
    GK_U22Q10 u22q10MaxVar;
    GK_U22Q10 u22q10MinVar;
    GK_U0Q16 u0q16LearnRate;
    GK_U0Q16 u0q16BgRatio;
    GK_U8Q8 u8q8VarThr;
    GK_U0Q16 u0q16InitWeight;
    GK_U8 u8ModelNum;
} IVE_GMM_CTRL_S;

typedef enum IVE_GMM2_SNS_FACTOR_MODE_E {
    IVE_GMM2_SNS_FACTOR_MODE_GLB = 0x0,
    IVE_GMM2_SNS_FACTOR_MODE_PIX = 0x1,

    IVE_GMM2_SNS_FACTOR_MODE_BUTT
} IVE_GMM2_SNS_FACTOR_MODE_E;

typedef enum IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_E {
    IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_GLB = 0x0,
    IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_PIX = 0x1,

    IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_BUTT
} IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_E;

typedef struct IVE_GMM2_CTRL_S {
    IVE_GMM2_SNS_FACTOR_MODE_E enSnsFactorMode;
    IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_E enLifeUpdateFactorMode;
    GK_U16 u16GlbLifeUpdateFactor;
    GK_U16 u16LifeThr;
    GK_U16 u16FreqInitVal;
    GK_U16 u16FreqReduFactor;
    GK_U16 u16FreqAddFactor;
    GK_U16 u16FreqThr;
    GK_U16 u16VarRate;
    GK_U9Q7 u9q7MaxVar;
    GK_U9Q7 u9q7MinVar;
    GK_U8 u8GlbSnsFactor;
    GK_U8 u8ModelNum;
} IVE_GMM2_CTRL_S;

typedef struct IVE_CANNY_HYS_EDGE_CTRL_S {
    IVE_MEM_INFO_S stMem;
    GK_U16 u16LowThr;
    GK_U16 u16HighThr;
    GK_S8 as8Mask[25];
} IVE_CANNY_HYS_EDGE_CTRL_S;

typedef struct IVE_CANNY_STACK_SIZE_S {
    GK_U32 u32StackSize;
    GK_U8 u8Reserved[12];
} IVE_CANNY_STACK_SIZE_S;

typedef enum IVE_LBP_CMP_MODE_E {
    IVE_LBP_CMP_MODE_NORMAL = 0x0,
    IVE_LBP_CMP_MODE_ABS = 0x1,

    IVE_LBP_CMP_MODE_BUTT
} IVE_LBP_CMP_MODE_E;

typedef struct IVE_LBP_CTRL_S {
    IVE_LBP_CMP_MODE_E enMode;
    IVE_8BIT_U un8BitThr;
} IVE_LBP_CTRL_S;

typedef enum IVE_NORM_GRAD_OUT_CTRL_E {
    IVE_NORM_GRAD_OUT_CTRL_HOR_AND_VER = 0x0,
    IVE_NORM_GRAD_OUT_CTRL_HOR = 0x1,
    IVE_NORM_GRAD_OUT_CTRL_VER = 0x2,
    IVE_NORM_GRAD_OUT_CTRL_COMBINE = 0x3,

    IVE_NORM_GRAD_OUT_CTRL_BUTT
} IVE_NORM_GRAD_OUT_CTRL_E;

typedef struct IVE_NORM_GRAD_CTRL_S {
    IVE_NORM_GRAD_OUT_CTRL_E enOutCtrl;
    GK_S8 as8Mask[25];
    GK_U8 u8Norm;
} IVE_NORM_GRAD_CTRL_S;

typedef enum IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_E {
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_NONE = 0,
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_STATUS = 1,
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH = 2,

    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BUTT
} IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_E;

typedef struct IVE_LK_OPTICAL_FLOW_PYR_CTRL_S {
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_E enOutMode;
    GK_BOOL bUseInitFlow;
    GK_U16 u16PtsNum;
    GK_U8 u8MaxLevel;
    GK_U0Q8 u0q8MinEigThr;
    GK_U8 u8IterCnt;
    GK_U0Q8 u0q8Eps;
} IVE_LK_OPTICAL_FLOW_PYR_CTRL_S;

typedef struct IVE_ST_MAX_EIG_S {
    GK_U16 u16MaxEig;
    GK_U8 u8Reserved[14];
} IVE_ST_MAX_EIG_S;

typedef struct IVE_ST_CANDI_CORNER_CTRL_S {
    IVE_MEM_INFO_S stMem;
    GK_U0Q8 u0q8QualityLevel;
} IVE_ST_CANDI_CORNER_CTRL_S;

typedef struct IVE_ST_CORNER_INFO_S {
    GK_U16 u16CornerNum;
    IVE_POINT_U16_S astCorner[IVE_ST_MAX_CORNER_NUM];
} IVE_ST_CORNER_INFO_S;

typedef struct IVE_ST_CORNER_CTRL_S {
    GK_U16 u16MaxCornerNum;
    GK_U16 u16MinDist;
} IVE_ST_CORNER_CTRL_S;

typedef enum IVE_GRAD_FG_MODE_E {
    IVE_GRAD_FG_MODE_USE_CUR_GRAD = 0x0,
    IVE_GRAD_FG_MODE_FIND_MIN_GRAD = 0x1,

    IVE_GRAD_FG_MODE_BUTT
} IVE_GRAD_FG_MODE_E;

typedef struct IVE_GRAD_FG_CTRL_S {
    IVE_GRAD_FG_MODE_E enMode;
    GK_U16 u16EdwFactor;
    GK_U8 u8CrlCoefThr;
    GK_U8 u8MagCrlThr;
    GK_U8 u8MinMagDiff;
    GK_U8 u8NoiseVal;
    GK_U8 u8EdwDark;
} IVE_GRAD_FG_CTRL_S;

typedef struct IVE_CANDI_BG_PIX_S {
    GK_U8Q4F4 u8q4f4Mean;
    GK_U16 u16StartTime;
    GK_U16 u16SumAccessTime;
    GK_U16 u16ShortKeepTime;
    GK_U8 u8ChgCond;
    GK_U8 u8PotenBgLife;
} IVE_CANDI_BG_PIX_S;

typedef struct IVE_WORK_BG_PIX_S {
    GK_U8Q4F4 u8q4f4Mean;
    GK_U16 u16AccTime;
    GK_U8 u8PreGray;
    GK_U5Q3 u5q3DiffThr;
    GK_U8 u8AccFlag;
    GK_U8 u8BgGray[3];
} IVE_WORK_BG_PIX_S;

typedef struct IVE_BG_LIFE_S {
    GK_U8 u8WorkBgLife[3];
    GK_U8 u8CandiBgLife;
} IVE_BG_LIFE_S;

typedef struct IVE_BG_MODEL_PIX_S {
    IVE_WORK_BG_PIX_S stWorkBgPixel;
    IVE_CANDI_BG_PIX_S stCandiPixel;
    IVE_BG_LIFE_S stBgLife;
} IVE_BG_MODEL_PIX_S;

typedef struct IVE_FG_STAT_DATA_S {
    GK_U32 u32PixNum;
    GK_U32 u32SumLum;
    GK_U8 u8Reserved[8];
} IVE_FG_STAT_DATA_S;

typedef struct IVE_BG_STAT_DATA_S {
    GK_U32 u32PixNum;
    GK_U32 u32SumLum;
    GK_U8 u8Reserved[8];
} IVE_BG_STAT_DATA_S;

typedef struct IVE_MATCH_BG_MODEL_CTRL_S {
    GK_U32 u32CurFrmNum;
    GK_U32 u32PreFrmNum;
    GK_U16 u16TimeThr;

    GK_U8 u8DiffThrCrlCoef;
    GK_U8 u8DiffMaxThr;
    GK_U8 u8DiffMinThr;
    GK_U8 u8DiffThrInc;
    GK_U8 u8FastLearnRate;
    GK_U8 u8DetChgRegion;
} IVE_MATCH_BG_MODEL_CTRL_S;

typedef struct IVE_UPDATE_BG_MODEL_CTRL_S {
    GK_U32 u32CurFrmNum;
    GK_U32 u32PreChkTime;
    GK_U32 u32FrmChkPeriod;

    GK_U32 u32InitMinTime;
    GK_U32 u32StyBgMinBlendTime;
    GK_U32 u32StyBgMaxBlendTime;
    GK_U32 u32DynBgMinBlendTime;
    GK_U32 u32StaticDetMinTime;
    GK_U16 u16FgMaxFadeTime;
    GK_U16 u16BgMaxFadeTime;

    GK_U8 u8StyBgAccTimeRateThr;
    GK_U8 u8ChgBgAccTimeRateThr;
    GK_U8 u8DynBgAccTimeThr;
    GK_U8 u8DynBgDepth;
    GK_U8 u8BgEffStaRateThr;

    GK_U8 u8AcceBgLearn;
    GK_U8 u8DetChgRegion;
} IVE_UPDATE_BG_MODEL_CTRL_S;

typedef enum IVE_ANN_MLP_ACTIV_FUNC_E {
    IVE_ANN_MLP_ACTIV_FUNC_IDENTITY = 0x0,
    IVE_ANN_MLP_ACTIV_FUNC_SIGMOID_SYM = 0x1,
    IVE_ANN_MLP_ACTIV_FUNC_GAUSSIAN = 0x2,

    IVE_ANN_MLP_ACTIV_FUNC_BUTT
} IVE_ANN_MLP_ACTIV_FUNC_E;
typedef enum IVE_ANN_MLP_ACCURATE_E {
    IVE_ANN_MLP_ACCURATE_SRC16_WGT16 = 0x0,
    IVE_ANN_MLP_ACCURATE_SRC14_WGT20 = 0x1,

    IVE_ANN_MLP_ACCURATE_BUTT
} IVE_ANN_MLP_ACCURATE_E;

typedef struct IVE_ANN_MLP_MODEL_S {
    IVE_ANN_MLP_ACTIV_FUNC_E enActivFunc;
    IVE_ANN_MLP_ACCURATE_E enAccurate;
    IVE_MEM_INFO_S stWeight;
    GK_U32 u32TotalWeightSize;

    GK_U16 au16LayerCount[8];
    GK_U16 u16MaxCount;
    GK_U8 u8LayerNum;
    GK_U8 u8Reserved;
} IVE_ANN_MLP_MODEL_S;

typedef enum IVE_SVM_TYPE_E {
    IVE_SVM_TYPE_C_SVC = 0x0,
    IVE_SVM_TYPE_NU_SVC = 0x1,

    IVE_SVM_TYPE_BUTT
} IVE_SVM_TYPE_E;

typedef enum IVE_SVM_KERNEL_TYPE_E {
    IVE_SVM_KERNEL_TYPE_LINEAR = 0x0,
    IVE_SVM_KERNEL_TYPE_POLY = 0x1,
    IVE_SVM_KERNEL_TYPE_RBF = 0x2,
    IVE_SVM_KERNEL_TYPE_SIGMOID = 0x3,

    IVE_SVM_KERNEL_TYPE_BUTT
} IVE_SVM_KERNEL_TYPE_E;

typedef struct IVE_SVM_MODEL_S {
    IVE_SVM_TYPE_E enType;
    IVE_SVM_KERNEL_TYPE_E enKernelType;

    IVE_MEM_INFO_S stSv;
    IVE_MEM_INFO_S stDf;
    GK_U32 u32TotalDfSize;

    GK_U16 u16FeatureDim;
    GK_U16 u16SvTotal;
    GK_U8 u8ClassCount;
} IVE_SVM_MODEL_S;

typedef enum IVE_SAD_MODE_E {
    IVE_SAD_MODE_MB_4X4 = 0x0,
    IVE_SAD_MODE_MB_8X8 = 0x1,
    IVE_SAD_MODE_MB_16X16 = 0x2,

    IVE_SAD_MODE_BUTT
} IVE_SAD_MODE_E;
typedef enum IVE_SAD_OUT_CTRL_E {
    IVE_SAD_OUT_CTRL_16BIT_BOTH = 0x0,
    IVE_SAD_OUT_CTRL_8BIT_BOTH = 0x1,
    IVE_SAD_OUT_CTRL_16BIT_SAD = 0x2,
    IVE_SAD_OUT_CTRL_8BIT_SAD = 0x3,
    IVE_SAD_OUT_CTRL_THRESH = 0x4,

    IVE_SAD_OUT_CTRL_BUTT
} IVE_SAD_OUT_CTRL_E;
typedef struct IVE_SAD_CTRL_S {
    IVE_SAD_MODE_E enMode;
    IVE_SAD_OUT_CTRL_E enOutCtrl;
    GK_U16 u16Thr;
    GK_U8 u8MinVal;
    GK_U8 u8MaxVal;
} IVE_SAD_CTRL_S;

typedef enum IVE_RESIZE_MODE_E {
    IVE_RESIZE_MODE_LINEAR = 0x0,
    IVE_RESIZE_MODE_AREA = 0x1,

    IVE_RESIZE_MODE_BUTT
} IVE_RESIZE_MODE_E;

typedef struct IVE_RESIZE_CTRL_S {
    IVE_RESIZE_MODE_E enMode;
    IVE_MEM_INFO_S stMem;
    GK_U16 u16Num;
} IVE_RESIZE_CTRL_S;

typedef enum IVE_CNN_ACTIV_FUNC_E {
    IVE_CNN_ACTIV_FUNC_NONE = 0x0,
    IVE_CNN_ACTIV_FUNC_RELU = 0x1,
    IVE_CNN_ACTIV_FUNC_SIGMOID = 0x2,

    IVE_CNN_ACTIV_FUNC_BUTT
} IVE_CNN_ACTIV_FUNC_E;

typedef enum IVE_CNN_POOLING_E {
    IVE_CNN_POOLING_NONE = 0x0,
    IVE_CNN_POOLING_MAX = 0x1,
    IVE_CNN_POOLING_AVG = 0x2,

    IVE_CNN_POOLING_BUTT
} IVE_CNN_POOLING_E;

typedef struct IVE_CNN_CONV_POOLING_S {
    IVE_CNN_ACTIV_FUNC_E enActivFunc;
    IVE_CNN_POOLING_E enPooling;

    GK_U8 u8FeatureMapNum;
    GK_U8 u8KernelSize;
    GK_U8 u8ConvStep;

    GK_U8 u8PoolSize;
    GK_U8 u8PoolStep;
    GK_U8 u8Reserved[3];

} IVE_CNN_CONV_POOLING_S;

typedef struct IVE_CNN_FULL_CONNECT_S {
    GK_U16 au16LayerCnt[8];
    GK_U16 u16MaxCnt;
    GK_U8 u8LayerNum;
    GK_U8 u8Reserved;
} IVE_CNN_FULL_CONNECT_S;

typedef struct IVE_CNN_MODEL_S {
    IVE_CNN_CONV_POOLING_S astConvPool[8];
    IVE_CNN_FULL_CONNECT_S stFullConnect;

    IVE_MEM_INFO_S stConvKernelBias;
    GK_U32 u32ConvKernelBiasSize;

    IVE_MEM_INFO_S stFCLWgtBias;
    GK_U32 u32FCLWgtBiasSize;

    GK_U32 u32TotalMemSize;

    IVE_IMAGE_TYPE_E enType;
    GK_U32 u32Width;
    GK_U32 u32Height;

    GK_U16 u16ClassCount;
    GK_U8 u8ConvPoolLayerNum;
    GK_U8 u8Reserved;
} IVE_CNN_MODEL_S;

typedef struct IVE_CNN_CTRL_S {
    IVE_MEM_INFO_S stMem;
    GK_U32 u32Num;
} IVE_CNN_CTRL_S;

typedef struct IVE_CNN_RESULT_S {
    GK_S32 s32ClassIdx;
    GK_S32 s32Confidence;
} IVE_CNN_RESULT_S;

typedef enum IVE_PERSP_TRANS_ALG_MODE_E {
    IVE_PERSP_TRANS_ALG_MODE_NR_SIM = 0x0,
    IVE_PERSP_TRANS_ALG_MODE_SIM = 0x1,
    IVE_PERSP_TRANS_ALG_MODE_AFFINE = 0x2,

    IVE_PERSP_TRANS_ALG_MODE_BUTT
} IVE_PERSP_TRANS_ALG_MODE_E;

typedef struct IVE_PERSP_TRANS_POINT_PAIR_S {
    IVE_POINT_U14Q2_S stSrcPoint;
    IVE_POINT_U14Q2_S stDstPoint;
} IVE_PERSP_TRANS_POINT_PAIR_S;

typedef enum IVE_PERSP_TRANS_CSC_MODE_E {
    IVE_PERSP_TRANS_CSC_MODE_NONE = 0x0,
    IVE_PERSP_TRANS_CSC_MODE_VIDEO_BT601_YUV2RGB = 0x1,
    IVE_PERSP_TRANS_CSC_MODE_VIDEO_BT709_YUV2RGB = 0x2,
    IVE_PERSP_TRANS_CSC_MODE_PIC_BT601_YUV2RGB = 0x3,
    IVE_PERSP_TRANS_CSC_MODE_PIC_BT709_YUV2RGB = 0x4,

    IVE_PERSP_TRANS_CSC_MODE_BUTT
} IVE_PERSP_TRANS_CSC_MODE_E;

typedef struct IVE_PERSP_TRANS_CTRL_S {
    IVE_PERSP_TRANS_ALG_MODE_E enAlgMode;
    IVE_PERSP_TRANS_CSC_MODE_E enCscMode;
    GK_U16 u16RoiNum;
    GK_U16 u16PointPairNum;
} IVE_PERSP_TRANS_CTRL_S;

typedef struct IVE_ROI_INFO_S {
    IVE_RECT_S24Q8_S stRoi;
    GK_U32 u32RoiId;
} IVE_ROI_INFO_S;

typedef struct IVE_KCF_PRO_CTRL_S {
    IVE_CSC_MODE_E enCscMode;
    IVE_MEM_INFO_S stTmpBuf;
    GK_U1Q15 u1q15InterFactor;
    GK_U0Q16 u0q16Lamda;
    GK_U4Q12 u4q12TrancAlfa;
    GK_U0Q8 u0q8Sigma;
    GK_U8 u8RespThr;
} IVE_KCF_PRO_CTRL_S;

typedef struct IVE_KCF_OBJ_S {
    IVE_ROI_INFO_S stRoiInfo;
    IVE_MEM_INFO_S stCosWinX;
    IVE_MEM_INFO_S stCosWinY;
    IVE_MEM_INFO_S stGaussPeak;
    IVE_MEM_INFO_S stHogFeature;
    IVE_MEM_INFO_S stAlpha;
    IVE_MEM_INFO_S stDst;
    GK_U3Q5 u3q5Padding;
    GK_U8 au8Reserved[3];
} IVE_KCF_OBJ_S;

typedef struct IVE_LIST_HEAD_S {
    struct IVE_LIST_HEAD_S *pstNext, *pstPrev;
} IVE_LIST_HEAD_S;

typedef struct IVE_KCF_OBJ_NODE_S {
    IVE_LIST_HEAD_S stList;
    IVE_KCF_OBJ_S stKcfObj;
} IVE_KCF_OBJ_NODE_S;

typedef enum IVE_KCF_LIST_STATE_E {
    IVE_KCF_LIST_STATE_CREATE = 0x1,
    IVE_KCF_LIST_STATE_DESTORY = 0x2,
    IVE_KCF_LIST_STATE_BUTT
} IVE_KCF_LIST_STATE_E;

typedef struct IVE_KCF_OBJ_LIST_S {
    IVE_KCF_OBJ_NODE_S *pstObjNodeBuf;
    IVE_LIST_HEAD_S stFreeObjList;
    IVE_LIST_HEAD_S stTrainObjList;
    IVE_LIST_HEAD_S stTrackObjList;

    GK_U32 u32FreeObjNum;
    GK_U32 u32TrainObjNum;
    GK_U32 u32TrackObjNum;
    GK_U32 u32MaxObjNum;
    IVE_KCF_LIST_STATE_E enListState;
    GK_U8 *pu8TmpBuf;
} IVE_KCF_OBJ_LIST_S;

typedef struct IVE_KCF_BBOX_S {
    IVE_KCF_OBJ_NODE_S *pstNode;
    GK_S32 s32Response;

    IVE_ROI_INFO_S stRoiInfo;
    GK_BOOL bTrackOk;
    GK_BOOL bRoiRefresh;
} IVE_KCF_BBOX_S;

typedef struct IVE_KCF_BBOX_CTRL_S {
    GK_U32 u32MaxBboxNum;
    GK_S32 s32RespThr;
} IVE_KCF_BBOX_CTRL_S;

typedef enum IVE_HOG_MODE_E {
    IVE_HOG_MODE_VERTICAL_TANGENT_PLANE = 0x1,
    IVE_HOG_MODE_HORIZONTAL_TANGENT_PLANE = 0x2,
    IVE_HOG_MODE_BUTT
} IVE_HOG_MODE_E;

typedef struct IVE_HOG_CTRL_S {
    IVE_CSC_MODE_E enCscMode;
    IVE_HOG_MODE_E enHogMode;
    GK_U32 u32RoiNum;
    GK_U4Q12 u4q12TrancAlfa;
    GK_U8 au8Rsv[2];
} IVE_HOG_CTRL_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
