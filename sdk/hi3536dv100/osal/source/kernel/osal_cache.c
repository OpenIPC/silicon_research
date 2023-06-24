#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <asm/cacheflush.h>
#include <linux/dma-direction.h>
#include "osal_mmz.h"

void osal_flush_cache_all(void)
{
	flush_cache_all();
}
EXPORT_SYMBOL(osal_flush_cache_all);

void osal_cpuc_flush_dcache_area(void *addr, int size)
{
	__cpuc_flush_dcache_area(addr, size);
}
EXPORT_SYMBOL(osal_cpuc_flush_dcache_area);

extern void hi_dmac_map_area(void *addr, int size, enum dma_data_direction dir);
void osal_dma_map_area(void *addr, int size, int dir)
{
    #if HICHIP==0x3516A100
    dmac_map_area(addr, size, dir);
    #elif HICHIP==0x3518E200
    //hi_dmac_map_area(addr, size, dir);
    #elif HICHIP==0x3519100
    __cpuc_flush_dcache_area(addr, size);
    #endif
}
EXPORT_SYMBOL(osal_dma_map_area);


void osal_flush_dcache_area(void *kvirt, unsigned long phys_addr, unsigned long length)
{
    hil_mmb_flush_dcache_byaddr(kvirt,phys_addr,length);
}
EXPORT_SYMBOL(osal_flush_dcache_area);

void osal_flush_l2cache_area(void *kvirt, unsigned long phys_addr, unsigned long length)
{
    outer_flush_range(phys_addr, phys_addr + length);
}
EXPORT_SYMBOL(osal_flush_l2cache_area);

int osal_flush_dcache_all(void)
{
#ifdef CONFIG_SMP
    on_each_cpu((smp_call_func_t)__cpuc_flush_kern_all, NULL, 1);
#else
    __cpuc_flush_kern_all();
#endif
    
    outer_flush_all();

    return 0;
}
EXPORT_SYMBOL(osal_flush_dcache_all);

