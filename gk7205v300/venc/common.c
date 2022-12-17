#include "main.h"

/**
 * @brief Set MIPI HS mode
 * @param device - MIPI device handle
 * @param mode - New mode
 */
int mipi_set_hs_mode(int device, lane_divide_mode_t mode) {
  return ioctl(device, HI_MIPI_SET_HS_MODE, &mode);
}

/**
 * @brief Enable MIPI clock
 * @param device - MI
 * @param device_id - Device identifier
 * @param enable - 
 */
int mipi_enable_clock(int device, uint32_t device_id, int enable) {
  return ioctl(device, enable ? HI_MIPI_ENABLE_MIPI_CLOCK : HI_MIPI_DISABLE_MIPI_CLOCK, &device_id);
}

/**
 * @brief Set MIPI reset state
 * @param device 
 * @param device_id
 * @param enable - Activate reset state
 */
int mipi_set_reset(int device, uint32_t device_id, int enable) {
  return ioctl(device, enable ? HI_MIPI_RESET_MIPI : HI_MIPI_UNRESET_MIPI, &device_id);
}

/**
 * @brief Enable MIPI sensor clock
 * @param device
 * @param sensor_id
 * @param enable
 */
int mipi_enable_sensor_clock(int device, sns_clk_source_t sensor_id, int enable) {
  return ioctl(device, enable ? HI_MIPI_ENABLE_SENSOR_CLOCK : HI_MIPI_DISABLE_SENSOR_CLOCK, &sensor_id);
}

/**
 * @brief Set sensor reset state
 * @param device
 * @param sensor_id
 * @param enable
 */
int mipi_set_sensor_reset(int device, sns_clk_source_t sensor_id, int enable) {
  return ioctl(device, enable ? HI_MIPI_RESET_SENSOR : HI_MIPI_UNRESET_SENSOR, &sensor_id);
}

/**
 * @brief Configure MIPI
 * @param device - 
 * @param config - Pointer to MIPI configuration
 */
int mipi_configure(int device, combo_dev_attr_t* config) {
  return ioctl(device, HI_MIPI_SET_DEV_ATTR, config);
}

HI_S32 getGOPAttributes(VENC_GOP_MODE_E enGopMode,VENC_GOP_ATTR_S *pstGopAttr) {
    switch(enGopMode) {
        case VENC_GOPMODE_NORMALP:
            pstGopAttr->enGopMode  = VENC_GOPMODE_NORMALP;
            pstGopAttr->stNormalP.s32IPQpDelta = 2;
        break;
        case VENC_GOPMODE_SMARTP:
            pstGopAttr->enGopMode  = VENC_GOPMODE_SMARTP;
            pstGopAttr->stSmartP.s32BgQpDelta  = 4;
            pstGopAttr->stSmartP.s32ViQpDelta  = 2;
            pstGopAttr->stSmartP.u32BgInterval =  90;
        break;

        case VENC_GOPMODE_DUALP:
            pstGopAttr->enGopMode  = VENC_GOPMODE_DUALP;
            pstGopAttr->stDualP.s32IPQpDelta  = 4;
            pstGopAttr->stDualP.s32SPQpDelta  = 2;
            pstGopAttr->stDualP.u32SPInterval = 3;
        break;

        case VENC_GOPMODE_BIPREDB:
            pstGopAttr->enGopMode  = VENC_GOPMODE_BIPREDB;
            pstGopAttr->stBipredB.s32BQpDelta  = -2;
            pstGopAttr->stBipredB.s32IPQpDelta = 3;
            pstGopAttr->stBipredB.u32BFrmNum   = 2;
        break;
        
        case VENC_GOPMODE_LOWDELAYB:
            pstGopAttr->enGopMode   = VENC_GOPMODE_LOWDELAYB;
            pstGopAttr->stAdvSmartP.s32BgQpDelta  = 4;
            pstGopAttr->stAdvSmartP.s32ViQpDelta  = 2;
            pstGopAttr->stAdvSmartP.u32BgInterval =90;
            break;

        default:
            printf("not support the gop mode !\n");
            return HI_FAILURE;
        break;
    }
    return HI_SUCCESS;
}