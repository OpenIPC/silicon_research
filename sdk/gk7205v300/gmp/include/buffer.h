/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "math_fun.h"
#include "type.h"
#include "common.h"
#include "comm_video.h"
#ifndef __LITEOS__
#ifdef __KERNEL__
#include "osal.h"
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SVP_MAXINUM_LIMIT 10000

__inline static GK_VOID COMMON_GetPicBufferConfig(GK_U32 u32Width, GK_U32 u32Height,
    PIXEL_FORMAT_E enPixelFormat, DATA_BITWIDTH_E enBitWidth,
    COMPRESS_MODE_E enCmpMode, GK_U32 u32Align, VB_CAL_CONFIG_S *pstCalConfig)
{
    GK_U32 u32BitWidth = 0;
    GK_U32 u32VBSize = 0;
    GK_U32 u32HeadSize = 0;
    GK_U32 u32AlignHeight;
    GK_U32 u32MainStride = 0;
    GK_U32 u32MainSize = 0;
    GK_U32 u32HeadStride = 0;
    GK_U32 u32HeadYSize = 0;
    GK_U32 u32YSize = 0;

    if ((u32Width > SVP_MAXINUM_LIMIT) || (u32Height > SVP_MAXINUM_LIMIT)) {
        pstCalConfig->u32VBSize = 0;
    }

    if (u32Align == 0) {
        u32Align = DEFAULT_ALIGN;
    } else if (u32Align > MAX_ALIGN) {
        u32Align = MAX_ALIGN;
    } else {
        u32Align = (ALIGN_UP(u32Align, DEFAULT_ALIGN));
    }

    switch (enBitWidth) {
        case DATA_BITWIDTH_8: {
            u32BitWidth = 8;
            break;
        }
        case DATA_BITWIDTH_16: {
            u32BitWidth = 16;
            break;
        }
        default:
        {
            u32BitWidth = 0;
            break;
        }
    }

    u32AlignHeight = ALIGN_UP(u32Height, 2);

    if (enCmpMode == COMPRESS_MODE_NONE) {
        u32MainStride = ALIGN_UP((u32Width * u32BitWidth + 7) >> 3, u32Align);
        u32YSize = u32MainStride * u32AlignHeight;

        if ((PIXEL_FORMAT_YVU_SEMIPLANAR_420 == enPixelFormat) ||
            (PIXEL_FORMAT_YUV_SEMIPLANAR_420 == enPixelFormat)) {
            u32MainSize = (u32MainStride * u32AlignHeight * 3) >> 1;
        } else if ((PIXEL_FORMAT_YVU_SEMIPLANAR_422 == enPixelFormat) ||
                   (PIXEL_FORMAT_YUV_SEMIPLANAR_422 == enPixelFormat)) {
            u32MainSize = u32MainStride * u32AlignHeight * 2;
        } else if ((enPixelFormat == PIXEL_FORMAT_YUV_400) ||
                   (enPixelFormat == PIXEL_FORMAT_S16C1) ||
                   (enPixelFormat == PIXEL_FORMAT_U16C1) ||
                   (enPixelFormat == PIXEL_FORMAT_S8C1) ||
                   (enPixelFormat == PIXEL_FORMAT_U8C1)) {
            u32MainSize = u32MainStride * u32AlignHeight;
        } else if ((PIXEL_FORMAT_YUYV_PACKAGE_422 == enPixelFormat) ||
                   (PIXEL_FORMAT_YVYU_PACKAGE_422 == enPixelFormat) ||
                   (PIXEL_FORMAT_UYVY_PACKAGE_422 == enPixelFormat) ||
                   (PIXEL_FORMAT_VYUY_PACKAGE_422 == enPixelFormat) ||
                   (PIXEL_FORMAT_YYUV_PACKAGE_422 == enPixelFormat) ||
                   (PIXEL_FORMAT_YYVU_PACKAGE_422 == enPixelFormat) ||
                   (PIXEL_FORMAT_UVYY_PACKAGE_422 == enPixelFormat) ||
                   (PIXEL_FORMAT_VUYY_PACKAGE_422 == enPixelFormat) ||
                   (PIXEL_FORMAT_VY1UY0_PACKAGE_422 == enPixelFormat)) {
            u32MainStride = ALIGN_UP((u32Width * u32BitWidth + 7) >> 3, u32Align) * 2;
            u32MainSize = u32MainStride * u32AlignHeight;
        } else {
            u32MainSize = u32MainStride * u32AlignHeight * 3;
        }

        u32VBSize = u32MainSize;
    } else {
        GK_U32 u32CmpRatioLuma = 1450;
        GK_U32 u32CmpRatioChroma = 1800;
        GK_U32 u32CSizeFor420;

        u32HeadStride = 16;
        u32HeadYSize = u32HeadStride * u32AlignHeight;

#ifndef __LITEOS__
#ifdef __KERNEL__
        u32YSize = osal_div64_u64(u32Width * u32AlignHeight * 1000ULL, u32CmpRatioLuma);
#else
        u32YSize = u32Width * u32AlignHeight * 1000ULL / u32CmpRatioLuma;
#endif
#else
        u32YSize = u32Width * u32AlignHeight * 1000ULL / u32CmpRatioLuma;
#endif
        u32YSize = ALIGN_UP(u32YSize, DEFAULT_ALIGN);

#ifndef __LITEOS__
#ifdef __KERNEL__
        u32CSizeFor420 = osal_div64_u64(u32Width * u32AlignHeight * 1000ULL, u32CmpRatioChroma * 2);
#else
        u32CSizeFor420 = (u32Width * u32AlignHeight * 1000ULL) / (u32CmpRatioChroma*2);
#endif
#else
        u32CSizeFor420 = (u32Width * u32AlignHeight * 1000ULL) / (u32CmpRatioChroma*2);
#endif
        u32CSizeFor420 = ALIGN_UP(u32CSizeFor420, DEFAULT_ALIGN);

        if ((PIXEL_FORMAT_YVU_SEMIPLANAR_420 == enPixelFormat) ||
            (PIXEL_FORMAT_YUV_SEMIPLANAR_420 == enPixelFormat)) {
            u32HeadSize = u32HeadYSize + u32HeadYSize / 2;
            u32MainSize = u32YSize + u32CSizeFor420;
        } else if (PIXEL_FORMAT_YVU_SEMIPLANAR_422 == enPixelFormat ||
                   PIXEL_FORMAT_YUV_SEMIPLANAR_422 == enPixelFormat) {
            u32HeadSize = u32HeadYSize * 2;
            u32MainSize = u32YSize + (u32CSizeFor420 * 2);
        } else if (enPixelFormat == PIXEL_FORMAT_YUV_400) {
            u32HeadSize = u32HeadYSize;
            u32MainSize = u32YSize;
        } else {
            u32HeadSize = u32HeadYSize * 3;
            u32MainSize = u32YSize + (u32CSizeFor420 * 2) * 2;
        }

        if (u32Width <= VPSS_LINE_BUFFER) {
            u32HeadSize = 64 + ALIGN_UP(u32HeadSize, u32Align);
            u32VBSize = u32HeadSize + u32MainSize;
        } else {
            u32HeadSize = (64 + ALIGN_UP(u32HeadSize, u32Align)) * 2;
            u32VBSize = u32HeadSize + u32MainSize + 2 * DEFAULT_ALIGN;
        }
    }

    pstCalConfig->u32VBSize = u32VBSize;
    pstCalConfig->u32HeadStride = u32HeadStride;
    pstCalConfig->u32HeadYSize = u32HeadYSize;
    pstCalConfig->u32HeadSize = u32HeadSize;
    pstCalConfig->u32MainStride = u32MainStride;
    pstCalConfig->u32MainYSize = u32YSize;
    pstCalConfig->u32MainSize = u32MainSize;
    pstCalConfig->u32ExtStride = 0;
    pstCalConfig->u32ExtYSize = 0;

    return;
}

__inline static GK_U32 COMMON_GetPicBufferSize(GK_U32 u32Width, GK_U32 u32Height,
    PIXEL_FORMAT_E enPixelFormat, DATA_BITWIDTH_E enBitWidth, COMPRESS_MODE_E enCmpMode, GK_U32 u32Align)
{
    VB_CAL_CONFIG_S stCalConfig;

    COMMON_GetPicBufferConfig(u32Width, u32Height, enPixelFormat, enBitWidth, enCmpMode, u32Align, &stCalConfig);

    return stCalConfig.u32VBSize;
}

__inline static GK_U32 VPSS_GetWrapBufferSize(GK_U32 u32Width, GK_U32 u32Height, GK_U32 u32BufLine,
    PIXEL_FORMAT_E enPixelFormat, DATA_BITWIDTH_E enBitWidth, COMPRESS_MODE_E enCmpMode, GK_U32 u32Align)
{
    VB_CAL_CONFIG_S stCalConfig;

    if (u32Align == 0) {
        u32Align = DEFAULT_ALIGN;
    } else if (u32Align > MAX_ALIGN) {
        u32Align = MAX_ALIGN;
    } else {
        u32Align = (ALIGN_UP(u32Align, DEFAULT_ALIGN));
    }

    if ((u32BufLine != 0) && (u32BufLine < u32Height)) {
        COMMON_GetPicBufferConfig(u32Width, u32BufLine, enPixelFormat, enBitWidth, enCmpMode, u32Align, &stCalConfig);

        if(enCmpMode == COMPRESS_MODE_SEG){
            stCalConfig.u32HeadYSize = stCalConfig.u32HeadStride * ALIGN_UP(u32Height, 2);
            if ((PIXEL_FORMAT_YVU_SEMIPLANAR_420 == enPixelFormat) || (PIXEL_FORMAT_YUV_SEMIPLANAR_420 == enPixelFormat)) {
                stCalConfig.u32HeadSize = stCalConfig.u32HeadYSize + stCalConfig.u32HeadYSize / 2;
            } else if (PIXEL_FORMAT_YVU_SEMIPLANAR_422 == enPixelFormat ||
                       PIXEL_FORMAT_YUV_SEMIPLANAR_422 == enPixelFormat) {
                stCalConfig.u32HeadSize = stCalConfig.u32HeadYSize * 2;
            } else if (enPixelFormat == PIXEL_FORMAT_YUV_400) {
                stCalConfig.u32HeadSize = stCalConfig.u32HeadYSize;
            } else {
                stCalConfig.u32HeadSize = stCalConfig.u32HeadYSize * 3;
            }

            if (u32Width <= VPSS_LINE_BUFFER) {
                stCalConfig.u32HeadSize = 64 + ALIGN_UP(stCalConfig.u32HeadSize, u32Align);
                stCalConfig.u32VBSize = stCalConfig.u32HeadSize + stCalConfig.u32MainSize;
            } else {
                stCalConfig.u32HeadSize = (64 + ALIGN_UP(stCalConfig.u32HeadSize, u32Align)) * 2;
                stCalConfig.u32VBSize = stCalConfig.u32HeadSize + stCalConfig.u32MainSize + 2 * DEFAULT_ALIGN;
            }

                }
    } else {
        COMMON_GetPicBufferConfig(u32Width, u32Height, enPixelFormat, enBitWidth, enCmpMode, u32Align, &stCalConfig);
    }

    return stCalConfig.u32VBSize;
}

__inline static GK_U32 VI_GetRawBufferSize(GK_U32 u32Width, GK_U32 u32Height,
                                           PIXEL_FORMAT_E enPixelFormat, COMPRESS_MODE_E enCmpMode, GK_U32 u32Align)
{
    GK_U32 u32BitWidth;
    GK_U32 u32Size = 0;
    GK_U32 u32Stride = 0;
    GK_U32 u32CmpRatioLine = 1600;
    GK_U32 u32CmpRatioFrame = 2000;

    if ((u32Width > SVP_MAXINUM_LIMIT) || (u32Height > SVP_MAXINUM_LIMIT)) {
        return 0;
    }

    if (u32Align == 0) {
        u32Align = DEFAULT_ALIGN;
    } else if (u32Align > MAX_ALIGN) {
        u32Align = MAX_ALIGN;
    } else {
        u32Align = (ALIGN_UP(u32Align, DEFAULT_ALIGN));
    }

    switch (enPixelFormat) {
        case PIXEL_FORMAT_RGB_BAYER_8BPP: {
            u32BitWidth = 8;
            break;
        }

        case PIXEL_FORMAT_RGB_BAYER_10BPP: {
            u32BitWidth = 10;
            break;
        }

        case PIXEL_FORMAT_RGB_BAYER_12BPP: {
            u32BitWidth = 12;
            break;
        }

        case PIXEL_FORMAT_RGB_BAYER_14BPP: {
            u32BitWidth = 14;
            break;
        }

        case PIXEL_FORMAT_RGB_BAYER_16BPP: {
            u32BitWidth = 16;
            break;
        }

        default:
        {
            u32BitWidth = 0;
            break;
        }
    }

    if (enCmpMode == COMPRESS_MODE_NONE) {
        u32Stride = ALIGN_UP(ALIGN_UP(u32Width * u32BitWidth, 8) / 8, u32Align);
        u32Size = u32Stride * u32Height;
    } else if (enCmpMode == COMPRESS_MODE_LINE) {
        GK_U32 u32Tmp;
        u32Tmp = ALIGN_UP((16 + u32Width * u32BitWidth * 1000UL / u32CmpRatioLine + 8192 + 127) / 128, 2);
        u32Stride = ALIGN_UP(u32Tmp * 16, u32Align);
        u32Size = u32Stride * u32Height;
    } else if (enCmpMode == COMPRESS_MODE_FRAME) {
        u32Size = ALIGN_UP(u32Height * u32Width * u32BitWidth * 1000UL / (u32CmpRatioFrame * 8), u32Align);
    }

    return u32Size;
}

__inline static GK_U32 VENC_GetRefPicInfoBufferSize(PAYLOAD_TYPE_E enType, GK_U32 u32Width, GK_U32 u32Height,
                                                    GK_U32 u32Align)
{
    GK_U32 u32Size;
    GK_U32 u32AlignWidth, u32AlignHeight;
    GK_U32 u32TmvSize, u32PmeSize, u32PmeInfoSize;

    if ((u32Width > SVP_MAXINUM_LIMIT) || (u32Height > SVP_MAXINUM_LIMIT)) {
        return 0;
    }

    if (enType == PT_H265) {
        u32AlignWidth = ALIGN_UP(u32Width, 32) >> 5;
        u32AlignHeight = ALIGN_UP(u32Height, 32) >> 5;

        u32TmvSize = ALIGN_UP(u32AlignWidth * u32AlignHeight, 2) * 16;
        u32PmeSize = (u32AlignWidth << 3) * (u32AlignHeight << 3);

        u32AlignWidth = ALIGN_UP(u32Width, 512) >> 9;
        u32AlignHeight = ALIGN_UP(u32Height, 32) >> 5;
        u32PmeInfoSize = (u32AlignWidth * u32AlignHeight) << 3;

        u32Size = u32TmvSize + u32PmeSize + u32PmeInfoSize;

    } else if (enType == PT_H264) {
        u32TmvSize = 0;

        u32AlignWidth = ALIGN_UP(u32Width, 64) / 4;
        u32AlignHeight = ALIGN_UP(u32Height, 16) / 4;
        u32PmeSize = u32AlignWidth * u32AlignHeight;

        u32AlignWidth = ALIGN_UP(u32Width, 1024) >> 10;
        u32AlignHeight = ALIGN_UP(u32Height, 16) >> 4;
        u32PmeInfoSize = (u32AlignWidth * u32AlignHeight) << 3;

        u32Size = u32TmvSize + u32PmeSize + u32PmeInfoSize;
    } else {
        u32Size = 0;
    }

    return u32Size;
}

__inline static GK_U32 VENC_GetRefBufferSize(PAYLOAD_TYPE_E enType, GK_U32 u32Width, GK_U32 u32Height,
                                             DATA_BITWIDTH_E enBitWidth, GK_U32 u32Align)
{
    GK_U32 u32Size = 0;
    GK_U32 u32AlignWidth, u32AlignHeight, u32BitWidth;
    GK_U32 u32YHeaderSize, u32CHeaderSize, u32YSize, u32CSize;

    if ((u32Width > SVP_MAXINUM_LIMIT) || (u32Height > SVP_MAXINUM_LIMIT)) {
        return 0;
    }

    if (enBitWidth == DATA_BITWIDTH_8) {
        u32BitWidth = 8;
    } else {
        return 0;
    }

    if (enType == PT_H265) {
        u32AlignWidth = ALIGN_UP(u32Width, 128);
        u32AlignHeight = ALIGN_UP(u32Height, 32);

        u32YHeaderSize = ((u32AlignWidth >> 7) << 4) * ((u32AlignHeight >> 5) + 1);
        u32CHeaderSize = u32YHeaderSize;

        u32AlignWidth = ALIGN_UP(u32Width, 64);
        u32AlignHeight = ALIGN_UP(u32Height, 16);
        u32YSize = (u32AlignWidth * u32AlignHeight * u32BitWidth) >> 3;
        u32CSize = u32YSize >> 1;

        u32Size = u32YHeaderSize + u32CHeaderSize + u32YSize + u32CSize;
    } else if (enType == PT_H264) {
        u32AlignWidth = ALIGN_UP(u32Width, 256);
        u32AlignHeight = ALIGN_UP(u32Height, 16);
        u32YHeaderSize = ((u32AlignWidth >> 8) << 4) * ((u32AlignHeight >> 4) + 1);
        u32CHeaderSize = u32YHeaderSize;

        u32AlignWidth = ALIGN_UP(u32Width, 64);
        u32AlignHeight = ALIGN_UP(u32Height, 16);
        u32YSize = u32AlignWidth * u32AlignHeight;
        u32CSize = u32YSize >> 1;

        u32Size = u32YHeaderSize + u32CHeaderSize + u32YSize + u32CSize;
    } else {
        u32Size = 0;
    }

    return u32Size;
}

__inline static GK_U32 VENC_GetQpmapSizeStride(GK_U32 u32Width)
{
    if (u32Width > SVP_MAXINUM_LIMIT) {
        return 0;
    }

    return DIV_UP(u32Width, 128) * 8;
}

__inline static GK_U32 VENC_GetQpmapSize(GK_U32 u32Width, GK_U32 u32Height)
{
    GK_U32 u32Stride, u32AlignHeight;

    if ((u32Width > SVP_MAXINUM_LIMIT) || (u32Height > SVP_MAXINUM_LIMIT)) {
        return 0;
    }

    u32Stride = VENC_GetQpmapSizeStride(u32Width);
    u32AlignHeight = DIV_UP(u32Height, 16);

    return u32Stride * u32AlignHeight;
}

__inline static GK_U32 VENC_GetSkipWeightSizeStride(PAYLOAD_TYPE_E enType, GK_U32 u32Width)
{
    GK_U32 u32Stride;

    if (u32Width > SVP_MAXINUM_LIMIT) {
        return 0;
    }

    if (enType == PT_H265) {
        u32Stride = DIV_UP(u32Width, 512) * 8;
    } else if (enType == PT_H264) {
        u32Stride = DIV_UP(u32Width, 256) * 8;
    } else {
        u32Stride = 0;
    }

    return u32Stride;
}
__inline static GK_U32 VENC_GetSkipWeightSize(PAYLOAD_TYPE_E enType, GK_U32 u32Width, GK_U32 u32Height)
{
    GK_U32 u32Stride, u32AlignHeight;

    if ((u32Width > SVP_MAXINUM_LIMIT) || (u32Height > SVP_MAXINUM_LIMIT)) {
        return 0;
    }

    u32Stride = VENC_GetSkipWeightSizeStride(enType, u32Width);

    if (enType == PT_H265) {
        u32AlignHeight = DIV_UP(u32Height, 32);
    } else if (enType == PT_H264) {
        u32AlignHeight = DIV_UP(u32Height, 16);
    } else {
        u32AlignHeight = 0;
    }

    return u32Stride * u32AlignHeight;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

