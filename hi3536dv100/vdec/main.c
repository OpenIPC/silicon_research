#include "main.h"

static HI_VOID SAMPLE_COMM_VO_HdmiConvertSync(VO_INTF_SYNC_E enIntfSync, HI_HDMI_VIDEO_FMT_E *penVideoFmt) {
    switch (enIntfSync) {
        case VO_OUTPUT_PAL:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_PAL;
            break;
        case VO_OUTPUT_NTSC:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_NTSC;
            break;
        case VO_OUTPUT_1080P24:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_24;
            break;
        case VO_OUTPUT_1080P25:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_25;
            break;
        case VO_OUTPUT_1080P30:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_30;
            break;
        case VO_OUTPUT_720P50:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_720P_50;
            break;
        case VO_OUTPUT_720P60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_720P_60;
            break;
        case VO_OUTPUT_1080I50:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080i_50;
            break;
        case VO_OUTPUT_1080I60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080i_60;
            break;
        case VO_OUTPUT_1080P50:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_50;
            break;
        case VO_OUTPUT_1080P60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_60;
            break;
        case VO_OUTPUT_576P50:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_576P_50;
            break;
        case VO_OUTPUT_480P60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_480P_60;
            break;
        case VO_OUTPUT_800x600_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_800X600_60;
            break;
        case VO_OUTPUT_1024x768_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1024X768_60;
            break;
        case VO_OUTPUT_1280x1024_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1280X1024_60;
            break;
        case VO_OUTPUT_1366x768_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1366X768_60;
            break;
        case VO_OUTPUT_1440x900_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1440X900_60;
            break;
        case VO_OUTPUT_1280x800_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1280X800_60;
            break;
        case VO_OUTPUT_1600x1200_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1600X1200_60;
            break;
        case VO_OUTPUT_2560x1440_30:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_2560x1440_30;
            break;
        case VO_OUTPUT_2560x1600_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_2560x1600_60;
            break;
        case VO_OUTPUT_3840x2160_30:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_3840X2160P_30;
            break;
        default :
            //SAMPLE_PRT("Unkonw VO_INTF_SYNC_E value!\n");
            break;
    }

    return;
}

#define HI_HDMI_ID_0 0
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

int main(int argc, const char* argv[]) {
  
  VO_INTF_SYNC_E  vo_mode     = VO_OUTPUT_720P60;
  uint32_t        vo_width    = 1280;
  uint32_t        vo_height   = 720;
  
  
  
  
  uint32_t  sensor_width      = 2592;
  uint32_t  sensor_height     = 1520;
  uint32_t  sensor_framerate  = 30;
  
  uint32_t  image_width       = 2592; // - Encoded image width
  uint32_t  image_height      = 1520; // - Encoded image height
  
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
    
    // - Use two memory pools
    vb_conf.u32MaxPoolCnt = 16;
    
    // - Memory pool for VI
    vb_conf.astCommPool[0].u32BlkCnt  = 5; 
    vb_conf.astCommPool[0].u32BlkSize = ((sensor_width * sensor_height * 4) >> 1); 
    
    printf("> Main pool block size = %d\n", vb_conf.astCommPool[0].u32BlkSize);
      
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
  
    
  // - Configure video buffer
  { VB_CONF_S vb_conf;
  
    // - Release previous pool configuration
    int ret =  HI_MPI_VB_ExitModCommPool(VB_UID_VDEC);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to release VDEC memory pool\n");
      return 1;
    }
    
    vb_conf.u32MaxPoolCnt             = 2;
    vb_conf.astCommPool[0].u32BlkCnt  = 5;
    vb_conf.astCommPool[0].u32BlkSize = 1280 * 720 * 3;
    
    VB_PIC_BLK_SIZE(1280, 720, PT_H264, vb_conf.astCommPool[0].u32BlkSize);
    
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
  
  VDEC_CHN        vdec_channel_id   = 0;
  
  VPSS_GRP        vpss_group_id     = 0;
  VPSS_CHN        vpss_channel_id   = 0;
  
  
  VO_DEV          vo_device_id  = 0;
  VO_LAYER        vo_layer_id   = 0;
  VO_CHN          vo_channel_id = 0;
  
  {
    int ret = HI_MPI_VO_SetDispBufLen(vo_layer_id, 3);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to set display buffer length\n");
      return 1;
    }
  }
  
  // - Initialize VO
  VO_init(vo_device_id, VO_INTF_HDMI, vo_mode, 0x00FF00);

  // - Initialize HDMI
  //VO_HDMI_init(0, vo_mode);


  // - Configure video layer
  { VO_VIDEO_LAYER_ATTR_S layer_config;
    int ret = HI_MPI_VO_GetVideoLayerAttr(vo_layer_id, &layer_config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to get VO layer information\n");
      return 1;
    }
    
    layer_config.enPixFormat  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    layer_config.bDoubleFrame = HI_FALSE;
    layer_config.bClusterMode = HI_FALSE;
    
    layer_config.u32DispFrmRt = 30;
    
    layer_config.stDispRect.u32Width    = 1280;
    layer_config.stDispRect.u32Height   = 720;
    layer_config.stDispRect.s32X        = 0;
    layer_config.stDispRect.s32Y        = 0;
    
    layer_config.stImageSize.u32Width   = 1280;
    layer_config.stImageSize.u32Height  = 720;
    
    ret = HI_MPI_VO_SetVideoLayerAttr(vo_layer_id, &layer_config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to configure video layer\n");
      return 1;
    }
  }
  
  // - Enable video layer
  { int ret = HI_MPI_VO_EnableVideoLayer(vo_layer_id);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to enable video layer\n");
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
    channel_config.stRect.u32Width  = 1280;
    channel_config.stRect.u32Height = 720;
    
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
    
    //HI_MPI_VO_GetChnFrameRate(vo_layer_id, vo_channel_id, &rate);
    //printf("Channel framerate = %d\n", rate);
  }
  

  
  // --------------------------------------------
  // --- Start VPSS
  // --------------------------------------------
  // - Create VPSS group
  { VPSS_GRP_ATTR_S group_config;
    int ret = HI_MPI_VPSS_GetGrpAttr(vpss_group_id, &group_config);
    
    group_config.u32MaxW    = ALIGN_UP(1280,  16);
    group_config.u32MaxH    = ALIGN_UP(720,   16);
    group_config.enPixFmt   = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    
    group_config.bIeEn      = HI_FALSE;
    group_config.bDciEn     = HI_FALSE;
    group_config.bEsEn      = HI_FALSE;
    group_config.bHistEn    = HI_FALSE;
    group_config.bNrEn      = HI_FALSE;
    
    group_config.enDieMode  = VPSS_DIE_MODE_NODIE;
    
    ret = HI_MPI_VPSS_CreateGrp(vpss_group_id, &group_config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to create VPSS group\n");
    }
  }

  // - Activate channel border
  { VPSS_CHN_ATTR_S channel_config;
    int ret = HI_MPI_VPSS_GetChnAttr(vpss_group_id, vpss_channel_id, &channel_config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to get VPSS channel config\n");
      return 1;
    }
    
    channel_config.bBorderEn                = HI_TRUE;
    channel_config.stBorder.u32TopWidth     = 10;
    channel_config.stBorder.u32BottomWidth  = 10;
    channel_config.stBorder.u32LeftWidth    = 10;
    channel_config.stBorder.u32RightWidth   = 10;
    channel_config.stBorder.u32Color        = 0xFF0000;
    
    ret = HI_MPI_VPSS_SetChnAttr(vpss_group_id, vpss_channel_id, &channel_config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to configure VPSS channel\n");
      return 1;
    }
  }

  // - Start VPSS group 
  { int ret = HI_MPI_VPSS_StartGrp(vpss_group_id);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to start VPSS group\n");
      return 1;
    }
  }

  // - Enable VPSS channel 
  { int ret = HI_MPI_VPSS_EnableChn(vpss_group_id, vpss_channel_id);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to enable VPSS channel\n");
      return 1;
    }
  }
  
  // --------------------------------------------
  // --- Start VDEC
  // --------------------------------------------
  // - Start decode at 1920x1080 maximum resolution
  { VDEC_CHN_ATTR_S config;
    memset(&config, 0x00, sizeof(config));
    config.enType       = PT_H264;
    config.u32BufSize   = vo_width * vo_height * 2;
    config.u32Priority  = 5;
    config.u32PicWidth  = vo_width;
    config.u32PicHeight = vo_height;    
  
    config.stVdecVideoAttr.bTemporalMvpEnable = HI_FALSE;
    config.stVdecVideoAttr.enMode             = VIDEO_MODE_FRAME;
    config.stVdecVideoAttr.u32RefFrameNum     = 2;
    
    // - Create VDEC channel
    int ret = HI_MPI_VDEC_CreateChn(vdec_channel_id, &config);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to create VDEC channel\n");
      return 1;
    }
    
    //- Set channel extra parameters
    { VDEC_CHN_PARAM_S channel;
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
    }
  
    // - Set display mode
    ret = HI_MPI_VDEC_SetDisplayMode(vdec_channel_id, VIDEO_DISPLAY_MODE_PREVIEW);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to set VDEC display mode\n");
      return 1;
    }
    
    // - Read decoder protocol information
    VDEC_PRTCL_PARAM_S protocol;
    HI_MPI_VDEC_GetProtocolParam(vdec_channel_id, &protocol);
    printf("> VDEC Protocol = Type: %s, PPS: %d, SLICE: %d, SPS: %d\n",
      protocol.enType == PT_H264 ? "h264" : "Unknown",
      protocol.stH264PrtclParam.s32MaxPpsNum,
      protocol.stH264PrtclParam.s32MaxSliceNum,
      protocol.stH264PrtclParam.s32MaxSpsNum
    );
    
    /*ret = HI_MPI_VDEC_SetProtocolParam(vdec_channel_id, &protocol);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to set VDEC protocol parameters\n");
      return 1;
    }*/
  }

  // --------------------------------------------
  // --- Assemble pipeline
  // --------------------------------------------
  // - Bind VDEC -> VPSS
  { MPP_CHN_S src;
    MPP_CHN_S dst;

    src.enModId   = HI_ID_VDEC;
    src.s32DevId  = 0;
    src.s32ChnId  = vdec_channel_id;

    dst.enModId   = HI_ID_VPSS;
    dst.s32DevId  = vpss_group_id;
    dst.s32ChnId  = vpss_channel_id;

    int ret = HI_MPI_SYS_Bind(&src, &dst);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to bind VDEC -> VPSS\n");
      return 1;
    }
  }
  
  // - Bind VPSS -> VO
  { MPP_CHN_S src;
    MPP_CHN_S dst;

    src.enModId   = HI_ID_VPSS;
    src.s32DevId  = vpss_group_id;
    src.s32ChnId  = vpss_channel_id;

    dst.enModId   = HI_ID_VOU;
    dst.s32DevId  = vo_layer_id;
    dst.s32ChnId  = vo_channel_id;

    int ret = HI_MPI_SYS_Bind(&src, &dst);
    if (ret != HI_SUCCESS) {
      printf("ERROR: Unable to bind VPSS -> VO\n");
      return 1;
    }
  }
  
  /*{ MPP_CHN_S src;
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
  }*/
  
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
    address.sin_port    = htons(5000);
    bind(port, (struct sockaddr*)&address, sizeof(struct sockaddr_in));
  }
  
  uint8_t*  rx_buffer       = malloc(1024 * 1024);
  uint8_t*  nal_buffer      = malloc(1024 * 1024);
  uint32_t  nal_buffer_used = 0;
  while (1) {
    int rx = recv(port, rx_buffer + 8, 1500, 0);
    if (rx <= 0) {
      printf("ERROR: UDP RX failed\n");
      usleep(1);
      continue;
    }

    VDEC_STREAM_S stream;
    memset(&stream, 0x00, sizeof(stream));
    stream.bEndOfStream = HI_FALSE;
    stream.bEndOfFrame  = HI_TRUE;
    
    // - Decode UDP stream
    stream.pu8Addr = decodeUDPFrame(rx_buffer + 8, (uint32_t)rx, 0, nal_buffer, &nal_buffer_used, &stream.u32Len);
    if (!stream.pu8Addr) {
      continue;
    }
    
    // - Send frame into decoder
    { int ret = HI_MPI_VDEC_SendStream(vdec_channel_id, &stream, 0);
      if (ret != HI_SUCCESS) {
        printf("WARN: Unable to send data into VDEC\n");
      }
    }
    
    // - Query decoder status
    { VDEC_CHN_STAT_S stats;
      memset(&stats, 0x00, sizeof(stats));
      int ret = HI_MPI_VDEC_Query(vdec_channel_id, &stats);
      if (ret != HI_SUCCESS) {
        printf("WARN: Unable to query statistics\n");
        
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
    }
    
    /*{ VIDEO_FRAME_INFO_S frame;
      int ret = HI_MPI_VDEC_GetImage(vdec_channel_id, &frame, 0);
      if (ret == 0) {
        
        printf("> FRAME: %d x %d\n", frame.stVFrame.u32Width, frame.stVFrame.u32Height);
        
        
        // - Release frame
        HI_MPI_VDEC_ReleaseImage(vdec_channel_id, &frame);
      } else {
        printf("> NO frame = 0x%x\n", ret);
      }
    }*/
  }

  return 0;
}