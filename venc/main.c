#include "main.h"
#include <stdbool.h>
#include <signal.h>
#include <time.h>

// Configuration profiles
extern combo_dev_attr_t MIPI_4lane_CHN0_SENSOR_IMX335_12BIT_4M_NOWDR_ATTR;
extern VI_DEV_ATTR_S DEV_ATTR_IMX335_4M_BASE;
extern VI_PIPE_ATTR_S PIPE_ATTR_RAW10_420_3DNR_RFR;
extern VI_CHN_ATTR_S CHN_ATTR_420_SDR8_LINEAR;
extern ISP_PUB_ATTR_S ISP_PROFILE_IMX335_MIPI_4M_30FPS;

extern combo_dev_attr_t MIPI_4lane_CHN0_SENSOR_IMX307_12BIT_2M_NOWDR_ATTR;
extern combo_dev_attr_t MIPI_4lane_CHN0_SENSOR_IMX307_12BIT_2M_WDR2to1_ATTR;
extern VI_DEV_ATTR_S DEV_ATTR_IMX307_2M_BASE;
extern VI_PIPE_ATTR_S PIPE_ATTR_RAW12_420_3DNR_RFR;
extern ISP_PUB_ATTR_S ISP_PROFILE_IMX307_MIPI_2M_30FPS;
extern ISP_PUB_ATTR_S ISP_PROFILE_IMX307_MIPI_2M_30FPS_WDR2TO1_LINE;

uint8_t* tx_buffer;
uint32_t tx_buffer_used = 0;

void printHelp() {
  printf(
    "\n\t\tOpenIPC FPV Streamer for HiSilicon/Goke (%s)\n"
    "\n"
    "  Usage:\n"
    "    venc [Arguments]\n"
    "\n"
    "  Arguments:\n"
    "    -v [Version]   - Camera version                  (Default: "
    "200_imx307)\n"
    "\n"
    "      GK7205v200 / IMX307\n"
    "        200_imx307B  - v200, IMX307, 2-lane MIPI | 720p  | any fps\n"
    "        200_imx307F  - v200, IMX307, 2-lane MIPI | 1080p | 30  fps only\n"
    "\n"
    "      GK7205v300 / IMX307\n"
    "        300_imx307B  - v300, IMX307, 4-lane MIPI | 720p  | any fps\n"
    "        300_imx307F  - v300, IMX307, 4-lane MIPI | 1080p | 30  fps only\n"
    "\n"
    "      GK7205v300 / IMX335\n"
    "        300_imx335F4 - v300, IMX335, 4-lane MIPI | 2592x1520  | 25  fps only\n"
    "        300_imx335F5 - v300, IMX335, 4-lane MIPI | 2592x1944  | 25  fps only\n"
    "        300_imx335B  - v300, IMX335, 4-lane MIPI | 1292x972   | 30 / 60  fps only\n"
    "\n"
    "    -h [IP]        - Sink IP address                 (Default: "
    "127.0.0.1)\n"
    "    -p [Port]      - Sink port                       (Default: 5000)\n"
    "    -r [Rate]      - Max video rate in Kbit/sec.     (Default: 8192)\n"
    "    -n [Size]      - Max payload frame size in bytes (Default: 1400)\n"
    "    -m [Mode]      - Streaming mode                  (Default: "
    "compact)\n"
    "       compact       - Compact UDP stream \n"
    "       rtp           - RTP stream\n"
    "\n"
    "    -s [Size]      - Encoded image size              (Default: "
    "version specific)\n"
    "\n"
    "      Standard resolutions\n"
    "        D1           - 720  x 480\n"
    "        960h         - 960  x 576\n"
    "        720p         - 1280 x 720\n"
    "        1.3MP        - 1280 x 1024\n"
    "        1080p        - 1920 x 1080\n"
    "        4MP          - 2592 x 1520\n"
    "\n"
    "      Custom resolution format\n"
    "        WxH          - Custom resolution W x H pixels\n"
    "\n"
    "    -f [FPS]       - Encoder FPS (25,30,50,60)       (Default: 60)\n"
    "    -g [Value]     - GOP denominator                 (Default: 10)\n"
    "    -c [Codec]     - Encoder mode                    (Default: "
    "264avbr)\n"
    "\n"
    "           --- H264 ---\n"
    "       264avbr       - h264 AVBR\n"
    "       264qvbr       - h264 QVBR\n"
    "       264vbr        - h264 VBR\n"
    "       264cbr        - h264 CBR\n"
    "\n"
    "           --- H265 ---\n"
    "       265avbr       - h265 AVBR\n"
    "       265qvbr       - h265 QVBR\n"
    "       265vbr        - h265 VBR\n"
    "       265cbr        - h265 CBR\n"
    "\n"
    "    -d [Format]    - Data format                       (Default: "
    "stream)\n"
    "      stream         - Produce NALUs in stream mode\n"
    "      frame          - Produce NALUs in packet mode\n"
    "\n"
    "    --no-slices          - Disable slices\n"
    "    --slice-size [size]  - Slices size in lines      (Default: 4)\n"
    "\n"
    "    --low-delay    - Enable low delay mode\n"
    "    --mirror       - Mirror image\n"
    "    --flip         - Flip image\n"
    "    --exp          - Limit exposure\n"
    "\n"
    "    --roi          - Enable ROI\n"
    "    --roi-qp [QP]  - ROI quality points              (Default: 20)\n"
    "\n", __DATE__
  );
}

uint8_t stream_mode = 0;
uint16_t goke_version = 200;
SensorType sensor_type = IMX307;
uint32_t sensor_width = 1280;
uint32_t sensor_height = 720;
uint32_t sensor_framerate = 60;
bool loop_running = true;

static void handler(int value) {
  loop_running = false;
}

int main(int argc, const char* argv[]) {
  if (argc == 2 && !strcmp(argv[1], "help")) {
    printHelp();
    return 1;
  }

  uint32_t mipi_device_id = 0;
  uint32_t mipi_sensor_id = 0;
  uint32_t isp_framerate = 45;
  
  uint32_t image_width = 1280; // Encoded image width
  uint32_t image_height = 720; // Encoded image height

  VI_DEV vi_dev_id = 0;
  VI_PIPE vi_pipe_id = 0;
  VI_CHN vi_channel_id = 0;

  uint32_t vi_vpss_mode = VI_ONLINE_VPSS_ONLINE;
  VPSS_GRP vpss_group_id = 0;
  VPSS_CHN vpss_first_ch_id = 0;
  VPSS_CHN vpss_second_ch_id = 1;

  uint32_t venc_gop_denom = 10;
  uint32_t venc_gop_size = sensor_framerate / venc_gop_denom;
  uint32_t venc_max_rate = 1024 * 8;

  VENC_CHN venc_first_ch_id = 0;
  VENC_CHN venc_second_ch_id = 1;
  HI_BOOL venc_by_frame = HI_FALSE;
  uint32_t venc_slice_size = 4;

  uint32_t udp_sink_ip = inet_addr("127.0.0.1");
  uint16_t udp_sink_port = 5000;
  uint16_t max_frame_size = 1400;

  int enable_slices = 1;
  int enable_lowdelay = 0;
  int enable_roi = 0;
  bool limit_exposure = false;
  int ret = 0;

  int image_mirror = HI_FALSE;
  int image_flip = HI_FALSE;
  uint16_t roi_qp = 20;

  PAYLOAD_TYPE_E rc_codec = PT_H264;
  int rc_mode = VENC_RC_MODE_H264AVBR;

  // Load console arguments
  __BeginParseConsoleArguments__(printHelp) __OnArgument("-h") {
    udp_sink_ip = inet_addr(__ArgValue);
    continue;
  }

  __OnArgument("-p") {
    udp_sink_port = atoi(__ArgValue);
    continue;
  }

  __OnArgument("-r") {
    venc_max_rate = atoi(__ArgValue);
    continue;
  }

  __OnArgument("-n") {
    max_frame_size = atoi(__ArgValue);
    continue;
  }

  __OnArgument("-m") {
    const char* value = __ArgValue;
    if (!strcmp(value, "compact")) {
      stream_mode = 0;
    } else if (!strcmp(value, "rtp")) {
      stream_mode = 1;
    } else {
      printf("> ERROR: Unknown streaming mode\n");
      exit(1);
    }
    continue;
  }

  __OnArgument("-v") {
    const char* value = __ArgValue;
    // v200 IMX307 720P
    if (!strcmp(value, "200_imx307B")) {
      goke_version = 200;
      sensor_type = IMX307;
      sensor_width = image_width = 1280;
      sensor_height = image_height = 720;
      isp_framerate = 45;
    // v200 IMX307 1080P
    } else if (!strcmp(value, "200_imx307F")) {
      goke_version = 200;
      sensor_type = IMX307;
      sensor_width = image_width = 1920;
      sensor_height = image_height = 1080;
      sensor_framerate = 30;
      isp_framerate = 30;
    // v300 IMX307 720P
    } else if (!strcmp(value, "300_imx307B")) {
      goke_version = 300;
      sensor_type = IMX307;
      sensor_width = image_width = 1280;
      sensor_height = image_height = 720;
      isp_framerate = 45;
    // v300 IMX307 1080P
    } else if (!strcmp(value, "300_imx307F")) {
      goke_version = 300;
      sensor_type = IMX307;
      sensor_width = image_width = 1920;
      sensor_height = image_height = 1080;
      sensor_framerate = 30;
      isp_framerate = 30;
      
    // - v300 IMX335 4MP
    } else if (!strcmp(value, "300_imx335F4")) {
      goke_version = 300;
      sensor_type = IMX335;
      sensor_width = image_width = 2592;
      sensor_height = image_height = 1520;
      sensor_framerate = 25;
      isp_framerate = 25;
      vi_vpss_mode      = VI_OFFLINE_VPSS_ONLINE;
      
    // - v300 IMX335 5MP
    } else if (!strcmp(value, "300_imx335F5")) {
      goke_version      = 300;
      sensor_type       = IMX335;
      sensor_width      = image_width   = 2592;
      sensor_height     = image_height  = 1944;
      sensor_framerate  = 25;
      isp_framerate     = 25;
      vi_vpss_mode      = VI_OFFLINE_VPSS_ONLINE;
    
    } else if (!strcmp(value, "300_imx335B")) {
      goke_version      = 300;
      sensor_type       = IMX335;
      sensor_width      = image_width   = 1296;
      sensor_height     = image_height  = 972;
      sensor_framerate  = 60;
      isp_framerate     = 60;
      vi_vpss_mode      = VI_ONLINE_VPSS_ONLINE;

    } else {
      printf("> ERROR: Unknown version [%s]\n", value);
      exit(1);
    }
    continue;
  }

  __OnArgument("--no-slices") {
    enable_slices = 0;
    continue;
  }

  __OnArgument("--slice-size") {
    venc_slice_size = atoi(__ArgValue);
    continue;
  }

  __OnArgument("--low-delay") {
    enable_lowdelay = 1;
    continue;
  }

  __OnArgument("--mirror") {
    image_mirror = HI_TRUE;
    continue;
  }

  __OnArgument("--flip") {
    image_flip = HI_TRUE;
    continue;
  }

  __OnArgument("--exp") {
    limit_exposure = true;
    continue;
  }

  __OnArgument("-c") {
    const char* value = __ArgValue;
    if (!strcmp(value, "264avbr")) {
      rc_codec = PT_H264;
      rc_mode = VENC_RC_MODE_H264AVBR;
    } else if (!strcmp(value, "264qvbr")) {
      rc_codec = PT_H264;
      rc_mode = VENC_RC_MODE_H264QVBR;
    } else if (!strcmp(value, "264vbr")) {
      rc_codec = PT_H264;
      rc_mode = VENC_RC_MODE_H264VBR;
    } else if (!strcmp(value, "264cbr")) {
      rc_codec = PT_H264;
      rc_mode = VENC_RC_MODE_H264CBR;
    } else if (!strcmp(value, "265avbr")) {
      rc_codec = PT_H265;
      rc_mode = VENC_RC_MODE_H265AVBR;
    } else if (!strcmp(value, "265qvbr")) {
      rc_codec = PT_H265;
      rc_mode = VENC_RC_MODE_H265QVBR;
    } else if (!strcmp(value, "265vbr")) {
      rc_codec = PT_H265;
      rc_mode = VENC_RC_MODE_H265VBR;
    } else if (!strcmp(value, "265cbr")) {
      rc_codec = PT_H265;
      rc_mode = VENC_RC_MODE_H265CBR;
    } else {
      printf("> ERROR: Unsuported codec [%s]\n", value);
      exit(1);
    }
    continue;
  }

  __OnArgument("-d") {
    const char* format = __ArgValue;
    if (!strcmp(format, "stream")) {
      venc_by_frame = 0;
    } else if (!strcmp(format, "frame")) {
      venc_by_frame = 1;
    } else {
      printf("> ERROR: Unsupported data format [%s]\n", format);
    }
    continue;
  }

  __OnArgument("-f") {
    sensor_framerate = atoi(__ArgValue);
    // Normalize sensor framerate
    if (sensor_framerate > 60) {
      sensor_framerate = 60;
    }
    continue;
  }

  __OnArgument("-g") {
    venc_gop_denom = atoi(__ArgValue);
    continue;
  }

  __OnArgument("--roi") {
    enable_roi = 1;
    continue;
  }

  __OnArgument("--roi-qp") {
    roi_qp = atoi(__ArgValue);
    continue;
  }

  __OnArgument("-s") {
    const char* value = __ArgValue;
    if (!strcmp(value, "D1")) {
      image_width = 720;
      image_height = 480;
    } else if (!strcmp(value, "960p")) {
      image_width = 960;
      image_height = 576;
    } else if (!strcmp(value, "1.3MP")) {
      image_width = 1280;
      image_height = 1024;
    } else if (!strcmp(value, "720p")) {
      image_width = 1280;
      image_height = 720;
    } else if (!strcmp(value, "1080p")) {
      image_width = 1920;
      image_height = 1080;
    } else if (!strcmp(value, "4MP")) {
      image_width = 2592;
      image_height = 1520;
    } else {
      if (sscanf(value, "%dx%d", &image_width, &image_height) != 2) {
        printf("> ERROR: Unsuported image size [%s]\n", value);
        exit(1);
      }
    }
    continue;
  }

  __OnArgument("--mirror") {
    image_mirror = HI_TRUE;
    continue;
  }

  __OnArgument("--flip") {
    image_flip = HI_TRUE;
    continue;
  }

  __EndParseConsoleArguments__

  // Normalize sensor framerate
  if (sensor_framerate > 60) {
    sensor_framerate = 60;
  }

  // Normalize GOP
  venc_gop_size = sensor_framerate / venc_gop_denom;

  /* --- v300 IMX307 --- */
  combo_dev_attr_t* mipi_profile = 0;
  ISP_PUB_ATTR_S* isp_profile = 0;
  ISP_SNS_OBJ_S* sns_object = 0;
  VI_DEV_ATTR_S* sns_profile = 0;

  VI_PIPE_ATTR_S* vi_pipe_profile = 0;
  VI_CHN_ATTR_S* vi_channel_profile = 0;

  switch (sensor_type) {
    case IMX307:
      mipi_profile = &MIPI_4lane_CHN0_SENSOR_IMX307_12BIT_2M_NOWDR_ATTR;
      isp_profile = &ISP_PROFILE_IMX307_MIPI_2M_30FPS;
      sns_object = &stSnsImx307_2l_Obj;
      sns_profile = &DEV_ATTR_IMX307_2M_BASE;

      vi_pipe_profile = &PIPE_ATTR_RAW10_420_3DNR_RFR;
      vi_channel_profile = &CHN_ATTR_420_SDR8_LINEAR;
      vi_vpss_mode = VI_ONLINE_VPSS_ONLINE;

      // 4-lane for v300
      if (goke_version == 300) {
        mipi_profile->mipi_attr.lane_id[0] = 0;
        mipi_profile->mipi_attr.lane_id[1] = 1;
        mipi_profile->mipi_attr.lane_id[2] = 2;
        mipi_profile->mipi_attr.lane_id[3] = 3;
      }

      mipi_profile->mipi_attr.input_data_type = DATA_TYPE_RAW_12BIT;
      break;

    case IMX335:
      mipi_profile = &MIPI_4lane_CHN0_SENSOR_IMX335_12BIT_4M_NOWDR_ATTR;
      isp_profile = &ISP_PROFILE_IMX335_MIPI_4M_30FPS;
      sns_object = &stSnsImx335Obj;
      sns_profile = &DEV_ATTR_IMX335_4M_BASE;

      vi_pipe_profile = &PIPE_ATTR_RAW10_420_3DNR_RFR;
      vi_channel_profile = &CHN_ATTR_420_SDR8_LINEAR;
      
      mipi_profile->mipi_attr.input_data_type = DATA_TYPE_RAW_12BIT;
      break;
  }

  // Update VI pipe / channel resolution
  vi_pipe_profile->bSharpenEn = 1;
  vi_pipe_profile->u32MaxW = sensor_width;
  vi_pipe_profile->u32MaxH = sensor_height;
  vi_channel_profile->stSize.u32Width = sensor_width;
  vi_channel_profile->stSize.u32Height = sensor_height;
  vi_channel_profile->stFrameRate.s32SrcFrameRate = sensor_framerate;
  vi_channel_profile->stFrameRate.s32DstFrameRate = sensor_framerate;

  // Update ISP profile
  isp_profile->f32FrameRate = isp_framerate;
  isp_profile->stSnsSize.u32Width = sensor_width;
  isp_profile->stSnsSize.u32Height = sensor_height;
  isp_profile->stWndRect.u32Width = sensor_width;
  isp_profile->stWndRect.u32Height = sensor_height;

  printf(
    "> Starting\n"
    "  - CPU     : v%d\n"
    "  - Sensor  : %d x %d @ %d\n"
    "  - Image   : %d x %d\n"
    ,
    goke_version,
    sensor_width, sensor_height, sensor_framerate,
    image_width,  image_height
    
  );

  // Reset previous configuration
  ret = HI_MPI_SYS_Exit();

  // Setup memory pools and initialize system
  VB_CONFIG_S vb_conf;
  memset(&vb_conf, 0x00, sizeof(vb_conf));

  // Use two memory pools
  vb_conf.u32MaxPoolCnt = 2;

  // Memory pool for VI
  vb_conf.astCommPool[0].u32BlkCnt  = (goke_version == 300 && sensor_type == IMX335)
    ? 3
    : 0
  ;
  vb_conf.astCommPool[0].u64BlkSize = VI_GetRawBufferSize(sensor_width, sensor_height,
    PIXEL_FORMAT_RGB_BAYER_12BPP, COMPRESS_MODE_NONE, DEFAULT_ALIGN);

  // Memory pool for VENC
  vb_conf.astCommPool[1].u32BlkCnt = 2;
  vb_conf.astCommPool[1].u64BlkSize = COMMON_GetPicBufferSize(image_width,
    image_height, PIXEL_FORMAT_YVU_SEMIPLANAR_420, DATA_BITWIDTH_8,
    COMPRESS_MODE_NONE, DEFAULT_ALIGN);

  // Configure video buffer
  ret = HI_MPI_VB_SetConfig(&vb_conf);
  if (ret) {
    printf("ERROR: Configure VB failed : 0x%x\n", ret);
  }

  // Initilize video buffer
  ret = HI_MPI_VB_Init();
  if (ret) {
    printf("ERROR: Init VB failed : 0x%x\n", ret);
  }

  // Initialize system
  ret = HI_MPI_SYS_Init();
  if (ret) {
    printf("ERROR: Init SYS failed : 0x%x\n", ret);
    HI_MPI_VB_Exit();
  }

  // Set VI-VPSS mode to VI offline and VPSS online
  VI_VPSS_MODE_S vi_vpss_mode_config;
  HI_MPI_SYS_GetVIVPSSMode(&vi_vpss_mode_config);
  vi_vpss_mode_config.aenMode[vi_dev_id] = vi_vpss_mode;

  ret = HI_MPI_SYS_SetVIVPSSMode(&vi_vpss_mode_config);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to set VI-VPSS mode\n");
    return ret;
  }

  // Open MIPI device
  int mipi_device = open(DEV_MIPI, O_RDWR);

  // Activate MIPI
  mipi_set_hs_mode(mipi_device, LANE_DIVIDE_MODE_0);
  mipi_enable_clock(mipi_device, mipi_device_id, 1);
  mipi_set_reset(mipi_device, mipi_device_id, 1);

  // Activate sensor
  mipi_enable_sensor_clock(mipi_device, mipi_sensor_id, 1);
  mipi_set_sensor_reset(mipi_device, mipi_sensor_id, 1);

  // Configure
  combo_dev_attr_t mipi_config;
  memcpy(&mipi_config, mipi_profile, sizeof(combo_dev_attr_t));
  mipi_config.devno = mipi_device_id;
  mipi_config.img_rect.width = sensor_width;
  mipi_config.img_rect.height = sensor_height;
  mipi_configure(mipi_device, &mipi_config);

  // Remove reset states
  mipi_set_reset(mipi_device, mipi_device_id, 0);
  mipi_set_sensor_reset(mipi_device, mipi_sensor_id, 0);

  // Close MIPI (not needed anymore)
  close(mipi_device);

  // Set VI device configuration
  sns_profile->stSize.u32Width = sensor_width;
  sns_profile->stSize.u32Height = sensor_height;
  sns_profile->stBasAttr.stSacleAttr.stBasSize.u32Width = sensor_width;
  sns_profile->stBasAttr.stSacleAttr.stBasSize.u32Height = sensor_height;
  sns_profile->stSynCfg.stTimingBlank.u32HsyncAct = sensor_width;
  sns_profile->stSynCfg.stTimingBlank.u32VsyncVact = sensor_height;
  sns_profile->stWDRAttr.u32CacheLine = sensor_height;
  HI_MPI_VI_SetDevAttr(vi_dev_id, sns_profile);

  VI_DEV_ATTR_S attr;
  HI_MPI_VI_GetDevAttr(vi_dev_id, &attr);

  // Enable VI device
  ret = HI_MPI_VI_EnableDev(vi_dev_id);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to enable VI device\n");
    return ret;
  }

  // Create pipe on VI device
  VI_DEV_BIND_PIPE_S pipe;
  pipe.u32Num = 1;
  pipe.PipeId[0] = vi_pipe_id;
  ret = HI_MPI_VI_SetDevBindPipe(vi_dev_id, &pipe);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to bind VI pipe\n");
    return ret;
  }

  // Configure pipe
  ret = HI_MPI_VI_CreatePipe(vi_pipe_id, vi_pipe_profile);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to create VI pipe = 0x%x\n", ret);
    return ret;
  }

  // Start pipe
  ret = HI_MPI_VI_StartPipe(vi_pipe_id);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to start VI pipe\n");
    return ret;
  }

  // Configure channel
  HI_MPI_VI_SetChnAttr(vi_pipe_id, vi_channel_id, vi_channel_profile);

  // Start channel
  ret = HI_MPI_VI_EnableChn(vi_pipe_id, vi_channel_id);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to enable VI channel\n");
    return ret;
  }

  // Initialize ISP for VI pipe
  ISP_SNS_COMMBUS_U bus;
  bus.s8I2cDev = 0; // I2C device #0
  sns_object->pfnSetBusInfo(vi_pipe_id, bus);

  // Register ISP libraries in sensor driver
  ALG_LIB_S ae_lib;
  ALG_LIB_S awb_lib;

  ae_lib.s32Id = 0;
  awb_lib.s32Id = 0;

  strncpy(ae_lib.acLibName, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
  strncpy(awb_lib.acLibName, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));

  // Register library callbacks
  sns_object->pfnRegisterCallback(vi_pipe_id, &ae_lib, &awb_lib);

  // Load (register) ISP libraries in MPI
  HI_MPI_AE_Register(vi_pipe_id, &ae_lib);
  HI_MPI_AWB_Register(vi_pipe_id, &awb_lib);

  // Initialize ISP memory for VI pipe
  HI_MPI_ISP_MemInit(vi_pipe_id);

  // Configure ISP
  HI_MPI_ISP_SetPubAttr(vi_pipe_id, isp_profile);

  // Initialize ISP
  ret = HI_MPI_ISP_Init(vi_pipe_id);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to init ISP\n");
    return ret;
  }

  if (limit_exposure) {
    ISP_EXPOSURE_ATTR_S attr;
    ret = HI_MPI_ISP_GetExposureAttr(0, &attr);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to get exposure\n");
      return ret;
    }

    attr.stAuto.stExpTimeRange.u32Max = 10000 * 1000 / sensor_framerate;
    ret = HI_MPI_ISP_SetExposureAttr(0, &attr);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to set exposure\n");
      return ret;
    }
  }

  // Create VPSS group
  VPSS_GRP_ATTR_S grp_attr;
  memset(&grp_attr, 0x00, sizeof(grp_attr));
  grp_attr.enDynamicRange = DYNAMIC_RANGE_SDR8;
  grp_attr.enPixelFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
  grp_attr.u32MaxW = MAX2(sensor_width, image_width);
  grp_attr.u32MaxH = MAX2(sensor_height, image_height);
  grp_attr.bNrEn = HI_TRUE;
  grp_attr.stNrAttr.enNrType = VPSS_NR_TYPE_VIDEO;
  grp_attr.stNrAttr.enNrMotionMode = NR_MOTION_MODE_NORMAL;
  grp_attr.stNrAttr.enCompressMode = COMPRESS_MODE_NONE;
  grp_attr.stFrameRate.s32SrcFrameRate = sensor_framerate;
  grp_attr.stFrameRate.s32DstFrameRate = sensor_framerate;
  HI_MPI_VPSS_CreateGrp(vpss_group_id, &grp_attr);

  // Create second VPSS channel #1 for small stream (secondary stream)
  VPSS_CHN_ATTR_S chn_attr;
  memset(&chn_attr, 0x00, sizeof(chn_attr));
  chn_attr.u32Width = image_width;
  chn_attr.u32Height = image_height;
  chn_attr.enChnMode = VPSS_CHN_MODE_USER;
  chn_attr.enCompressMode = COMPRESS_MODE_NONE;
  chn_attr.enDynamicRange = DYNAMIC_RANGE_SDR8;
  chn_attr.enPixelFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
  if (chn_attr.u32Width * chn_attr.u32Height > 2688 * 1520) {
    chn_attr.stFrameRate.s32SrcFrameRate = sensor_framerate;
    chn_attr.stFrameRate.s32DstFrameRate = 20;
  } else {
    chn_attr.stFrameRate.s32SrcFrameRate = sensor_framerate;
    chn_attr.stFrameRate.s32DstFrameRate = sensor_framerate;
  }

  chn_attr.u32Depth = 0;
  chn_attr.bMirror = image_mirror;
  chn_attr.bFlip = image_flip;
  chn_attr.enVideoFormat = VIDEO_FORMAT_LINEAR;
  chn_attr.stAspectRatio.enMode = ASPECT_RATIO_NONE;

  ret = HI_MPI_VPSS_SetChnAttr(vpss_group_id, vpss_second_ch_id, &chn_attr);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to set VPSS channel configuration = 0x0%x\n", ret);
    return ret;
  }

  VPSS_LOW_DELAY_INFO_S info;
  ret = HI_MPI_VPSS_GetLowDelayAttr(vpss_group_id, vpss_second_ch_id, &info);
  if (enable_lowdelay) {
    info.bEnable = HI_TRUE;
    info.u32LineCnt = image_height / 4;
    ret = HI_MPI_VPSS_SetLowDelayAttr(vpss_group_id, vpss_second_ch_id, &info);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to set low delay mode\n");
      return ret;
    }
  }

  ret = HI_MPI_VPSS_GetLowDelayAttr(vpss_group_id, vpss_second_ch_id, &info);
  printf("> Low delay is %s, line count = %d\n",
    info.bEnable ? "[Enabled]" : "[Disabled]", info.u32LineCnt);

  // Enable channels
  ret = HI_MPI_VPSS_EnableChn(vpss_group_id, vpss_second_ch_id);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to enable VPSS channel\n");
    return ret;
  }

  // Start group
  ret = HI_MPI_VPSS_StartGrp(vpss_group_id);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to start VPSS group\n");
    return ret;
  }

  // Connect VI to VPSS
  MPP_CHN_S vi_src;
  MPP_CHN_S vpss_dst;

  vi_src.enModId = HI_ID_VI;
  vi_src.s32DevId = vi_dev_id;
  vi_src.s32ChnId = vi_channel_id;

  vpss_dst.enModId = HI_ID_VPSS;
  vpss_dst.s32DevId = vpss_group_id;
  vpss_dst.s32ChnId = vpss_second_ch_id;

  HI_MPI_SYS_Bind(&vi_src, &vpss_dst);

  // Configure h264 encoder
  VENC_CHN_ATTR_S config;
  memset(&config, 0x00, sizeof(config));
  config.stVencAttr.enType = rc_codec;
  config.stVencAttr.u32MaxPicWidth = image_width;
  config.stVencAttr.u32MaxPicHeight = image_height;
  config.stVencAttr.u32PicWidth = image_width;
  config.stVencAttr.u32PicHeight = image_height;
  config.stVencAttr.u32BufSize = ALIGN_UP(image_width * image_height * 3 / 4, 64);
  config.stVencAttr.u32Profile = 0; // Baseline (0), Main(1), High(1)
  config.stVencAttr.bByFrame = venc_by_frame;
  config.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
  config.stGopAttr.stNormalP.s32IPQpDelta = 4;
  config.stRcAttr.enRcMode = rc_mode;

  switch (rc_codec) {
    case PT_H264:
      config.stVencAttr.stAttrH264e.bRcnRefShareBuf = HI_TRUE;
      break;

    case PT_H265:
      config.stVencAttr.stAttrH265e.bRcnRefShareBuf = HI_TRUE;
      break;
  }

  switch (rc_mode) {
    case VENC_RC_MODE_H264AVBR:
      printf("> Codec: h264 AVBR\n");
      config.stRcAttr.stH264AVbr.u32SrcFrameRate = sensor_framerate;
      config.stRcAttr.stH264AVbr.fr32DstFrameRate = sensor_framerate;
      config.stRcAttr.stH264AVbr.u32Gop = venc_gop_size;
      config.stRcAttr.stH264AVbr.u32MaxBitRate = venc_max_rate;
      config.stRcAttr.stH264AVbr.u32StatTime = 1;
      break;

    case VENC_RC_MODE_H264QVBR:
      printf("> Codec: h264 QVBR\n");
      config.stRcAttr.stH264QVbr.u32SrcFrameRate = sensor_framerate;
      config.stRcAttr.stH264QVbr.fr32DstFrameRate = sensor_framerate;
      config.stRcAttr.stH264QVbr.u32StatTime = 1;
      config.stRcAttr.stH264QVbr.u32Gop = venc_gop_size;
      config.stRcAttr.stH264QVbr.u32TargetBitRate = venc_max_rate;

    case VENC_RC_MODE_H264VBR:
      printf("> Codec: h264 VBR\n");
      config.stRcAttr.stH264Vbr.u32SrcFrameRate = sensor_framerate;
      config.stRcAttr.stH264Vbr.fr32DstFrameRate = sensor_framerate;
      config.stRcAttr.stH264Vbr.u32StatTime = 1;
      config.stRcAttr.stH264Vbr.u32Gop = venc_gop_size;
      config.stRcAttr.stH264Vbr.u32MaxBitRate = venc_max_rate;
      break;

    case VENC_RC_MODE_H264CBR:
      printf("> Codec: h264 CBR\n");
      config.stRcAttr.stH264Cbr.u32SrcFrameRate = sensor_framerate;
      config.stRcAttr.stH264Cbr.fr32DstFrameRate = sensor_framerate;
      config.stRcAttr.stH264Cbr.u32StatTime = 1;
      config.stRcAttr.stH264Cbr.u32Gop = venc_gop_size;
      config.stRcAttr.stH264Cbr.u32BitRate = venc_max_rate;
      break;

    case VENC_RC_MODE_H265AVBR:
      printf("> Codec: h265 AVBR\n");
      config.stRcAttr.stH265AVbr.u32SrcFrameRate = sensor_framerate;
      config.stRcAttr.stH265AVbr.fr32DstFrameRate = sensor_framerate;
      config.stRcAttr.stH265AVbr.u32StatTime = 1;
      config.stRcAttr.stH265AVbr.u32Gop = venc_gop_size;
      config.stRcAttr.stH265AVbr.u32MaxBitRate = venc_max_rate;
      break;

    case VENC_RC_MODE_H265VBR:
      printf("> Codec: h265 VBR\n");
      config.stRcAttr.stH265Vbr.u32SrcFrameRate = sensor_framerate;
      config.stRcAttr.stH265Vbr.fr32DstFrameRate = sensor_framerate;
      config.stRcAttr.stH265Vbr.u32StatTime = 1;
      config.stRcAttr.stH265Vbr.u32Gop = venc_gop_size;
      config.stRcAttr.stH265Vbr.u32MaxBitRate = venc_max_rate;
      break;

    case VENC_RC_MODE_H265CBR:
      printf("> Codec: h265 CBR\n");
      config.stRcAttr.stH265Cbr.u32SrcFrameRate = sensor_framerate;
      config.stRcAttr.stH265Cbr.fr32DstFrameRate = sensor_framerate;
      config.stRcAttr.stH265Cbr.u32StatTime = 1;
      config.stRcAttr.stH265Cbr.u32Gop = venc_gop_size;
      config.stRcAttr.stH265Cbr.u32BitRate = venc_max_rate;
      break;

    case VENC_RC_MODE_H265QVBR:
      printf("> Codec: h265 QVBR\n");
      config.stRcAttr.stH265QVbr.u32SrcFrameRate = sensor_framerate;
      config.stRcAttr.stH265QVbr.fr32DstFrameRate = sensor_framerate;
      config.stRcAttr.stH265QVbr.u32StatTime = 1;
      config.stRcAttr.stH265QVbr.u32Gop = venc_gop_size;
      config.stRcAttr.stH265QVbr.u32TargetBitRate = venc_max_rate;
      break;
  }

  // Create channel #1
  ret = HI_MPI_VENC_CreateChn(venc_second_ch_id, &config);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to create VENC channel = 0x%x\n", ret);
    return ret;
  }

  // Configure rate control for channel #1
  VENC_RC_PARAM_S rc_param;
  HI_MPI_VENC_GetRcParam(venc_second_ch_id, &rc_param);
  switch (rc_mode) {
    case VENC_RC_MODE_H264AVBR:
      rc_param.stParamH264AVbr.s32MaxReEncodeTimes = 0;
      break;

    case VENC_RC_MODE_H264QVBR:
      rc_param.stParamH264QVbr.s32MaxReEncodeTimes = 0;
      break;

    case VENC_RC_MODE_H264VBR:
      rc_param.stParamH264Vbr.s32MaxReEncodeTimes = 0;
      break;

    case VENC_RC_MODE_H264CBR:
      rc_param.stParamH264Cbr.s32MaxReEncodeTimes = 0;
      break;

    case VENC_RC_MODE_H265AVBR:
      rc_param.stParamH265AVbr.s32MaxReEncodeTimes = 0;
      break;

    case VENC_RC_MODE_H265QVBR:
      rc_param.stParamH265QVbr.s32MaxReEncodeTimes = 0;
      break;

    case VENC_RC_MODE_H265VBR:
      rc_param.stParamH265Vbr.s32MaxReEncodeTimes = 0;
      break;

    case VENC_RC_MODE_H265CBR:
      rc_param.stParamH265Cbr.s32MaxReEncodeTimes = 0;
      break;
  }

  rc_param.s32FirstFrameStartQp = -1;
  rc_param.stSceneChangeDetect.bAdaptiveInsertIDRFrame = HI_TRUE;
  rc_param.stSceneChangeDetect.bDetectSceneChange = HI_TRUE;

  ret = HI_MPI_VENC_SetRcParam(venc_second_ch_id, &rc_param);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to set VENC RC options = 0x%x\n", ret);
    return ret;
  }

  HI_MPI_VENC_GetRcParam(venc_second_ch_id, &rc_param);
  printf("> Scene detect = %s, Adaptive IDR = %s, Start Qp = %d, Row dQp = %d\n",
    rc_param.stSceneChangeDetect.bDetectSceneChange ? "YES" : "NO",
    rc_param.stSceneChangeDetect.bAdaptiveInsertIDRFrame ? "YES" : "NO",
    rc_param.s32FirstFrameStartQp, rc_param.u32RowQpDelta);

  // Enable slices (not available in frame mode)
  switch (rc_codec) {
    case PT_H264: {
      VENC_H264_SLICE_SPLIT_S avc_param;
      HI_MPI_VENC_GetH264SliceSplit(venc_second_ch_id, &avc_param);
      avc_param.bSplitEnable = 1;
      avc_param.u32MbLineNum = venc_slice_size;

      if (enable_slices) {
        if (venc_by_frame) {
          printf("WARN: Slices are not available in [frame] data format\n");
        } else {
          int ret = HI_MPI_VENC_SetH264SliceSplit(venc_second_ch_id, &avc_param);
          if (ret != HI_SUCCESS) {
            printf("ERROR: Unable to set VENC h264 slice size = 0x%x\n", ret);
            return ret;
          }
        }
      }

      HI_MPI_VENC_GetH264SliceSplit(venc_second_ch_id, &avc_param);
      printf("> H264 slices is [%s] | Slice size = %d lines\n",
        avc_param.bSplitEnable ? "Enabled" : "Disabled", avc_param.u32MbLineNum);
      break;
    }

    case PT_H265: {
      VENC_H265_SLICE_SPLIT_S hevc_param;
      HI_MPI_VENC_GetH265SliceSplit(venc_second_ch_id, &hevc_param);
      hevc_param.bSplitEnable = 1;
      hevc_param.u32LcuLineNum = venc_slice_size;

      if (enable_slices) {
        if (venc_by_frame) {
          printf("WARN: Slices are not available in [frame] data format\n");
        } else {
          int ret = HI_MPI_VENC_SetH265SliceSplit(venc_second_ch_id, &hevc_param);
          if (ret != HI_SUCCESS) {
            printf("ERROR: Unable to set VENC h265 slice size = 0x%x\n", ret);
            return ret;
          }
        }
      }

      HI_MPI_VENC_GetH265SliceSplit(venc_second_ch_id, &hevc_param);
      printf("> H265 slices is [%s] | Slice size = %d lines\n",
        hevc_param.bSplitEnable ? "Enabled" : "Disabled", hevc_param.u32LcuLineNum);
      break;
    }
  }

  VENC_REF_PARAM_S ref_param;
  HI_MPI_VENC_GetRefParam(venc_second_ch_id, &ref_param);
  printf("> Reference = EN: %d, Base: %d, Enhance: %d\n",
    ref_param.bEnablePred, ref_param.u32Base, ref_param.u32Enhance);

  ref_param.bEnablePred = 1;
  ref_param.u32Enhance = 0;
  ref_param.u32Base = 1;

  ret = HI_MPI_VENC_SetRefParam(venc_second_ch_id, &ref_param);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to set VENC REF options = 0x%x\n", ret);
    return ret;
  }

  // Setup frame lost strategy
  if (rc_codec == PT_H265) {
    VENC_FRAMELOST_S lost_param;
    ret = HI_MPI_VENC_GetFrameLostStrategy(venc_second_ch_id, &lost_param);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to get frame lost strategy = 0x%x\n", ret);
      return ret;
    }

    lost_param.bFrmLostOpen = 1;
    lost_param.enFrmLostMode = FRMLOST_PSKIP;
    lost_param.u32FrmLostBpsThr = venc_max_rate * 1024 / 2;
    lost_param.u32EncFrmGaps = 1;

    ret = HI_MPI_VENC_SetFrameLostStrategy(venc_second_ch_id, &lost_param);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to set frame lost strategy = 0x%x\n", ret);
      return ret;
    }
  }

  if (enable_roi) {
    VENC_ROI_ATTR_S roi_config;
    roi_config.bEnable = HI_TRUE;
    roi_config.u32Index = 0;
    roi_config.stRect.s32X = ALIGN_UP(image_width / 4, 16);
    roi_config.stRect.s32Y = ALIGN_UP(image_height / 4, 16);
    roi_config.stRect.u32Width = ALIGN_UP(image_width / 2, 16);
    roi_config.stRect.u32Height = ALIGN_UP(image_height / 2, 16);
    roi_config.bAbsQp = HI_TRUE;
    roi_config.s32Qp = roi_qp;

    ret = HI_MPI_VENC_SetRoiAttr(venc_second_ch_id, &roi_config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to setup VENC ROI = 0x%x\n", ret);
      return ret;
    }

    printf("> ROI is [Enabled]\n");
  }

  // Connect VPSS channel #1 to VENC channel #1
  MPP_CHN_S vpss_src;
  MPP_CHN_S venc_dst;

  vpss_src.enModId = HI_ID_VPSS;
  vpss_src.s32DevId = vpss_group_id;
  vpss_src.s32ChnId = vpss_second_ch_id;

  venc_dst.enModId = HI_ID_VENC;
  venc_dst.s32DevId = 0;
  venc_dst.s32ChnId = venc_second_ch_id;

  HI_MPI_SYS_Bind(&vpss_src, &venc_dst);

  // Start VENC channel #1 without frames count limit
  VENC_RECV_PIC_PARAM_S recv_param;
  recv_param.s32RecvPicNum = -1;
  ret = HI_MPI_VENC_StartRecvFrame(venc_second_ch_id, &recv_param);
  if (ret != HI_SUCCESS) {
    printf("ERROR: Unable to start Rx frames\n");
    return ret;
  }

  // Start ISP service thread
  pthread_t isp_thread;
  pthread_create(&isp_thread, NULL, __ISP_THREAD__, (void*)vi_pipe_id);

  // Open socket handle
  int socket_handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  struct sockaddr_in dst_addr;
  dst_addr.sin_family = AF_INET;
  dst_addr.sin_port = htons(udp_sink_port);
  dst_addr.sin_addr.s_addr = udp_sink_ip;

  // Prepare Tx buffer
  tx_buffer = malloc(65536);
  printf("> Ready for streaming\n");
  signal(SIGINT, handler);

  while (loop_running) {
    // Process stream on encoder channel #1
    if (!processStream(venc_second_ch_id, socket_handle,
        (struct sockaddr*)&dst_addr, max_frame_size)) {
      // --- Take a rest if no frames received
      // Another way: HI_MPI_VENC_GetFd(vecn_channel_id) + epoll
      usleep(1);
    }
  }

  printf("> Stop streaming\n");

  HI_MPI_ISP_Exit(vi_pipe_id);
  HI_MPI_VPSS_StopGrp(vpss_group_id);
  HI_MPI_VPSS_DestroyGrp(vpss_group_id);

  HI_MPI_VI_DisableChn(vi_pipe_id, vi_channel_id);
  HI_MPI_VI_StopPipe(vi_pipe_id);
  HI_MPI_VI_DestroyPipe(vi_pipe_id);
  HI_MPI_VI_DisableDev(vi_pipe_id);

  HI_MPI_SYS_Exit();
  HI_MPI_VB_Exit();

  return 0;
}

void* __ISP_THREAD__(void* param) {
  HI_MPI_ISP_Run((VI_PIPE)param);
}

double getTimeInterval(
  struct timespec* timestamp, struct timespec* last_meansure_timestamp) {
  return (timestamp->tv_sec - last_meansure_timestamp->tv_sec) +
       (timestamp->tv_nsec - last_meansure_timestamp->tv_nsec) / 1000000000.;
}

struct timespec last_timestamp = {0, 0};
uint32_t bytes_sent = 0;
uint32_t frames_sent = 0;
uint64_t seq_last = 0;

uint64_t jitter_sum = 0;
uint64_t jitter_cnt = 0;
uint32_t nal_max_size = 0;
uint32_t single_packets = 0;

uint32_t pps_count = 0;
uint32_t sps_count = 0;
uint32_t idr_count = 0;
uint32_t sei_count = 0;
uint32_t s_count = 0;
uint32_t packets_sent = 0;

int processStream(VENC_CHN channel_id, int socket_handle,
  struct sockaddr* dst_address, uint16_t max_frame_size) {
  // Get channel status
  VENC_CHN_STATUS_S channel_status;
  int ret = HI_MPI_VENC_QueryStatus(channel_id, &channel_status);
  if (ret != HI_SUCCESS) {
    printf("WARN: Unable to query VENC status = 0x%x\n", ret);
    usleep(100000);
    return 0;
  }

  // Check if has encoded data
  if (!channel_status.u32CurPacks) {
    // Nothing to get
    return 0;
  }

  // Stream packets descriptors
  // Per-Slice mode always return one slice at a time.
  // Per-Frame mode may return multiple slices, we need to allocate memory for all reported slices.
  VENC_PACK_S packet_descriptor[32];

  // Stream buffer
  VENC_STREAM_S stream;
  memset(&stream, 0x00, sizeof(stream));
  stream.pstPack = packet_descriptor;
  stream.u32PackCount = channel_status.u32CurPacks;

  // Acquire stream
  ret = HI_MPI_VENC_GetStream(channel_id, &stream, 0);
  if (ret != HI_SUCCESS) {
    printf("WARN: Failed to get VENC stream  = 0x%x. Current packs = %d\n",
      ret, channel_status.u32CurPacks);
    usleep(100000);
    return 0;
  }

  // Send encoded packets
  for (uint32_t i = 0; i < stream.u32PackCount; i++) {
    sendPacket(stream.pstPack[i].pu8Addr + stream.pstPack[i].u32Offset,
      stream.pstPack[i].u32Len - stream.pstPack[i].u32Offset,
      socket_handle, dst_address, max_frame_size);
  }

  // Release stream
  HI_MPI_VENC_ReleaseStream(channel_id, &stream);

  // Print rate stats
  struct timespec current_timestamp;
  if (!clock_gettime(CLOCK_MONOTONIC_COARSE, &current_timestamp)) {
    double interval = getTimeInterval(&current_timestamp, &last_timestamp);
    if (interval > 1) {
      printf("> Rate: %.2f Mbit/sec. (%.1f pps) | Frames: %d, NotFrag: "
           "%d | AVG Size: %d, MAX Size: %d | S: %d, IDR: %d, SEI: %d, "
           "PPS: %d, SPS: %d | Packets: %d\n",
        ((double)bytes_sent * 8) / interval / 1024 / 1024,
        (double)frames_sent / interval, /* jitter_sum / jitter_cnt,*/
        frames_sent, single_packets, bytes_sent / frames_sent,
        nal_max_size, s_count, idr_count, sei_count, pps_count,
        sps_count, packets_sent);

      bytes_sent = 0;
      frames_sent = 0;
      jitter_sum = 0;
      jitter_cnt = 0;
      nal_max_size = 0;
      s_count = 0;
      idr_count = 0;
      pps_count = 0;
      sps_count = 0;
      sei_count = 0;
      single_packets = 0;
      packets_sent = 0;
      last_timestamp = current_timestamp;
    }
  }

  return 1;
}

uint32_t sequence_id = 0;
uint32_t frame_id = 0;
uint16_t rtp_sequence = 0;

void transmit(int socket_handle, uint8_t* tx_buffer, uint32_t tx_size,
  struct sockaddr* dst_address) {

  switch (stream_mode) {
    // Compact mode
    case 0:
      sendto(socket_handle, tx_buffer, tx_size, 0, dst_address, sizeof(struct sockaddr_in));
      break;

    // RTP mode
    case 1: {
      struct RTPHeader rtp_header;
      rtp_header.version = 0x80;
      rtp_header.sequence = htobe16(rtp_sequence++);
      rtp_header.payload_type = 0x60;
      rtp_header.timestamp = 0;
      rtp_header.ssrc_id = 0xDEADBEEF;

      struct iovec iov[2];
      iov[0].iov_base = &rtp_header;
      iov[0].iov_len = sizeof(struct RTPHeader);
      iov[1].iov_base = tx_buffer;
      iov[1].iov_len = tx_size;

      struct msghdr msg;
      msg.msg_iovlen = 2;
      msg.msg_iov = iov;
      msg.msg_name = dst_address;
      msg.msg_namelen = sizeof(struct sockaddr_in);

      sendmsg(socket_handle, &msg, 0);
      break;
    }
  }
}

void sendPacket(uint8_t* pack_data, uint32_t pack_size, int socket_handle,
    struct sockaddr* dst_address, uint32_t max_size) {
  uint8_t prefix = 4;
  pack_data += prefix;
  pack_size -= prefix;

  frame_id++;
  frames_sent++;

  if (pack_size > nal_max_size) {
    nal_max_size = pack_size;
  }

  if (pack_size <= max_size) {
    single_packets++;
  }

  // Get NAL type
  uint8_t nal_type = pack_data[0] & 0x1F;
  switch (nal_type) {
    case 1:
      s_count++;
      break;

    case 5:
      idr_count++;
      break;

    case 6:
      sei_count++;
      break;

    case 7:
      sps_count++;
      break;

    case 8:
      pps_count++;
      break;

    default:
      break;
  }

  if (pack_size > max_size + prefix) {
    uint8_t nal_type_avc = pack_data[0] & 0x1F;
    uint8_t nal_type_hevc = (pack_data[0] >> 1) & 0x3F;
    uint8_t nal_bits_avc = pack_data[0] & 0xE0;
    uint8_t nal_bits_hevc = pack_data[0] & 0x81;

    bool start_bit = true;
    uint8_t tx_size = 2;

    while (pack_size) {
      uint32_t chunk_size = pack_size > max_size ? max_size : pack_size;
      if (nal_type_avc == 1 || nal_type_avc == 5) {
        tx_buffer[0] = nal_bits_avc | 28;
        tx_buffer[1] = nal_type_avc;

        if (start_bit) {
          pack_data++;
          pack_size--;
          tx_buffer[1] = 0x80 | nal_type_avc;
          start_bit = false;
        }

        if (chunk_size == pack_size) {
          tx_buffer[1] |= 0x40;
        }
      }

      if (nal_type_hevc == 1 || nal_type_hevc == 19) {
        tx_buffer[0] = nal_bits_hevc | 49 << 1;
        tx_buffer[1] = 1;
        tx_buffer[2] = nal_type_hevc;
        tx_size = 3;

        if (start_bit) {
          pack_data += 2;
          pack_size -= 2;
          tx_buffer[2] = 0x80 | nal_type_hevc;
          start_bit = false;
        }

        if (chunk_size == pack_size) {
          tx_buffer[2] |= 0x40;
        }
      }

      memcpy(tx_buffer + tx_size, pack_data, chunk_size + tx_size);
      transmit(socket_handle, tx_buffer, chunk_size + tx_size, dst_address);

      packets_sent++;
      bytes_sent += chunk_size + tx_size;

      pack_data += chunk_size;
      pack_size -= chunk_size;
    }
  } else {
    transmit(socket_handle, pack_data, pack_size, dst_address);
    packets_sent++;
  }
}
