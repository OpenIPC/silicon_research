/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "comm_video.h"
#include "sns_ctrl.h"

#ifdef GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "i2c.h"
#endif

extern uint16_t goke_version;
extern uint32_t sensor_framerate;

const unsigned char imx307_2l_i2c_addr = 0x34; /* I2C Address of IMX307 */
const unsigned int imx307_2l_addr_byte = 2;
const unsigned int imx307_2l_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern ISP_SNS_STATE_S* g_pastImx307_2l[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunImx307_2l_BusInfo[];

int imx307_2l_i2c_init(VI_PIPE ViPipe)
{
    char acDevFile[16] = { 0 };
    GK_U8 u8DevNum;

    if (g_fd[ViPipe] >= 0) {
        return GK_SUCCESS;
    }
#ifdef GPIO_I2C
    int ret;

    g_fd[ViPipe] = open("/dev/gpioi2c_ex", O_RDONLY, S_IRUSR);
    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open gpioi2c_ex error!\n");
        return GK_FAILURE;
    }
#else
    int ret;

    u8DevNum = g_aunImx307_2l_BusInfo[ViPipe].s8I2cDev;
    snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
            u8DevNum);
        return GK_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (imx307_2l_i2c_addr >> 1));
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return GK_SUCCESS;
}

int imx307_2l_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return GK_SUCCESS;
    }
    return GK_FAILURE;
}

int imx307_2l_read_register(VI_PIPE ViPipe, int addr)
{
    return GK_SUCCESS;
}

int imx307_2l_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
    if (g_fd[ViPipe] < 0) {
        return GK_SUCCESS;
    }

#ifdef GPIO_I2C
    i2c_data.dev_addr = imx307_2l_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = imx307_2l_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = imx307_2l_data_byte;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

    if (ret) {
        ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int idx = 0;
    int ret;
    char buf[8];

    if (imx307_2l_addr_byte == 2) {
        buf[idx] = (addr >> 8) & 0xff;
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (imx307_2l_data_byte == 2) {
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf,
        imx307_2l_addr_byte + imx307_2l_data_byte);
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return GK_FAILURE;
    }

#endif
    return GK_SUCCESS;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000);
}

void imx307_2l_standby(VI_PIPE ViPipe)
{
    imx307_2l_write_register(ViPipe, 0x3000, 0x01); /* STANDBY */
    imx307_2l_write_register(ViPipe, 0x3002, 0x01); /* XTMSTA */

    return;
}

void imx307_2l_restart(VI_PIPE ViPipe)
{
    imx307_2l_write_register(ViPipe, 0x3000, 0x00); /* standby */
    delay_ms(20);
    imx307_2l_write_register(ViPipe, 0x3002, 0x00); /* master mode start */
    imx307_2l_write_register(ViPipe, 0x304b, 0x0a);

    return;
}

#define IMX307_SENSOR_1080P_30FPS_LINEAR_MODE (1)
#define IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE (2)
#define IMX307_SENSOR_720P_30FPS_LINEAR_MODE (3)

void imx307_2l_wdr_1080p30_2to1_init(VI_PIPE ViPipe);
void imx307_2l_linear_1080p30_init(VI_PIPE ViPipe);
void imx307_2l_linear_720p30_init(VI_PIPE ViPipe);

void imx307_2l_default_reg_init(VI_PIPE ViPipe)
{
    GK_U32 i;

    for (i = 0; i < g_pastImx307_2l[ViPipe]->astRegsInfo[0].u32RegNum;
         i++) {
        imx307_2l_write_register(ViPipe,
            g_pastImx307_2l[ViPipe]
                ->astRegsInfo[0]
                .astI2cData[i]
                .u32RegAddr,
            g_pastImx307_2l[ViPipe]
                ->astRegsInfo[0]
                .astI2cData[i]
                .u32Data);
    }
}

void imx307_2l_init(VI_PIPE ViPipe)
{
    WDR_MODE_E enWDRMode;
    GK_U8 u8ImgMode;

    enWDRMode = g_pastImx307_2l[ViPipe]->enWDRMode;
    u8ImgMode = g_pastImx307_2l[ViPipe]->u8ImgMode;

    imx307_2l_i2c_init(ViPipe);

    /* When sensor first init, config all registers */
    if (WDR_MODE_2To1_LINE == enWDRMode) {
        if (IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE == u8ImgMode) { /* IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE */
            imx307_2l_wdr_1080p30_2to1_init(ViPipe);
        } else {
        }
    } else {
        if (IMX307_SENSOR_720P_30FPS_LINEAR_MODE == u8ImgMode)
            imx307_2l_linear_720p30_init(ViPipe);
        else
            imx307_2l_linear_1080p30_init(ViPipe);
    }
    g_pastImx307_2l[ViPipe]->bInit = GK_TRUE;
    return;
}

void imx307_2l_exit(VI_PIPE ViPipe)
{
    imx307_2l_i2c_exit(ViPipe);

    return;
}

enum WINMODE {
    WINMODE_1080P = 0,
    WINMODE_720P = 1,
    WINMODE_CROP = 4,
};

enum MIPI_LANES {
    MIPI_LANES_2 = 2,
    MIPI_LANES_4 = 4,
};

static void imx307_write_adjacent(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
    imx307_2l_write_register(ViPipe, addr, data & 0xff);
    imx307_2l_write_register(ViPipe, addr + 1, (data & 0xff00) >> 8);
}

void imx307_2l_init_universal(VI_PIPE ViPipe, enum WINMODE winmode,
    unsigned fps, enum MIPI_LANES mipi_lanes,
    int bitness)
{
    if (fps <= 25)
        fps = 25;
    else if (fps <= 30)
        fps = 30;
    else if (fps <= 50)
        fps = 50;
    else if (fps <= 60)
        fps = 60;

    // printf("MIPI LANES: %d, FPS: %d\n", mipi_lanes, fps);

    // Enter Standby
    imx307_2l_write_register(ViPipe, 0x3000, 0x01); // Standby mode
    imx307_2l_write_register(ViPipe, 0x3002, 0x00); // XMSTA
    imx307_2l_write_register(ViPipe, 0x3005, bitness == 12 ? 1 : 0); // ADBIT
    imx307_2l_write_register(ViPipe, 0x3007, winmode << 4); // VREVERSE & HREVERSE & WINMODE
    imx307_2l_write_register(ViPipe, 0x3009, fps <= 30 ? 2 : 1); // FRSEL & FDG_SEL

    if (bitness == 10) {
        imx307_2l_write_register(ViPipe, 0x300A, 0x3C); // BLKLEVEL
    }

    imx307_2l_write_register(ViPipe, 0x3011, 0x0A);

    // VMAX
    if (winmode == WINMODE_720P) {
        imx307_write_adjacent(ViPipe, 0x3018, 0x2EE); // 750
    } else if (winmode == WINMODE_1080P) {
        imx307_write_adjacent(ViPipe, 0x3018, 0x465); // 1125
    }

    // HMAX
    if (winmode == WINMODE_720P) {
        if (fps == 25)
            imx307_write_adjacent(ViPipe, 0x301C, 0x1EF0);
        else if (fps == 30)
            imx307_write_adjacent(ViPipe, 0x301C, 0x19C8);
        else if (fps == 50)
            imx307_write_adjacent(ViPipe, 0x301C, 0x0F78);
        else if (fps == 60)
            imx307_write_adjacent(ViPipe, 0x301C, 0x0CE4);

    } else if (winmode == WINMODE_1080P) {
        if (fps == 25)
            imx307_write_adjacent(ViPipe, 0x301C, 0x14A0);
        else if (fps == 30)
            imx307_write_adjacent(ViPipe, 0x301C, 0x1130);
        else if (fps == 50)
            imx307_write_adjacent(ViPipe, 0x301C, 0x0A50);
        else if (fps == 60)
            imx307_write_adjacent(ViPipe, 0x301C, 0x0898);
    }

    // TODO: +LVDS mode
    imx307_2l_write_register(ViPipe, 0x3046, bitness == 12 ? 1 : 0); // ODBIT & OPORTSEL
    imx307_2l_write_register(ViPipe, 0x304B, 0x0A); // XVSOUTSEL & XHSOUTSEL

    imx307_2l_write_register(ViPipe, 0x305C, 0x18); // INCKSEL1 37.125MHz
    imx307_2l_write_register(ViPipe, 0x305D, 0x03); // INCKSEL2
    imx307_2l_write_register(ViPipe, 0x305E, 0x20);
    imx307_2l_write_register(ViPipe, 0x305F, 0x01);

    imx307_2l_write_register(ViPipe, 0x309E, 0x4A);
    imx307_2l_write_register(ViPipe, 0x309F, 0x4A);

    imx307_2l_write_register(ViPipe, 0x311C, 0x0E);
    imx307_2l_write_register(ViPipe, 0x3128, 0x04);
    imx307_2l_write_register(ViPipe, 0x313B, 0x41);
    imx307_2l_write_register(ViPipe, 0x315E, 0x1A); // INCKSEL5 37.125MHz INCK5 Setting
    imx307_2l_write_register(ViPipe, 0x3164, 0x1A); // INCKSEL6 37.125MHz
    if (bitness == 12) {
        imx307_2l_write_register(ViPipe, 0x3129, 0); // ADBIT1
        imx307_2l_write_register(ViPipe, 0x317C, 0); // ADBIT2
        imx307_2l_write_register(ViPipe, 0x31EC, 0); // ADBIT3
        imx307_2l_write_register(ViPipe, 0x3441, 0x0C); // CSI_DT_FMT
        imx307_2l_write_register(ViPipe, 0x3442, 0x0C); // CSI_DT_FMT
    } else { /* bitness == 10 */
        imx307_2l_write_register(ViPipe, 0x3129, 0x1D); // ADBIT1
        imx307_2l_write_register(ViPipe, 0x317C, 0x12); // ADBIT2
        imx307_2l_write_register(ViPipe, 0x31EC, 0x37); // ADBIT3
        imx307_2l_write_register(ViPipe, 0x3441, 0x0A); // CSI_DT_FMT
        imx307_2l_write_register(ViPipe, 0x3442, 0x0A); // CSI_DT_FMT
    }

    if (mipi_lanes == MIPI_LANES_2) {
        // if (fps <= 30)
        imx307_2l_write_register(ViPipe, 0x3405, 0x10); // REPETITION
        // else
        // imx307_2l_write_register(ViPipe, 0x3405, 0x0); // REPETITION
    }

    if (winmode == WINMODE_720P) {
        imx307_write_adjacent(ViPipe, 0x3418, 0x2D9); // Y_OUT_SIZE
        imx307_2l_write_register(ViPipe, 0x3414, 0x04); // OPB_SIZE_V
    } else if (winmode == WINMODE_1080P) {
        imx307_write_adjacent(ViPipe, 0x3418, 0x449); // Y_OUT_SIZE
        imx307_2l_write_register(ViPipe, 0x3414, 0x0A); // OPB_SIZE_V
    }

    if (mipi_lanes == MIPI_LANES_2) {
        imx307_2l_write_register(ViPipe, 0x3443, 0x01); // CSI_LANE_MODE
        imx307_2l_write_register(ViPipe, 0x3407, 0x01); // PHYSICAL_LANE_NUM
    } else if (mipi_lanes == MIPI_LANES_4) {
        imx307_2l_write_register(ViPipe, 0x3443, 0x03); // CSI_LANE_MODE
        imx307_2l_write_register(ViPipe, 0x3407, 0x03); // PHYSICAL_LANE_NUM
    }

    imx307_2l_write_register(ViPipe, 0x3444, 0x20); // EXTCK_FREQ
    imx307_2l_write_register(ViPipe, 0x3445, 0x25); // EXTCK_FREQ

    if (winmode == WINMODE_720P) {
        if (mipi_lanes == MIPI_LANES_2 && fps <= 30) {
            imx307_2l_write_register(ViPipe, 0x3446,
                0x4F); // TCLKPOST
            imx307_2l_write_register(ViPipe, 0x3448,
                0x2F); // THSZERO
            imx307_2l_write_register(ViPipe, 0x344A,
                0x17); // THSPREPARE
            imx307_2l_write_register(ViPipe, 0x344C,
                0x17); // TCLKTRAIL
            imx307_2l_write_register(ViPipe, 0x344E,
                0x17); // THSTRAIL
            imx307_2l_write_register(ViPipe, 0x3450,
                0x57); // TCLKZERO
            imx307_2l_write_register(ViPipe, 0x3452,
                0x17); // TCLKPREPARE
            imx307_2l_write_register(ViPipe, 0x3454, 0x17); // TLPX

        } else if (mipi_lanes == MIPI_LANES_2 && fps > 30) {
            imx307_2l_write_register(ViPipe, 0x3446,
                0x67); // TCLKPOST
            imx307_2l_write_register(ViPipe, 0x3448,
                0x57); // THSZERO
            imx307_2l_write_register(ViPipe, 0x344A,
                0x2F); // THSPREPARE
            imx307_2l_write_register(ViPipe, 0x344C,
                0x27); // TCLKTRAIL
            imx307_2l_write_register(ViPipe, 0x344E,
                0x2F); // THSTRAIL
            imx307_2l_write_register(ViPipe, 0x3450,
                0xBF); // TCLKZERO
            imx307_2l_write_register(ViPipe, 0x3452,
                0x2F); // TCLKPREPARE
            imx307_2l_write_register(ViPipe, 0x3454, 0x27); // TLPX

        } else if (mipi_lanes == MIPI_LANES_4 && fps <= 30) {
            imx307_2l_write_register(ViPipe, 0x3446,
                0x47); // TCLKPOST
            imx307_2l_write_register(ViPipe, 0x3448,
                0x17); // THSZERO
            imx307_2l_write_register(ViPipe, 0x344A,
                0x0F); // THSPREPARE
            imx307_2l_write_register(ViPipe, 0x344C,
                0x0F); // TCLKTRAIL
            imx307_2l_write_register(ViPipe, 0x344E,
                0x0F); // THSTRAIL
            imx307_2l_write_register(ViPipe, 0x3450,
                0x2B); // TCLKZERO
            imx307_2l_write_register(ViPipe, 0x3452,
                0x0B); // TCLKPREPARE
            imx307_2l_write_register(ViPipe, 0x3454, 0x0F); // TLPX

        } else if (mipi_lanes == MIPI_LANES_4 && fps > 30) {
            printf("> Global timing 4 lanes %d fps\n", fps);
            imx307_2l_write_register(ViPipe, 0x3446, 0x4F); // TCLKPOST
            imx307_2l_write_register(ViPipe, 0x3448, 0x2F); // THSZERO
            imx307_2l_write_register(ViPipe, 0x344A, 0x17); // THSPREPARE
            imx307_2l_write_register(ViPipe, 0x344C, 0x17); // TCLKTRAIL
            imx307_2l_write_register(ViPipe, 0x344E, 0x17); // THSTRAIL
            imx307_2l_write_register(ViPipe, 0x3450, 0x57); // TCLKZERO
            imx307_2l_write_register(ViPipe, 0x3452, 0x17); // TCLKPREPARE
            imx307_2l_write_register(ViPipe, 0x3454, 0x17); // TLPX
        }
    } else if (winmode == WINMODE_1080P) {
        if (mipi_lanes == MIPI_LANES_2 && fps <= 30) {
            imx307_2l_write_register(ViPipe, 0x3446,
                0x57); // TCLKPOST
            imx307_2l_write_register(ViPipe, 0x3448,
                0x37); // THSZERO
            imx307_2l_write_register(ViPipe, 0x344A,
                0x1F); // THSPREPARE
            imx307_2l_write_register(ViPipe, 0x344C,
                0x1F); // TCLKTRAIL
            imx307_2l_write_register(ViPipe, 0x344E,
                0x1F); // THSTRAIL
            imx307_2l_write_register(ViPipe, 0x3450,
                0x77); // TCLKZERO
            imx307_2l_write_register(ViPipe, 0x3452,
                0x1F); // TCLKPREPARE
            imx307_2l_write_register(ViPipe, 0x3454, 0x17); // TLPX
        } else if (mipi_lanes == MIPI_LANES_2 && fps > 30) {
            imx307_2l_write_register(ViPipe, 0x3446,
                0x77); // TCLKPOST
            imx307_2l_write_register(ViPipe, 0x3448,
                0x67); // THSZERO
            imx307_2l_write_register(ViPipe, 0x344A,
                0x47); // THSPREPARE
            imx307_2l_write_register(ViPipe, 0x344C,
                0x37); // TCLKTRAIL
            imx307_2l_write_register(ViPipe, 0x344E,
                0x3F); // THSTRAIL
            imx307_2l_write_register(ViPipe, 0x3450,
                0xFF); // TCLKZERO
            imx307_2l_write_register(ViPipe, 0x3452,
                0x3F); // TCLKPREPARE
            imx307_2l_write_register(ViPipe, 0x3454, 0x37); // TLPX
        } else if (mipi_lanes == MIPI_LANES_4 && fps <= 30) {
            imx307_2l_write_register(ViPipe, 0x3446,
                0x47); // TCLKPOST
            imx307_2l_write_register(ViPipe, 0x3448,
                0x1F); // THSZERO
            imx307_2l_write_register(ViPipe, 0x344A,
                0x17); // THSPREPARE
            imx307_2l_write_register(ViPipe, 0x344C,
                0x0F); // TCLKTRAIL
            imx307_2l_write_register(ViPipe, 0x344E,
                0x17); // THSTRAIL
            imx307_2l_write_register(ViPipe, 0x3450,
                0x47); // TCLKZERO
            imx307_2l_write_register(ViPipe, 0x3452,
                0x0F); // TCLKPREPARE
            imx307_2l_write_register(ViPipe, 0x3454, 0x0F); // TLPX
        } else if (mipi_lanes == MIPI_LANES_4 && fps > 30) {
            imx307_2l_write_register(ViPipe, 0x3446,
                0x57); // TCLKPOST
            imx307_2l_write_register(ViPipe, 0x3448,
                0x37); // THSZERO
            imx307_2l_write_register(ViPipe, 0x344A,
                0x1F); // THSPREPARE
            imx307_2l_write_register(ViPipe, 0x344C,
                0x1F); // TCLKTRAIL
            imx307_2l_write_register(ViPipe, 0x344E,
                0x1F); // THSTRAIL
            imx307_2l_write_register(ViPipe, 0x3450,
                0x77); // TCLKZERO
            imx307_2l_write_register(ViPipe, 0x3452,
                0x1F); // TCLKPREPARE
            imx307_2l_write_register(ViPipe, 0x3454, 0x17); // TLPX
        }
    }
    // Clear clock high halves
    imx307_2l_write_register(ViPipe, 0x3447, 0x00);
    imx307_2l_write_register(ViPipe, 0x3449, 0x00);
    imx307_2l_write_register(ViPipe, 0x344B, 0x00);
    imx307_2l_write_register(ViPipe, 0x344D, 0x00);
    imx307_2l_write_register(ViPipe, 0x344F, 0x00);
    imx307_2l_write_register(ViPipe, 0x3451, 0x00);
    imx307_2l_write_register(ViPipe, 0x3453, 0x00);
    imx307_2l_write_register(ViPipe, 0x3455, 0x00);

    // X_OUT_SIZE
    if (winmode == WINMODE_720P) {
        imx307_write_adjacent(ViPipe, 0x3472, 0x51C);
    } else if (winmode == WINMODE_1080P) {
        imx307_write_adjacent(ViPipe, 0x3472, 0x79C);
    }
    imx307_2l_write_register(ViPipe, 0x3480, 0x49); // INCKSEL7

    imx307_2l_default_reg_init(ViPipe);

    // Standby Cancel
    imx307_2l_write_register(ViPipe, 0x3000, 0x00); // standby

    const char* mode_name = "1080P";
    if (winmode == WINMODE_720P)
        mode_name = "720P";
    printf("=====Sony imx307_%dl sensor %s%dfps(MIPI, %dbit) init success!=====\n",
        mipi_lanes, mode_name, fps, bitness);
}

/* 720p-HD readout mode */
void imx307_2l_linear_720p30_init(VI_PIPE ViPipe)
{
    printf("> Starting 720p\n");
    imx307_2l_init_universal(ViPipe, WINMODE_720P, sensor_framerate, goke_version == 300 ? MIPI_LANES_4 : MIPI_LANES_2, 10);
    /*
    imx307_write_adjacent(ViPipe, 0x301C, 0x0CE4);
          imx307_2l_write_register(ViPipe, 0x305C, 0x10); // INCKSEL1 37.125MHz
          imx307_2l_write_register(ViPipe, 0x305D, 0x00); // INCKSEL2
          imx307_2l_write_register(ViPipe, 0x305E, 0x10);
          imx307_2l_write_register(ViPipe, 0x305F, 0x01);

          imx307_2l_write_register(ViPipe, 0x315E,  0x1B); //  INCKSEL5,1080P,CSI-2,37.125MHz;74.25MHz->0x1B
          imx307_2l_write_register(ViPipe, 0x3164,  0x1B); //  INCKSEL6,1080P,CSI-2,37.125MHz;74.25MHz->0x1B

    imx307_2l_write_register(ViPipe, 0x3405, 0x01); // REPETITION

          imx307_2l_write_register(ViPipe, 0x3444, 0x40); //  EXTCK_FREQ
          imx307_2l_write_register(ViPipe, 0x3445, 0x4A); //  EXTCK_FREQ

    imx307_2l_write_register(ViPipe, 0x3480, 0x92); //  INCKSEL7,1080P,CSI-2,37.125MHz;74.25MHz->0x92*/
}

/* 1080P30 and 1080P25 */
void imx307_2l_linear_1080p30_init(VI_PIPE ViPipe)
{
    imx307_2l_init_universal(ViPipe, WINMODE_1080P, sensor_framerate, goke_version == 300 ? MIPI_LANES_4 : MIPI_LANES_2, 10);
}

void imx307_2l_wdr_1080p30_2to1_init(VI_PIPE ViPipe)
{
    // 10bit
    imx307_2l_write_register(ViPipe, 0x3000, 0x01); //  standby

    imx307_2l_write_register(ViPipe, 0x3002, 0x00); //  XMSTA;
    imx307_2l_write_register(ViPipe, 0x3005, 0x00); //  ADBIT 0x00->10Bit;
    imx307_2l_write_register(ViPipe, 0x3007, 0x00); // VREVERSE & HREVERSE & WINMODE
    imx307_2l_write_register(ViPipe, 0x3009, 0x01); // FRSEL & FDG SEL
    imx307_2l_write_register(ViPipe, 0x300A, 0x3C); // BLKLEVEL
    imx307_2l_write_register(ViPipe, 0x300C, 0x11);
    imx307_2l_write_register(ViPipe, 0x3011, 0x0A); //  Change after reset;

    imx307_2l_write_register(ViPipe, 0x3018, 0x65); //  VMAX[7:0]
    imx307_2l_write_register(ViPipe, 0x3019, 0x04); //  VMAX[15:8]

    imx307_2l_write_register(ViPipe, 0x301C, 0x98); //  HMAX[7:0]      0x14a0->25fps;
    imx307_2l_write_register(ViPipe, 0x301D, 0x08); //  HMAX[15:8]     0x1130->30fps;

    imx307_2l_write_register(ViPipe, 0x3020, 0x02); //  SHS1
    imx307_2l_write_register(ViPipe, 0x3021, 0x00); //  SHS1
    imx307_2l_write_register(ViPipe, 0x3024, 0x49); //  SHS2
    imx307_2l_write_register(ViPipe, 0x3025, 0x08); //  SHS2
    imx307_2l_write_register(ViPipe, 0x3030, 0x0B); //  RHS1
    imx307_2l_write_register(ViPipe, 0x3031, 0x00); //  RHS1
    imx307_2l_write_register(ViPipe, 0x3045, 0x05); //  DOLSCDEN & DOLSYDINFOEN & HINFOEN
    imx307_2l_write_register(ViPipe, 0x3046, 0x00); //  OPORTSEL & ODBIT
    imx307_2l_write_register(ViPipe, 0x304B, 0x0A); //  XVSOUTSEL & XHSOUTSEL

    imx307_2l_write_register(ViPipe, 0x305C, 0x18); //  INCKSEL1,1080P,CSI-2,37.125MHz;74.25MHz->0x0C
    imx307_2l_write_register(ViPipe, 0x305D, 0x03); //  INCKSEL2,1080P,CSI-2,37.125MHz;74.25MHz->0x03
    imx307_2l_write_register(ViPipe, 0x305E, 0x20); //  INCKSEL3,1080P,CSI-2,37.125MHz;74.25MHz->0x10
    imx307_2l_write_register(ViPipe, 0x305F, 0x01); //  INCKSEL4,1080P,CSI-2,37.125MHz;74.25MHz->0x01

    imx307_2l_write_register(ViPipe, 0x309E, 0x4A);
    imx307_2l_write_register(ViPipe, 0x309F, 0x4A);

    imx307_2l_write_register(ViPipe, 0x3106, 0x11);
    imx307_2l_write_register(ViPipe, 0x311C, 0x0E);
    imx307_2l_write_register(ViPipe, 0x3128, 0x04);
    imx307_2l_write_register(ViPipe, 0x3129,
        0x1D); //  ADBIT1,12Bit; 0x1D->10Bit;
    imx307_2l_write_register(ViPipe, 0x313B, 0x41);
    imx307_2l_write_register(ViPipe, 0x315E, 0x1A); //  INCKSEL5,1080P,CSI-2,37.125MHz;74.25MHz->0x1B
    imx307_2l_write_register(ViPipe, 0x3164, 0x1A); //  INCKSEL6,1080P,CSI-2,37.125MHz;74.25MHz->0x1B
    imx307_2l_write_register(ViPipe, 0x317C,
        0x12); //  ADBIT2,12Bit;0x12->10Bit;
    imx307_2l_write_register(ViPipe, 0x31EC,
        0x37); //  ADBIT3,12Bit;0x37->10Bit;

    // MIPI setting
    imx307_2l_write_register(ViPipe, 0x3405, 0x00); //  REPETITION
    imx307_2l_write_register(ViPipe, 0x3407, 0x01); //  PHYSICAL_LANE_NUM
    imx307_2l_write_register(ViPipe, 0x3414, 0x0A); //  OPB_SIZE_V
    imx307_2l_write_register(ViPipe, 0x3415, 0x00);
    imx307_2l_write_register(ViPipe, 0x3418, 0x9C); //  Y_OUT_SIZE
    imx307_2l_write_register(ViPipe, 0x3419, 0x08); //  Y_OUT_SIZE
    imx307_2l_write_register(ViPipe, 0x3441,
        0x0A); //  CSI_DT_FMT 10Bit
    imx307_2l_write_register(ViPipe, 0x3442, 0x0A);
    imx307_2l_write_register(ViPipe, 0x3443,
        0x01); //  CSI_LANE_MODE
    imx307_2l_write_register(ViPipe, 0x3444, 0x20); //  EXTCK_FREQ
    imx307_2l_write_register(ViPipe, 0x3445, 0x25); //  EXTCK_FREQ
    imx307_2l_write_register(ViPipe, 0x3446, 0x77); //  TCLKPOST
    imx307_2l_write_register(ViPipe, 0x3447, 0x00);
    imx307_2l_write_register(ViPipe, 0x3448, 0x67); //  THSZERO
    imx307_2l_write_register(ViPipe, 0x3449, 0x00);
    imx307_2l_write_register(ViPipe, 0x344A, 0x47); //  THSPREPARE
    imx307_2l_write_register(ViPipe, 0x344B, 0x00);
    imx307_2l_write_register(ViPipe, 0x344C, 0x37); //  TCLKTRAIL
    imx307_2l_write_register(ViPipe, 0x344D, 0x00);
    imx307_2l_write_register(ViPipe, 0x344E, 0x3F); //  THSTRAIL
    imx307_2l_write_register(ViPipe, 0x344F, 0x00);
    imx307_2l_write_register(ViPipe, 0x3450, 0xFF); //  TCLKZERO
    imx307_2l_write_register(ViPipe, 0x3451, 0x00);
    imx307_2l_write_register(ViPipe, 0x3452, 0x3F); //  TCLKPREPARE
    imx307_2l_write_register(ViPipe, 0x3453, 0x00);
    imx307_2l_write_register(ViPipe, 0x3454, 0x37); //  TLPX
    imx307_2l_write_register(ViPipe, 0x3455, 0x00);
    imx307_2l_write_register(ViPipe, 0x3472, 0xA0); //  X_OUT_SIZE
    imx307_2l_write_register(ViPipe, 0x3473, 0x07);
    imx307_2l_write_register(ViPipe, 0x347B, 0x23);
    imx307_2l_write_register(ViPipe, 0x3480, 0x49); //  INCKSEL7,1080P,CSI-2,37.125MHz;74.25MHz->0x92

    imx307_2l_default_reg_init(ViPipe);

    imx307_2l_write_register(ViPipe, 0x3000,
        0x00); //  Standby Cancel

    printf("============================================================================\n");
    printf("===Imx307_2l sensor 1080P30fps 10bit 2to1 WDR(60fps->30fps) init success!===\n");
    printf("============================================================================\n");

    return;
}
