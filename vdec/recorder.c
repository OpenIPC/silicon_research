/**************************DVR made by Dinh Cong Bang from VietNam***************************/
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "recorder.h"

FILE* fpRecordFile = HI_NULL;
HI_BOOL allowSavingThreadRun = HI_FALSE;
HI_BOOL bIsRecorderReady = HI_FALSE;
HI_BOOL isFoundIFrame = HI_FALSE;
RingBuffer ringbuff;
Data videoDataBuffer;

extern double getTimeInterval(struct timespec* timestamp, struct timespec* last_meansure_timestamp) ;

void init(RingBuffer* rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    pthread_mutex_init(&rb->lock, NULL);
    pthread_cond_init(&rb->not_full, NULL);
    pthread_cond_init(&rb->not_empty, NULL);
    for(int i = 0; i<RINGBUFFER_SIZE; i++)
    {
      rb->buffer[i].pData = malloc(BUFFER_SIZE);
    }
}

void destroy(RingBuffer* rb) {
    for(int i = 0; i<RINGBUFFER_SIZE; i++)
    {
      free(rb->buffer[i].pData);
    }
    pthread_mutex_destroy(&rb->lock);
    pthread_cond_destroy(&rb->not_full);
    pthread_cond_destroy(&rb->not_empty);
}

void enqueue(RingBuffer* rb, HI_U8* pData, HI_U32 size) {
  if(bIsRecorderReady == HI_TRUE)
  {
    pthread_mutex_lock(&rb->lock);
    while (rb->count == RINGBUFFER_SIZE) {
        pthread_cond_wait(&rb->not_full, &rb->lock);
    }
    memcpy(rb->buffer[rb->head].pData, pData, size);
    rb->buffer[rb->head].size = size;
    rb->head = (rb->head + 1) % RINGBUFFER_SIZE;
    rb->count++;
    pthread_cond_signal(&rb->not_empty);
    pthread_mutex_unlock(&rb->lock);
    // printf("Enque %d byte\n", size);
  }
}

Data* dequeue(RingBuffer* rb) {
    pthread_mutex_lock(&rb->lock);
    while (rb->count == 0) {
        pthread_cond_wait(&rb->not_empty, &rb->lock);
    }
    Data* data = &rb->buffer[rb->tail];
    // printf("dequeue %d byte\n", data->size);
    rb->tail = (rb->tail + 1) % RINGBUFFER_SIZE;
    rb->count--;
    pthread_cond_signal(&rb->not_full);
    pthread_mutex_unlock(&rb->lock);
    return data;
}

HI_BOOL RingIsEmpty(RingBuffer* rb)
{
  return rb->count == 0;
}

void recorder_int(const char* pPath)
{
    init(&ringbuff);
    videoDataBuffer.pData = malloc(BUFFER_SIZE);
    fpRecordFile = fopen(pPath, "wb");
    if(!fpRecordFile)
    {
      printf("ERROR: Can not record video\n");
    }
    else
    {
      pthread_t recording_thread;
      allowSavingThreadRun = HI_TRUE;
      pthread_create(&recording_thread, NULL, recorder_save_file_thread, &ringbuff);
    }
    printf("Finish setup video recorder\n");
}

void recorder_input_data(const VDEC_STREAM_S *pStream)
{
    if(bIsRecorderReady == HI_FALSE)
        return;

    if(isFoundIFrame == HI_FALSE)
    {
      HI_U8 t = (pStream->pu8Addr[4] >> 1) & 0x3f;
      if(t == 32)
      {
        printf("Found I Frame, start recording\n");
        isFoundIFrame = HI_TRUE;
      }
      else
      {
        printf("Not I Frame %d\n", t);
        return;
      }
    }

    if(isFoundIFrame == HI_TRUE)
    {
      if(videoDataBuffer.size + pStream->u32Len <= BUFFER_SIZE)
      {
        memcpy(videoDataBuffer.pData + videoDataBuffer.size, pStream->pu8Addr, pStream->u32Len);
        videoDataBuffer.size += pStream->u32Len;
      }
      else
      {
        enqueue(&ringbuff, videoDataBuffer.pData, videoDataBuffer.size);
        memcpy(videoDataBuffer.pData, pStream->pu8Addr, pStream->u32Len);
        videoDataBuffer.size = pStream->u32Len;
      }
    }
}

void recorder_stop()
{
    bIsRecorderReady = HI_FALSE;
}

void* recorder_save_file_thread(void* arg)
{    
    RingBuffer* rb = (RingBuffer*)arg;
    HI_U8* video_buffer = malloc(BUFFER_SIZE);
    HI_U8* nal_buffer = malloc(BUFFER_SIZE);
    struct timespec current_timestamp;
    struct timespec prev_timestamp = {0, 0};

    printf("Starting to record video\n");
    bIsRecorderReady = HI_TRUE;
    
    while(allowSavingThreadRun == HI_TRUE)
    {      
        Data* data = dequeue(rb);

        fwrite(data->pData, data->size, 1, fpRecordFile);
        fflush(fpRecordFile);
    }
    /******************************************
     stop recording video
    ******************************************/
    printf("Close video file\n");
    bIsRecorderReady = HI_FALSE;
    fflush(fpRecordFile);
    fclose(fpRecordFile);
    free(video_buffer);
    video_buffer = HI_NULL;
    free(nal_buffer);
    nal_buffer = HI_NULL;
    destroy(rb);
}
