#ifndef _HIMEDIA_BASE_H_
#define _HIMEDIA_BASE_H_

#include "himedia.h"

int    himedia_bus_init(void);
void   himedia_bus_exit(void);

#if 0
struct device *himedia_device_register(struct class *class,
		dev_t devt, const char *name);

void himedia_device_unregister(struct class *class, dev_t devt);

#else

int himedia_device_register(struct himedia_device *pdev);

void himedia_device_unregister(struct himedia_device *pdev);

#endif

struct himedia_driver *himedia_driver_register(const char *name,
        struct module *owner,  struct himedia_ops *ops);

void himedia_driver_unregister(struct himedia_driver *pdrv);

#endif
