/**************************DVR made by Dinh Cong Bang from VietNam***************************/
#ifndef  __RECORDER_H__
#define  __RECORDER_H__

#include "hi_type.h"
#include "hi_comm_vdec.h"

#define RINGBUFFER_SIZE 100
#define BUFFER_SIZE 512*1024

typedef struct {
    HI_U8* pData; // Pointer to 1MB memory block
    HI_U32 size;
} Data;

typedef struct {
    Data buffer[RINGBUFFER_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t lock;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} RingBuffer;

void recorder_int(const char* pPath);
void recorder_input_data(const VDEC_STREAM_S *pStream);
void* recorder_save_file_thread(void* arg);
void recorder_stop();

#endif