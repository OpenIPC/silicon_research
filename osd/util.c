#include "util.h"
#include <math.h>

#define EARTH_RADIUS 6371000  // Earth radius in meters

uint16_t mapu32(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
    return ((x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
    return ((x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
}

float Rad2Deg(float x)
{
  return x * (180.0F / M_PI);
}

double deg2rad(double degrees) {
    return degrees * M_PI / 180.0;
}

double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    // Convert degrees to radians
    double radLat1 = deg2rad(lat1);
    double radLon1 = deg2rad(lon1);
    double radLat2 = deg2rad(lat2);
    double radLon2 = deg2rad(lon2);

    // Calculate differences in latitude and longitude
    double deltaLat = radLat2 - radLat1;
    double deltaLon = radLon2 - radLon1;

    // Haversine formula
    double a = sin(deltaLat / 2) * sin(deltaLat / 2) +
               cos(radLat1) * cos(radLat2) *
               sin(deltaLon / 2) * sin(deltaLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double distance = EARTH_RADIUS * c;

    return distance;
}
void uint16ToByteArray(float value, signed char* byteArray) {
    memcpy(byteArray, &value, sizeof(uint16_t));
}

void floatToByteArray(float value, signed char* byteArray) {
    memcpy(byteArray, &value, sizeof(float));
}

void doubleToByteArray(double value, unsigned char* byteArray) {
    memcpy(byteArray, &value, sizeof(double));
}

float byteArrayToFloat(const signed char* byteArray) {
    float result;
    memcpy(&result, byteArray, sizeof(float));
    return result;
}

double byteArrayToDouble(const unsigned char* byteArray) {
    double result;
    memcpy(&result, byteArray, sizeof(double));
    return result;
}

uint16_t byteArrayToUint16(const signed char* byteArray) {
    uint16_t result;
    memcpy(&result, byteArray, sizeof(uint16_t));
    return result;
}