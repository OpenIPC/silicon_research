#include "main.h"

#define SNS_WIDTH   2592
#define SNS_HEIGHT  1520


combo_dev_attr_t MIPI_Config = {
    .devno      = 0,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate  = MIPI_DATA_RATE_X1,
    .img_rect   = {12, 14, SNS_WIDTH, SNS_HEIGHT},
    {
      .mipi_attr = {
        DATA_TYPE_RAW_10BIT,
        HI_MIPI_WDR_MODE_VC,
        {0, 1, 2, 3}
      }
    }
};

VI_DEV_ATTR_S SNS_Config = {
  VI_MODE_MIPI,
  VI_WORK_MODE_1Multiplex,
  {0xFFF00000,    0x0},
  VI_SCAN_PROGRESSIVE,
  { -1, -1, -1, -1},
  VI_DATA_SEQ_YUYV,
  {
    /*port_vsync   port_vsync_neg     port_hsync        port_hsync_neg        */
    VI_VSYNC_PULSE, VI_VSYNC_NEG_LOW, VI_HSYNC_VALID_SINGNAL, VI_HSYNC_NEG_HIGH, VI_VSYNC_VALID_SINGAL, VI_VSYNC_VALID_NEG_HIGH,

    /*hsync_hfb    hsync_act    hsync_hhb*/
    {
        0,            1280,        0,
        /*vsync0_vhb vsync0_act vsync0_hhb*/
        0,            720,        0,
        /*vsync1_vhb vsync1_act vsync1_hhb*/
        0,            0,            0
    }
  },
  VI_DATA_TYPE_RGB,
  HI_FALSE,
  {SNS_WIDTH , SNS_HEIGHT},
  {
    {
        {SNS_WIDTH , SNS_HEIGHT},
    },
    {
        VI_REPHASE_MODE_NONE,
        VI_REPHASE_MODE_NONE
    }
  },
  {
      WDR_MODE_NONE,
      SNS_HEIGHT
  },
  DATA_RATE_X1
};

VI_PIPE_ATTR_S PIPE_Config = {
    VI_PIPE_BYPASS_NONE, HI_FALSE, HI_FALSE,
    SNS_WIDTH, SNS_HEIGHT,
    PIXEL_FORMAT_RGB_BAYER_10BPP,
    COMPRESS_MODE_NONE,
    DATA_BITWIDTH_10,
    HI_FALSE,
    {
        PIXEL_FORMAT_YVU_SEMIPLANAR_420,
        DATA_BITWIDTH_8,
        VI_NR_REF_FROM_RFR,
        COMPRESS_MODE_NONE
    },
    HI_FALSE,
    { -1, -1}
};

VI_CHN_ATTR_S CHN_Config = {
    {SNS_WIDTH, SNS_HEIGHT},
    PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    DYNAMIC_RANGE_SDR8,
    VIDEO_FORMAT_LINEAR,
    COMPRESS_MODE_NONE,
    0,      0,
    0,
    { -1, -1}
};

ISP_PUB_ATTR_S ISP_Config = {
    {0, 0, SNS_WIDTH, SNS_HEIGHT},
    {SNS_WIDTH, SNS_HEIGHT},
    30,
    BAYER_RGGB,
    WDR_MODE_NONE,
    0,
};