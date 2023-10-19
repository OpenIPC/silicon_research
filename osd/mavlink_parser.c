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
    if (ret < 0) {
        printf("WARN: MavLink rx error: %s\n", strerror(errno));
    } else if (ret == 0) {
      // peer has done an orderly shutdown
      return 0;
    }
    
    // - Parse
    mavlink_message_t message;
    mavlink_status_t  status;
    for (int i = 0; i < ret; ++i) {
      if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &message, &status) == 1) {
        switch (message.msgid) {
          case MAVLINK_MSG_ID_HEARTBEAT:
            mavlink_heartbeat_t heartbeat;
            mavlink_msg_heartbeat_decode(&message, &heartbeat);
            uint8_t vehicleType = heartbeat.type;

            uint32_t tmp32 = mavlink_msg_heartbeat_get_custom_mode(&message);
            uint8_t tmp8 = mavlink_msg_heartbeat_get_base_mode(&message);
            char textMode[15] = "";
            if ( vehicleType == MAV_TYPE_QUADROTOR )
            {
              switch ( tmp32 )
              {
                case COPTER_MODE_STABILIZE: sprintf(textMode, "%s", "STAB"); break;
                case COPTER_MODE_ALT_HOLD:  sprintf(textMode, "%s", "ALTH"); break;
                case COPTER_MODE_LOITER:  sprintf(textMode, "%s", "LOITER"); break;
                case COPTER_MODE_AUTO:  sprintf(textMode, "%s", "AUTO"); break;
                case COPTER_MODE_LAND:  sprintf(textMode, "%s", "LAND"); break;
                case COPTER_MODE_RTL:  sprintf(textMode, "%s", "RTL"); break;
                case COPTER_MODE_SMART_RTL:  sprintf(textMode, "%s", "RTL"); break;
                case COPTER_MODE_AUTOTUNE:  sprintf(textMode, "%s", "AUTOTUNE;"); break;
                case COPTER_MODE_POSHOLD:  sprintf(textMode, "%s", "POSHOLD"); break;
                case COPTER_MODE_ACRO:  sprintf(textMode, "%s", "ACRO"); break;
                case COPTER_MODE_CIRCLE:  sprintf(textMode, "%s", "CIRCLE"); break;
              };
            
            }            
            else
            {
              switch ( tmp32 )
              {
                case PLANE_MODE_MANUAL:  sprintf(textMode, "%s", "MANUAL"); break;
                case PLANE_MODE_CIRCLE:  sprintf(textMode, "%s", "CIRCLE"); break;
                case PLANE_MODE_STABILIZE:  sprintf(textMode, "%s", "STAB"); break;
                case PLANE_MODE_FLY_BY_WIRE_A:  sprintf(textMode, "%s", "FBWA"); break;
                case PLANE_MODE_FLY_BY_WIRE_B:  sprintf(textMode, "%s", "FBWB"); break;
                case PLANE_MODE_ACRO:  sprintf(textMode, "%s", "FBWB"); break;
                case PLANE_MODE_AUTO:  sprintf(textMode, "%s", "AUTO"); break;
                case PLANE_MODE_AUTOTUNE:  sprintf(textMode, "%s", "AUTOTUNE"); break;
                case PLANE_MODE_RTL:  sprintf(textMode, "%s", "RTL"); break;
                case PLANE_MODE_LOITER:  sprintf(textMode, "%s", "LOITER"); break;
                case PLANE_MODE_TAKEOFF:  sprintf(textMode, "%s", "TAKEOFF"); break;
                case PLANE_MODE_CRUISE:  sprintf(textMode, "%s", "CRUISE"); break;
                case PLANE_MODE_QSTABILIZE:  sprintf(textMode, "%s", "QSTAB"); break;
                case PLANE_MODE_QHOVER:  sprintf(textMode, "%s", "QHOVER"); break;
                case PLANE_MODE_QLOITER:  sprintf(textMode, "%s", "QLOITER"); break;
                case PLANE_MODE_QLAND:  sprintf(textMode, "%s", "QLAND"); break;
                case PLANE_MODE_QRTL:  sprintf(textMode, "%s", "QRTL"); break;
              };
            }
            Data item;
            memset(&item, 0, sizeof(Data));
            item.id = E_FLY_MODE;
            // Access the text field
            strncpy(item.data, textMode, 15);
            enqueue(&queue, item);
         break;

          case MAVLINK_MSG_ID_ALTITUDE: {
            mavlink_altitude_t alt;
            mavlink_msg_altitude_decode(&message, &alt);
            Data item;
            memset(&item, 0, sizeof(Data));
            item.id = E_ALTITUDE;
            floatToByteArray(alt.altitude_relative, item.data);
            enqueue(&queue, item);

          }; break;
          case MAVLINK_MSG_ID_STATUSTEXT: {
              // Create a union to hold the message payload
              mavlink_statustext_t statusText;
              memset(&statusText, 0, sizeof(statusText));
              
              // Decode the message
              mavlink_msg_statustext_decode(&message, &statusText);
              
              Data item;
              memset(&item, 0, sizeof(Data));
              item.id = E_STATUS_TEXT;
              // Access the text field
              strncpy(item.data, statusText.text, MAVLINK_MSG_STATUSTEXT_FIELD_TEXT_LEN);
              enqueue(&queue, item);

          }; break;
          case MAVLINK_MSG_ID_SYS_STATUS: {
            mavlink_sys_status_t bat;
            mavlink_msg_sys_status_decode(&message, &bat);
            Data item;
            memset(&item, 0, sizeof(Data));
            item.id = E_BATTERY;
            floatToByteArray(bat.voltage_battery, item.data);
            enqueue(&queue, item);
            item.id = E_CURRENT;
            floatToByteArray(bat.current_battery, item.data);
            enqueue(&queue, item);

          }; break;
          case MAVLINK_MSG_ID_BATTERY_STATUS: {
              // Create a union to hold the message payload
              mavlink_battery_status_t batteryStatus;
              memset(&batteryStatus, 0, sizeof(batteryStatus));
              
              // Decode the message
              mavlink_msg_battery_status_decode(&message, &batteryStatus);
              
              // Access the cell voltage and total voltage fields
              float cellVoltage = batteryStatus.voltages[0] / 1000.0; // Convert from millivolts to volts
              float totalVoltage = 0;
              for(int i = 0; i < 10; i++)
              {
                totalVoltage += batteryStatus.voltages[i] / 1000.0;
              }
              uint16_t current_consumed = batteryStatus.current_consumed;
              
              Data item;
              memset(&item, 0, sizeof(Data));
              item.id = E_BATTERY;
              floatToByteArray(totalVoltage, item.data);
              enqueue(&queue, item);
              item.id = E_CURRENT;
              floatToByteArray(current_consumed, item.data);
              enqueue(&queue, item);
              memset(&item, 0, sizeof(Data));
              item.id = E_CELL;
              uint16ToByteArray(cellVoltage, item.data);
              enqueue(&queue, item);
          }
          case MAVLINK_MSG_ID_RC_CHANNELS_RAW: {
            mavlink_rc_channels_raw_t rc_channels_raw;
            mavlink_msg_rc_channels_raw_decode(&message, &rc_channels_raw);

            Data item;
            memset(&item, 0, sizeof(Data));
            item.id = E_RSSI;
            uint16ToByteArray(rc_channels_raw.rssi, item.data);
            enqueue(&queue, item);

            uint16_t throttle =  (rc_channels_raw.chan4_raw - 1000) / 10;
            if (throttle < 0){
                 throttle = 0;
            };
            item.id = E_THROTTLE;
            uint16ToByteArray(throttle, item.data);
            enqueue(&queue, item);

            unsigned char arm = rc_channels_raw.chan5_raw > 1500 ? 1 : 0;
            if(g_armed == 0 && arm == 1)
            {
               s_set_home = 1;
            }
            else if (arm == 0)
            {
                s_set_home = 0;
            }
            g_armed = arm;
            memset(&item, 0, sizeof(Data));
            item.id = E_ARMED;
            item.data[0] = arm;
            enqueue(&queue, item);

          }; break;

          case MAVLINK_MSG_ID_GPS_RAW_INT: {
            mavlink_gps_raw_int_t gps;
            mavlink_msg_gps_raw_int_decode(&message, &gps);

            Data item;
            memset(&item, 0, sizeof(Data));
            item.id = E_SATS;
            item.data[0] = gps.satellites_visible;
            enqueue(&queue, item);

            memset(&item, 0, sizeof(Data));
            item.id = E_LAT;
            doubleToByteArray(gps.lat, item.data);
            enqueue(&queue, item);

            item.id = E_LON;
            doubleToByteArray(gps.lon, item.data);
            enqueue(&queue, item);

            if(s_set_home == 1)
            {
                s_set_home = 0;
                s_lat_home = gps.lat;
                s_lon_home = gps.lon;
            }

            double distance = calculateDistance(s_lat_home, s_lon_home, gps.lat, gps.lon);
            item.id = E_GPS_DISTANCE;
            doubleToByteArray(distance, item.data);
            enqueue(&queue, item);

          }; break;

          case MAVLINK_MSG_ID_GLOBAL_POSITION_INT_COV:
          case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: {
            mavlink_global_position_int_t global_position_int;
            mavlink_msg_global_position_int_decode(&message, &global_position_int);
            
            uint16_t hdg = global_position_int.hdg/100;
            Data item;
            memset(&item, 0, sizeof(Data));
            item.id = E_HDG;
            uint16ToByteArray(hdg, item.data);
            enqueue(&queue, item);

            uint16_t altitude = global_position_int.alt / 1000;  // Convert from millimeters to meters
            memset(&item, 0, sizeof(Data));
            item.id = E_ALTITUDE;
            uint16ToByteArray(altitude, item.data);
            enqueue(&queue, item);

            // Access the speed field
            float gspeed = sqrt(global_position_int.vx * global_position_int.vx +
                              global_position_int.vy * global_position_int.vy +
                              global_position_int.vz * global_position_int.vz);
            memset(&item, 0, sizeof(Data));
            item.id = E_GSPEED;
            floatToByteArray(gspeed, item.data);
            enqueue(&queue, item);
            
          }; break;
          //Todo:
          //Get Roll, Pitch to control vertical line
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

