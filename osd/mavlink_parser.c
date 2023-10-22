// Author: Dinh Cong Bang from Viet Nam
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

#include "queue.h"
#include "mavlink_parser.h"
#include "util.h"
#include "mavlink/common/mavlink.h"

#define MAVLINK_PORT  14550

extern Queue queue;

static unsigned char g_armed     = 0;
static double s_lat_home = 0.0;
static double s_lon_home = 0.0;
static char s_set_home = 0; 
bool s_bHasReceivedGPSInfo = false;
bool s_bHasReceivedGPSPos = false;

char *ardupilot_modes_copter(int mode)
{
    switch(mode)
    {
    case COPTER_MODE_STABILIZE:
        return "STAB";
    case COPTER_MODE_ACRO:
        return "ACRO";
    case COPTER_MODE_ALT_HOLD:
        return "ALT";
    case COPTER_MODE_AUTO:
        return "AUTO";
    case COPTER_MODE_GUIDED:
        return "GUIDED";
    case COPTER_MODE_LOITER:
        return "LOITER";
    case COPTER_MODE_RTL:
        return "RTL";
    case COPTER_MODE_CIRCLE:
        return "CIRCLE";
    case COPTER_MODE_LAND:
        return "LAND";
    case COPTER_MODE_DRIFT:
        return "DRIFT";
    case COPTER_MODE_SPORT:
        return "SPORT";
    case COPTER_MODE_FLIP:
        return "FLIP";
    case COPTER_MODE_AUTOTUNE:
        return "AUTOTUNE";
    case COPTER_MODE_POSHOLD:
        return "POSHOLD";
    case COPTER_MODE_BRAKE:
        return "BRAKE";
    case COPTER_MODE_THROW:
        return "THROW";
    case COPTER_MODE_AVOID_ADSB:
        return "AVOID";
    case COPTER_MODE_GUIDED_NOGPS:
        return "GUIDED";
    case COPTER_MODE_SMART_RTL:
        return "SMART";
    default:
        return "UNKNOWN";
    }
}

char *ardupilot_modes_plane(int mode)
{
    switch(mode)
    {
    case PLANE_MODE_MANUAL:
        return "MANUAL";
    case PLANE_MODE_CIRCLE:
        return "CIRCLE";
    case PLANE_MODE_STABILIZE:
        return "STAB";
    case PLANE_MODE_TRAINING:
        return "TRAINING";
    case PLANE_MODE_ACRO:
        return "ACRO";
    case PLANE_MODE_FLY_BY_WIRE_A:
        return "FLYA";
    case PLANE_MODE_FLY_BY_WIRE_B:
        return "FLYB";
    case PLANE_MODE_CRUISE:
        return "CRUISE";
    case PLANE_MODE_AUTOTUNE:
        return "AUTOTUNE";
    case PLANE_MODE_AUTO:
        return "AUTO";
    case PLANE_MODE_RTL:
        return "RTL";
    case PLANE_MODE_LOITER:
        return "LOITER";
    case PLANE_MODE_TAKEOFF:
        return "TAKEOFF";
    case PLANE_MODE_AVOID_ADSB:
        return "AVOID";
    case PLANE_MODE_GUIDED:
        return "GUIDED";
    case PLANE_MODE_INITIALIZING:
        return "INITIALIZING";
    case PLANE_MODE_QSTABILIZE:
        return "QSTABILIZE";
    case PLANE_MODE_QHOVER:
        return "QHOVER";
    case PLANE_MODE_QLOITER:
        return "QLOITER";
    case PLANE_MODE_QLAND:
        return "QLAND";
    case PLANE_MODE_QRTL:
        return "QRTL";
    case PLANE_MODE_QAUTOTUNE:
        return "QAUTOTUNE";
    default:
        return "UNKNOWN";
    }
}

void* mavlink_handler(void* arg) {

  // - Create socket
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    printf("ERROR: Unable to create MavLink socket: %s\n", strerror(errno));
    return 0;
  }

  // - Bind port
  { struct sockaddr_in addr = {};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &(addr.sin_addr));    // listen on all network interfaces
    addr.sin_port = htons(MAVLINK_PORT);                // default port on the ground
    if (bind(fd, (struct sockaddr*)(&addr), sizeof(addr)) != 0) {
        printf("ERROR: Unable to bind MavLink port: %s\n", strerror(errno));
        return 0;
    }
  }

  // - Set Rx timeout
  { struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        printf("ERROR: Unable to bind MavLink rx timeout: %s\n", strerror(errno));
        return 0;
    }
  }

  char buffer[2048];

  while (1) {
    memset(buffer, 0x00, sizeof(buffer));

    int ret = recv(fd, buffer, sizeof(buffer), 0);
    if (ret == 0) {
      // peer has done an orderly shutdown
      printf("ERROR: MavLink rx error: %s\n", strerror(errno));
      return 0;
    }
    
    // - Parse
    mavlink_message_t message;
    mavlink_status_t  status;
    for (int i = 0; i < ret; ++i) {
      if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &message, &status) == 1) {
        // printf("MavLink msgid %d\n",message.msgid);
        switch (message.msgid) {
            case MAVLINK_MSG_ID_HEARTBEAT:
            {
              mavlink_heartbeat_t heartbeat;
              mavlink_msg_heartbeat_decode(&message, &heartbeat);
              uint8_t vehicleType = heartbeat.type;

              uint32_t tmp32 = mavlink_msg_heartbeat_get_custom_mode(&message);
              uint8_t tmp8 = mavlink_msg_heartbeat_get_base_mode(&message);
              char* textMode = (char*)malloc(15*sizeof(char));
              if ( vehicleType == MAV_TYPE_QUADROTOR )
              {
                sprintf(textMode, "%s", ardupilot_modes_copter(tmp32));             
              }            
              else
              {
                sprintf(textMode, "%s", ardupilot_modes_plane(tmp32));      
              }

              Data item;
              memset(&item, 0, sizeof(Data));
              unsigned char arm = tmp8 & MAV_MODE_FLAG_SAFETY_ARMED ? 1 : 0;
              // printf("Arm: %d - g: %d\n", arm, g_armed);
              if ( g_armed !=  arm)
              {
                g_armed = arm;
                char* curr_arm = (char*)malloc(sizeof(char));
                *curr_arm = g_armed;
                item.id = E_ARMED;
                item.data = (void*)curr_arm;
                enqueue(&queue, item);
              }

              if (g_armed == 0)
              {
                s_set_home = 0;
                g_armed = 0;
              }

              // printf("Flymode: %s, arm: %d, tmp32: %d\n", textMode, g_armed, tmp32);
              item.id = E_FLY_MODE;
              item.data = (void*)textMode;
              enqueue(&queue, item);
          }break;

          case MAVLINK_MSG_ID_STATUSTEXT: {
              // Create a union to hold the message payload
              mavlink_statustext_t statusText;
              memset(&statusText, 0, sizeof(statusText));
              
              // Decode the message
              mavlink_msg_statustext_decode(&message, &statusText);
              
              Data item;
              memset(&item, 0, sizeof(Data));
              item.id = E_STATUS_TEXT;
              int i = 0;
              int cnt = 0;
              //Detect string length
              while(cnt <= 3 || i < MAVLINK_MSG_STATUSTEXT_FIELD_TEXT_LEN)
              {
                if(statusText.text[i] == ' ')
                  cnt++;
                else
                  cnt = 0;
                i++;
              }
              char* tmp = (char*)malloc((i-cnt)*sizeof(char));
              memcpy(tmp, statusText.text, i-cnt);
              item.data = (void*)tmp;
              enqueue(&queue, item);

          }; break;

          case MAVLINK_MSG_ID_SYS_STATUS: {
            float *voltage = (float*)malloc(sizeof(float));
            *voltage = mavlink_msg_sys_status_get_voltage_battery(&message)/1000.0f;
            float *current = (float*)malloc(sizeof(float));
            *current = mavlink_msg_sys_status_get_current_battery(&message)*10/100.0f;

            mavlink_sys_status_t bat;
            mavlink_msg_sys_status_decode(&message, &bat);
            Data item;
            memset(&item, 0, sizeof(Data));
            item.id = E_BATTERY;
            item.data = (void*)voltage;
            enqueue(&queue, item);
            item.id = E_CURRENT;
            item.data = (void*)current;
            enqueue(&queue, item);

          }; break;

          case MAVLINK_MSG_ID_BATTERY_STATUS: {
              uint16_t *current_consumed = (uint16_t*)malloc(sizeof(uint16_t));
              *current_consumed = mavlink_msg_battery_status_get_current_consumed(&message);
              Data item;
              memset(&item, 0, sizeof(Data));
              item.id = E_CURRENT_CONSUMED;
              item.data = (void*)current_consumed;
              enqueue(&queue, item);
          } break;

          case MAVLINK_MSG_ID_VFR_HUD: {
            uint16_t *throttle = (uint16_t*)malloc(sizeof(uint16_t));
            *throttle = mavlink_msg_vfr_hud_get_throttle(&message); 
            Data item;
            memset(&item, 0, sizeof(Data));
            item.id = E_THROTTLE;
            item.data = (void*)throttle;
            enqueue(&queue, item);
          } break;

          case MAVLINK_MSG_ID_HOME_POSITION:
          {
              s_lat_home = mavlink_msg_home_position_get_latitude(&message);
              s_lon_home = mavlink_msg_home_position_get_longitude(&message);
              s_set_home = 1;
          }break;

          case MAVLINK_MSG_ID_RC_CHANNELS:
          case MAVLINK_MSG_ID_RC_CHANNELS_RAW: {
            Data item;
            memset(&item, 0, sizeof(Data));
            item.id = E_RSSI;
            uint16_t *rssi = (uint16_t*)malloc(sizeof(uint16_t));
            *rssi = mavlink_msg_rc_channels_raw_get_rssi(&message);
            item.data = (void*)rssi;
            enqueue(&queue, item);
          }; break;

          case MAVLINK_MSG_ID_GPS_RAW_INT: {
            s_bHasReceivedGPSInfo = true;
            mavlink_gps_raw_int_t gps;
            mavlink_msg_gps_raw_int_decode(&message, &gps);

            Data item;
            memset(&item, 0, sizeof(Data));
            item.id = E_SATS;
            char* sats = (char*)malloc(sizeof(char));
            *sats = gps.satellites_visible;
            item.data = (void*)sats;
            enqueue(&queue, item);

            GPSCordinates *gpsdata = (GPSCordinates*)malloc(sizeof(GPSCordinates));
            gpsdata->alt = gps.alt / 1000;  // Convert from millimeters to meters
            gpsdata->lat = gps.lat;
            gpsdata->lon = gps.lon;
            item.id = E_GPS_CORDINATE;
            item.data = (void*)gpsdata;
            enqueue(&queue, item);

            double* distance = (double*)malloc(sizeof(double));
            if(s_set_home == 1)
              *distance = calculateDistance(s_lat_home, s_lon_home, gps.lat, gps.lon);
            else
              *distance = 0;
            item.id = E_GPS_DISTANCE;
            item.data = (void*)distance;
            enqueue(&queue, item);

          }; break;

          case MAVLINK_MSG_ID_GLOBAL_POSITION_INT_COV:
          case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: {
            s_bHasReceivedGPSPos = true;
            mavlink_global_position_int_t global_position_int;
            mavlink_msg_global_position_int_decode(&message, &global_position_int);
            
            uint16_t *hdg = (uint16_t*)malloc(sizeof(uint16_t));
            *hdg = global_position_int.hdg/100;
            Data item;
            memset(&item, 0, sizeof(Data));
            item.id = E_HDG;
            item.data = (void*)hdg;
            enqueue(&queue, item);

            GPSCordinates *gpsdata = (GPSCordinates*)malloc(sizeof(GPSCordinates));
            gpsdata->alt = global_position_int.alt / 1000;  // Convert from millimeters to meters
            gpsdata->lat = global_position_int.lat;
            gpsdata->lon = global_position_int.lon;
            item.id = E_GPS_CORDINATE;
            item.data = (void*)gpsdata;
            enqueue(&queue, item);

            // Access the speed field
            float* gspeed = (float*)malloc(sizeof(float)); 
            *gspeed = sqrt(global_position_int.vx * global_position_int.vx +
                          global_position_int.vy * global_position_int.vy +
                          global_position_int.vz * global_position_int.vz);
            item.id = E_GSPEED;
            item.data = (void*)gspeed;
            enqueue(&queue, item);
            
            double* distance = (double*)malloc(sizeof(double));
            *distance = calculateDistance(s_lat_home, s_lon_home, gpsdata->lat, gpsdata->lon);
            item.id = E_GPS_DISTANCE;
            item.data = (void*)distance;
            enqueue(&queue, item);
          }; break;

          case MAVLINK_MSG_ID_ATTITUDE:
          {
              MPUData* mpu = (MPUData*)malloc(sizeof(MPUData));
              mpu->pitch = Rad2Deg(mavlink_msg_attitude_get_pitch(&message));
              mpu->roll = Rad2Deg(mavlink_msg_attitude_get_roll(&message));                      
              // float yaw = Rad2Deg(mavlink_msg_attitude_get_yaw(&message));
              Data item;
              memset(&item, 0, sizeof(Data));
              item.id = E_MPU;
              item.data = (void*)mpu;
              enqueue(&queue, item);
          }
          break;

          case MAVLINK_MSG_ID_HIGH_LATENCY:
          {
              uint16_t *temp = (uint16_t*)malloc(sizeof(uint16_t));
              int iTemp = mavlink_msg_high_latency_get_temperature(&message);
              if ( iTemp < 100 && iTemp > -100 )
                *temp = 100 + (int) iTemp;

              iTemp = mavlink_msg_high_latency_get_temperature_air(&message);
              if ( iTemp < 100 && iTemp > -100 )
                *temp= 100 + (int) iTemp;

              Data item;
              memset(&item, 0, sizeof(Data));
              item.id = E_TEMP;
              item.data = (void*)temp;
              enqueue(&queue, item);
          }
          break;

          case MAVLINK_MSG_ID_HIGH_LATENCY2:
          {
              uint16_t *temp = (uint16_t*)malloc(sizeof(uint16_t));
              int iTemp = mavlink_msg_high_latency2_get_temperature_air(&message);
              if ( iTemp < 100 && iTemp > -100 )
                *temp = 100 + (int) iTemp;
              Data item;
              memset(&item, 0, sizeof(Data));
              item.id = E_TEMP;
              item.data = (void*)temp;
              enqueue(&queue, item);
          }
          break;

          case MAVLINK_MSG_ID_SCALED_PRESSURE:
          {
            uint16_t *temp = (uint16_t*)malloc(sizeof(uint16_t));
            int iTemp = mavlink_msg_scaled_pressure_get_temperature(&message);
            iTemp = iTemp/100;
            if ( iTemp < 100 && iTemp > -100 )
               *temp = 100 + (int) iTemp;
            Data item;
            memset(&item, 0, sizeof(Data));
            item.id = E_TEMP;
            item.data = (void*)temp;
            enqueue(&queue, item);
          }break;

          default:
            printf("> MavLink message %d from %d/%d\n", message.msgid, message.sysid, message.compid);
            break;
        }
      }
    }

    usleep(1);
  }

  return 0;
}

