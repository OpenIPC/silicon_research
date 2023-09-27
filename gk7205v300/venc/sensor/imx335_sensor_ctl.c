/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <ctype.h>
#include <sys/mman.h>
#include <memory.h>

#include "comm_video.h"
#include "sns_ctrl.h"

#ifdef GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "i2c.h"
#endif

const unsigned char imx335_i2c_addr = 0x34; /* I2C Address of imx335 */
const unsigned int imx335_addr_byte = 2;
const unsigned int imx335_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern WDR_MODE_E genSensorMode;
extern GK_U8 gu8SensorImageMode;
extern GK_BOOL bSensorInit;

extern ISP_SNS_STATE_S *g_pastImx335[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunImx335BusInfo[];

#define IMX335_5M_30FPS_12BIT_LINEAR_MODE (0) //2592x1944
#define IMX335_5M_30FPS_10BIT_WDR_MODE (1) //2592x1944
#define IMX335_4M_25FPS_10BIT_WDR_MODE (2) //2592x1520
#define IMX335_4M_30FPS_10BIT_WDR_MODE (3) //2592x1520
int IMX335_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunImx335BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (imx335_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int IMX335_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

int IMX335_read_register(VI_PIPE ViPipe, int addr)
{
	return GK_SUCCESS;
}
int IMX335_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = imx335_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = imx335_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = imx335_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (imx335_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
	}

	if (imx335_data_byte == 2) {
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf, imx335_addr_byte + imx335_data_byte);
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
		return GK_FAILURE;
	}

#endif
	return GK_SUCCESS;
}

void IMX335_standby(VI_PIPE ViPipe)
{
	return;
}

void IMX335_restart(VI_PIPE ViPipe)
{
	return;
}

static void delay_ms(int ms)
{
	usleep(ms * 1000);
}

void IMX335_linear_5M30_12bit_init(VI_PIPE ViPipe);
void IMX335_wdr_5M30_10bit_init(VI_PIPE ViPipe);
void IMX335_wdr_4M25_10bit_init(VI_PIPE ViPipe);
void IMX335_wdr_4M30_10bit_init(VI_PIPE ViPipe);

void imx335_default_reg_init(VI_PIPE ViPipe)
{
	GK_U32 i;
	for (i = 0; i < g_pastImx335[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
		IMX335_write_register(ViPipe,
				      g_pastImx335[ViPipe]
					      ->astRegsInfo[0]
					      .astI2cData[i]
					      .u32RegAddr,
				      g_pastImx335[ViPipe]
					      ->astRegsInfo[0]
					      .astI2cData[i]
					      .u32Data);
	}
}

void IMX335_init(VI_PIPE ViPipe)
{
	WDR_MODE_E enWDRMode;
	GK_BOOL bInit;
	GK_U8 u8ImgMode;

	bInit = g_pastImx335[ViPipe]->bInit;
	enWDRMode = g_pastImx335[ViPipe]->enWDRMode;
	u8ImgMode = g_pastImx335[ViPipe]->u8ImgMode;

	if (bInit == GK_FALSE) {
		/* sensor i2c init */
		printf("binit false IMX335 i2c init\n");
		IMX335_i2c_init(ViPipe);
	}

	/* When sensor first init, config all registers */

	if (WDR_MODE_2To1_LINE == enWDRMode) {
		if (u8ImgMode == IMX335_5M_30FPS_10BIT_WDR_MODE) {
			IMX335_wdr_5M30_10bit_init(ViPipe);
		} else if (u8ImgMode == IMX335_4M_25FPS_10BIT_WDR_MODE) {
			IMX335_wdr_4M25_10bit_init(ViPipe);
		} else if (u8ImgMode == IMX335_4M_30FPS_10BIT_WDR_MODE) {
			IMX335_wdr_4M30_10bit_init(ViPipe);
		}

		else {
			printf("IMX335_init Not support this mode\n");
			return;
		}
	} else if (enWDRMode == WDR_MODE_NONE) {
		IMX335_linear_5M30_12bit_init(ViPipe);
	} else {
		printf("IMX335_init Not support this mode\n");
		return;
	}

	imx335_default_reg_init(ViPipe);
	g_pastImx335[ViPipe]->bInit = GK_TRUE;

	return;
}

void IMX335_exit(VI_PIPE ViPipe)
{
	IMX335_i2c_exit(ViPipe);

	return;
}

void IMX335_linear_5M30_12bit_init(VI_PIPE ViPipe)
{
	IMX335_write_register(ViPipe, 0x3000, 0x01); // standby

	IMX335_write_register(ViPipe, 0x300C, 0x5B);
	IMX335_write_register(ViPipe, 0x300D, 0x40);
	IMX335_write_register(ViPipe, 0x3050, 0x00);

	IMX335_write_register(ViPipe, 0x314C, 0xC0);

	IMX335_write_register(ViPipe, 0x315A, 0x06);
	IMX335_write_register(ViPipe, 0x316A, 0x7E);

	IMX335_write_register(ViPipe, 0x319D, 0x00);
	IMX335_write_register(ViPipe, 0x319E, 0x02);
	IMX335_write_register(ViPipe, 0x31A1, 0x00);
	IMX335_write_register(ViPipe, 0x3288, 0x21);
	IMX335_write_register(ViPipe, 0x328A, 0x02);

	IMX335_write_register(ViPipe, 0x3414, 0x05);
	IMX335_write_register(ViPipe, 0x3416, 0x18);

	IMX335_write_register(ViPipe, 0x341C, 0xFF);
	IMX335_write_register(ViPipe, 0x341D, 0x01);

	IMX335_write_register(ViPipe, 0x3648, 0x01);
	IMX335_write_register(ViPipe, 0x364A, 0x04);
	IMX335_write_register(ViPipe, 0x364C, 0x04);

	IMX335_write_register(ViPipe, 0x3678, 0x01);
	IMX335_write_register(ViPipe, 0x367C, 0x31);
	IMX335_write_register(ViPipe, 0x367E, 0x31);

	IMX335_write_register(ViPipe, 0x3706, 0x10);
	IMX335_write_register(ViPipe, 0x3708, 0x03);

	IMX335_write_register(ViPipe, 0x3714, 0x02);
	IMX335_write_register(ViPipe, 0x3715, 0x02);
	IMX335_write_register(ViPipe, 0x3716, 0x01);
	IMX335_write_register(ViPipe, 0x3717, 0x03);
	IMX335_write_register(ViPipe, 0x371C, 0x3D);
	IMX335_write_register(ViPipe, 0x371D, 0x3F);

	IMX335_write_register(ViPipe, 0x372C, 0x00);
	IMX335_write_register(ViPipe, 0x372D, 0x00);
	IMX335_write_register(ViPipe, 0x372E, 0x46);
	IMX335_write_register(ViPipe, 0x372F, 0x00);
	IMX335_write_register(ViPipe, 0x3730, 0x89);
	IMX335_write_register(ViPipe, 0x3731, 0x00);
	IMX335_write_register(ViPipe, 0x3732, 0x08);
	IMX335_write_register(ViPipe, 0x3733, 0x01);
	IMX335_write_register(ViPipe, 0x3734, 0xFE);
	IMX335_write_register(ViPipe, 0x3735, 0x05);

	IMX335_write_register(ViPipe, 0x3740, 0x02);

	IMX335_write_register(ViPipe, 0x375D, 0x00);
	IMX335_write_register(ViPipe, 0x375E, 0x00);
	IMX335_write_register(ViPipe, 0x375F, 0x11);
	IMX335_write_register(ViPipe, 0x3760, 0x01);

	IMX335_write_register(ViPipe, 0x3768, 0x1B);
	IMX335_write_register(ViPipe, 0x3769, 0x1B);
	IMX335_write_register(ViPipe, 0x376A, 0x1B);
	IMX335_write_register(ViPipe, 0x376B, 0x1B);
	IMX335_write_register(ViPipe, 0x376C, 0x1A);
	IMX335_write_register(ViPipe, 0x376D, 0x17);
	IMX335_write_register(ViPipe, 0x376E, 0x0F);

	IMX335_write_register(ViPipe, 0x3776, 0x00);
	IMX335_write_register(ViPipe, 0x3777, 0x00);
	IMX335_write_register(ViPipe, 0x3778, 0x46);
	IMX335_write_register(ViPipe, 0x3779, 0x00);
	IMX335_write_register(ViPipe, 0x377A, 0x89);
	IMX335_write_register(ViPipe, 0x377B, 0x00);
	IMX335_write_register(ViPipe, 0x377C, 0x08);
	IMX335_write_register(ViPipe, 0x377D, 0x01);
	IMX335_write_register(ViPipe, 0x377E, 0x23);
	IMX335_write_register(ViPipe, 0x377F, 0x02);
	IMX335_write_register(ViPipe, 0x3780, 0xD9);
	IMX335_write_register(ViPipe, 0x3781, 0x03);
	IMX335_write_register(ViPipe, 0x3782, 0xF5);
	IMX335_write_register(ViPipe, 0x3783, 0x06);
	IMX335_write_register(ViPipe, 0x3784, 0xA5);
	IMX335_write_register(ViPipe, 0x3788, 0x0F);
	IMX335_write_register(ViPipe, 0x378A, 0xD9);
	IMX335_write_register(ViPipe, 0x378B, 0x03);
	IMX335_write_register(ViPipe, 0x378C, 0xEB);
	IMX335_write_register(ViPipe, 0x378D, 0x05);
	IMX335_write_register(ViPipe, 0x378E, 0x87);
	IMX335_write_register(ViPipe, 0x378F, 0x06);
	IMX335_write_register(ViPipe, 0x3790, 0xF5);
	IMX335_write_register(ViPipe, 0x3792, 0x43);
	IMX335_write_register(ViPipe, 0x3794, 0x7A);
	IMX335_write_register(ViPipe, 0x3796, 0xA1);
	IMX335_write_register(ViPipe, 0x3A18, 0x7F);
	IMX335_write_register(ViPipe, 0x3A1A, 0x37);
	IMX335_write_register(ViPipe, 0x3A1C, 0x37);
	IMX335_write_register(ViPipe, 0x3A1E, 0xF7);
	IMX335_write_register(ViPipe, 0x3A1F, 0x00);
	IMX335_write_register(ViPipe, 0x3A20, 0x3F);
	IMX335_write_register(ViPipe, 0x3A22, 0x6F);
	IMX335_write_register(ViPipe, 0x3A24, 0x3F);
	IMX335_write_register(ViPipe, 0x3A26, 0x5F);
	IMX335_write_register(ViPipe, 0x3A28, 0x2F);

	imx335_default_reg_init(ViPipe);
	IMX335_write_register(ViPipe, 0x3000, 0x00); // Standby Cancel
	delay_ms(18);
	IMX335_write_register(ViPipe, 0x3002, 0x00);

	printf("-------Sony IMX335_init_5M_2592x1944_12bit_linear30 Initial OK!-------\n");
}

void IMX335_wdr_5M30_10bit_init(VI_PIPE ViPipe)
{
	IMX335_write_register(ViPipe, 0x3000, 0x01);
	IMX335_write_register(ViPipe, 0x3002, 0x01);
	IMX335_write_register(ViPipe, 0x300C, 0x5B);
	IMX335_write_register(ViPipe, 0x300D, 0x40);
	IMX335_write_register(ViPipe, 0x3018, 0x00);
	IMX335_write_register(ViPipe, 0x302C, 0x30);
	IMX335_write_register(ViPipe, 0x302D, 0x00);
	IMX335_write_register(ViPipe, 0x302E, 0x38);
	IMX335_write_register(ViPipe, 0x302F, 0x0A);
	IMX335_write_register(ViPipe, 0x3030, 0xB5);
	IMX335_write_register(ViPipe, 0x3031, 0x0F);
	IMX335_write_register(ViPipe, 0x3032, 0x00);
	IMX335_write_register(ViPipe, 0x3034, 0x2C);
	IMX335_write_register(ViPipe, 0x3035, 0x01);
	IMX335_write_register(ViPipe, 0x304E, 0x00);
	IMX335_write_register(ViPipe, 0x304F, 0x00);
	IMX335_write_register(ViPipe, 0x3050, 0x00);
	IMX335_write_register(ViPipe, 0x3072, 0x28);
	IMX335_write_register(ViPipe, 0x3073, 0x00);
	IMX335_write_register(ViPipe, 0x3074, 0xB0);
	IMX335_write_register(ViPipe, 0x3075, 0x00);
	IMX335_write_register(ViPipe, 0x3076, 0x58);
	IMX335_write_register(ViPipe, 0x3077, 0x0F);
	IMX335_write_register(ViPipe, 0x3078, 0x01);
	IMX335_write_register(ViPipe, 0x3079, 0x02);
	IMX335_write_register(ViPipe, 0x307A, 0xFF);
	IMX335_write_register(ViPipe, 0x307B, 0x02);
	IMX335_write_register(ViPipe, 0x307C, 0x00);
	IMX335_write_register(ViPipe, 0x307D, 0x00);
	IMX335_write_register(ViPipe, 0x307E, 0x00);
	IMX335_write_register(ViPipe, 0x307F, 0x00);
	IMX335_write_register(ViPipe, 0x3080, 0x01);
	IMX335_write_register(ViPipe, 0x3081, 0x02);
	IMX335_write_register(ViPipe, 0x3082, 0xFF);
	IMX335_write_register(ViPipe, 0x3083, 0x02);
	IMX335_write_register(ViPipe, 0x3084, 0x00);
	IMX335_write_register(ViPipe, 0x3085, 0x00);
	IMX335_write_register(ViPipe, 0x3086, 0x00);
	IMX335_write_register(ViPipe, 0x3087, 0x00);
	IMX335_write_register(ViPipe, 0x30A4, 0x33);
	IMX335_write_register(ViPipe, 0x30A8, 0x10);
	IMX335_write_register(ViPipe, 0x30A9, 0x04);
	IMX335_write_register(ViPipe, 0x30AC, 0x00);
	IMX335_write_register(ViPipe, 0x30AD, 0x00);
	IMX335_write_register(ViPipe, 0x30B0, 0x10);
	IMX335_write_register(ViPipe, 0x30B1, 0x08);
	IMX335_write_register(ViPipe, 0x30B4, 0x00);
	IMX335_write_register(ViPipe, 0x30B5, 0x00);
	IMX335_write_register(ViPipe, 0x30B6, 0x00);
	IMX335_write_register(ViPipe, 0x30B7, 0x00);
	IMX335_write_register(ViPipe, 0x3112, 0x08);
	IMX335_write_register(ViPipe, 0x3113, 0x00);
	IMX335_write_register(ViPipe, 0x3116, 0x08);
	IMX335_write_register(ViPipe, 0x3117, 0x00);
	IMX335_write_register(ViPipe, 0x314C, 0x38);
	IMX335_write_register(ViPipe, 0x314D, 0x01);
	IMX335_write_register(ViPipe, 0x315A, 0x07);
	IMX335_write_register(ViPipe, 0x3167, 0x02);
	IMX335_write_register(ViPipe, 0x3168, 0xA9);
	IMX335_write_register(ViPipe, 0x316A, 0x7F);
	IMX335_write_register(ViPipe, 0x3199, 0x00);
	IMX335_write_register(ViPipe, 0x319D, 0x00);
	IMX335_write_register(ViPipe, 0x319E, 0x02);
	IMX335_write_register(ViPipe, 0x31A0, 0x2A);
	IMX335_write_register(ViPipe, 0x3300, 0x00);
	IMX335_write_register(ViPipe, 0x3302, 0x32);
	IMX335_write_register(ViPipe, 0x341C, 0xFF);
	IMX335_write_register(ViPipe, 0x341D, 0x01);
	IMX335_write_register(ViPipe, 0x3A00, 0x01);
	IMX335_write_register(ViPipe, 0x3A01, 0x03);
	IMX335_write_register(ViPipe, 0x3A04, 0x48);
	IMX335_write_register(ViPipe, 0x3A05, 0x09);
	IMX335_write_register(ViPipe, 0x3A18, 0x6F);
	IMX335_write_register(ViPipe, 0x3A19, 0x00);
	IMX335_write_register(ViPipe, 0x3A1A, 0x2F);
	IMX335_write_register(ViPipe, 0x3A1B, 0x00);
	IMX335_write_register(ViPipe, 0x3A1C, 0x2F);
	IMX335_write_register(ViPipe, 0x3A1D, 0x00);
	IMX335_write_register(ViPipe, 0x3A1E, 0xD7);
	IMX335_write_register(ViPipe, 0x3A1F, 0x00);
	IMX335_write_register(ViPipe, 0x3A20, 0x37);
	IMX335_write_register(ViPipe, 0x3A21, 0x00);
	IMX335_write_register(ViPipe, 0x3A22, 0x5F);
	IMX335_write_register(ViPipe, 0x3A23, 0x00);
	IMX335_write_register(ViPipe, 0x3A24, 0x37);
	IMX335_write_register(ViPipe, 0x3A25, 0x00);
	IMX335_write_register(ViPipe, 0x3A26, 0x57);
	IMX335_write_register(ViPipe, 0x3A27, 0x00);
	IMX335_write_register(ViPipe, 0x3A28, 0x2F);
	IMX335_write_register(ViPipe, 0x3A29, 0x00);

	IMX335_write_register(ViPipe, 0x3048, 0x01);
	IMX335_write_register(ViPipe, 0x3049, 0x01);
	IMX335_write_register(ViPipe, 0x304A, 0x04);
	IMX335_write_register(ViPipe, 0x304B, 0x03);
	IMX335_write_register(ViPipe, 0x304C, 0x13);
	IMX335_write_register(ViPipe, 0x3056, 0xAC);
	IMX335_write_register(ViPipe, 0x3057, 0x07);

	IMX335_write_register(ViPipe, 0x319F, 0x01);
	IMX335_write_register(ViPipe, 0x31D7, 0x01);

	IMX335_write_register(ViPipe, 0x3058, 0x68);
	IMX335_write_register(ViPipe, 0x3059, 0x01);
	IMX335_write_register(ViPipe, 0x305A, 0x00);

	IMX335_write_register(ViPipe, 0x305C, 0x1A);
	IMX335_write_register(ViPipe, 0x305D, 0x00);
	IMX335_write_register(ViPipe, 0x305E, 0x00);

	IMX335_write_register(ViPipe, 0x3068, 0x2A);
	IMX335_write_register(ViPipe, 0x3069, 0x00);
	IMX335_write_register(ViPipe, 0x306A, 0x00);

	IMX335_write_register(ViPipe, 0x31A0, 0x2A);
	IMX335_write_register(ViPipe, 0x31A1, 0x00);

	imx335_default_reg_init(ViPipe);
	IMX335_write_register(ViPipe, 0x3000, 0x00); // Standby Cancel
	delay_ms(20);
	IMX335_write_register(ViPipe, 0x3002, 0x00);

	printf("-------Sony IMX335_init_5M_2592x1944_10bit_wdr30 Initial OK!-------\n");
}

void IMX335_wdr_4M25_10bit_init(VI_PIPE ViPipe) // use weighted binning mode
{
	IMX335_write_register(ViPipe, 0x3004, 0x04);
	IMX335_write_register(ViPipe, 0x3004, 0x00);

	IMX335_write_register(ViPipe, 0x3000, 0x01);
	IMX335_write_register(ViPipe, 0x3002, 0x01);
	IMX335_write_register(ViPipe, 0x3004, 0x04);
	IMX335_write_register(ViPipe, 0x3004, 0x00);
	IMX335_write_register(ViPipe, 0x300C, 0x5B);
	IMX335_write_register(ViPipe, 0x300D, 0x40);
	IMX335_write_register(ViPipe, 0x3018, 0x04);
	IMX335_write_register(ViPipe, 0x302C, 0x3c);
	IMX335_write_register(ViPipe, 0x302D, 0x00);
	IMX335_write_register(ViPipe, 0x302E, 0x20);
	IMX335_write_register(ViPipe, 0x302F, 0x0A);

	IMX335_write_register(ViPipe, 0x304E, 0x00);
	IMX335_write_register(ViPipe, 0x304F, 0x00);
	IMX335_write_register(ViPipe, 0x3050, 0x00);
	IMX335_write_register(ViPipe, 0x3056, 0xb4);
	IMX335_write_register(ViPipe, 0x3057, 0x05);

	IMX335_write_register(ViPipe, 0x3072, 0x28);
	IMX335_write_register(ViPipe, 0x3073, 0x00);
	IMX335_write_register(ViPipe, 0x3074, 0xA8);
	IMX335_write_register(ViPipe, 0x3075, 0x02);
	IMX335_write_register(ViPipe, 0x3076, 0x68);
	IMX335_write_register(ViPipe, 0x3077, 0x0b);
	IMX335_write_register(ViPipe, 0x30c6, 0x12);
	IMX335_write_register(ViPipe, 0x30ce, 0x64);
	IMX335_write_register(ViPipe, 0x30d8, 0x6c);
	IMX335_write_register(ViPipe, 0x30d9, 0x0e);
	IMX335_write_register(ViPipe, 0x314C, 0x80);
	IMX335_write_register(ViPipe, 0x314D, 0x00);
	IMX335_write_register(ViPipe, 0x315A, 0x06);
	IMX335_write_register(ViPipe, 0x3168, 0x68);
	IMX335_write_register(ViPipe, 0x316A, 0x7E);
	IMX335_write_register(ViPipe, 0x3199, 0x00);
	IMX335_write_register(ViPipe, 0x319D, 0x00);
	IMX335_write_register(ViPipe, 0x319E, 0x03);
	IMX335_write_register(ViPipe, 0x319f, 0x01);

	IMX335_write_register(ViPipe, 0x31A0, 0x2A);
	IMX335_write_register(ViPipe, 0x31d7, 0x00);
	IMX335_write_register(ViPipe, 0x3288, 0x21);
	IMX335_write_register(ViPipe, 0x328A, 0x02);
	IMX335_write_register(ViPipe, 0x3300, 0x00);
	IMX335_write_register(ViPipe, 0x3302, 0x32);
	IMX335_write_register(ViPipe, 0x3414, 0x05);
	IMX335_write_register(ViPipe, 0x3416, 0x18);
	IMX335_write_register(ViPipe, 0x341C, 0xFF);
	IMX335_write_register(ViPipe, 0x341D, 0x01);
	IMX335_write_register(ViPipe, 0x3648, 0x01);
	IMX335_write_register(ViPipe, 0x364A, 0x04);
	IMX335_write_register(ViPipe, 0x364C, 0x04);
	IMX335_write_register(ViPipe, 0x3678, 0x01);
	IMX335_write_register(ViPipe, 0x367C, 0x31);
	IMX335_write_register(ViPipe, 0x367E, 0x31);
	IMX335_write_register(ViPipe, 0x3706, 0x10);
	IMX335_write_register(ViPipe, 0x3708, 0x03);
	IMX335_write_register(ViPipe, 0x3714, 0x02);
	IMX335_write_register(ViPipe, 0x3715, 0x02);
	IMX335_write_register(ViPipe, 0x3716, 0x01);
	IMX335_write_register(ViPipe, 0x3717, 0x03);
	IMX335_write_register(ViPipe, 0x371C, 0x3D);
	IMX335_write_register(ViPipe, 0x371D, 0x3F);
	IMX335_write_register(ViPipe, 0x372C, 0x00);
	IMX335_write_register(ViPipe, 0x372D, 0x00);
	IMX335_write_register(ViPipe, 0x372E, 0x46);
	IMX335_write_register(ViPipe, 0x372F, 0x00);
	IMX335_write_register(ViPipe, 0x3730, 0x89);
	IMX335_write_register(ViPipe, 0x3731, 0x00);
	IMX335_write_register(ViPipe, 0x3732, 0x08);
	IMX335_write_register(ViPipe, 0x3733, 0x01);
	IMX335_write_register(ViPipe, 0x3734, 0xFE);
	IMX335_write_register(ViPipe, 0x3735, 0x05);
	IMX335_write_register(ViPipe, 0x3740, 0x02);
	IMX335_write_register(ViPipe, 0x375D, 0x00);
	IMX335_write_register(ViPipe, 0x375E, 0x00);
	IMX335_write_register(ViPipe, 0x375F, 0x11);
	IMX335_write_register(ViPipe, 0x3760, 0x01);
	IMX335_write_register(ViPipe, 0x3768, 0x1B);
	IMX335_write_register(ViPipe, 0x3769, 0x1B);
	IMX335_write_register(ViPipe, 0x376A, 0x1B);
	IMX335_write_register(ViPipe, 0x376B, 0x1B);
	IMX335_write_register(ViPipe, 0x376C, 0x1A);
	IMX335_write_register(ViPipe, 0x376D, 0x17);
	IMX335_write_register(ViPipe, 0x376E, 0x0F);
	IMX335_write_register(ViPipe, 0x3776, 0x00);
	IMX335_write_register(ViPipe, 0x3777, 0x00);
	IMX335_write_register(ViPipe, 0x3778, 0x46);
	IMX335_write_register(ViPipe, 0x3779, 0x00);
	IMX335_write_register(ViPipe, 0x377A, 0x89);
	IMX335_write_register(ViPipe, 0x377B, 0x00);
	IMX335_write_register(ViPipe, 0x377C, 0x08);
	IMX335_write_register(ViPipe, 0x377D, 0x01);
	IMX335_write_register(ViPipe, 0x377E, 0x23);
	IMX335_write_register(ViPipe, 0x377F, 0x02);
	IMX335_write_register(ViPipe, 0x3780, 0xD9);
	IMX335_write_register(ViPipe, 0x3781, 0x03);
	IMX335_write_register(ViPipe, 0x3782, 0xF5);
	IMX335_write_register(ViPipe, 0x3783, 0x06);
	IMX335_write_register(ViPipe, 0x3784, 0xA5);
	IMX335_write_register(ViPipe, 0x3788, 0x0F);
	IMX335_write_register(ViPipe, 0x378A, 0xD9);
	IMX335_write_register(ViPipe, 0x378B, 0x03);
	IMX335_write_register(ViPipe, 0x378C, 0xEB);
	IMX335_write_register(ViPipe, 0x378D, 0x05);
	IMX335_write_register(ViPipe, 0x378E, 0x87);
	IMX335_write_register(ViPipe, 0x378F, 0x06);
	IMX335_write_register(ViPipe, 0x3790, 0xF5);
	IMX335_write_register(ViPipe, 0x3792, 0x43);
	IMX335_write_register(ViPipe, 0x3794, 0x7A);
	IMX335_write_register(ViPipe, 0x3796, 0xA1);
	IMX335_write_register(ViPipe, 0x37B0, 0x36);
	IMX335_write_register(ViPipe, 0x3A00, 0x01);
	IMX335_write_register(ViPipe, 0x3A01, 0x03);
	IMX335_write_register(ViPipe, 0x3A04, 0x48);
	IMX335_write_register(ViPipe, 0x3A05, 0x09);

	IMX335_write_register(ViPipe, 0x3A18, 0x67);
	IMX335_write_register(ViPipe, 0x3A19, 0x00);
	IMX335_write_register(ViPipe, 0x3A1A, 0x27);
	IMX335_write_register(ViPipe, 0x3A1B, 0x00);
	IMX335_write_register(ViPipe, 0x3A1C, 0x27);
	IMX335_write_register(ViPipe, 0x3A1D, 0x00);
	IMX335_write_register(ViPipe, 0x3A1E, 0xb7);
	IMX335_write_register(ViPipe, 0x3A1F, 0x00);
	IMX335_write_register(ViPipe, 0x3A20, 0x2F);
	IMX335_write_register(ViPipe, 0x3A21, 0x00);
	IMX335_write_register(ViPipe, 0x3A22, 0x4f);
	IMX335_write_register(ViPipe, 0x3A23, 0x00);
	IMX335_write_register(ViPipe, 0x3A24, 0x2F);
	IMX335_write_register(ViPipe, 0x3A25, 0x00);
	IMX335_write_register(ViPipe, 0x3A26, 0x47);
	IMX335_write_register(ViPipe, 0x3A27, 0x00);
	IMX335_write_register(ViPipe, 0x3A28, 0x27);
	IMX335_write_register(ViPipe, 0x3A29, 0x00);

	IMX335_write_register(ViPipe, 0x3048, 0x01);
	IMX335_write_register(ViPipe, 0x3049, 0x01);
	IMX335_write_register(ViPipe, 0x304A, 0x04);
	IMX335_write_register(ViPipe, 0x304B, 0x03);
	IMX335_write_register(ViPipe, 0x304C, 0x13);
	IMX335_write_register(ViPipe, 0x3050, 0x00);
	IMX335_write_register(ViPipe, 0x3056, 0xb4); // Y_OUT_SIZE_LOW
	IMX335_write_register(ViPipe, 0x3057, 0x05);
	IMX335_write_register(ViPipe, 0x319f, 0x01);
	IMX335_write_register(ViPipe, 0x31d7, 0x01);
	IMX335_write_register(ViPipe, 0x3200, 0x00);
	IMX335_write_register(ViPipe, 0x305C, 0x12); // SHR1_LOW
	IMX335_write_register(ViPipe, 0x305D, 0x00);
	IMX335_write_register(ViPipe, 0x305E, 0x00);
	IMX335_write_register(ViPipe, 0x3068, 0x1a); // RHS1_LOW
	IMX335_write_register(ViPipe, 0x3069, 0x00);
	IMX335_write_register(ViPipe, 0x306A, 0x00);
	IMX335_write_register(ViPipe, 0x3058, 0x70); // SHR0_LOW
	IMX335_write_register(ViPipe, 0x3059, 0x13);
	IMX335_write_register(ViPipe, 0x305a, 0x00);
	IMX335_write_register(ViPipe, 0x3060, 0x00);
	IMX335_write_register(ViPipe, 0x3061, 0x00);
	IMX335_write_register(ViPipe, 0x3062, 0x00);
	IMX335_write_register(ViPipe, 0x306C, 0x00);
	IMX335_write_register(ViPipe, 0x306D, 0x00);
	IMX335_write_register(ViPipe, 0x306E, 0x00);
	IMX335_write_register(ViPipe, 0x30E8, 0x00); // GAIN_LONG_LOW
	IMX335_write_register(ViPipe, 0x30E9, 0x00);
	IMX335_write_register(ViPipe, 0x3030, 0xb8); // VMAX
	IMX335_write_register(ViPipe, 0x3031, 0x0b);
	IMX335_write_register(ViPipe, 0x3032, 0x00);
	IMX335_write_register(ViPipe, 0x3034, 0xef);
	IMX335_write_register(ViPipe, 0x3035, 0x01);
	IMX335_write_register(ViPipe, 0x304E, 0x00);
	IMX335_write_register(ViPipe, 0x304F, 0x00);

	imx335_default_reg_init(ViPipe);
	IMX335_write_register(ViPipe, 0x3000, 0x00); // Standby Cancel

	IMX335_write_register(ViPipe, 0x3002, 0x00);

	printf("-------Sony IMX335_init_4M_2560x1440_10bit_wdr25 Initial OK!-------\n");
}

void IMX335_wdr_4M30_10bit_init(VI_PIPE ViPipe)
{
	IMX335_write_register(ViPipe, 0x3004, 0x04);
	IMX335_write_register(ViPipe, 0x3004, 0x00);

	IMX335_write_register(ViPipe, 0x300C, 0x5B);
	IMX335_write_register(ViPipe, 0x300D, 0x40);
	IMX335_write_register(ViPipe, 0x3018, 0x04);
	IMX335_write_register(ViPipe, 0x3030, 0xE4);
	IMX335_write_register(ViPipe, 0x3031, 0x0C);
	IMX335_write_register(ViPipe, 0x3034, 0x77);
	IMX335_write_register(ViPipe, 0x3035, 0x01);
	IMX335_write_register(ViPipe, 0x3048, 0x01);
	IMX335_write_register(ViPipe, 0x3049, 0x01);
	IMX335_write_register(ViPipe, 0x304A, 0x04);
	IMX335_write_register(ViPipe, 0x304B, 0x03);
	IMX335_write_register(ViPipe, 0x304C, 0x13);
	IMX335_write_register(ViPipe, 0x3050, 0x00);
	IMX335_write_register(ViPipe, 0x3056, 0x0C);
	IMX335_write_register(ViPipe, 0x3057, 0x06);
	IMX335_write_register(ViPipe, 0x3074, 0x50);
	IMX335_write_register(ViPipe, 0x3075, 0x02);
	IMX335_write_register(ViPipe, 0x3076, 0x18);
	IMX335_write_register(ViPipe, 0x3077, 0x0C);
	IMX335_write_register(ViPipe, 0x30C6, 0x12);
	IMX335_write_register(ViPipe, 0x30CE, 0x64);
	IMX335_write_register(ViPipe, 0x30D8, 0x38);
	IMX335_write_register(ViPipe, 0x30D9, 0x0F);
	IMX335_write_register(ViPipe, 0x314C, 0xC0);
	IMX335_write_register(ViPipe, 0x315A, 0x06);
	IMX335_write_register(ViPipe, 0x316A, 0x7E);
	IMX335_write_register(ViPipe, 0x319D, 0x00);
	IMX335_write_register(ViPipe, 0x319E, 0x02);
	IMX335_write_register(ViPipe, 0x3200, 0x00);
	IMX335_write_register(ViPipe, 0x3288, 0x21);
	IMX335_write_register(ViPipe, 0x328A, 0x02);
	IMX335_write_register(ViPipe, 0x3414, 0x05);
	IMX335_write_register(ViPipe, 0x3416, 0x18);
	IMX335_write_register(ViPipe, 0x341C, 0xFF);
	IMX335_write_register(ViPipe, 0x341D, 0x01);
	IMX335_write_register(ViPipe, 0x3648, 0x01);
	IMX335_write_register(ViPipe, 0x364A, 0x04);
	IMX335_write_register(ViPipe, 0x364C, 0x04);
	IMX335_write_register(ViPipe, 0x3678, 0x01);
	IMX335_write_register(ViPipe, 0x367C, 0x31);
	IMX335_write_register(ViPipe, 0x367E, 0x31);
	IMX335_write_register(ViPipe, 0x3706, 0x10);
	IMX335_write_register(ViPipe, 0x3708, 0x03);
	IMX335_write_register(ViPipe, 0x3714, 0x02);
	IMX335_write_register(ViPipe, 0x3715, 0x02);
	IMX335_write_register(ViPipe, 0x3716, 0x01);
	IMX335_write_register(ViPipe, 0x3717, 0x03);
	IMX335_write_register(ViPipe, 0x371C, 0x3D);
	IMX335_write_register(ViPipe, 0x371D, 0x3F);
	IMX335_write_register(ViPipe, 0x372C, 0x00);
	IMX335_write_register(ViPipe, 0x372D, 0x00);
	IMX335_write_register(ViPipe, 0x372E, 0x46);
	IMX335_write_register(ViPipe, 0x372F, 0x00);
	IMX335_write_register(ViPipe, 0x3730, 0x89);
	IMX335_write_register(ViPipe, 0x3731, 0x00);
	IMX335_write_register(ViPipe, 0x3732, 0x08);
	IMX335_write_register(ViPipe, 0x3733, 0x01);
	IMX335_write_register(ViPipe, 0x3734, 0xFE);
	IMX335_write_register(ViPipe, 0x3735, 0x05);
	IMX335_write_register(ViPipe, 0x3740, 0x02);
	IMX335_write_register(ViPipe, 0x375D, 0x00);
	IMX335_write_register(ViPipe, 0x375E, 0x00);
	IMX335_write_register(ViPipe, 0x375F, 0x11);
	IMX335_write_register(ViPipe, 0x3760, 0x01);
	IMX335_write_register(ViPipe, 0x3768, 0x1B);
	IMX335_write_register(ViPipe, 0x3769, 0x1B);
	IMX335_write_register(ViPipe, 0x376A, 0x1B);
	IMX335_write_register(ViPipe, 0x376B, 0x1B);
	IMX335_write_register(ViPipe, 0x376C, 0x1A);
	IMX335_write_register(ViPipe, 0x376D, 0x17);
	IMX335_write_register(ViPipe, 0x376E, 0x0F);
	IMX335_write_register(ViPipe, 0x3776, 0x00);
	IMX335_write_register(ViPipe, 0x3777, 0x00);
	IMX335_write_register(ViPipe, 0x3778, 0x46);
	IMX335_write_register(ViPipe, 0x3779, 0x00);
	IMX335_write_register(ViPipe, 0x377A, 0x89);
	IMX335_write_register(ViPipe, 0x377B, 0x00);
	IMX335_write_register(ViPipe, 0x377C, 0x08);
	IMX335_write_register(ViPipe, 0x377D, 0x01);
	IMX335_write_register(ViPipe, 0x377E, 0x23);
	IMX335_write_register(ViPipe, 0x377F, 0x02);
	IMX335_write_register(ViPipe, 0x3780, 0xD9);
	IMX335_write_register(ViPipe, 0x3781, 0x03);
	IMX335_write_register(ViPipe, 0x3782, 0xF5);
	IMX335_write_register(ViPipe, 0x3783, 0x06);
	IMX335_write_register(ViPipe, 0x3784, 0xA5);
	IMX335_write_register(ViPipe, 0x3788, 0x0F);
	IMX335_write_register(ViPipe, 0x378A, 0xD9);
	IMX335_write_register(ViPipe, 0x378B, 0x03);
	IMX335_write_register(ViPipe, 0x378C, 0xEB);
	IMX335_write_register(ViPipe, 0x378D, 0x05);
	IMX335_write_register(ViPipe, 0x378E, 0x87);
	IMX335_write_register(ViPipe, 0x378F, 0x06);
	IMX335_write_register(ViPipe, 0x3790, 0xF5);
	IMX335_write_register(ViPipe, 0x3792, 0x43);
	IMX335_write_register(ViPipe, 0x3794, 0x7A);
	IMX335_write_register(ViPipe, 0x3796, 0xA1);
	IMX335_write_register(ViPipe, 0x3A18, 0x7F);
	IMX335_write_register(ViPipe, 0x3A1A, 0x37);
	IMX335_write_register(ViPipe, 0x3A1C, 0x37);
	IMX335_write_register(ViPipe, 0x3A1E, 0xF7);
	IMX335_write_register(ViPipe, 0x3A1F, 0x00);
	IMX335_write_register(ViPipe, 0x3A20, 0x3F);
	IMX335_write_register(ViPipe, 0x3A22, 0x6F);
	IMX335_write_register(ViPipe, 0x3A24, 0x3F);
	IMX335_write_register(ViPipe, 0x3A26, 0x5F);
	IMX335_write_register(ViPipe, 0x3A28, 0x2F);
	IMX335_write_register(ViPipe, 0x31D7, 0x01);

	imx335_default_reg_init(ViPipe);
	IMX335_write_register(ViPipe, 0x3000, 0x00); //Standby Cancel

	IMX335_write_register(ViPipe, 0x3002, 0x00);

	printf("-------Sony IMX335_init_4M_2592x1520_10bit_wdr30 Initial OK!-------\n");
}
