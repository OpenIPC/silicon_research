#include "main.h"

typedef struct hiHDMI_ARGS_S
{
    HI_HDMI_ID_E  enHdmi;
}HDMI_ARGS_S;
HDMI_ARGS_S      g_stHdmiArgs;
HI_HDMI_CALLBACK_FUNC_S g_stCallbackFunc;


static HI_VOID HDMI_HotPlug_Proc(HI_VOID *pPrivateData) {
    HDMI_ARGS_S    stArgs;
    HI_S32         s32Ret = HI_FAILURE;
    HI_HDMI_ATTR_S stAttr;
    HI_HDMI_SINK_CAPABILITY_S stCaps;

    //printf("EVENT: HPD\n");

    //CHECK_POINTER_NO_RET(pPrivateData);

    memset(&stAttr, 0, sizeof(HI_HDMI_ATTR_S));
    memset(&stArgs, 0, sizeof(HDMI_ARGS_S));
    memcpy(&stArgs, pPrivateData, sizeof(HDMI_ARGS_S));
    memset(&stCaps, 0, sizeof(HI_HDMI_SINK_CAPABILITY_S));


    s32Ret = HI_MPI_HDMI_GetSinkCapability(stArgs.enHdmi, &stCaps);
    if(s32Ret != HI_SUCCESS)
    {
        printf("get sink caps failed!\n");
    }
    else
    {
        printf("get sink caps success!\n");
        printf("Video latency: %d\n", stCaps.u8Video_Latency);
        
        printf("Native format = %d\n", stCaps.enNativeVideoFormat);
        
        for (int i = 0; i < 49; i++) {
          printf("FMT #%d = %s\n", i, stCaps.bVideoFmtSupported[i] ? "YES" : "-");
        }
    }

    s32Ret = HI_MPI_HDMI_GetAttr(stArgs.enHdmi, &stAttr);
    //CHECK_RET_SUCCESS_NO_RET(s32Ret);

    s32Ret = HI_MPI_HDMI_SetAttr(stArgs.enHdmi, &stAttr);
    //CHECK_RET_SUCCESS_NO_RET(s32Ret);

    HI_MPI_HDMI_Start(stArgs.enHdmi);

    return;
}

static HI_VOID HDMI_UnPlug_Proc(HI_VOID *pPrivateData)
{
    HDMI_ARGS_S  stArgs;

    //printf("EVENT: UN-HPD\n");

    //CHECK_POINTER_NO_RET(pPrivateData);

    memset(&stArgs, 0, sizeof(HDMI_ARGS_S));
    memcpy(&stArgs, pPrivateData, sizeof(HDMI_ARGS_S));

    HI_MPI_HDMI_Stop(stArgs.enHdmi);

    return;
}

static HI_VOID HDMI_EventCallBack(HI_HDMI_EVENT_TYPE_E event, HI_VOID *pPrivateData)
{
    switch ( event )
    {
        case HI_HDMI_EVENT_HOTPLUG:
            HDMI_HotPlug_Proc(pPrivateData);
            break;
        case HI_HDMI_EVENT_NO_PLUG:
            HDMI_UnPlug_Proc(pPrivateData);
            break;
        default:
            break;
    }

    return;
}


void printHelp() {
  printf(
    "\n"
    "\t\tOpenIPC FPV Decoder for HI3536 v23.09-27\n"
    "\n"
    "  Usage:\n"
    "    vdec [Arguments]\n"
    "\n"
    "  Arguments:\n"
    "    -p [Port]      - Listen port                       (Default: 5000)\n"
    "    -c [Codec]     - Video codec                       (Default: h264)\n"
    "      h264           - H264\n"
    "      h265           - H265\n"
    "\n"
    "    -d [Format]    - Data format                       (Default: stream)\n"
    "      stream         - Incoming data is stream\n"
    "      frame          - Incoming data is frame\n"
    "\n"
    "    -m [Mode]      - Screen output mode                (Default: 720p60)\n"
    "      720p60         - 1280 x 720    @ 60 fps\n"
    "      1080p60        - 1920 x 1080   @ 60 fps\n"
    "      1024x768x60    - 1024 x 768    @ 60 fps\n" 
    "      1366x768x60    - 1366 x 768    @ 60 fps\n"
    "      1280x1024x60   - 1280 x 1024   @ 60 fps\n" 
    "\n"
    "    -w [Path]      - Write stream into file\n"
    "\n"
    "    --osd          - Enable OSD\n"
    "    --bg-r [Value] - Background color red component    (Default: 0)\n"
    "    --bg-g [Value] - Background color green component  (Default: 96)\n"
    "    --bg-b [Value] - Background color blue component   (Default: 0)\n"
    "\n"
  );
}

extern uint32_t frames_received;


uint32_t stats_rx_bytes = 0;
struct timespec last_timestamp = {0, 0};

double getTimeInterval(struct timespec* timestamp, struct timespec* last_meansure_timestamp) {
  return (timestamp->tv_sec - last_meansure_timestamp->tv_sec) + (timestamp->tv_nsec - last_meansure_timestamp->tv_nsec) / 1000000000.;
}


int main(int argc, const char* argv[]) {
  VO_INTF_SYNC_E  vo_mode           = VO_OUTPUT_720P60;// VO_OUTPUT_1080P60; //VO_OUTPUT_2560x1440_30; //VO_OUTPUT_2560x1440_30;// VO_OUTPUT_1080P60;
  uint32_t        vo_width          = 1280;
  uint32_t        vo_height         = 720;
  uint32_t        vo_framerate      = 60;
  
  VDEC_CHN        vdec_channel_id   = 0;
  uint32_t        vdec_max_width    = 1920;
  uint32_t        vdec_max_height   = 1080;
  
  VPSS_GRP        vpss_group_id     = 0;
  VPSS_CHN        vpss_channel_id   = 0;
  
  VO_DEV          vo_device_id      = 0;
  VO_LAYER        vo_layer_id       = 0;
  VO_CHN          vo_channel_id     = 0;
  
  uint16_t        listen_port       = 5000;
  uint32_t        background_color  = 0x006000;
  
  const char*     write_stream_path = 0;
  int             write_stream_file = -1;
  
  int             enable_osd        = 0;

  PAYLOAD_TYPE_E  codec_id          = PT_H264;
  
  int             codec_mode_stream = 1;
  
  // --------------------------------------------------------------
  // --- Load console arguments
  // --------------------------------------------------------------
  __BeginParseConsoleArguments__(printHelp)
    __OnArgument("-p") {
      listen_port = atoi(__ArgValue);
      continue;
    }
    __OnArgument("-c") {
      const char* codec = __ArgValue;
      if        (!strcmp(codec, "h264")) {
        codec_id = PT_H264;
      } else if (!strcmp(codec, "h265")) {
        codec_id = PT_H265;
      } else {
        printf("> ERROR: Unsupported video codec [%s]\n", codec);
      }
      continue;
    }
    __OnArgument("-d") {
      const char* format = __ArgValue;
      if        (!strcmp(format, "stream")) {
        codec_mode_stream = 1;
      } else if (!strcmp(format, "frame")) {
        codec_mode_stream = 0;
      } else {
        printf("> ERROR: Unsupported data format [%s]\n", format);
      }
      continue;
    }
    __OnArgument("-m") {
      const char* mode = __ArgValue;
      if        (!strcmp(mode, "720p60")) {
        vo_mode       = VO_OUTPUT_720P60;
        vo_width      = 1280;
        vo_height     = 720;
        vo_framerate  = 60;
      } else if (!strcmp(mode, "1080p60")) {
        vo_mode       = VO_OUTPUT_1080P60;
        vo_width      = 1920;
        vo_height     = 1080;
        vo_framerate  = 60;
      } else if (!strcmp(mode, "1024x768x60")) {
        vo_mode       = VO_OUTPUT_1024x768_60;
        vo_width      = 1024;
        vo_height     = 768;
        vo_framerate  = 60;
      } else if (!strcmp(mode, "1366x768x60")) {
        vo_mode       = VO_OUTPUT_1366x768_60;
        vo_width      = 1024;
        vo_height     = 768;
        vo_framerate  = 60;
      } else if (!strcmp(mode, "1280x1024x60")) {
        vo_mode       = VO_OUTPUT_1280x1024_60;
        vo_width      = 1280;
        vo_height     = 1024;
        vo_framerate  = 60;
      }else {
        printf("> ERROR: Unsupported video mode [%s]\n", mode);
      }
      continue;
    }
    
    __OnArgument("--bg-r") {
      uint8_t v = atoi(__ArgValue);
      background_color &= ~(0xFF << 16);
      background_color |= (v << 16);
      continue;
    }
    
    __OnArgument("--bg-g") {
      uint8_t v = atoi(__ArgValue);
      background_color &= ~(0xFF << 8);
      background_color |= (v << 8);
      continue;
    }
    
    __OnArgument("--bg-b") {
      uint8_t v = atoi(__ArgValue);
      background_color &= ~(0xFF);
      background_color |= (v);
      continue;
    }
    
    __OnArgument("-w") {
      write_stream_path = __ArgValue;
      continue;
    }
    
    __OnArgument("--osd") {
      enable_osd = 1;
      continue;
    }
    
  __EndParseConsoleArguments__
  // --------------------------------------------------------------
  // --- Reset previous configuration
  // --------------------------------------------------------------
  { int ret = HI_MPI_SYS_Exit();
  
    for (uint32_t i = 0; i < VB_MAX_USER; i++) {
      ret = HI_MPI_VB_ExitModCommPool(i);
    }
    
    for (uint32_t i = 0; i < VB_MAX_POOLS; i++) {
      ret = HI_MPI_VB_DestroyPool(i);
    }

    ret = HI_MPI_VB_Exit();
  }
  
  
  // --------------------------------------------------------------
  // --- Setup memory pools and initialize system
  // --------------------------------------------------------------
  { VB_CONF_S vb_conf;
    memset(&vb_conf, 0x00, sizeof(vb_conf));
    
    // - Set maximum memory pools count
    vb_conf.u32MaxPoolCnt = 16;
 
    // - Configure video buffer
    { int ret = HI_MPI_VB_SetConf(&vb_conf);
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
  }
  
  // -- Configure system
  { MPP_SYS_CONF_S sys_config; 
    sys_config.u32AlignWidth = 16;
    int ret = HI_MPI_SYS_SetConf(&sys_config);
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
  
  // - Configure video buffer for decoder
  { VB_CONF_S vb_conf;
  
    // - Release previous pool configuration
    int ret =  HI_MPI_VB_ExitModCommPool(VB_UID_VDEC);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to release VDEC memory pool\n");
      return 1;
    }
    
    vb_conf.u32MaxPoolCnt             = 1;
    vb_conf.astCommPool[0].u32BlkCnt  = 4;
    vb_conf.astCommPool[0].u32BlkSize = vdec_max_width * vdec_max_height * 3;
    
    // - Calculate required size for video buffer
    VB_PIC_BLK_SIZE(vdec_max_width, vdec_max_height, codec_id, vb_conf.astCommPool[0].u32BlkSize);
  
    printf("> VDEC picture block size = %d\n", vb_conf.astCommPool[0].u32BlkSize);
  
    ret = HI_MPI_VB_SetModPoolConf(VB_UID_VDEC, &vb_conf);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to configure ModComPool\n");
      return 1;
    }
    
    ret = HI_MPI_VB_InitModCommPool(VB_UID_VDEC);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to init ModComPool = 0x%x\n", ret);
      return 1;
    }
  }
  

  {
    int ret = HI_MPI_VO_SetDispBufLen(vo_layer_id, 2);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to set display buffer length\n");
      return 1;
    }
  }
  
  // - Initialize VO
  VO_init(vo_device_id, VO_INTF_HDMI, vo_mode, vo_framerate, background_color);

  // - Initialize HDMI
  VO_HDMI_init(0, vo_mode);

 /*{ VO_PUB_ATTR_S vo_config;
    memset(&vo_config, 0x00, sizeof(VO_PUB_ATTR_S));
    
    int ret = HI_MPI_VO_GetPubAttr(vo_device_id, &vo_config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to GET video timing = 0x%x\n");
      return 1;
    }
    
    printf("> Video timing for mode = %d:\n"
      "bSynm      = %d\n"
      "\n"
      "bIop       = %d\n"
      "u8Intfb    = %d\n"
      "\n"
      "u16Vact    = %d\n"
      "u16Vbb     = %d\n"
      "u16Vfb     = %d\n"
      "\n"
      "u16Hact    = %d\n"
      "u16Hbb     = %d\n"
      "u16Hfb     = %d\n"
      "u16Hmid    = %d\n"
      "\n"
      "u16Bvact   = %d\n"
      "u16Bvbb    = %d\n"
      "u16Bvfb    = %d\n"
      "\n"
      "u16Hpw     = %d\n"
      "u16Vpw     = %d\n"
      "\n"
      "bIdv       = %d\n"
      "bIhs       = %d\n"
      "bIvs       = %d\n"
    
      ,
      vo_config.enIntfType,
      vo_config.stSyncInfo.bSynm,
      
      vo_config.stSyncInfo.bIop,
      vo_config.stSyncInfo.u8Intfb,
      
      vo_config.stSyncInfo.u16Vact,
      vo_config.stSyncInfo.u16Vbb,
      vo_config.stSyncInfo.u16Vfb,
      
      vo_config.stSyncInfo.u16Hact,
      vo_config.stSyncInfo.u16Hbb,
      vo_config.stSyncInfo.u16Hfb,
      vo_config.stSyncInfo.u16Hmid,
      
      vo_config.stSyncInfo.u16Bvact,
      vo_config.stSyncInfo.u16Bvbb,
      vo_config.stSyncInfo.u16Bvfb,

      vo_config.stSyncInfo.u16Hpw,
      vo_config.stSyncInfo.u16Vpw,

      vo_config.stSyncInfo.bIdv,
      vo_config.stSyncInfo.bIhs,
      vo_config.stSyncInfo.bIvs
      
    );
  }*/
  
  { int ret = HI_MPI_VO_GetDevFrameRate(vo_device_id, &vo_framerate);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to set VO framerate = 0x%x\n");
      return ret;
    }
    printf("> VO framerate = %d\n", vo_framerate);
  }

  // - Configure video layer
  { VO_VIDEO_LAYER_ATTR_S layer_config;
    memset(&layer_config, 0x00, sizeof(layer_config));
    int ret = HI_MPI_VO_GetVideoLayerAttr(vo_layer_id, &layer_config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to get VO layer information\n");
      return 1;
    }
    
    layer_config.enPixFormat  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    layer_config.bDoubleFrame = HI_FALSE;
    layer_config.bClusterMode = HI_TRUE;
    
    layer_config.u32DispFrmRt = vo_framerate;
    
    layer_config.stDispRect.u32Width    = vo_width;
    layer_config.stDispRect.u32Height   = vo_height;
    layer_config.stDispRect.s32X        = 0;
    layer_config.stDispRect.s32Y        = 0;
    
    layer_config.stImageSize.u32Width   = vo_width;
    layer_config.stImageSize.u32Height  = vo_height;
    
    ret = HI_MPI_VO_SetVideoLayerAttr(vo_layer_id, &layer_config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to configure video layer = 0x%x\n", ret);
      return 1;
    }
  }
  
  // - Enable video layer
  { int ret = HI_MPI_VO_EnableVideoLayer(vo_layer_id);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to enable video layer = 0x%x\n", ret);
      return 1;
    }
  }
  
  // - Configure video channel
  { VO_CHN_ATTR_S channel_config;
    int ret = HI_MPI_VO_GetChnAttr(vo_layer_id, vo_channel_id, &channel_config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to get channel configuration\n");
      return 1;
    }
    
    channel_config.bDeflicker       = HI_FALSE;
    channel_config.u32Priority      = 0;
    
    channel_config.stRect.s32X      = 0;
    channel_config.stRect.s32Y      = 0;
    channel_config.stRect.u32Width  = vo_width;
    channel_config.stRect.u32Height = vo_height;
    
    ret = HI_MPI_VO_SetChnAttr(vo_layer_id, vo_channel_id, &channel_config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to configure channel\n");
      return 1;
    }
    
    ret = HI_MPI_VO_EnableChn(vo_layer_id, vo_channel_id);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to enable video channel\n");
      return 1;
    }
    
    /*uint32_t rate = 0;
    HI_MPI_VO_GetChnFrameRate(vo_layer_id, vo_channel_id, &rate);
    printf("Channel framerate = %d\n", rate);*/
  }
  
  // --------------------------------------------
  // --- Start VDEC
  // --------------------------------------------
  // - Start decode at 1920x1080 maximum resolution
  { VDEC_CHN_ATTR_S config;
    memset(&config, 0x00, sizeof(config));
    config.enType       = codec_id;
    config.u32BufSize   = vdec_max_width * vdec_max_height * 3; // vdec_max_width * vdec_max_height;
    config.u32Priority  = 128;
    config.u32PicWidth  = vdec_max_width;
    config.u32PicHeight = vdec_max_height;  

    config.stVdecVideoAttr.bTemporalMvpEnable = HI_FALSE;
    config.stVdecVideoAttr.enMode             = codec_mode_stream ? VIDEO_MODE_STREAM : VIDEO_MODE_FRAME;
    config.stVdecVideoAttr.u32RefFrameNum     = 1;

    // - Create VDEC channel
    int ret = HI_MPI_VDEC_CreateChn(vdec_channel_id, &config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to create VDEC channel\n");
      return 1;
    }
    
    //- Set channel extra parameters
    /*{ VDEC_CHN_PARAM_S channel;
      ret = HI_MPI_VDEC_GetChnParam(vdec_channel_id, &channel);
      if (ret != HI_SUCCESS) {
        printf("ERROR: Unable to get VDEC channel parameters\n");
        return 1;
      }
      
      channel.s32DisplayFrameNum  = 2;
      channel.s32ChanErrThr       = 40;
      channel.s32ChanStrmOFThr    = 0;
      channel.s32DecMode          = 2;
      channel.s32DecOrderOutput   = 1;
      channel.enVideoFormat       = VIDEO_FORMAT_TILE64;
      channel.enCompressMode      = COMPRESS_MODE_NONE;
      
      ret = HI_MPI_VDEC_SetChnParam(vdec_channel_id, &channel);
      if (ret != HI_SUCCESS) {
        printf("ERROR: Unable to set VDEC channel config\n");
        return 1;
      }
    }*/
  
    // - Set display mode
    ret = HI_MPI_VDEC_SetDisplayMode(vdec_channel_id, VIDEO_DISPLAY_MODE_PREVIEW);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to set VDEC display mode\n");
      return 1;
    }
    
    // - Read decoder protocol information
    { VDEC_PRTCL_PARAM_S protocol;
      HI_MPI_VDEC_GetProtocolParam(vdec_channel_id, &protocol);

      switch (protocol.enType) {
        case PT_H264: {
          // - Configure video stream
          protocol.stH264PrtclParam.s32MaxPpsNum    = 32;
          protocol.stH264PrtclParam.s32MaxSpsNum    = 32;
          protocol.stH264PrtclParam.s32MaxSliceNum  = 32;

          ret = HI_MPI_VDEC_SetProtocolParam(vdec_channel_id, &protocol);
          if (ret != HI_SUCCESS) {
            printf("ERROR: Unable to set VDEC protocol parameters\n");
            return 1;
          }

          HI_MPI_VDEC_GetProtocolParam(vdec_channel_id, &protocol);
          printf("> VDEC Protocol = Type: %s, PPS: %d, SLICE: %d, SPS: %d\n",
            (codec_id == PT_H264 ? "H264" : (codec_id == PT_H265 ? "H265" : "Unknown")),
            protocol.stH264PrtclParam.s32MaxPpsNum,
            protocol.stH264PrtclParam.s32MaxSliceNum,
            protocol.stH264PrtclParam.s32MaxSpsNum
          );


        }; break;

        case PT_H265: {
          // - Configure video stream
          protocol.stH265PrtclParam.s32MaxPpsNum          = 16;
          protocol.stH265PrtclParam.s32MaxSpsNum          = 16;
          protocol.stH265PrtclParam.s32MaxSliceSegmentNum = 16;

          ret = HI_MPI_VDEC_SetProtocolParam(vdec_channel_id, &protocol);
          if (ret != HI_SUCCESS) {
            printf("ERROR: Unable to set VDEC protocol parameters\n");
            return 1;
          }

          HI_MPI_VDEC_GetProtocolParam(vdec_channel_id, &protocol);
          printf("> VDEC Protocol = Type: %s, PPS: %d, SLICE: %d, SPS: %d\n",
            (codec_id == PT_H264 ? "H264" : (codec_id == PT_H265 ? "H265" : "Unknown")),
            protocol.stH265PrtclParam.s32MaxPpsNum,
            protocol.stH265PrtclParam.s32MaxSliceSegmentNum,
            protocol.stH265PrtclParam.s32MaxSpsNum
          );
          
        }; break;

        default:
          break;
      }


          
    }
  }

  // --------------------------------------------
  // --- Assemble pipeline
  // --------------------------------------------
  { MPP_CHN_S src;
    MPP_CHN_S dst;

    src.enModId   = HI_ID_VDEC;
    src.s32DevId  = 0;
    src.s32ChnId  = vdec_channel_id;

    dst.enModId   = HI_ID_VOU;
    dst.s32DevId  = vo_layer_id;
    dst.s32ChnId  = vo_channel_id;

    int ret = HI_MPI_SYS_Bind(&src, &dst);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to bind VDEC -> VO\n");
      return 1;
    }
  }
  
  // - Start VDEC
  { int ret = HI_MPI_VDEC_StartRecvStream(vdec_channel_id);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to start VDEC channel\n");
      return 1;
    }
  }
  
  // --------------------------------------------
  // --- Create socket
  // --------------------------------------------
  int port = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  { struct sockaddr_in address;
    memset(&address, 0x00, sizeof(address));
    address.sin_family  = AF_INET;
    address.sin_port    = htons(listen_port);
    bind(port, (struct sockaddr*)&address, sizeof(struct sockaddr_in));
  }
  
  if (fcntl(port, F_SETFL, O_NONBLOCK) == -1) {
    printf("ERROR: Unable to set non-blocking mode\n");
    return 1;
  }
  
  uint8_t*  rx_buffer       = malloc(1024 * 1024);
  uint8_t*  nal_buffer      = malloc(1024 * 1024);
  uint32_t  nal_buffer_used = 0;
  
  // - Open write file
  if (write_stream_path) {
    write_stream_file = open(write_stream_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  }
    
  // - Start ISP service thread
  pthread_t osd_thread;
  if (enable_osd) {
    pthread_create(&osd_thread, NULL, __OSD_THREAD__, 0);
  }

  
  uint32_t  write_buffer_capacity = 1024 * 1024 * 2;
  uint8_t*  write_buffer = malloc(write_buffer_capacity);
  uint32_t  write_buffer_size = 0;

  while (1) {
    int rx = recv(port, rx_buffer + 8, 2048, 0);
    if (rx <= 0) {
      usleep(1);
      continue;
    }

    VDEC_STREAM_S stream;
    memset(&stream, 0x00, sizeof(stream));
    stream.bEndOfStream = HI_FALSE;
    stream.bEndOfFrame  = codec_mode_stream ? HI_FALSE : HI_TRUE;

    // - Decode UDP stream
    stream.pu8Addr = decodeUDPFrame(rx_buffer + 8, (uint32_t)rx, 0, nal_buffer, &nal_buffer_used, &stream.u32Len);
    if (!stream.pu8Addr) {
      continue;
    }
    if (stream.u32Len < 5) {
      printf("> Broken frame\n");
    }

    uint8_t nal_type = stream.pu8Addr[4]  & 0x1F;
    //printf("NAL Type = %d, Size = %d\n", nal_type, stream.u32Len - 4);
    
    stats_rx_bytes += stream.u32Len;
    
    // - Write file
    if (write_stream_file != -1) {
      uint32_t  write_size = stream.u32Len;
      uint32_t  write_done = 0;
      
      #define MIN(a,b) (((a)<(b))?(a):(b))
      while (write_size) {
        uint32_t chunk_size = MIN(write_size, write_buffer_capacity - write_buffer_size);
        
        memcpy(write_buffer + write_buffer_size, stream.pu8Addr + write_done,  chunk_size);
        write_buffer_size += chunk_size;
        
        // - Flush buffer
        if (write_buffer_size == write_buffer_capacity) {
          write(write_stream_file, write_buffer, write_buffer_size);
          write_buffer_size = 0;
        }
          
        write_size -= chunk_size;
        write_done += chunk_size;
      }
    }

    
    // - Send frame into decoder
    { int ret = HI_MPI_VDEC_SendStream(vdec_channel_id, &stream, 0);
      if (ret != HI_SUCCESS) {
        printf("WARN: Unable to send data into VDEC = 0x%x\n", ret);
      }
    }
  

    // - Query decoder status
    /*{ VDEC_CHN_STAT_S stats;
      memset(&stats, 0x00, sizeof(stats));
      int ret = HI_MPI_VDEC_Query(vdec_channel_id, &stats);
      if (ret != HI_SUCCESS) {
        printf("WARN: Unable to query statistics = 0x%x\n", ret);
        
      } else {
        printf("> R: %d, RxFrames: %d, DecFrames: %d | BufSize: %d, BufPic: %d | E_SUP: %d, E_FMT: %d, E_PAK: %d, E_PBUF: %d, E_PSIZE: %d, E_PRTCL: %d, E_REF: %d\n",
          stats.bStartRecvStream,
          stats.u32RecvStreamFrames,
          stats.u32DecodeStreamFrames,
          stats.u32LeftStreamBytes,
          stats.u32LeftPics,
          
          stats.stVdecDecErr.s32StreamUnsprt,
          stats.stVdecDecErr.s32FormatErr,
          stats.stVdecDecErr.s32PackErr,
          stats.stVdecDecErr.s32PicBufSizeErrSet,
          stats.stVdecDecErr.s32PicSizeErrSet,
          stats.stVdecDecErr.s32PrtclNumErrSet,
          stats.stVdecDecErr.s32RefErrSet
        );
      }
    }*/
    
    /*{ VIDEO_FRAME_INFO_S frame;
      int ret = HI_MPI_VDEC_GetImage(vdec_channel_id, &frame, 0);
      if (ret == 0) {
        
        printf("> FRAME: %d x %d\n", frame.stVFrame.u32Width, frame.stVFrame.u32Height);
        
        HI_MPI_VO_SendFrame(vo_layer_id, vo_channel_id, &frame, 0);
        
        // - Release frame
        HI_MPI_VDEC_ReleaseImage(vdec_channel_id, &frame);
      } else {
        printf("> NO frame = 0x%x\n", ret);
      }
    }*/
  }

  return 0;
}

extern const char font_14_23[];
extern uint32_t frames_lost;

float rx_rate = 0;

// array size is 7602
extern const char openipc[];

void* __OSD_THREAD__(void* arg) {
  struct _fbg *fbg = fbg_fbdevInit();
  
  
  //struct _fbg_img *texture = fbg_loadImage(fbg, "texture.png");
  
  
  struct _fbg_img *bb_font_img = fbg_loadPNGFromMemory(fbg, font_14_23, 26197);
  struct _fbg_font *bbfont = fbg_createFont(fbg, bb_font_img, 14, 23, 33);


  struct _fbg_img* openipc_img = fbg_loadPNGFromMemory(fbg, openipc, 11761);

  while (1) {
    fbg_clear(fbg, 0);
    fbg_draw(fbg);
    
    uint32_t x_center = fbg->width / 2;
    
    
    for (int i = 0; i < 4; i++) {
      fbg_line(fbg, x_center - 180, fbg->height / 2 + i - 2, x_center - 60, fbg->height / 2 + i - 2, 255,255,255);
      fbg_line(fbg, x_center + 60, fbg->height / 2 + i - 2, x_center + 180, fbg->height / 2 + i - 2, 255,255,255);
    }
    
    for (int i = 0; i < 25; i++) {
      
      uint32_t width = (i == 12) ? 10 : 0;
      
      fbg_line(fbg, x_center - 240 - width, fbg->height / 2 - 120 + i * 10,       x_center - 220, fbg->height / 2 - 120 + i * 10, 255,255,255);
      fbg_line(fbg, x_center - 240 - width, fbg->height / 2 - 120  + i * 10 + 1,  x_center - 220, fbg->height / 2 - 120 + i * 10 + 1, 255,255,255);

      fbg_line(fbg, x_center + 220, fbg->height / 2 - 120 + i * 10,       x_center + 240 + width, fbg->height / 2 - 120 + i * 10, 255,255,255);
      fbg_line(fbg, x_center + 220, fbg->height / 2 - 120  + i * 10 + 1,  x_center + 240 + width, fbg->height / 2 - 120 + i * 10 + 1, 255,255,255);
    }
    
    fbg_write(fbg, "SPD", x_center - (16 * 3 + 20) - 240, fbg->height / 2 - 8);
    fbg_write(fbg, "ALT", x_center + ( 20) + 240,         fbg->height / 2 - 8);
    
    //fbg_imageClip(fbg, openipc, x_center - 80, 700, 0, 0, 160, 46);
    fbg_image(fbg, openipc_img, 1280 - 160 - 80, 40);
    //fbg_write(fbg, "OpenIPC FPV", 60, 40);
   
    
    

    // - Print rate stats
    struct timespec current_timestamp;
    if (!clock_gettime(CLOCK_MONOTONIC_COARSE, &current_timestamp)) {
      double interval = getTimeInterval(&current_timestamp, &last_timestamp);
      if (interval > 1) {
        last_timestamp = current_timestamp;
        rx_rate = (float)stats_rx_bytes / 1024.0f * 8; 
        stats_rx_bytes = 0;
      }
    }
  
    char hud_frames_rx[32];
    memset(hud_frames_rx, 0, sizeof(hud_frames_rx));
    sprintf(hud_frames_rx, "RX Packets %d", frames_received);
    fbg_write(fbg, hud_frames_rx, 60, 660);
    
    
    

    memset(hud_frames_rx, 0, sizeof(hud_frames_rx));
    sprintf(hud_frames_rx, "Rate %.02f Kbit/s", rx_rate);
    
    
    
    fbg_write(fbg, hud_frames_rx, x_center - strlen(hud_frames_rx) / 2 * 16, 40);
    
    float percent = rx_rate / (1024 * 10);
    if (percent > 1) percent = 1;
    
    uint32_t width =  (strlen(hud_frames_rx) * 16) * percent;
    
    fbg_rect(fbg, x_center - strlen(hud_frames_rx) / 2 * 16, 64, width, 8,  255, 255, 255);
    
    
    fbg_flip(fbg);
    usleep(1);
  }
}
