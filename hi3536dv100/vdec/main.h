#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <pthread.h>

#include <sys/sem.h>
#include <sys/prctl.h>
#include "hi_common.h"
#include "hi_comm_sys.h"
#include "hi_comm_vb.h"
#include "hi_comm_vi.h"
#include "hi_comm_vo.h"
#include "hi_comm_venc.h"
#include "hi_comm_vpss.h"
#include "hi_comm_vdec.h"
#include "hi_comm_region.h"
#include "hi_comm_adec.h"
#include "hi_comm_aenc.h"
#include "hi_comm_ai.h"
#include "hi_comm_ao.h"
#include "hi_comm_aio.h"
#include "hi_comm_hdmi.h"
#include "hi_defines.h"

#include "mpi_sys.h"
#include "mpi_vb.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "mpi_venc.h"
#include "mpi_vpss.h"
#include "mpi_vdec.h"
#include "mpi_region.h"
#include "mpi_adec.h"
#include "mpi_aenc.h"
#include "mpi_ai.h"
#include "mpi_ao.h"
#include "mpi_hdmi.h"

#define ALIGN_UP(x, a)      ((x+a-1)&(~(a-1)))
#define ALIGN_BACK(x, a)    ((a) * (((x) / (a))))

#include "fbgraphics.h"
#include "fbg_fbdev.h"
#include "mavlink/common/mavlink.h"

/**
 * @brief Initialize VO device
 * @param device_id - Device identifier
 * @param interface_type - Interface type
 * @param interface_mode - Interface sync mode
 * @param background_color - Background color
 */
int VO_init(VO_DEV device_id, VO_INTF_TYPE_E interface_type, VO_INTF_SYNC_E interface_mode, uint32_t framerate, uint32_t background_color);

/**
 * @brief 
 * @param device_id
 * @param interface_mode
 * @return 
 */
int VO_HDMI_init(HI_HDMI_ID_E device_id, VO_INTF_SYNC_E interface_mode);

/**
 * @brief 
 * @param rx_buffer - UDP data
 * @param rx_size - Size of UDP data
 * @param header_size - Size of data before payload
 * @param nal_buffer - Buffer for NAL reassembly
 * @param nal_buffer_used - Current 
 * @param out_nal_size
 */
uint8_t* decodeUDPFrame(uint8_t* rx_buffer, uint32_t rx_size, uint32_t header_size, uint8_t* nal_buffer, uint32_t* nal_buffer_used, uint32_t* out_nal_size);

/* --- Console arguments parser --- */
#define __BeginParseConsoleArguments__( printHelpFunction ) if( argc < 2 || (argc == 2 &&  (!strcmp( argv[ 1 ], "--help" ) || !strcmp( argv[ 1 ], "/?" ) || !strcmp( argv[ 1 ], "/h" )))) { printHelpFunction(); return 1; } for (int ArgID = 1; ArgID < argc; ArgID++) {const char* Arg = argv[ ArgID ];
#define __OnArgument(Name) if(!strcmp(Arg, Name))
#define __ArgValue (argc > ArgID + 1 ? argv[++ArgID] : "")
#define __EndParseConsoleArguments__  else { printf("ERROR: Unknown argument\n"); return 1; } }



void* __OSD_THREAD__(void*);
void* __MAVLINK_THREAD__(void*);