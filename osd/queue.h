#ifndef _QUEUE_H
#define _QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif
// Author: Dinh Cong Bang from Viet Nam
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_QUEUE_SIZE 10
#define MAX_DATA_LENGTH 100

typedef enum {
    E_ALTITUDE = 1,
    E_PITCH,
    E_ROLL,
    E_YAW,
    E_BATTERY,
    E_CELL,
    E_CURRENT,
    E_GPS_HDG,
    E_GPS_DISTANCE,
    E_LAT,
    E_LON,
    E_SATS,
    E_GSPEED,
    E_HDG,
    E_RSSI,
    E_THROTTLE,
    E_ARMED,
    E_STATUS_TEXT,
    E_FLY_MODE
} MSG_TYPE;

typedef struct {
    MSG_TYPE id;
    unsigned char data[MAX_DATA_LENGTH];
} Data;

typedef struct {
    Data data[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Queue;

// Function prototypes
void initQueue(Queue* queue);
void enqueue(Queue* queue, Data item);
Data dequeue(Queue* queue);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif