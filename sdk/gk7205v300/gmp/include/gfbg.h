/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __GFBG_H__
#define __GFBG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "type.h"
#include "common.h"
#ifdef __LITEOS__
#include <fb.h>
#include <linux/wait.h>
#include "stdlib.h"
#include <linux/workqueue.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include "liteos/fb.h"


#else
#include <linux/fb.h>
#endif

/*************************** Structure Definition ****************************/

#define IOC_TYPE_GFBG       'F'
/** To obtain the colorkey of an overlay layer */
#define FBIOGET_COLORKEY_GFBG       _IOR(IOC_TYPE_GFBG, 90, GFBG_COLORKEY_S)
/** To set the colorkey of an overlay layer */
#define FBIOPUT_COLORKEY_GFBG       _IOW(IOC_TYPE_GFBG, 91, GFBG_COLORKEY_S)
/** To get the alpha of an overlay layer */
#define FBIOGET_ALPHA_GFBG          _IOR(IOC_TYPE_GFBG, 92, GFBG_ALPHA_S)
/** To set the alpha of an overlay layer */
#define FBIOPUT_ALPHA_GFBG          _IOW(IOC_TYPE_GFBG, 93, GFBG_ALPHA_S)
/** To get the origin of an overlay layer on the screen */
#define FBIOGET_SCREEN_ORIGIN_GFBG  _IOR(IOC_TYPE_GFBG, 94, GFBG_POINT_S)
/** To set the origin of an overlay layer on the screen */
#define FBIOPUT_SCREEN_ORIGIN_GFBG  _IOW(IOC_TYPE_GFBG, 95, GFBG_POINT_S)
/** To obtain the anti-flicker setting of an overlay layer */
#define FBIOGET_DEFLICKER_GFBG       _IOR(IOC_TYPE_GFBG, 98, GFBG_DEFLICKER_S)
/** To set the anti-flicker setting of an overlay layer */
#define FBIOPUT_DEFLICKER_GFBG       _IOW(IOC_TYPE_GFBG, 99, GFBG_DEFLICKER_S)
/** To wait for the vertical blanking region of an overlay layer */
#define FBIOGET_VBLANK_GFBG         _IO(IOC_TYPE_GFBG, 100)
/** To set the display state of an overlay layer */
#define FBIOPUT_SHOW_GFBG           _IOW(IOC_TYPE_GFBG, 101, GK_BOOL)
/** To obtain the display state of an overlay layer */
#define FBIOGET_SHOW_GFBG           _IOR(IOC_TYPE_GFBG, 102, GK_BOOL)
/** to obtain the capability of an overlay layer */
#define FBIOGET_CAPABILITY_GFBG    _IOR(IOC_TYPE_GFBG, 103, GFBG_CAPABILITY_S)
/** set the screen output size */
#define FBIOPUT_SCREENSIZE          _IOW(IOC_TYPE_GFBG, 130, GFBG_SIZE_S)
/** get the screen output size */
#define FBIOGET_SCREENSIZE          _IOR(IOC_TYPE_GFBG, 131, GFBG_SIZE_S)

/** To display multiple surfaces in turn and set the alpha and colorkey attributes */
#define FBIOFLIP_SURFACE            _IOW(IOC_TYPE_GFBG, 132, GFBG_SURFACEEX_S)

/**To set the compression function status of an overlay layer*/
#define FBIOPUT_COMPRESSION_GFBG    _IOW(IOC_TYPE_GFBG, 133, GK_BOOL)
/**To obtain the compression function status of an overlay layer*/
#define FBIOGET_COMPRESSION_GFBG    _IOR(IOC_TYPE_GFBG, 134, GK_BOOL)

/**To set the hdr function status of an overlay layer*/
#define FBIOPUT_DYNAMIC_RANGE_GFBG  _IOW(IOC_TYPE_GFBG, 139, GFBG_DYNAMIC_RANGE_E)
/**To get the hdr function status of an overlay layer*/
#define FBIOGET_DYNAMIC_RANGE_GFBG  _IOR(IOC_TYPE_GFBG, 140, GFBG_DYNAMIC_RANGE_E)

/**To release the layer*/
#define FBIO_RELEASE_GFBG           _IO(IOC_TYPE_GFBG, 150)

typedef struct
{
    GK_U32  u32Width;
    GK_U32  u32Height;
}GFBG_SIZE_S;
#ifndef __LITEOS__
static inline GK_U8  gfbg_rgb(const struct fb_bitfield* pBit, GK_S32 color)
{
    return ((GK_U8)((((GK_U32)color)>>pBit->offset) << (8-pBit->length)) +
             ((GK_U8)(((GK_U32)(color)>>pBit->offset) << (8-pBit->length)) >> pBit->length));
}

static inline GK_S32 gfbg_color2key(const struct fb_var_screeninfo* pVar, GK_S32 color)
{
   if (pVar->bits_per_pixel <= 8)
   {
       return color;
   }
   else
   {
      GK_U8 r, g, b;
      r = gfbg_rgb(&pVar->red, color);
      g = gfbg_rgb(&pVar->green, color);
      b = gfbg_rgb(&pVar->blue, color);
      return (r<<16) + (g<<8) + b;
   }
}
#endif
typedef enum fbDYNAMIC_RANGE_E
{
    GFBG_DYNAMIC_RANGE_SDR8 = 0,
    GFBG_DYNAMIC_RANGE_SDR10,
    GFBG_DYNAMIC_RANGE_HDR10,
    GFBG_DYNAMIC_RANGE_HLG,
    GFBG_DYNAMIC_RANGE_SLF,
    GFBG_DYNAMIC_RANGE_BUTT
} GFBG_DYNAMIC_RANGE_E;


typedef struct
{
    GK_BOOL bKeyEnable;         /* colorkey enable flag */
    GK_U32 u32Key;              /* colorkey value, maybe contains alpha */
}GFBG_COLORKEY_S;

typedef struct
{
    GK_S32 x;
    GK_S32 y;
    GK_S32 w;
    GK_S32 h;
} GFBG_RECT;

typedef struct
{
    GK_S32 s32XPos;         /**<  horizontal position */
    GK_S32 s32YPos;         /**<  vertical position */
}GFBG_POINT_S;

typedef struct GFBG_DEFLICKER_S
{
    GK_U32  u32HDfLevel;    /**<  horizontal deflicker level */
    GK_U32  u32VDfLevel;    /**<  vertical deflicker level */
    GK_U8   ATTRIBUTE *pu8HDfCoef;    /**<  horizontal deflicker coefficient */
    GK_U8   ATTRIBUTE *pu8VDfCoef;    /**<  vertical deflicker coefficient */
}GFBG_DEFLICKER_S;

/** Alpha info */
typedef struct
{
    GK_BOOL bAlphaEnable;   /**<  alpha enable flag */
    GK_BOOL bAlphaChannel;  /**<  alpha channel enable flag */
    GK_U8 u8Alpha0;         /**<  alpha0 value, used in ARGB1555 */
    GK_U8 u8Alpha1;         /**<  alpha1 value, used in ARGB1555 */
    GK_U8 u8GlobalAlpha;    /**<  global alpha value */
    GK_U8 u8Reserved;
}GFBG_ALPHA_S;

typedef enum
{
    GFBG_FMT_RGB565 = 0,
    GFBG_FMT_RGB888,              /**<  RGB888 24bpp */

    GFBG_FMT_KRGB444,       /**<  RGB444 16bpp */
    GFBG_FMT_KRGB555,       /**<  RGB555 16bpp */
    GFBG_FMT_KRGB888,       /**<  RGB888 32bpp */

    GFBG_FMT_ARGB4444,      /**< ARGB4444 */
    GFBG_FMT_ARGB1555,      /**< ARGB1555 */
    GFBG_FMT_ARGB8888,      /**< ARGB8888 */
    GFBG_FMT_ARGB8565,      /**< ARGB8565 */

    GFBG_FMT_RGBA4444,      /**< ARGB4444 */
    GFBG_FMT_RGBA5551,      /**< RGBA5551 */
    GFBG_FMT_RGBA5658,      /**< RGBA5658 */
    GFBG_FMT_RGBA8888,      /**< RGBA8888 */

    GFBG_FMT_BGR565,        /**< BGR565 */
    GFBG_FMT_BGR888,        /**< BGR888 */
    GFBG_FMT_ABGR4444,      /**< ABGR4444 */
    GFBG_FMT_ABGR1555,      /**< ABGR1555 */
    GFBG_FMT_ABGR8888,      /**< ABGR8888 */
    GFBG_FMT_ABGR8565,      /**< ABGR8565 */
    GFBG_FMT_KBGR444,       /**< BGR444 16bpp */
    GFBG_FMT_KBGR555,       /**< BGR555 16bpp */
    GFBG_FMT_KBGR888,       /**< BGR888 32bpp */

    GFBG_FMT_1BPP,          /**<  clut1 */
    GFBG_FMT_2BPP,          /**<  clut2 */
    GFBG_FMT_4BPP,          /**<  clut4 */
    GFBG_FMT_8BPP,          /**< clut8 */
    GFBG_FMT_ACLUT44,       /**< AClUT44*/
    GFBG_FMT_ACLUT88,         /**< ACLUT88 */
    GFBG_FMT_PUYVY,         /**< UYVY */
    GFBG_FMT_PYUYV,         /**< YUYV */
    GFBG_FMT_PYVYU,         /**< YVYU */
    GFBG_FMT_YUV888,        /**< YUV888 */
    GFBG_FMT_AYUV8888,      /**< AYUV8888 */
    GFBG_FMT_YUVA8888,      /**< YUVA8888 */
    GFBG_FMT_BUTT
}GFBG_COLOR_FMT_E;

typedef struct
{
    GK_BOOL bKeyRgb;
    GK_BOOL bKeyAlpha;      /**<  whether support colorkey alpha */
    GK_BOOL bGlobalAlpha;   /**<  whether support global alpha */
    GK_BOOL bCmap;          /**<  whether support color map */
    GK_BOOL bHasCmapReg;    /**<  whether has color map register*/
    GK_BOOL bColFmt[GFBG_FMT_BUTT]; /**<  support which color format */
    GK_BOOL bVoScale;       /**< support vo scale*/
    GK_BOOL bLayerSupported;    /**< whether support a certain layer, for example:x5 HD support GFBG_SD_0 not support GFBG_SD_1*/
    GK_U32  u32MaxWidth;    /**<  the max pixels per line */
    GK_U32  u32MaxHeight;   /**<  the max lines */
    GK_U32  u32MinWidth;    /**<  the min pixels per line */
    GK_U32  u32MinHeight;   /**<  the min lines */
    GK_U32  u32VDefLevel;   /**<  vertical deflicker level, 0 means vertical deflicker is unsupported */
    GK_U32  u32HDefLevel;   /**<  horizontal deflicker level, 0 means horizontal deflicker is unsupported  */
    GK_BOOL  bDcmp;
    GK_BOOL  bPreMul;
    GK_BOOL  bGHDR;         /* NEW Feature. Is GHDR supported. */
}GFBG_CAPABILITY_S;

/*refresh mode*/
typedef enum
{
    GFBG_LAYER_BUF_DOUBLE = 0x0,       /**<  2 display buf in fb */
    GFBG_LAYER_BUF_ONE    = 0x1,       /**<  1 display buf in fb */
    GFBG_LAYER_BUF_NONE   = 0x2,       /**<  no display buf in fb,the buf user refreshed will be directly set to VO*/
    GFBG_LAYER_BUF_DOUBLE_IMMEDIATE=0x3, /**< 2 display buf in fb, each refresh will be displayed*/
    GFBG_LAYER_BUF_FENCE = 0x4, /**<  2 display buf in fb with fence */
    GFBG_LAYER_BUF_BUTT
} GFBG_LAYER_BUF_E;

/* surface info */
typedef struct
{
    GK_U64  u64PhyAddr;     /**<  start physical address */
    GK_U64  u64GBPhyAddr;   // TODO new
    GK_U32  u32Width;       /**<  width pixels */
    GK_U32  u32Height;      /**<  height pixels */
    GK_U32  u32Pitch;       /**<  line pixels */
    GFBG_COLOR_FMT_E enFmt; /**<  color format */
    GFBG_DYNAMIC_RANGE_E  enDynamicRange;    /* NEW dynamic range. */
}GFBG_SURFACE_S;

typedef struct
{
    GK_U64 u64PhyAddr;
    GFBG_ALPHA_S stAlpha;
    GFBG_COLORKEY_S stColorkey;
}GFBG_SURFACEEX_S;

/* refresh surface info */
typedef struct
{
    GFBG_SURFACE_S stCanvas;
    GFBG_RECT UpdateRect;       /* refresh region*/
}GFBG_BUFFER_S;

/* cursor info */
typedef struct
{
    GFBG_SURFACE_S stCursor;
    GFBG_POINT_S stHotPos;
} GFBG_CURSOR_S;

/* DDR detect zone info */
typedef struct
{
    GK_U32 u32StartSection;
    GK_U32 u32ZoneNums;
} GFBG_DDRZONE_S;

/* crusor handle */
/* Attention:surface in cursor will be released by user*/
#define FBIOPUT_CURSOR_INFO           _IOW(IOC_TYPE_GFBG, 104, GFBG_CURSOR_S )
#define FBIOGET_CURSOR_INFO           _IOW(IOC_TYPE_GFBG, 105, GFBG_CURSOR_S )

#define FBIOPUT_CURSOR_STATE          _IOW(IOC_TYPE_GFBG, 106, GK_BOOL )
#define FBIOGET_CURSOR_STATE          _IOW(IOC_TYPE_GFBG, 107, GK_BOOL )

#define FBIOPUT_CURSOR_POS            _IOW(IOC_TYPE_GFBG, 108, GFBG_POINT_S )
#define FBIOGET_CURSOR_POS            _IOR(IOC_TYPE_GFBG, 109, GFBG_POINT_S )

#define FBIOPUT_CURSOR_COLORKEY       _IOR(IOC_TYPE_GFBG, 110, GFBG_COLORKEY_S )
#define FBIOGET_CURSOR_COLORKEY       _IOW(IOC_TYPE_GFBG, 111, GFBG_COLORKEY_S )
#define FBIOPUT_CURSOR_ALPHA          _IOR(IOC_TYPE_GFBG, 112, GFBG_ALPHA_S )
#define FBIOGET_CURSOR_ALPHA          _IOW(IOC_TYPE_GFBG, 113, GFBG_ALPHA_S )

/** cursor will be separated from attached layer automatically if you attach cursor to another layer,that means
cursor can be attached to only one layer at any time*/
#define FBIOPUT_CURSOR_ATTCHCURSOR    _IOW(IOC_TYPE_GFBG, 114, GK_U32 )
#define FBIOPUT_CURSOR_DETACHCURSOR   _IOW(IOC_TYPE_GFBG, 115, GK_U32 )

/**antiflicker level*/
/**Auto means fb will choose a appropriate antiflicker level automatically according to the color info of map*/
typedef enum
{
    GFBG_LAYER_ANTIFLICKER_NONE = 0x0,  /**< no antiflicker*/
    GFBG_LAYER_ANTIFLICKER_LOW = 0x1,   /**< low level*/
    GFBG_LAYER_ANTIFLICKER_MIDDLE = 0x2,/**< middle level*/
    GFBG_LAYER_ANTIFLICKER_HIGH = 0x3, /**< high level*/
    GFBG_LAYER_ANTIFLICKER_AUTO = 0x4, /**< auto*/
    GFBG_LAYER_ANTIFLICKER_BUTT
}GFBG_LAYER_ANTIFLICKER_LEVEL_E;

/* MIRROR mode  */
typedef enum
{
    GFBG_MIRROR_NONE = 0x0,
    GFBG_MIRROR_HORIZONTAL = 0x1,
    GFBG_MIRROR_VERTICAL = 0x2,
    GFBG_MIRROR_BOTH= 0x3,
    GFBG_MIRROR_BUTT
}GFBG_MIRROR_MODE_E;

/* ROTATE mode  */
typedef enum
{
    GFBG_ROTATE_NONE = 0x0,
    GFBG_ROTATE_90 = 0x1,
    GFBG_ROTATE_180 = 0x2,
    GFBG_ROTATE_270= 0x3,
    GFBG_ROTATE_BUTT
}GFBG_ROTATE_MODE_E;

/*layer info maskbit*/
typedef enum
{
    GFBG_LAYERMASK_BUFMODE = 0x1,           /**< BUFMODE bitmask */
    GFBG_LAYERMASK_ANTIFLICKER_MODE = 0x2,  /**< ANTIFLICKER_MODE bitmask */
    GFBG_LAYERMASK_POS = 0x4,               /**< the position bitmask */
    GFBG_LAYERMASK_CANVASSIZE = 0x8,      /**< canvassize bitmask */
    GFBG_LAYERMASK_DISPSIZE = 0x10,       /**< displaysize bitmask */
    GFBG_LAYERMASK_SCREENSIZE = 0x20,     /**< screensize bitmask */
    GFBG_LAYERMASK_BMUL = 0x40,           /**< pre-mult bitmask */
    GFBG_LAYERMASK_BUTT
}GFBG_LAYER_INFO_MASKBIT;

/**layer info*/
typedef struct
{
    GFBG_LAYER_BUF_E BufMode;
    GFBG_LAYER_ANTIFLICKER_LEVEL_E eAntiflickerLevel;
    GK_S32 s32XPos;           /**<  the x pos of origion point in screen */
    GK_S32 s32YPos;           /**<  the y pos of origion point in screen */
    GK_U32 u32CanvasWidth;    /**<  the width of canvas buffer */
    GK_U32 u32CanvasHeight;   /**<  the height of canvas buffer */
    GK_U32 u32DisplayWidth;   /**<  the width of display buf in fb.for 0 buf ,there is no display buf in fb, so it's effectless*/
    GK_U32 u32DisplayHeight;  /**<  the height of display buf in fb. */
    GK_U32 u32ScreenWidth;    /**<  the width of screen */
    GK_U32 u32ScreenHeight;   /**<  the height of screen */
    GK_BOOL bPreMul;          /**<  The data drawed in buf is premul data or not*/
    GK_U32 u32Mask;           /**<  param modify mask bit*/
}GFBG_LAYER_INFO_S;

/** To set the layer information */
#define FBIOPUT_LAYER_INFO                _IOW(IOC_TYPE_GFBG, 120, GFBG_LAYER_INFO_S)
/** To get the layer information */
#define FBIOGET_LAYER_INFO                _IOR(IOC_TYPE_GFBG, 121, GFBG_LAYER_INFO_S)
/** To get canvas buf */
#define FBIOGET_CANVAS_BUFFER             _IOR(IOC_TYPE_GFBG, 123, GFBG_BUFFER_S)
/** To refresh the displayed contents in extended mode */
#define FBIO_REFRESH                      _IOW(IOC_TYPE_GFBG, 124, GFBG_BUFFER_S)

/**sync refresh*/
#define FBIO_WAITFOR_FREFRESH_DONE        _IO(IOC_TYPE_GFBG, 125)

/* To set the mirror mode */
#define FBIOPUT_MIRROR_MODE            _IOW(IOC_TYPE_GFBG, 126, GFBG_MIRROR_MODE_E)
/* To get the mirror mode */
#define FBIOGET_MIRROR_MODE            _IOW(IOC_TYPE_GFBG, 127, GFBG_MIRROR_MODE_E)

/* To set the rotate mode */
#define FBIOPUT_ROTATE_MODE            _IOW(IOC_TYPE_GFBG, 128, GFBG_ROTATE_MODE_E)
/* To get the rotate mode */
#define FBIOGET_ROTATE_MODE            _IOW(IOC_TYPE_GFBG, 129, GFBG_ROTATE_MODE_E)

/**To set the DDR detect zone of an overlay layer*/
#define FBIOPUT_MDDRDETECT_GFBG    _IOW(IOC_TYPE_GFBG, 135, GFBG_DDRZONE_S)
/**To get the DDR detect zone of an overlay layer*/
#define FBIOGET_MDDRDETECT_GFBG    _IOW(IOC_TYPE_GFBG, 136, GFBG_DDRZONE_S)

#ifdef __LITEOS__
#define FBIOGET_SCREENINFO_GFBG 0x4600
#define FBIOPUT_SCREENINFO_GFBG 0x4601
#define FBIOPAN_DISPLAY_GFBG 0x4606

struct gfbg_info
{
    struct fb_vtable_s vtable; /* FB interfaces */
    struct fb_videoinfo_s vinfo;    /* This structure descrides the overall video controller */
    struct fb_overlayinfo_s oinfo;  /* This structure descrides one overlay */
#ifdef CONFIG_FB_CMAP
    struct fb_cmap_s cmap; /* Current camp */
#endif

    int activate;
    void *par; /* Private data */
};

extern GK_S32 gfbg_init(void* pArgs);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __GFBG_H__ */

