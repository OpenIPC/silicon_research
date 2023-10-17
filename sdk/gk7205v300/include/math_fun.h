/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __MATH_H__
#define __MATH_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ABS(x) ((x) >= 0 ? (x) : (-(x)))
#define _SIGN(x) ((x) >= 0 ? 1 : -1)
#define CMP(x, y) (((x) == (y)) ? 0 : (((x) > (y)) ? 1 : -1))

#define MAX2(x, y) ((x) > (y) ? (x) : (y))
#define MIN2(x, y) ((x) < (y) ? (x) : (y))
#define MAX3(x, y, z) ((x) > (y) ? MAX2(x, z) : MAX2(y, z))
#define MIN3(x, y, z) ((x) < (y) ? MIN2(x, z) : MIN2(y, z))
#define MEDIAN(x, y, z) (((x) + (y) + (z)-MAX3(x, y, z)) - MIN3(x, y, z))
#define MEAN2(x, y) (((x) + (y)) >> 1)

#define CLIP_MIN(x, min) (((x) >= min) ? (x) : min)
#define CLIP3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define CLIP_MAX(x, max) ((x) > (max) ? (max) : (x))
#define WRAP_MAX(x, max, min) ((x) >= (max) ? (min) : (x))
#define WRAP_MIN(x, min, max) ((x) <= (min) ? (max) : (x))
#define VALUE_BETWEEN(x, min, max) (((x) >= (min)) && ((x) <= (max)))

#define MULTI_OF_2_POWER(x, a) (!((x) & ((a)-1)))
#define CEILING(x, a) (((x) + (a)-1) / (a))

#define ALIGN_UP(x, a) ((((x) + ((a)-1)) / a) * a)
#define ALIGN_DOWN(x, a) (((x) / (a)) * (a))

#define DIV_UP(x, a) (((x) + ((a)-1)) / a)

#define MPP_ALIGN_UP(x, a) ALIGN_UP(x, a)
#define MPP_ALIGN_DOWN(x, a) ALIGN_DOWN(x, a)

/******************************************************************************
** Get the span between two unsinged number, such as
** SPAN(GK_U32, 100, 200) is 200 - 100 = 100
** SPAN(GK_U32, 200, 100) is 0xFFFFFFFF - 200 + 100
** SPAN(GK_U64, 200, 100) is 0xFFFFFFFFFFFFFFFF - 200 + 100
******************************************************************************/
#define SPAN(type, begin, end)                                         \
	({                                                             \
		type b = (begin);                                      \
		type e = (end);                                        \
		(type)((b >= e) ? (b - e) : (b + ((~((type)0)) - e))); \
	})

#define ENDIAN32(x)                                                        \
	(((x) << 24) | (((x)&0x0000ff00) << 8) | (((x)&0x00ff0000) >> 8) | \
	 (((x) >> 24) & 0x000000ff))

#define ENDIAN16(x) ((((x) << 8) & 0xff00) | (((x) >> 8) & 255))

__inline static GK_BOOL IS_LITTLE_END(void)
{
	union unEND_TEST_U {
		GK_CHAR cTest[4];
		GK_U32 u32Test;
	} unEndTest;

	unEndTest.cTest[0] = 0x01;
	unEndTest.cTest[1] = 0x02;
	unEndTest.cTest[2] = 0x03;
	unEndTest.cTest[3] = 0x04;

	return (unEndTest.u32Test > 0x01020304) ? (GK_TRUE) : (GK_FALSE);
}

#define FRACTION32(de, nu) (((de) << 16) | (nu))
#define NUMERATOR32(x) ((x)&0xffff)
#define DENOMINATOR32(x) ((x) >> 16)

#define RGB(r, g, b) ((((r)&0xff) << 16) | (((g)&0xff) << 8) | ((b)&0xff))
#define RGB_R(c) (((c)&0xff0000) >> 16)
#define RGB_G(c) (((c)&0xff00) >> 8)
#define RGB_B(c) ((c)&0xff)

#define YUV(y, u, v) \
	((((y)&0x03ff) << 20) | (((u)&0x03ff) << 10) | ((v)&0x03ff))
#define YUV_Y(c) (((c)&0x3ff00000) >> 20)
#define YUV_U(c) (((c)&0x000ffc00) >> 10)
#define YUV_V(c) ((c)&0x000003ff)

#define YUV_8BIT(y, u, v) ((((y)&0xff) << 16) | (((u)&0xff) << 8) | ((v)&0xff))
#define YUV_8BIT_Y(c) (((c)&0xff0000) >> 16)
#define YUV_8BIT_U(c) (((c)&0xff00) >> 8)
#define YUV_8BIT_V(c) ((c)&0xff)

__inline static GK_VOID Rgb2Yc(GK_U16 r, GK_U16 g, GK_U16 b, GK_U16 *py,
			       GK_U16 *pcb, GK_U16 *pcr)
{
	*py = (GK_U16)((((r * 66 + g * 129 + b * 25) >> 8) + 16) << 2);

	*pcb = (GK_U16)(((((b * 112 - r * 38) - g * 74) >> 8) + 128) << 2);

	*pcr = (GK_U16)(((((r * 112 - g * 94) - b * 18) >> 8) + 128) << 2);
}

__inline static GK_U32 Rgb2Yuv(GK_U32 u32Rgb)
{
	GK_U16 y, u, v;

	Rgb2Yc(RGB_R(u32Rgb), RGB_G(u32Rgb), RGB_B(u32Rgb), &y, &u, &v);

	return YUV(y, u, v);
}

__inline static GK_VOID Rgb2Yc_full(GK_U16 r, GK_U16 g, GK_U16 b, GK_U16 *py,
				    GK_U16 *pcb, GK_U16 *pcr)
{
	GK_U16 py_temp, pcb_temp, pcr_temp;

	py_temp = (GK_U16)(((r * 76 + g * 150 + b * 29) >> 8) * 4);
	pcb_temp =
		(GK_U16)(CLIP_MIN(((((b * 130 - r * 44) - g * 86) >> 8) + 128),
				  0) *
			 4);
	pcr_temp =
		(GK_U16)(CLIP_MIN(((((r * 130 - g * 109) - b * 21) >> 8) + 128),
				  0) *
			 4);

	*py = MAX2(MIN2(py_temp, 1023), 0);
	*pcb = MAX2(MIN2(pcb_temp, 1023), 0);
	*pcr = MAX2(MIN2(pcr_temp, 1023), 0);
}

__inline static GK_U32 Rgb2Yuv_full(GK_U32 u32Rgb)
{
	GK_U16 y, u, v;

	Rgb2Yc_full(RGB_R(u32Rgb), RGB_G(u32Rgb), RGB_B(u32Rgb), &y, &u, &v);

	return YUV(y, u, v);
}

__inline static GK_VOID Rgb2Yc_8BIT(GK_U8 r, GK_U8 g, GK_U8 b, GK_U8 *py,
				    GK_U8 *pcb, GK_U8 *pcr)
{
	*py = (GK_U8)(((r * 66 + g * 129 + b * 25) >> 8) + 16);

	*pcb = (GK_U8)((((b * 112 - r * 38) - g * 74) >> 8) + 128);

	*pcr = (GK_U8)((((r * 112 - g * 94) - b * 18) >> 8) + 128);
}

__inline static GK_U32 Rgb2Yuv_8BIT(GK_U32 u32Rgb)
{
	GK_U8 y, u, v;

	Rgb2Yc_8BIT(RGB_R(u32Rgb), RGB_G(u32Rgb), RGB_B(u32Rgb), &y, &u, &v);

	return YUV_8BIT(y, u, v);
}

__inline static GK_VOID Rgb2Yc_full_8BIT(GK_U8 r, GK_U8 g, GK_U8 b, GK_U8 *py,
					 GK_U8 *pcb, GK_U8 *pcr)
{
	GK_S16 py_temp, pcb_temp, pcr_temp;

	py_temp = (r * 76 + g * 150 + b * 29) >> 8;
	pcb_temp = (((b * 130 - r * 44) - g * 86) >> 8) + 128;
	pcr_temp = (((r * 130 - g * 109) - b * 21) >> 8) + 128;

	*py = MAX2(MIN2(py_temp, 255), 0);
	*pcb = MAX2(MIN2(pcb_temp, 255), 0);
	*pcr = MAX2(MIN2(pcr_temp, 255), 0);
}

__inline static GK_U32 Rgb2Yuv_full_8BIT(GK_U32 u32Rgb)
{
	GK_U8 y, u, v;

	Rgb2Yc_full_8BIT(RGB_R(u32Rgb), RGB_G(u32Rgb), RGB_B(u32Rgb), &y, &u,
			 &v);

	return YUV_8BIT(y, u, v);
}

typedef struct FPS_CTRL_S {
	GK_U32 u32Ffps;
	GK_U32 u32Tfps;
	GK_U32 u32FrmKey;
} FPS_CTRL_S;

__inline static GK_VOID InitFps(FPS_CTRL_S *pFrmCtrl, GK_U32 u32FullFps,
				GK_U32 u32TagFps)
{
	pFrmCtrl->u32Ffps = u32FullFps;
	pFrmCtrl->u32Tfps = u32TagFps;
	pFrmCtrl->u32FrmKey = 0;
}

__inline static GK_BOOL FpsControl(FPS_CTRL_S *pFrmCtrl)
{
	GK_BOOL bReturn = GK_FALSE;

	pFrmCtrl->u32FrmKey += pFrmCtrl->u32Tfps;
	if (pFrmCtrl->u32FrmKey >= pFrmCtrl->u32Ffps) {
		pFrmCtrl->u32FrmKey -= pFrmCtrl->u32Ffps;
		bReturn = GK_TRUE;
	}

	return bReturn;
}

__inline static GK_U32 GetLowAddr(GK_U64 u64Phyaddr)
{
	return (GK_U32)u64Phyaddr;
}

__inline static GK_U32 GetHighAddr(GK_U64 u64Phyaddr)
{
	return (GK_U32)(u64Phyaddr >> 32);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
