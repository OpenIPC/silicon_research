/* media-mem.c
*
* Copyright (c) 2006 Hisilicon Co., Ltd. 
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*
*/

#include <generated/autoconf.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <asm/cacheflush.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/time.h>
#include <asm/outercache.h>
#include <linux/dma-mapping.h>
#include <linux/highmem.h>
#include "osal_mmz.h"
#include "mmz-proc.h"
#include "hi_osal.h"

static unsigned long _strtoul_ex(const char *s, char **ep, unsigned int base)
{
	char *__end_p;
	unsigned long __value;
	
	__value = osal_strtoul(s,&__end_p,base); 

	switch(*__end_p) { 
	case 'm': 
	case 'M':
		__value <<= 10; 
	case 'k': 
	case 'K': 
		__value <<= 10; 
		if(ep)
			(*ep) = __end_p + 1;
	default: 
		break; 
	} 

	return __value;
}


OSAL_LIST_HEAD(mmz_list);
static DEFINE_SEMAPHORE(mmz_lock);

int anony = 0;
module_param(anony, int, S_IRUGO);
static int mmz_info_phys_start = -1;
int zone_number = 0;
int block_number = 0;
unsigned int mmb_number=0; /*for mmb id*/

hil_mmz_t *hil_mmz_create(const char *name, unsigned long gfp, unsigned long phys_start, unsigned long nbytes)
{
	hil_mmz_t *p;

	mmz_trace_func();

	if(name == NULL) {
		printk(KERN_ERR "%s: 'name' can not be zero!", __FUNCTION__);
		return NULL;
	}

	p = kmalloc(sizeof(hil_mmz_t)+1, GFP_KERNEL);
	if (p == NULL)	{
	    return NULL;
	}

	memset(p, 0, sizeof(hil_mmz_t)+1);
    /* glibc unsupport strlcpy function */
#if 0    
	strlcpy(p->name, name, HIL_MMZ_NAME_LEN);
#else
    strncpy(p->name, name, HIL_MMZ_NAME_LEN);
    (p->name)[HIL_MMZ_NAME_LEN] = '\0';
#endif
    p->gfp = gfp;
	p->phys_start = phys_start;
	p->nbytes = nbytes;

    OSAL_INIT_LIST_HEAD(&p->list);
    OSAL_INIT_LIST_HEAD(&p->mmb_list);

	p->destructor = kfree;

	return p;
}
EXPORT_SYMBOL(hil_mmz_create);

hil_mmz_t *hil_mmz_create_v2(const char *name, unsigned long gfp, unsigned long phys_start, 
		unsigned long nbytes, unsigned int alloc_type, unsigned long block_align)
{
	hil_mmz_t *p;

	mmz_trace_func();

	if(name == NULL) {
		printk(KERN_ERR "%s: 'name' can not be zero!", __FUNCTION__);
		return NULL;
	}

	p = kmalloc(sizeof(hil_mmz_t), GFP_KERNEL);
	if (p == NULL) {
	    return NULL;
	}

	memset(p, 0, sizeof(hil_mmz_t));
    /* glibc unsupport strlcpy function */
#if 0    
    strlcpy(p->name, name, HIL_MMZ_NAME_LEN);
#else
    strncpy(p->name, name, HIL_MMZ_NAME_LEN);
    (p->name)[HIL_MMZ_NAME_LEN] = '\0';
#endif
    
	p->gfp = gfp;
	p->phys_start = phys_start;
	p->nbytes = nbytes;
	p->alloc_type = alloc_type;
	p->block_align = block_align;

    OSAL_INIT_LIST_HEAD(&p->list);
    OSAL_INIT_LIST_HEAD(&p->mmb_list);

	p->destructor = kfree;

	return p;
}

int hil_mmz_destroy(hil_mmz_t *zone)
{
	if(zone == NULL)
		return -1;
	if(zone->destructor)
		zone->destructor(zone);
	return 0;
}
EXPORT_SYMBOL(hil_mmz_destroy);

static int _check_mmz(hil_mmz_t *zone)
{
	hil_mmz_t *p;

	unsigned long new_start=zone->phys_start;
	unsigned long new_end=zone->phys_start+zone->nbytes;

	if (zone->nbytes == 0)
		return -1;

	if (!((new_start>=__pa(high_memory)) || (new_start<PHYS_OFFSET && new_end<=PHYS_OFFSET))) {
		printk(KERN_ERR "ERROR: Conflict MMZ:\n");
		printk(KERN_ERR HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(zone));
		printk(KERN_ERR "MMZ conflict to kernel memory (0x%08lX, 0x%08lX)\n",
				(long unsigned int)PHYS_OFFSET, (long unsigned int)(__pa(high_memory) - 1));
		return -1;
	}

    osal_list_for_each_entry(p,&mmz_list, list) {
        unsigned long start, end;
        start = p->phys_start;
        end   = p->phys_start + p->nbytes;

		if(new_start >= end)
			continue;
		else if (new_start < start && new_end <= start)
			continue;
		else
			;

		printk(KERN_ERR "ERROR: Conflict MMZ:\n");
		printk(KERN_ERR "MMZ new:   " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(zone));
		printk(KERN_ERR "MMZ exist: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(p));
		printk(KERN_ERR "Add new MMZ failed!\n");
		return -1;
	}

	return 0;
}

int hil_mmz_register(hil_mmz_t *zone)
{
	int ret = 0;

	mmz_trace(1, HIL_MMZ_FMT_S, hil_mmz_fmt_arg(zone));

	if(zone == NULL)
		return -1;

	down(&mmz_lock);

	ret = _check_mmz(zone);
	if (ret) {
		up(&mmz_lock);
		return ret;
	}

    OSAL_INIT_LIST_HEAD(&zone->mmb_list);

    osal_list_add(&zone->list, &mmz_list);

	up(&mmz_lock);

	return 0;
}

int hil_mmz_unregister(hil_mmz_t *zone)
{
	int losts = 0;
	hil_mmb_t *p;

	if(zone == NULL)
		return -1;

	mmz_trace_func();

	down(&mmz_lock);
    osal_list_for_each_entry(p,&zone->mmb_list, list) {
		printk(KERN_WARNING "          MB Lost: " HIL_MMB_FMT_S "\n", hil_mmb_fmt_arg(p));
		losts++;
	}

	if(losts) {
		printk(KERN_ERR "%d mmbs not free, mmz<%s> can not be deregistered!\n", losts, zone->name);
		up(&mmz_lock);
		return -1;
	}

    osal_list_del(&zone->list);
    up(&mmz_lock);

	return 0;
}

static unsigned long _find_fixed_region(unsigned long *region_len, hil_mmz_t *mmz,
		unsigned long size, unsigned long align)
{
	unsigned long start;
	unsigned long fixed_start = 0;
	unsigned long fixed_len = -1;
	unsigned long len =0;
	unsigned long blank_len =0;
	hil_mmb_t *p = NULL;

	mmz_trace_func();
	align = mmz_grain_align(align);
	if(align == 0)align = MMZ_GRAIN;
    
	start = mmz_align2(mmz->phys_start, align);
	len = mmz_grain_align(size);

	osal_list_for_each_entry(p,&mmz->mmb_list, list) {
		hil_mmb_t *next;
		mmz_trace(4,"p->phys_addr=0x%08lX p->length = %luKB \t",
				p->phys_addr, p->length/SZ_1K);
		next = osal_list_entry(p->list.next, typeof(*p), list);
		mmz_trace(4,",next = 0x%08lX\n\n", next->phys_addr);
		/*if p is the first entry or not*/
		if(osal_list_first_entry(&mmz->mmb_list, typeof(*p), list) == p) {
			blank_len = p->phys_addr - start;
			if((blank_len < fixed_len) && (blank_len>=len)) {
				fixed_len = blank_len;
				fixed_start = start;
				mmz_trace(4,"%d: fixed_region: start=0x%08lX, len=%luKB\n",
						__LINE__, fixed_start, fixed_len/SZ_1K);
			}
		}
		start = mmz_align2((p->phys_addr + p->length),align);
		//BUG_ON((start < mmz->phys_start) || (start > (mmz->phys_start + mmz->nbytes)));
		/*if we have to alloc after the last node*/
		if (osal_list_is_last(&p->list, &mmz->mmb_list)) {
			blank_len = mmz->phys_start + mmz->nbytes - start;
			if ((blank_len < fixed_len) && (blank_len >= len)){
				fixed_len = blank_len;
				fixed_start = start;
				mmz_trace(4,"%d: fixed_region: start=0x%08lX, len=%luKB\n",
						__LINE__, fixed_start, fixed_len/SZ_1K);
				break;
			} else {
				if(fixed_len != -1)
					goto out;
				fixed_start = 0;
				mmz_trace(4,"%d: fixed_region: start=0x%08lX, len=%luKB\n",
						__LINE__, fixed_start, fixed_len/SZ_1K);
				goto out;
			}
		}
		/* blank is too little */
		if ((start + len) > next->phys_addr) {
			mmz_trace(4,"start=0x%08lX ,len=%lu,next=0x%08lX\n",
					start,len,next->phys_addr);
			continue;
		}
		blank_len = next->phys_addr - start;
		if((blank_len < fixed_len) && (blank_len >= len)) {
			fixed_len = blank_len;
			fixed_start = start;
			mmz_trace(4,"%d: fixed_region: start=0x%08lX, len=%luKB\n",
					__LINE__, fixed_start, fixed_len/SZ_1K);
		}
	}

	if ((mmz_grain_align(start+len) <= (mmz->phys_start + mmz->nbytes))
			&& (start >= mmz->phys_start)
			&& (start < (mmz->phys_start + mmz->nbytes))) {
		fixed_len = len;
		fixed_start = start;
		mmz_trace(4,"%d: fixed_region: start=0x%08lX, len=%luKB\n",
				__LINE__, fixed_start, fixed_len/SZ_1K);
	} else {
		fixed_start = 0;
		mmz_trace(4,"%d: fixed_region: start=0x%08lX, len=%luKB\n",
				__LINE__, fixed_start, len/SZ_1K);
	}
out:
	*region_len = len;
	return fixed_start;
}

static unsigned long _find_fixed_region_from_highaddr(unsigned long *region_len, hil_mmz_t *mmz,
		unsigned long size, unsigned long align)
{
	int i, j;
	unsigned long fixed_start=0;
	unsigned long fixed_len=~1;

	mmz_trace_func();

	i = mmz_length2grain(mmz->nbytes);

	for(; i>0; i--) {
		unsigned long start;
		unsigned long len;
		unsigned long start_highaddr;
		
		if(mmz_get_bit(mmz,i))
			continue;

		len = 0;
		start_highaddr = mmz_pos2phy_addr(mmz,i);
		for(; i>0; i--) {
			if(mmz_get_bit(mmz,i)) {
				break;
			}

			len += MMZ_GRAIN;
		}

		if(len>=size) {
			j = mmz_phy_addr2pos(mmz, mmz_align2low(start_highaddr-size, align));
			//align = mmz_grain_align(align)/MMZ_GRAIN;
			//start = mmz_pos2phy_addr(mmz, j - align);
			start = mmz_pos2phy_addr(mmz, j);
			if((start_highaddr - len <= start) && (start <= start_highaddr - size)){
				fixed_len = len;
				fixed_start = start;
				break;
			}				
						
			mmz_trace(1,"fixed_region: start=0x%08lX, len=%luKB", fixed_start, fixed_len/SZ_1K);
		}
	}

	*region_len = fixed_len;

	return fixed_start;
}

static int _do_mmb_alloc(hil_mmb_t *mmb)
{
	hil_mmb_t *p=NULL;
	mmz_trace_func();

	/* add mmb sorted */
	osal_list_for_each_entry(p,&mmb->zone->mmb_list, list) {
		if(mmb->phys_addr < p->phys_addr)
			break;
		if(mmb->phys_addr == p->phys_addr){
			printk(KERN_ERR "ERROR: media-mem allocator bad in %s! (%s, %d)",
					mmb->zone->name,  __FUNCTION__, __LINE__);
		}
	}
	osal_list_add(&mmb->list, p->list.prev);

	mmz_trace(1,HIL_MMB_FMT_S,hil_mmb_fmt_arg(mmb));

	return 0;
}

static hil_mmb_t *__mmb_alloc(const char *name, unsigned long size, unsigned long align, 
		unsigned long gfp, const char *mmz_name, hil_mmz_t *_user_mmz)
{
	hil_mmz_t *mmz;
	hil_mmb_t *mmb;

	unsigned long start;
	unsigned long region_len;

	unsigned long fixed_start=0;
	unsigned long fixed_len=~1;
	hil_mmz_t *fixed_mmz=NULL;

	mmz_trace_func();

	if(size == 0 || size > 0x40000000UL)
		return NULL;
	if(align == 0)
		align = MMZ_GRAIN;

	size = mmz_grain_align(size);

	mmz_trace(1,"size=%luKB, align=%lu", size/SZ_1K, align);

	begin_list_for_each_mmz(mmz, gfp, mmz_name)
		if(_user_mmz!=NULL && _user_mmz!=mmz)
			continue;
		start = _find_fixed_region(&region_len, mmz, size, align);
		if( (fixed_len > region_len) && (start!=0)) {
			fixed_len = region_len;
			fixed_start = start;
			fixed_mmz = mmz;
		}
	end_list_for_each_mmz()

	if(fixed_mmz == NULL) {
		return NULL;
	}

	mmb = kmalloc(sizeof(hil_mmb_t), GFP_KERNEL);
	if (mmb == NULL){
		return NULL;
	}

	memset(mmb, 0, sizeof(hil_mmb_t));
	mmb->zone = fixed_mmz;
	mmb->phys_addr = fixed_start;
	mmb->length = size;
	mmb->id = ++mmb_number;
	if(name)
		strncpy(mmb->name, name, HIL_MMB_NAME_LEN);
	else 
		strncpy(mmb->name, "<null>", HIL_MMB_NAME_LEN);
    mmb->name[HIL_MMB_NAME_LEN] = '\0';

	if(_do_mmb_alloc(mmb)) {
		kfree(mmb);
		mmb = NULL;
	}

	return mmb;
}

static hil_mmb_t *__mmb_alloc_v2(const char *name, unsigned long size, unsigned long align, unsigned long gfp, 
			const char *mmz_name, hil_mmz_t *_user_mmz, unsigned int order)
{
	hil_mmz_t *mmz;
	hil_mmb_t *mmb;
	unsigned int i;

	unsigned long start = 0;
	unsigned long region_len = 0;

	unsigned long fixed_start=0;
	unsigned long fixed_len=~1;
	hil_mmz_t *fixed_mmz=NULL;

	mmz_trace_func();

	if(size == 0 || size > 0x40000000UL)
		return NULL;
	if(align == 0)
		align = 1;
		
	size = mmz_grain_align(size);

	mmz_trace(1,"size=%luKB, align=%lu", size/SZ_1K, align);

	begin_list_for_each_mmz(mmz, gfp, mmz_name)
		if(_user_mmz!=NULL && _user_mmz!=mmz)
			continue;
			
		if(mmz->alloc_type == SLAB_ALLOC){
			if((size-1) & size){
				for(i = 1; i <= 32; i++){
					if(!((size >> i) & ~0)){
						size = 1 << i;	
						break;
					}						
				}	
					
			}
				
		}
		else if(mmz->alloc_type == EQ_BLOCK_ALLOC){
			size = mmz_align2(size,mmz->block_align);
		}	
			
			
		if(order == LOW_TO_HIGH){			
			start = _find_fixed_region(&region_len, mmz, size, align);
		}
		else if(order == HIGH_TO_LOW)
			start = _find_fixed_region_from_highaddr(&region_len, mmz, size, align);
		if( (fixed_len > region_len) && (start!=0)) {
			fixed_len = region_len;
			fixed_start = start;
			fixed_mmz = mmz;
		}
	end_list_for_each_mmz()

	if(fixed_mmz == NULL) {
		return NULL;
	}

	mmb = kmalloc(sizeof(hil_mmb_t), GFP_KERNEL);
	if (mmb == NULL) {
	    return NULL;
	}

	memset(mmb, 0, sizeof(hil_mmb_t));
	mmb->zone = fixed_mmz;
	mmb->phys_addr = fixed_start;
	mmb->length = size;
	mmb->order = order;
	if(name)
		strncpy(mmb->name, name, HIL_MMB_NAME_LEN);
	else 
		strncpy(mmb->name, "<null>",strlen("<null>"));
    mmb->name[HIL_MMB_NAME_LEN] = '\0';

	if(_do_mmb_alloc(mmb)) {
		kfree(mmb);
		mmb = NULL;
	}

	return mmb;
}

hil_mmb_t *hil_mmb_alloc(const char *name, unsigned long size, unsigned long align, 
		unsigned long gfp, const char *mmz_name)
{
	hil_mmb_t *mmb;

	down(&mmz_lock);
	mmb = __mmb_alloc(name, size, align, gfp, mmz_name, NULL);
	up(&mmz_lock);

	return mmb;
}
EXPORT_SYMBOL(hil_mmb_alloc);

hil_mmb_t *hil_mmb_alloc_v2(const char *name, unsigned long size, unsigned long align, 
		unsigned long gfp, const char *mmz_name, unsigned int order)
{
	hil_mmb_t *mmb;

	down(&mmz_lock);
	mmb = __mmb_alloc_v2(name, size, align, gfp, mmz_name, NULL, order);
	up(&mmz_lock);

	return mmb;
}
EXPORT_SYMBOL(hil_mmb_alloc_v2);

hil_mmb_t *hil_mmb_alloc_in(const char *name, unsigned long size, unsigned long align, 
		hil_mmz_t *_user_mmz)
{
	hil_mmb_t *mmb;

	if(_user_mmz==NULL)
		return NULL;

	down(&mmz_lock);
	mmb = __mmb_alloc(name, size, align, _user_mmz->gfp, _user_mmz->name, _user_mmz);
	up(&mmz_lock);

	return mmb;
}

hil_mmb_t *hil_mmb_alloc_in_v2(const char *name, unsigned long size, unsigned long align, 
		hil_mmz_t *_user_mmz, unsigned int order)
{
	hil_mmb_t *mmb;

	if(_user_mmz==NULL)
		return NULL;

	down(&mmz_lock);
	mmb = __mmb_alloc_v2(name, size, align, _user_mmz->gfp, _user_mmz->name, _user_mmz, order);
	up(&mmz_lock);

	return mmb;
}

static void *_mmb_map2kern(hil_mmb_t *mmb, int cached)
{
	if(mmb->flags & HIL_MMB_MAP2KERN) {
		if((cached*HIL_MMB_MAP2KERN_CACHED) != (mmb->flags&HIL_MMB_MAP2KERN_CACHED)) {
			printk(KERN_ERR "mmb<%s> already kernel-mapped %s, can not be re-mapped as %s.",
					mmb->name,
					(mmb->flags&HIL_MMB_MAP2KERN_CACHED) ? "cached" : "non-cached",
					(cached) ? "cached" : "non-cached" );
			return NULL;
		}

		mmb->map_ref++;

		return mmb->kvirt;
	}

	if(cached) {
		mmb->flags |= HIL_MMB_MAP2KERN_CACHED;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,10)		
		mmb->kvirt = ioremap_cached(mmb->phys_addr, mmb->length);
#else
		mmb->kvirt = ioremap_cache(mmb->phys_addr, mmb->length);
#endif
	} else {
		mmb->flags &= ~HIL_MMB_MAP2KERN_CACHED;
		mmb->kvirt = ioremap_nocache(mmb->phys_addr, mmb->length);
	}

	if(mmb->kvirt) {
	       	mmb->flags |= HIL_MMB_MAP2KERN;
		mmb->map_ref++;
	}

	return mmb->kvirt;
}

void *hil_mmb_map2kern(hil_mmb_t *mmb)
{
	void *p;
	 
	if(mmb == NULL)
		return NULL;

	down(&mmz_lock);
	p =  _mmb_map2kern(mmb, 0);
	up(&mmz_lock);

	return p;
}
EXPORT_SYMBOL(hil_mmb_map2kern);

void *hil_mmb_map2kern_cached(hil_mmb_t *mmb)
{
	void *p;

	if(mmb == NULL)
		return NULL;

	down(&mmz_lock);
	p = _mmb_map2kern(mmb, 1);
	up(&mmz_lock);

	return p;
}
EXPORT_SYMBOL(hil_mmb_map2kern_cached);

int hil_mmb_flush_dcache_byaddr(void *kvirt, unsigned long phys_addr, 
    unsigned long length)
{
	if (NULL == kvirt)
		return -EINVAL;
	
	/*Use flush range to instead flush_cache_all, because flush_cache_all only flush
	  local cpu. And on_each_cpu macro cannot used to flush all cpus with irq disabled.*/

        /* flush without clean */
        __cpuc_flush_dcache_area(kvirt, length);

#if defined(CONFIG_CACHE_HIL2V200) || defined(CONFIG_CACHE_L2X0)
	/*flush l2 cache, use paddr*/
	/* if length > L2 cache size, then this interface will call <outer_flush_all> */
	outer_flush_range(phys_addr, phys_addr + length);
#endif
	return 0;
}
EXPORT_SYMBOL(hil_mmb_flush_dcache_byaddr);

int hil_mmb_invalid_cache_byaddr(void *kvirt, unsigned long phys_addr, unsigned long length)
{
	if (NULL == kvirt)
		return -EINVAL;

	/* dmac_map_area is invalid in  hi3518ev200 kernel, arm9 is not supported yet */
	#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,10)		
		dmac_map_area(kvirt, length, DMA_FROM_DEVICE);
	#else
		__cpuc_flush_dcache_area(kvirt, length);
	#endif
  
#if defined(CONFIG_CACHE_HIL2V200) || defined(CONFIG_CACHE_L2X0)
        /*invalid l2 cache, use paddr*/
        /* if length > L2 cache size, then this interface will call <outer_flush_all> */        
        outer_inv_range(phys_addr, phys_addr + length);
#endif

	return 0;
}
EXPORT_SYMBOL(hil_mmb_invalid_cache_byaddr);


static int _mmb_free(hil_mmb_t *mmb);

int hil_mmb_unmap(hil_mmb_t *mmb)
{
	int ref;

	if(mmb == NULL)
		return -1;

	down(&mmz_lock);

	if (mmb->flags & HIL_MMB_MAP2KERN_CACHED) {
		__cpuc_flush_dcache_area((void *)mmb->kvirt, (size_t)mmb->length);
#if defined(CONFIG_CACHE_HIL2V200) || defined(CONFIG_CACHE_L2X0)
        /* flush without clean */
		//dmac_map_area(mmb->kvirt, mmb->length, DMA_TO_DEVICE);
		outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
#endif
	}

	if(mmb->flags & HIL_MMB_MAP2KERN) {
		ref = --mmb->map_ref;
		if(mmb->map_ref !=0) {
			up(&mmz_lock);
			return ref;
		}

		iounmap(mmb->kvirt);
	}

	mmb->kvirt = NULL;
	mmb->flags &= ~HIL_MMB_MAP2KERN;
	mmb->flags &= ~HIL_MMB_MAP2KERN_CACHED;

	if((mmb->flags & HIL_MMB_RELEASED) && mmb->phy_ref ==0) {
		_mmb_free(mmb);
	}

	up(&mmz_lock);

	return 0;
}
EXPORT_SYMBOL(hil_mmb_unmap);

int hil_mmb_get(hil_mmb_t *mmb)
{
	int ref;

	if(mmb == NULL)
		return -1;

	down(&mmz_lock);

	if(mmb->flags & HIL_MMB_RELEASED)
		printk(KERN_WARNING "hil_mmb_get: amazing, mmb<%s> is released!\n", mmb->name);
	ref = ++mmb->phy_ref;

	up(&mmz_lock);

	return ref;
}

static int _mmb_free(hil_mmb_t *mmb)
{
	if (mmb->flags & HIL_MMB_MAP2KERN_CACHED) {
		__cpuc_flush_dcache_area((void *)mmb->kvirt, (size_t)mmb->length);
#if defined(CONFIG_CACHE_HIL2V200) || defined(CONFIG_CACHE_L2X0)
        /* flush without clean */
		//dmac_map_area(mmb->kvirt, mmb->length, DMA_TO_DEVICE);
		outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
#endif
	}
	osal_list_del(&mmb->list);
	kfree(mmb);

	return 0;
}

int hil_mmb_put(hil_mmb_t *mmb)
{
	int ref;

	if(mmb == NULL)
		return -1;

	down(&mmz_lock);

	if(mmb->flags & HIL_MMB_RELEASED)
		printk(KERN_WARNING "hil_mmb_put: amazing, mmb<%s> is released!\n", mmb->name);

	ref = --mmb->phy_ref;
	
	if((mmb->flags & HIL_MMB_RELEASED) && mmb->phy_ref ==0 && mmb->map_ref ==0) {
		_mmb_free(mmb);
	}

	up(&mmz_lock);

	return ref;
}

int hil_mmb_free(hil_mmb_t *mmb)
{
	mmz_trace_func();
	if(mmb == NULL)
		return -1;
	mmz_trace(1,HIL_MMB_FMT_S,hil_mmb_fmt_arg(mmb));
	down(&mmz_lock);

	if(mmb->flags & HIL_MMB_RELEASED) {
		printk(KERN_WARNING "hil_mmb_free: amazing, mmb<%s> is released before, but still used!\n", mmb->name);

		up(&mmz_lock);
		return 0;
	}

	if(mmb->phy_ref >0) {
		printk(KERN_WARNING "hil_mmb_free: free mmb<%s> delayed for which ref-count is %d!\n",
				mmb->name, mmb->map_ref);
		mmb->flags |= HIL_MMB_RELEASED;
		up(&mmz_lock);
		return 0;
	}

	if(mmb->flags & HIL_MMB_MAP2KERN) {
		printk(KERN_WARNING "hil_mmb_free: free mmb<%s> delayed for which is kernel-mapped to 0x%p with map_ref %d!\n",
				mmb->name, mmb->kvirt, mmb->map_ref);
		mmb->flags |= HIL_MMB_RELEASED;
		up(&mmz_lock);
		return 0;
	}
	_mmb_free(mmb);
	up(&mmz_lock);
	return 0;
}
EXPORT_SYMBOL(hil_mmb_free);

#define MACH_MMB(p, val, member) do{\
	hil_mmz_t *__mach_mmb_zone__; \
	(p) = NULL;\
	list_for_each_entry(__mach_mmb_zone__,&mmz_list, list) { \
		hil_mmb_t *__mach_mmb__;\
		list_for_each_entry(__mach_mmb__,&__mach_mmb_zone__->mmb_list, list) { \
			if(__mach_mmb__->member == (val)){ \
				(p) = __mach_mmb__; \
				break;\
			} \
		} \
		if(p)break;\
	} \
}while(0)

hil_mmb_t *hil_mmb_getby_phys(unsigned long addr)
{
	hil_mmb_t *p;
	down(&mmz_lock);
	MACH_MMB(p, addr, phys_addr);
	up(&mmz_lock);
	return p;
}
EXPORT_SYMBOL(hil_mmb_getby_phys);
unsigned long usr_virt_to_phys(unsigned long virt)
{
    pgd_t *pgd;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    unsigned int cacheable = 0;
    unsigned long page_addr = 0;
    unsigned long page_offset = 0;
    unsigned long phys_addr = 0;

    if (virt & 0x3) {
        printk("Invalid virtual address 0x%lx[not 4 bytes align]\n", virt);
        return 0;
    }

    if (virt >= CONFIG_PAGE_OFFSET) {
        printk("invalid usr virtual address 0x%lx\n", virt);
        return 0;
    }

    pgd = pgd_offset(current->mm, virt);
    if (pgd_none(*pgd)) {
        printk("error: not mapped in pgd!\n");
        return 0;
    }

    pud = pud_offset(pgd, virt);
    if (pud_none(*pud)) {
        printk("error: not mapped in pud!\n");
        return 0;
    }

    pmd = pmd_offset(pud, virt);
    if (pmd_none(*pmd)) {
        printk("error: not mapped in pmd!\n");
        return 0;
    }

    pte = pte_offset_map(pmd, virt);
    if (pte_none(*pte)) {
        printk("error: not mapped in pte!\n");
        return 0;
    }

    page_addr = pte_val(*pte) & PAGE_MASK;
    page_offset = virt & ~PAGE_MASK;
    phys_addr = page_addr | page_offset;

    if (pte_val(*pte) & (1<<3))
        cacheable = 1;

    /*
     * phys_addr: the lowest bit indicates its cache attribute
     * 1: cacheable
     * 0: uncacheable
     */
    phys_addr |= cacheable;

    //printk("cached[%d]!!\n", cacheable);
    //printk("--------     result phys 0x%lx\n", phys_addr);

    return phys_addr;
}
EXPORT_SYMBOL(usr_virt_to_phys);

hil_mmb_t *hil_mmb_getby_kvirt(void *virt)
{
	hil_mmb_t *p;

	if(virt == NULL)
		return NULL;

	down(&mmz_lock);
	MACH_MMB(p, virt, kvirt);
	up(&mmz_lock);

	return p;
}
EXPORT_SYMBOL(hil_mmb_getby_kvirt);

#define MACH_MMB_2(p, val, Outoffset) do{\
	hil_mmz_t *__mach_mmb_zone__; \
	(p) = NULL;\
	list_for_each_entry(__mach_mmb_zone__,&mmz_list, list) { \
		hil_mmb_t *__mach_mmb__;\
		list_for_each_entry(__mach_mmb__,&__mach_mmb_zone__->mmb_list, list) { \
			if ((__mach_mmb__->phys_addr <= (val)) && ((__mach_mmb__->length + __mach_mmb__->phys_addr) > (val))){ \
				(p) = __mach_mmb__; \
				Outoffset = val - __mach_mmb__->phys_addr;\
				break;\
			}\
		} \
		if(p)break;\
	} \
}while(0)



hil_mmb_t *hil_mmb_getby_phys_2(unsigned long addr, unsigned long *Outoffset)
{
	hil_mmb_t *p;

	down(&mmz_lock);
	MACH_MMB_2(p, addr, *Outoffset);
	up(&mmz_lock);
	return p;
}

hil_mmz_t *hil_mmz_find(unsigned long gfp, const char *mmz_name)
{
	hil_mmz_t *p;

	down(&mmz_lock);
	begin_list_for_each_mmz(p, gfp, mmz_name)
		up(&mmz_lock);
		return p;
	end_list_for_each_mmz()
	up(&mmz_lock);

	return NULL;
}

/*
 * name,gfp,phys_start,nbytes,alloc_type;...
 * All param in hex mode, except name.
 */
static int media_mem_parse_cmdline(char *s)
{
	hil_mmz_t *zone = NULL;
	char *line;
	unsigned long phys_end = 0;

	while( (line = strsep(&s,":")) !=NULL) {
		int i;
		char *argv[6];

		/*
		 * FIXME: We got 4 args in "line", formated "argv[0],argv[1],argv[2],argv[3],argv[4]".
		 * eg: "<mmz_name>,<gfp>,<phys_start_addr>,<size>,<alloc_type>"
		 * For more convenient, "hard code" are used such as "arg[0]", i.e.
		 */
		for (i=0; (argv[i] = strsep(&line, ",")) != NULL;)
			if (++i == ARRAY_SIZE(argv)) break;
		
		if (i == 4) {
			zone = hil_mmz_create("null",0,0,0);
			if (zone == NULL)
				continue;
            /* glibc unsupport strlcpy function */
#if 0    
            strlcpy(zone->name, argv[0], HIL_MMZ_NAME_LEN);
#else
            strncpy(zone->name, argv[0], HIL_MMZ_NAME_LEN);
            (zone->name)[HIL_MMZ_NAME_LEN] = '\0';
#endif
			zone->gfp = _strtoul_ex(argv[1], NULL, 0);
			zone->phys_start = _strtoul_ex(argv[2], NULL, 0);
			zone->nbytes = _strtoul_ex(argv[3], NULL, 0);
		} else if (i == 6) {
			zone = hil_mmz_create_v2("null",0,0,0,0,0);
			if (zone == NULL)
				continue;
            /* glibc unsupport strlcpy function */
#if 0    
            strlcpy(zone->name, argv[0], HIL_MMZ_NAME_LEN);
#else
            strncpy(zone->name, argv[0], HIL_MMZ_NAME_LEN);
            (zone->name)[HIL_MMZ_NAME_LEN] = '\0';
#endif
			zone->gfp = _strtoul_ex(argv[1], NULL, 0);
			zone->phys_start = _strtoul_ex(argv[2], NULL, 0);
			zone->nbytes = _strtoul_ex(argv[3], NULL, 0);
			zone->alloc_type = _strtoul_ex(argv[4], NULL, 0);
			zone->block_align = _strtoul_ex(argv[5], NULL, 0);
		} else {
			printk(KERN_ERR "MMZ: your parameter num is not correct!\n");	
			continue;
		}
		mmz_info_phys_start = zone->phys_start + zone->nbytes - 0x2000;	
		if (hil_mmz_register(zone)) {
			printk(KERN_WARNING "Add MMZ failed: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(zone));
			hil_mmz_destroy(zone);
		}
		// if phys_end is 0xFFFFFFFF (32bit)
        phys_end = (zone->phys_start + zone->nbytes);
        if ((phys_end == 0) && (zone->nbytes >= PAGE_SIZE))
        {
            // reserve last PAGE_SIZE memory
            zone->nbytes = zone->nbytes - PAGE_SIZE;
        }
        // if phys_end exceed 0xFFFFFFFF (32bit), wraping error
        if (zone->phys_start > phys_end && (phys_end != 0))
        {
            printk(KERN_ERR "MMZ: parameter is not correct! Address exceeds 0xFFFFFFFF\n");
            return -1;
        }
        
		zone = NULL;
	}

	return 0;
}


#define MEDIA_MEM_NAME  "media-mem"

#ifdef CONFIG_PROC_FS

int get_mmz_info_phys_start(void)
{
	return mmz_info_phys_start;
}

int mmz_read_proc(struct seq_file *sfile)
{
	hil_mmz_t *p;
	int len = 0;
	unsigned int zone_number = 0;
	unsigned int block_number = 0;
	unsigned int used_size = 0;
	unsigned int free_size = 0;
	unsigned int mmz_total_size = 0;

	mmz_trace_func();

	down(&mmz_lock);
	osal_list_for_each_entry(p,&mmz_list, list) {
		hil_mmb_t *mmb;
		seq_printf(sfile, "+---ZONE: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(p));
		mmz_total_size += p->nbytes / 1024;
		++zone_number;

		osal_list_for_each_entry(mmb,&p->mmb_list, list) {
			seq_printf(sfile, "   |-MMB: " HIL_MMB_FMT_S "\n", hil_mmb_fmt_arg(mmb));
			used_size += mmb->length / 1024;
			++block_number;
		}
	}

	if (0 != mmz_total_size) {
		free_size = mmz_total_size - used_size;
		seq_printf(sfile, "\n---MMZ_USE_INFO:\n total size=%dKB(%dMB),"
				"used=%dKB(%dMB + %dKB),remain=%dKB(%dMB + %dKB),"
				"zone_number=%d,block_number=%d\n",
				mmz_total_size, mmz_total_size / 1024,
				used_size, used_size / 1024, used_size % 1024,
				free_size, free_size / 1024, free_size % 1024,
				zone_number, block_number);
        //mmz_total_size = 0;
        //zone_number = 0;
        //block_number = 0;
	}
	up(&mmz_lock);

	return len;
}

static ssize_t mmz_write_proc(struct file *file, const char __user *buffer,
		                           size_t count, loff_t *data)
{
	char buf[256];

	if(count >= sizeof(buf)) {
		printk(KERN_ERR "MMZ: your parameter string is too long!\n");
		return -EIO;
	}

	memset(buf, 0, sizeof(buf));
	if (copy_from_user(buf, buffer, count))
	{
        printk("\nmmz_userdev_ioctl: copy_from_user error.\n");
		return 0;
	}
	media_mem_parse_cmdline(buf);

	return count;
}

#define MMZ_PROC_ROOT  NULL

static const struct seq_operations mmz_seq_ops = {
	.start = mmz_seq_start,
	.next = mmz_seq_next,
	.stop = mmz_seq_stop,
	.show = mmz_seq_show
};

static int mmz_proc_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &mmz_seq_ops);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0)

static struct file_operations mmz_proc_ops = {
	.owner = THIS_MODULE,
	.open = mmz_proc_open,
	.read = seq_read,
	.release = seq_release
};
static int __init media_mem_proc_init(void)
{
	struct proc_dir_entry *p;
	p = create_proc_entry(MEDIA_MEM_NAME, 0644, MMZ_PROC_ROOT);
	if(p == NULL)
		return -1;
	p->write_proc = mmz_write_proc;
	p->proc_fops = &mmz_proc_ops;
    return 0;
}
#else

static struct file_operations mmz_proc_ops = {
	.owner = THIS_MODULE,
	.open = mmz_proc_open,
	.read = seq_read,
	.write = mmz_write_proc,
	.release = seq_release
};

static int __init media_mem_proc_init(void)
{
	struct proc_dir_entry *p;

    p = proc_create(MEDIA_MEM_NAME, 0, MMZ_PROC_ROOT, &mmz_proc_ops); 
    if (!p){
        printk(KERN_ERR "Create mmz proc fail!\n");
        return -1;
    }
    return 0;
}
#endif

static void __exit media_mem_proc_exit(void)
{
	remove_proc_entry(MEDIA_MEM_NAME, MMZ_PROC_ROOT);
}

#else
static int __init media_mem_proc_init(void){ return 0; }
static void __exit media_mem_proc_exit(void){ }

#endif /* CONFIG_PROC_FS */

#define MMZ_SETUP_CMDLINE_LEN 256

#ifndef MODULE

static char __initdata setup_zones[MMZ_SETUP_CMDLINE_LEN+1] = CONFIG_HISILICON_MMZ_DEFAULT;
static int __init parse_kern_cmdline(char *line)
{
    /* glibc unsupport strlcpy function */
#if 0    
	strlcpy(setup_zones, line, sizeof(setup_zones));
#else
    strncpy(setup_zones, line, MMZ_SETUP_CMDLINE_LEN);
    setup_zones[MMZ_SETUP_CMDLINE_LEN] = '\0';
#endif
	return 1;
}
__setup("mmz=", parse_kern_cmdline);

#else
char setup_zones[MMZ_SETUP_CMDLINE_LEN+1]={'\0'};
module_param_string(mmz, setup_zones, MMZ_SETUP_CMDLINE_LEN+1, 0600);
MODULE_PARM_DESC(mmz,"mmz=name,0,start,size,type,eqsize:[others]");
#endif

int mem_check_module_param(void)
{
    if(1 != anony)
    {
        printk("param error,\"anony\" should only be 1 which is %d ,now set default value 1\n",anony);
        anony = 1;
        return -1;
    }
    return 0;
}



int media_mem_init(void)
{
    int ret = 0;

	printk(KERN_INFO "Hisilicon Media Memory Zone Manager\n");
	if (anony != 1)
	{	
		printk(KERN_INFO "Module Param [anony] must set to 1, now set to 1!\n");
		anony = 1;
	}

	//media_mem_parse_cmdline(setup_zones);

	ret = media_mem_parse_cmdline(setup_zones);
	if (ret < 0)
	{
	    return ret;
	}
	
	media_mem_proc_init();
	

	//kcom_mmz_register();

	mmz_userdev_init();

	return 0;
}

#ifdef MODULE

void  mmz_exit_check(void)
{
    hil_mmz_t *pmmz;
    struct osal_list_head *p, *n;

    mmz_trace_func();
    
    osal_list_for_each_safe(p, n, &mmz_list) {
        pmmz = osal_list_entry(p,hil_mmz_t,list);
        printk(KERN_WARNING "MMZ force removed: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(pmmz));
        hil_mmz_unregister(pmmz);
    }
}

void media_mem_exit(void)
{
	mmz_userdev_exit();

	//kcom_mmz_unregister();

	mmz_exit_check();

	media_mem_proc_exit();
}

//module_init(media_mem_init);
//module_exit(media_mem_exit);

//MODULE_LICENSE("GPL");
//MODULE_AUTHOR("Hisilicon");

#else

subsys_initcall(media_mem_init);

#endif

EXPORT_SYMBOL(hil_mmb_getby_phys_2);
