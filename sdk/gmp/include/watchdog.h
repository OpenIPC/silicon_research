/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef _LINUX_WATCHDOG_H
#define _LINUX_WATCHDOG_H

#define    WATCHDOG_IOCTL_BASE    'W'

struct watchdog_info {
    unsigned int options;
    unsigned int firmware_version;
    unsigned char identity[32];
};

#define    WDIOC_GETSUPPORT     _IOR(WATCHDOG_IOCTL_BASE, 0, struct watchdog_info)
#define    WDIOC_GETSTATUS      _IOR(WATCHDOG_IOCTL_BASE, 1, int)
#define    WDIOC_GETBOOTSTATUS  _IOR(WATCHDOG_IOCTL_BASE, 2, int)
#define    WDIOC_SETOPTIONS     _IOWR(WATCHDOG_IOCTL_BASE, 4, int)
#define    WDIOC_KEEPALIVE      _IO(WATCHDOG_IOCTL_BASE, 5)
#define    WDIOC_SETTIMEOUT     _IOWR(WATCHDOG_IOCTL_BASE, 6, int)
#define    WDIOC_GETTIMEOUT     _IOR(WATCHDOG_IOCTL_BASE, 7, int)

#define    WDIOF_UNKNOWN        -1
#define    WDIOS_UNKNOWN        -1

#define    WDIOF_OVERHEAT       0x0001
#define    WDIOF_FANFAULT       0x0002
#define    WDIOF_EXTERN1        0x0004
#define    WDIOF_EXTERN2        0x0008
#define    WDIOF_POWERUNDER     0x0010
#define    WDIOF_CARDRESET      0x0020
#define    WDIOF_POWEROVER      0x0040
#define    WDIOF_SETTIMEOUT     0x0080
#define    WDIOF_MAGICCLOSE     0x0100
#define    WDIOF_PRETIMEOUT     0x0200
#define    WDIOF_KEEPALIVEPING  0x8000

#define    WDIOS_DISABLECARD    0x0001
#define    WDIOS_ENABLECARD     0x0002
#define    WDIOS_TEMPPANIC      0x0004


#endif
