#include "main.h"

// - Configuration profiles
extern combo_dev_attr_t MIPI_4lane_CHN0_SENSOR_IMX335_10BIT_4M_WDR2TO1_ATTR;
extern VI_DEV_ATTR_S    DEV_ATTR_IMX335_4M_BASE;
extern VI_PIPE_ATTR_S   PIPE_ATTR_RAW10_420_3DNR_RFR;
extern VI_CHN_ATTR_S    CHN_ATTR_420_SDR8_LINEAR;
extern ISP_PUB_ATTR_S   ISP_PROFILE_IMX335_MIPI_4M_30FPS;

/**
 * @brief Example video capture (h264)
 * 
 *  Pipeline:
 *    VI(pipe:0, ch:0, ISP) -> VPSS(channel:1) -> VENC[channel:1] -> UDP Port
 * 
 * @param argc
 * @param argv
 */
int main(int argc, const char* argv[]) {
  // --------------------------------------------------------------
  // --- Resources
  // --------------------------------------------------------------
  uint32_t  mipi_device_id    = 0;
  uint32_t  mipi_sensor_id    = 0;
  
  uint32_t  sensor_width      = 2592;
  uint32_t  sensor_height     = 1520;
  uint32_t  sensor_framerate  = 30;
  
  uint32_t  image_width       = 1280; // - Encoded image width
  uint32_t  image_height      = 720;  // - Encoded image height
  
  VI_DEV    vi_dev_id         = 0;
  VI_PIPE   vi_pipe_id        = 0;
  VI_CHN    vi_channel_id     = 0;
  
  uint32_t  vi_vpss_mode      = VI_OFFLINE_VPSS_ONLINE;
  
  VPSS_GRP  vpss_group_id     = 0;
  VPSS_CHN  vpss_first_ch_id  = 0;  // - Channel for primary stream
  VPSS_CHN  vpss_second_ch_id = 1;  // - Channel for secondary stream
  
  uint32_t  venc_gop_size     = 30;
  uint32_t  venc_max_rate     = 8192;

  VENC_CHN  venc_first_ch_id  = 0;
  VENC_CHN  venc_second_ch_id = 1;
  HI_BOOL   venc_by_frame     = HI_TRUE;
  
  uint32_t  udp_sink_ip       = inet_addr("10.42.0.233");
  uint16_t  udp_sink_port     = 5000;
  
  // --------------------------------------------------------------
  // --- Board configuration
  // --------------------------------------------------------------
  combo_dev_attr_t* mipi_profile        = &MIPI_4lane_CHN0_SENSOR_IMX335_10BIT_4M_WDR2TO1_ATTR;
  ISP_PUB_ATTR_S*   isp_profile         = &ISP_PROFILE_IMX335_MIPI_4M_30FPS;
  ISP_SNS_OBJ_S*    sns_object          = &stSnsImx335Obj;
  VI_DEV_ATTR_S*    sns_profile         = &DEV_ATTR_IMX335_4M_BASE;
  
  VI_PIPE_ATTR_S*   vi_pipe_profile     = &PIPE_ATTR_RAW10_420_3DNR_RFR;
  VI_CHN_ATTR_S*    vi_channel_profile  = &CHN_ATTR_420_SDR8_LINEAR;
  
  // - Update VI pipe / channel resolution
  vi_pipe_profile->u32MaxW              = sensor_width;
  vi_pipe_profile->u32MaxH              = sensor_height;
  vi_channel_profile->stSize.u32Width   = sensor_width;
  vi_channel_profile->stSize.u32Height  = sensor_height;
  
  // --------------------------------------------------------------
  // --- Reset previous configuration
  // --------------------------------------------------------------
  HI_MPI_SYS_Exit();
  HI_MPI_VB_Exit();
  
  // --------------------------------------------------------------
  // --- Setup memory pools and initialize system
  // --------------------------------------------------------------
  VB_CONFIG_S vb_conf;
  memset(&vb_conf, 0x00, sizeof(vb_conf));
  
  // - Use two memory pools
  vb_conf.u32MaxPoolCnt = 2;
  
  // - Memory pool for VI
  vb_conf.astCommPool[0].u32BlkCnt  = 2; 
  vb_conf.astCommPool[0].u64BlkSize = VI_GetRawBufferSize(sensor_width, sensor_height,
    PIXEL_FORMAT_RGB_BAYER_12BPP, COMPRESS_MODE_NONE, DEFAULT_ALIGN
  );
  
  // - Memory pool for VENC
  vb_conf.astCommPool[1].u32BlkCnt  = 2;
  vb_conf.astCommPool[1].u64BlkSize = COMMON_GetPicBufferSize(image_width, image_height, 
    PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    DATA_BITWIDTH_8,
    COMPRESS_MODE_NONE,
    DEFAULT_ALIGN
  );
  
  // - Configure video buffer
  { int ret = HI_MPI_VB_SetConfig(&vb_conf);
    if (ret) {
      printf("ERROR: Configure VB failed : 0x%x\n", ret);
    }
  }
  
  // - Initilize video buffer
  { int ret = HI_MPI_VB_Init();
    if (ret) {
      printf("ERROR: Init VB failed : 0x%x\n", ret);
    }
  }

  // - Initialize system
  { int ret = HI_MPI_SYS_Init();
    if (ret) {
      printf("ERROR: Init SYS failed : 0x%x\n", ret);
      HI_MPI_VB_Exit();
    }
  }
  
  // - Set VI-VPSS mode to VI offline and VPSS online because 
  { VI_VPSS_MODE_S vi_vpss_mode_config;
    HI_MPI_SYS_GetVIVPSSMode(&vi_vpss_mode_config);
    vi_vpss_mode_config.aenMode[vi_dev_id] = vi_vpss_mode;
    HI_MPI_SYS_SetVIVPSSMode(&vi_vpss_mode_config);
  }
  
  // - Set ISP statistics interval to 1 sec
  { ISP_CTRL_PARAM_S isp_control_param;
    HI_MPI_ISP_GetCtrlParam(0, &isp_control_param);
    isp_control_param.u32StatIntvl = 1;
    HI_MPI_ISP_SetCtrlParam(0, &isp_control_param);
  }

  // --------------------------------------------------------------
  // --- Configure MIPI 
  // --------------------------------------------------------------
  // - Open MIPI device
  int mipi_device = open("/dev/mipi", O_RDWR);
  
  // - Activate MIPI
  mipi_set_hs_mode  (mipi_device, LANE_DIVIDE_MODE_0);
  mipi_enable_clock (mipi_device, mipi_device_id, 1);
  mipi_set_reset    (mipi_device, mipi_device_id, 1);
  
  // - Activate sensor
  mipi_enable_sensor_clock(mipi_device, mipi_sensor_id, 1);
  mipi_set_sensor_reset   (mipi_device, mipi_sensor_id, 1);

  // - Configure
  { combo_dev_attr_t mipi_config;
    memcpy(&mipi_config, mipi_profile, sizeof(combo_dev_attr_t));
    mipi_config.devno = mipi_device_id;
    mipi_configure(mipi_device, &mipi_config);
  }
  
  // - Remove reset states
  mipi_set_reset        (mipi_device, mipi_device_id, 0);
  mipi_set_sensor_reset (mipi_device, mipi_sensor_id, 0);
  
  // - Close MIPI (not needed anymore)
  close(mipi_device);
  
  // --------------------------------------------------------------
  // --- Create VI
  // --------------------------------------------------------------
  // - Set VI device configuration
  HI_MPI_VI_SetDevAttr(vi_dev_id, sns_profile);
  
  { VI_DEV_ATTR_S attr;
    HI_MPI_VI_GetDevAttr(vi_dev_id, &attr);
    //attr.stWDRAttr.enWDRMode  = WDR_MODE_2To1_LINE;
    HI_MPI_VI_SetDevAttr(vi_dev_id, &attr);
  }
  
  // - Enable VI device
  HI_MPI_VI_EnableDev(vi_dev_id);


  // --------------------------------------------------------------
  // --- Create VI pipe
  // --------------------------------------------------------------
  // - Create pipe on VI device
  { VI_DEV_BIND_PIPE_S pipe;
    pipe.u32Num     = 1;
    pipe.PipeId[0]  = vi_pipe_id;
    HI_MPI_VI_SetDevBindPipe(vi_dev_id, &pipe);
  }
  
  // - Configure pipe
  { VI_PIPE_ATTR_S attr;
    HI_MPI_VI_CreatePipe(vi_pipe_id, vi_pipe_profile);
  }
  
  // - Start pipe
  HI_MPI_VI_StartPipe(vi_pipe_id);

  // --------------------------------------------------------------
  // --- Create VI channel
  // --------------------------------------------------------------
  // - Configure channel
  HI_MPI_VI_SetChnAttr(vi_pipe_id, vi_channel_id, vi_channel_profile);
  { VI_CHN_ATTR_S attr;
    HI_MPI_VI_GetChnAttr(vi_pipe_id, vi_channel_id, &attr);
    attr.enDynamicRange = DYNAMIC_RANGE_HDR10;
    HI_MPI_VI_SetChnAttr(vi_pipe_id, vi_channel_id, &attr);
  }
  
  // - Start channel
  HI_MPI_VI_EnableChn(vi_pipe_id, vi_channel_id);

  // --------------------------------------------------------------
  // --- Initialize ISP for VI pipe
  // --------------------------------------------------------------
  // --- Set sensor control bus
  // - NOT FOR: BT1120_2M_30FPS_8BIT / BT656_2M_30FPS_8BIT / BT601_2M_30FPS_8BIT
  { ISP_SNS_COMMBUS_U bus;
    bus.s8I2cDev  = 0; // - I2C device #0
    sns_object->pfnSetBusInfo(vi_pipe_id, bus);
  }
  
  // --- Register ISP libraries in sensor driver
  // - NOT FOR: BT1120_2M_30FPS_8BIT / BT656_2M_30FPS_8BIT / BT601_2M_30FPS_8BIT
  { ALG_LIB_S ae_lib;   // - Auto exposure library
    ALG_LIB_S awb_lib;  // - Auto white balance library
    
    ae_lib.s32Id  = 0;
    awb_lib.s32Id = 0;
    strncpy(ae_lib.acLibName,   HI_AE_LIB_NAME,   sizeof(HI_AE_LIB_NAME));
    strncpy(awb_lib.acLibName,  HI_AWB_LIB_NAME,  sizeof(HI_AWB_LIB_NAME));
    
    // - Register library callbacks
    sns_object->pfnRegisterCallback(vi_pipe_id, &ae_lib, &awb_lib);
    
    // - Load (register) ISP libraries in MPI
    HI_MPI_AE_Register(vi_pipe_id,  &ae_lib);
    HI_MPI_AWB_Register(vi_pipe_id, &awb_lib);  
  }
  
  // - Initialize ISP memory for VI pipe
  HI_MPI_ISP_MemInit(vi_pipe_id);

  // - Configure ISP
  HI_MPI_ISP_SetPubAttr(vi_pipe_id, isp_profile);

  // - Initialize ISP
  HI_MPI_ISP_Init(vi_pipe_id);

  // --------------------------------------------------------------
  // --- Initialize VPSS
  // --------------------------------------------------------------
  // - Create VPSS group
  { VPSS_GRP_ATTR_S attr;
    memset(&attr, 0x00, sizeof(attr));
    attr.enDynamicRange               = DYNAMIC_RANGE_SDR8;
    attr.enPixelFormat                = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    attr.u32MaxW                      = sensor_width;
    attr.u32MaxH                      = sensor_height;
    attr.bNrEn                        = HI_TRUE;
    attr.stNrAttr.enNrType            = VPSS_NR_TYPE_VIDEO;
    attr.stNrAttr.enNrMotionMode      = NR_MOTION_MODE_NORMAL;
    attr.stNrAttr.enCompressMode      = COMPRESS_MODE_NONE;
    attr.stFrameRate.s32SrcFrameRate  = -1;
    attr.stFrameRate.s32DstFrameRate  = -1;
    HI_MPI_VPSS_CreateGrp(vpss_group_id, &attr);
  }
  
  
  // - Create first VPSS channel #0 for large stream (primary stream)
  {
    // - Not used in this example
  }
  
  // - Create second VPSS channel #1 for small stream (secondary stream)
  { VPSS_CHN_ATTR_S attr;
    memset(&attr, 0x00, sizeof(attr));
    
    attr.u32Width                     = image_width;
    attr.u32Height                    = image_height;
    attr.enChnMode                    = VPSS_CHN_MODE_USER;
    attr.enCompressMode               = COMPRESS_MODE_NONE;
    attr.enDynamicRange               = DYNAMIC_RANGE_SDR8;
    attr.enPixelFormat                = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    if (attr.u32Width * attr.u32Height > 2688 * 1520 ) {
        attr.stFrameRate.s32SrcFrameRate  = 30;
        attr.stFrameRate.s32DstFrameRate  = 20;
    } else {
        attr.stFrameRate.s32SrcFrameRate  = -1;
        attr.stFrameRate.s32DstFrameRate  = -1;
    }
    attr.u32Depth                     = 0;
    attr.bMirror                      = GK_FALSE;
    attr.bFlip                        = GK_FALSE;
    attr.enVideoFormat                = VIDEO_FORMAT_LINEAR;
    attr.stAspectRatio.enMode         = ASPECT_RATIO_NONE;
    
    HI_MPI_VPSS_SetChnAttr(vpss_group_id, vpss_second_ch_id, &attr);
  }
  
  // - Enable channels
  HI_MPI_VPSS_EnableChn(vpss_group_id, vpss_second_ch_id);
  
  // - Start group
  HI_MPI_VPSS_StartGrp(vpss_group_id);
  
  
  // --------------------------------------------------------------
  // - Connect VI to VPSS
  // --------------------------------------------------------------
  { MPP_CHN_S src;
    MPP_CHN_S dst;
    
    src.enModId   = HI_ID_VI;
    src.s32DevId  = vi_dev_id;
    src.s32ChnId  = vi_channel_id;

    dst.enModId   = HI_ID_VPSS;
    dst.s32DevId  = vpss_group_id;
    dst.s32ChnId  = vpss_second_ch_id;
    
    HI_MPI_SYS_Bind(&src, &dst);
  }
  
  // --------------------------------------------------------------
  // --- Create VENC channels
  // --------------------------------------------------------------
  VENC_GOP_MODE_E gop_mode = VENC_GOPMODE_NORMALP;
  VENC_GOP_ATTR_S gop_attr;
  getGOPAttributes(gop_mode, &gop_attr);
  
  // - Configure h264 encoder
  { VENC_CHN_ATTR_S config;
    memset(&config, 0x00, sizeof(config));
    config.stVencAttr.enType          = PT_H264;
    config.stVencAttr.u32MaxPicWidth  = image_width;  // - Max image size for dynamic resolution selection
    config.stVencAttr.u32MaxPicHeight = image_height;
    config.stVencAttr.u32PicWidth     = image_width;  // - Normal image size
    config.stVencAttr.u32PicHeight    = image_height;
    config.stVencAttr.u32BufSize      = ALIGN_UP(image_width * image_height * 3 / 4, 64);  /*stream buffer size*/
    config.stVencAttr.u32Profile      = 0;              // - Baseline (0), Main(1), High(1)
    config.stVencAttr.bByFrame        = venc_by_frame;  // - TRUE for get per-frame, FALSE for get per-slice
    
    // - Set rate control mode
    config.stRcAttr.enRcMode          = VENC_RC_MODE_H264AVBR;
    config.stRcAttr.stH264AVbr.u32Gop           = venc_gop_size;
    config.stRcAttr.stH264AVbr.u32MaxBitRate    = venc_max_rate;
    config.stRcAttr.stH264AVbr.u32StatTime      = 1;
    config.stRcAttr.stH264AVbr.u32SrcFrameRate  = sensor_framerate;
    config.stRcAttr.stH264AVbr.fr32DstFrameRate = sensor_framerate;
    
    
    // - Set GOP
    config.stGopAttr                  = gop_attr;
    
    // - Create channel #1
    HI_MPI_VENC_CreateChn(venc_second_ch_id, &config);
  }
  
  // - Configure rate control for channel #1
  { VENC_RC_PARAM_S param;
    HI_MPI_VENC_GetRcParam(venc_second_ch_id, &param);
    param.stParamH264AVbr.s32MaxReEncodeTimes         = 0;
    param.stSceneChangeDetect.bAdaptiveInsertIDRFrame = HI_TRUE;
    HI_MPI_VENC_SetRcParam(venc_second_ch_id, &param);
  }

  // - Connect VPSS channel #1 to VENC channel #1
  { MPP_CHN_S src;
    MPP_CHN_S dst;

    src.enModId   = HI_ID_VPSS;
    src.s32DevId  = vpss_group_id;
    src.s32ChnId  = vpss_second_ch_id;

    dst.enModId   = HI_ID_VENC;
    dst.s32DevId  = 0;
    dst.s32ChnId  = venc_second_ch_id;

    HI_MPI_SYS_Bind(&src, &dst);
  }
  
  // - Start VENC channel #1 without frames count limit
  { VENC_RECV_PIC_PARAM_S param;
    param.s32RecvPicNum = -1;
    HI_MPI_VENC_StartRecvFrame(venc_second_ch_id, &param);
  }

  // - Start ISP service thread
  pthread_t isp_thread;
  pthread_create(&isp_thread, NULL, __ISP_THREAD__, (void*)vi_pipe_id);
  
  // - Open socket handle
  int socket_handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  struct sockaddr_in dst_addr;
  dst_addr.sin_family       = AF_INET;
  dst_addr.sin_port         = htons(udp_sink_port);
  dst_addr.sin_addr.s_addr  = udp_sink_ip;
  
  
  // - Experiment with AE
  { ISP_EXPOSURE_ATTR_S attr;
    /*HI_MPI_ISP_GetExposureAttr(vi_pipe_id, &attr);
    attr.u8AERunInterval = 1;
    attr.stAuto.stAEDelayAttr.u16BlackDelayFrame = 1;
    attr.stAuto.stAEDelayAttr.u16WhiteDelayFrame = 1;
    attr.stAuto.u8Speed = 255;
    attr.stAuto.bWDRQuick = HI_TRUE;
    HI_MPI_ISP_SetExposureAttr(vi_pipe_id, &attr);*/
  }
  
  // --------------------------------------------------------------
  // --- Start polling
  // --------------------------------------------------------------
  printf("> Ready for streaming\n");
  while (1) {
    // - Process stream on encoder channel #1
    processStream(venc_second_ch_id, socket_handle, (struct sockaddr*)&dst_addr);
    
    // --- Take a rest.
    // - Another way: HI_MPI_VENC_GetFd(vecn_channel_id) + epoll
    usleep(1);
  }
  
  return 0;
}

/**
 * @brief Service thread for ISP
 * @param param - VI pipe identifier
 */
void* __ISP_THREAD__(void *param) {
  HI_MPI_ISP_Run((VI_PIPE)param);
}

void processStream(VENC_CHN channel_id, int socket_handle, struct sockaddr* dst_address) {
  // - Get channel status
  VENC_CHN_STATUS_S channel_status;
  HI_MPI_VENC_QueryStatus(channel_id, &channel_status);
  
  // - Check if has encoded data
  if (!channel_status.u32CurPacks) {
    // - Nothing to get
    return;
  }
  
  // --- Stream packets descriptors
  // - Per-Slice mode always return one slice at a time.
  // - Per-Frame mode may return multiple slices, we need to allocate memory for all reported slices.
  VENC_PACK_S   packet_descriptor[4];
  
  // - Stream buffer 
  VENC_STREAM_S stream;
  memset(&stream, 0x00, sizeof(stream));
  stream.pstPack      = packet_descriptor;
  stream.u32PackCount = 4;
  
  // - Acquire stream
  HI_MPI_VENC_GetStream(channel_id, &stream, 0);

  // - Send encoded packets
  for (uint32_t i = 0; i < stream.u32PackCount; i++) {
    printf("> SEQ: %d : %d, Size = %d, NAL = ", stream.u32Seq,  i, stream.pstPack[i].u32Len  - stream.pstPack[i].u32Offset);
    sendPacket(
      stream.pstPack[i].pu8Addr + stream.pstPack[i].u32Offset, 
      stream.pstPack[i].u32Len  - stream.pstPack[i].u32Offset,
      socket_handle, dst_address, 1024
    );
  }
  
  // - Release stream
  HI_MPI_VENC_ReleaseStream(channel_id, &stream);
}

void sendPacket(uint8_t* pack_data, uint32_t pack_size, int socket_handle, struct sockaddr* dst_address, uint32_t max_size) {
  // - Detect h264 NAL prefix
  if (pack_data[0] == 0 && pack_data[1] == 0 && pack_data[2] == 0 && pack_data[3] == 1) {
    pack_data += 4;
    pack_size -= 4;
    
    // - Get NAL type
    uint8_t nal_type  = pack_data[0] & 0x1F;
    printf("%d\n", nal_type);
  
    // - Check if fragmentation needed
    if (pack_size > max_size) {
      uint32_t  nal_size  = pack_size;
      uint8_t   nal_bits  = pack_data[0] & 0xE0;
      
      int is_first  = 1;
      int is_last   = 0;
      
      uint32_t bytes_sent = 0;
      while (pack_size) {
        uint32_t chunk_size = pack_size > max_size ? max_size : pack_size;
        is_last = (chunk_size == pack_size) ? 1 : 0;

        if (is_first) {
          // - Copy NAL bits and set frag type
          pack_data[-1] = nal_bits | 28;
        
          // - Set flags
          pack_data[0] = 0x80 | nal_type;
          
        } else {
          pack_data[-2] = nal_bits | 28;
          pack_data[-1] = nal_type;
          
          if (is_last == 1) pack_data[-1] |= 0x40;
        }

        sendto(socket_handle, pack_data - (is_first ? 1 : 2), chunk_size + (is_first ? 1 : 2), 0, dst_address, sizeof(struct sockaddr_in));
        
        pack_data   += chunk_size;
        bytes_sent  += chunk_size;
        pack_size   -= chunk_size;
        
        // - Reset first marker
        is_first = 0;
      }
      
    } else {
      sendto(socket_handle, pack_data, pack_size, 0, dst_address, sizeof(struct sockaddr_in));
    }
  
  } else {
    printf("*** Unknown NAL prefix\n");
  }
}
