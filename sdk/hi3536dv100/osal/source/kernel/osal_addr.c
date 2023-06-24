#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0)
#include <mach/io.h>
#endif

void *osal_ioremap(unsigned long phys_addr, int size)
{
	return ioremap(phys_addr, size);
}
EXPORT_SYMBOL(osal_ioremap);

void *osal_ioremap_nocache(unsigned long phys_addr, int size)
{
	return ioremap_nocache(phys_addr, size);
}
EXPORT_SYMBOL(osal_ioremap_nocache);

void *osal_ioremap_cached(unsigned long phys_addr, int size)
{
    #if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,0)
	return ioremap_cached(phys_addr, size);
    #else
	return ioremap_cache(phys_addr, size);
    #endif
}
EXPORT_SYMBOL(osal_ioremap_cached);

void osal_iounmap(void *addr)
{
	iounmap(addr);
}
EXPORT_SYMBOL(osal_iounmap);

#include <linux/kallsyms.h>
#include <mach/io.h>
void *osal_ioaddress(int addr)
{
	return (void *)IO_ADDRESS(addr);
}
EXPORT_SYMBOL(osal_ioaddress);

unsigned long osal_copy_from_user(void *to, const void *from, unsigned long n)
{
	return copy_from_user(to, from, n);
}
EXPORT_SYMBOL(osal_copy_from_user);

unsigned long osal_copy_to_user(void *to, const void *from, unsigned long n)
{
	return copy_to_user(to, from, n);
}
EXPORT_SYMBOL(osal_copy_to_user);

int osal_access_ok(int type, const void *addr, unsigned long size)
{
    return access_ok(type, addr, size);
}
EXPORT_SYMBOL(osal_access_ok);


