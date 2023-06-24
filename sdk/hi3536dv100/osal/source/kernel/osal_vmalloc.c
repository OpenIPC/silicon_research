#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include "hi_osal.h"

void *osal_vmalloc(unsigned long size)
{
	return vmalloc(size);
}
EXPORT_SYMBOL(osal_vmalloc);

void osal_vfree(const void *addr)
{
	vfree(addr);
}
EXPORT_SYMBOL(osal_vfree);

void *osal_kmalloc(unsigned long size, unsigned int osal_gfp_flag)
{
    if (osal_gfp_flag == osal_gfp_kernel)
        return kmalloc(size, GFP_KERNEL);
    else if (osal_gfp_flag == osal_gfp_atomic)
        return kmalloc(size, GFP_ATOMIC);
    else
        return NULL;
}
EXPORT_SYMBOL(osal_kmalloc);

void osal_kfree(const void *addr)
{
	kfree(addr);
}
EXPORT_SYMBOL(osal_kfree);
