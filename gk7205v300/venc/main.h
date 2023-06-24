#pragma once
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <pthread.h>

#define PROT_READ  0x1
#define PROT_WRITE 0x2
#define PROT_EXEC  0x4

#define MAP_SHARED  0x01
#define MAP_PRIVATE 0x02

#define MAP_FAILED ((void *)-1)

void *mmap64(void *start, size_t len, int prot, int flags, int fd, off_t off);
int munmap(void *__addr, size_t __len);

#include "hi_common.h"
#include "hi_buffer.h"
#include "hi_comm_sys.h"
#include "hi_comm_vb.h"
#include "hi_comm_isp.h"
#include "hi_comm_vi.h"
#include "hi_comm_vo.h"
#include "hi_comm_venc.h"
#include "hi_comm_vpss.h"
#include "hi_comm_region.h"
#include "hi_comm_adec.h"
#include "hi_comm_aenc.h"
#include "hi_comm_ai.h"
#include "hi_comm_ao.h"
#include "hi_comm_aio.h"
#include "hi_defines.h"
#include "hi_mipi.h"
#include "hi_comm_vgs.h"

#include "mpi_sys.h"
#include "mpi_vb.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "mpi_venc.h"
#include "mpi_vpss.h"
#include "mpi_region.h"
#include "mpi_audio.h"
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"
#include "hi_math.h"
#include "hi_sns_ctrl.h"
#include "mpi_vgs.h"


void* __ISP_THREAD__(void *param);
int processStream(VENC_CHN channel_id, int socket_handle, struct sockaddr* dst_address, uint16_t max_frame_size);
void sendPacket(uint8_t* pack_data, uint32_t pack_size, int socket_handle, struct sockaddr* dst_address, uint32_t max_size);
HI_S32 getGOPAttributes(VENC_GOP_MODE_E enGopMode,VENC_GOP_ATTR_S *pstGopAttr);

int mipi_set_hs_mode(int device, lane_divide_mode_t mode);
int mipi_enable_clock(int device, uint32_t device_id, int enable);
int mipi_set_reset(int device, uint32_t device_id, int enable);
int mipi_enable_sensor_clock(int device, sns_clk_source_t sensor_id, int enable);
int mipi_set_sensor_reset(int device, sns_clk_source_t sensor_id, int enable);
int mipi_configure(int device, combo_dev_attr_t* config);


/* --- Console arguments parser --- */
#define __BeginParseConsoleArguments__( printHelpFunction ) if( argc < 2 || (argc == 2 &&  (!strcmp( argv[ 1 ], "--help" ) || !strcmp( argv[ 1 ], "/?" ) || !strcmp( argv[ 1 ], "/h" )))) { printHelpFunction(); return 1; } for (int ArgID = 1; ArgID < argc; ArgID++) {const char* Arg = argv[ ArgID ];
#define __OnArgument(Name) if(!strcmp(Arg, Name))
#define __ArgValue (argc > ArgID + 1 ? argv[++ArgID] : "")
#define __EndParseConsoleArguments__  else { printf("ERROR: Unknown argument\n"); return 1; } }

