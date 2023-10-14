#ifndef _UTIL_H
#define _UTIL_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void uint16ToByteArray(float value, unsigned char* byteArray);
void floatToByteArray(float value, unsigned char* byteArray);
void doubleToByteArray(double value, unsigned char* byteArray);
float byteArrayToFloat(const unsigned char* byteArray);
double byteArrayToDouble(const unsigned char* byteArray);
uint16_t byteArrayToUint16(const unsigned char* byteArray);
double calculateDistance(double lat1, double lon1, double lat2, double lon2);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif