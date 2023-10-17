/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __SSP_H__
#define __SSP_H__

#define SSP_READ_ALT 0x1
#define SSP_WRITE_ALT 0X3

typedef struct SPI_DATA_S {
	unsigned int spi_no;
	unsigned char dev_addr;
	unsigned int dev_byte_num;
	unsigned int reg_addr;
	unsigned int addr_byte_num;
	unsigned int data;
	unsigned int data_byte_num;
} SPI_DATA_S;

#endif
