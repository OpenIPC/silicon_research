/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef _I2C_H
#define _I2C_H



#define I2C_RETRIES     0x0701
#define I2C_TIMEOUT     0x0702

#define I2C_SLAVE       0x0703
#define I2C_SLAVE_FORCE 0x0706
#define I2C_TENBIT      0x0704

#define I2C_FUNCS       0x0705

#define I2C_RDWR        0x0707

#define I2C_PEC         0x0708
#define I2C_SMBUS       0x0720
#define I2C_16BIT_REG   0x0709
#define I2C_16BIT_DATA  0x070a

typedef struct I2C_DATA_S
{
    unsigned char    dev_addr;
    unsigned int     reg_addr;
    unsigned int     addr_byte_num;
    unsigned int     data;
      unsigned int     data_byte_num;
}I2C_DATA_S;

#endif

