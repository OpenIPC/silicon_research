#ifndef _UTIL_H
#define _UTIL_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t mapu32(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
float mapf(float x, float in_min, float in_max, float out_min, float out_max);
float Rad2Deg(float x);
double deg2rad(double degrees) ;
void uint16ToByteArray(float value, signed char* byteArray);
void floatToByteArray(float value, signed char* byteArray);
void doubleToByteArray(double value, unsigned char* byteArray);
float byteArrayToFloat(const signed char* byteArray);
double byteArrayToDouble(const unsigned char* byteArray);
uint16_t byteArrayToUint16(const signed char* byteArray);
double calculateDistance(double lat1, double lon1, double lat2, double lon2);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif