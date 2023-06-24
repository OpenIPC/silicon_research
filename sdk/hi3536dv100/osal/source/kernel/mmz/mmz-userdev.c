/* userdev.c
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

#include <linux/kernel.h>
#include <linux/version.h>
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
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/time.h>
#include <linux/sched.h>
#include <linux/dma-mapping.h>
#include <asm/uaccess.h>
#include <mach/io.h>
#include <asm/io.h>
#include <asm/cacheflush.h>
#include "osal_mmz.h"
#include "hi_osal.h"

#define error(s...) do{ printk(KERN_ERR "mmz_userdev:%s: ", __FUNCTION__); printk(s); }while(0)
#define warning(s...) do{ printk(KERN_WARNING "mmz_userdev:%s: ", __FUNCTION__); printk(s); }while(0)

struct mmz_userdev_info
{
    pid_t pid;
    pid_t mmap_pid;
    struct semaphore sem;
    struct osal_list_head list;
};

static int mmz_flush_dcache_mmb_dirty(struct dirty_area* p_area)
{

    if (p_area == NULL)
    { return -EINVAL; }

    /*flush l1 cache, use vir addr*/
    __cpuc_flush_dcache_area((void*)p_area->dirty_virt_start, p_area->dirty_size);

#if defined(CONFIG_CACHE_HIL2V200) || defined(CONFIG_CACHE_L2X0)
    /*flush l2 cache, use paddr*/
    outer_flush_range(p_area->dirty_phys_start,
                      p_area->dirty_phys_start + p_area->dirty_size);
#endif


    return 0;
}

static int mmz_flush_dcache_mmb(struct mmb_info* pmi)
{
    hil_mmb_t* mmb;

    if (pmi == NULL)
    { return -EINVAL; }

    mmb = pmi->mmb;

    if (mmb == NULL || pmi->map_cached == 0)
    {
        printk("%s->%d,error!\n", __func__, __LINE__);
        return -EINVAL;
    }

    /*flush l1 cache, use vir addr*/
    __cpuc_flush_dcache_area(pmi->mapped, (size_t)pmi->size);

#if defined(CONFIG_CACHE_HIL2V200) || defined(CONFIG_CACHE_L2X0)
    /*flush l2 cache, use paddr*/
    outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
#endif

    return 0;
}

/*this function must not called with local irq disabled. because on_each_cpu
  marco will raise ipi interrupt. */
int mmz_flush_dcache_all(void)
{
#ifdef CONFIG_SMP
    on_each_cpu((smp_call_func_t)__cpuc_flush_kern_all, NULL, 1);
#else
    __cpuc_flush_kern_all();
#endif

    outer_flush_all();

    return 0;
}

static int mmz_userdev_open(struct inode* inode, struct file* file)
{
    struct mmz_userdev_info* pmu;
	
#ifdef ENABLE_CHECK_DEV
		if (!capable(CAP_SYS_RAWIO))
		return -EPERM;
#endif	

    pmu = kmalloc(sizeof(*pmu), GFP_KERNEL);

    if (pmu == NULL)
    {
        error("alloc mmz_userdev_info failed!\n");
        return -ENOMEM;
    }

    pmu->pid = current->pid;
    pmu->mmap_pid = 0;
    //init_MUTEX(&pmu->sem);
    sema_init(&pmu->sem, 1);
    OSAL_INIT_LIST_HEAD(&pmu->list);

    file->private_data = (void*)pmu;

    return 0;
}

static int ioctl_mmb_alloc(struct file* file, unsigned int iocmd,
                           struct mmb_info* pmi)
{
    struct mmz_userdev_info* pmu = file->private_data;
    struct mmb_info* new_mmbinfo;
    hil_mmb_t* mmb;

    mmb = hil_mmb_alloc(pmi->mmb_name, pmi->size, pmi->align, pmi->gfp,
                        pmi->mmz_name);

    if (mmb == NULL)
    {
        error("hil_mmb_alloc(%s, %lu, 0x%08lX, %lu, %s) failed!\n",
              pmi->mmb_name, pmi->size, pmi->align, pmi->gfp, pmi->mmz_name);
        return -ENOMEM;
    }

    new_mmbinfo = kmalloc(sizeof(*new_mmbinfo), GFP_KERNEL);

    if (new_mmbinfo == NULL)
    {
        hil_mmb_free(mmb);
        error("alloc mmb_info failed!\n");
        return -ENOMEM;
    }

    memcpy(new_mmbinfo, pmi, sizeof(*new_mmbinfo));
    new_mmbinfo->phys_addr = hil_mmb_phys(mmb);
    new_mmbinfo->mmb = mmb;
    new_mmbinfo->prot = PROT_READ;
    new_mmbinfo->flags = MAP_SHARED;
    osal_list_add_tail(&new_mmbinfo->list, &pmu->list);

    pmi->phys_addr = new_mmbinfo->phys_addr;

    hil_mmb_get(mmb);

    return 0;
}

static int ioctl_mmb_alloc_v2(struct file* file, unsigned int iocmd, struct mmb_info* pmi)
{
    struct mmz_userdev_info* pmu = file->private_data;
    struct mmb_info* new_mmbinfo;
    hil_mmb_t* mmb;

    mmb = hil_mmb_alloc_v2(pmi->mmb_name, pmi->size, pmi->align, pmi->gfp, pmi->mmz_name, pmi->order);

    if (mmb == NULL)
    {
        error("hil_mmb_alloc(%s, %lu, 0x%08lX, %lu, %s) failed!\n",
              pmi->mmb_name, pmi->size, pmi->align, pmi->gfp, pmi->mmz_name);
        return -ENOMEM;
    }

    new_mmbinfo = kmalloc(sizeof(*new_mmbinfo), GFP_KERNEL);

    if (new_mmbinfo == NULL)
    {
        hil_mmb_free(mmb);
        error("alloc mmb_info failed!\n");
        return -ENOMEM;
    }

    memcpy(new_mmbinfo, pmi, sizeof(*new_mmbinfo));
    new_mmbinfo->phys_addr = hil_mmb_phys(mmb);
    new_mmbinfo->mmb = mmb;
    new_mmbinfo->prot = PROT_READ;
    new_mmbinfo->flags = MAP_SHARED;
    osal_list_add_tail(&new_mmbinfo->list, &pmu->list);

    pmi->phys_addr = new_mmbinfo->phys_addr;

    hil_mmb_get(mmb);

    return 0;
}

static struct mmb_info* get_mmbinfo(unsigned long addr, struct mmz_userdev_info* pmu)
{
    struct mmb_info* p;

    osal_list_for_each_entry(p, &pmu->list, list)
    {
        if (p->phys_addr == addr)
        { break; }
    }

    if ( &p->list == &pmu->list)
    { return NULL; }

    return p;
}

static struct mmb_info* get_mmbinfo_safe(unsigned long addr, struct mmz_userdev_info* pmu)
{
    struct mmb_info* p;

    p = get_mmbinfo(addr, pmu);

    if ( p == NULL)
    {
        error("mmb(0x%08lX) not found!\n", addr);
        return NULL;
    }

    return p;
}

static int ioctl_mmb_user_unmap(struct file* file, unsigned int iocmd, struct mmb_info* pmi);

static int _usrdev_mmb_free(struct mmb_info* p)
{
    int ret = 0;

    osal_list_del(&p->list);
    hil_mmb_put(p->mmb);
    ret = hil_mmb_free(p->mmb);
    kfree(p);

    return ret;
}

static int ioctl_mmb_free(struct file* file, unsigned int iocmd, struct mmb_info* pmi)
{
    int ret = 0;
    struct mmz_userdev_info* pmu = file->private_data;
    struct mmb_info* p = get_mmbinfo_safe(pmi->phys_addr, pmu);

    if (p == NULL)
    { return -EPERM; }

    if (p->delayed_free)
    {
        warning("mmb<%s> is delayed_free, can not free again!\n", p->mmb->name);
        return -EBUSY;
    }

    if ((p->map_ref > 0) || (p->mmb_ref > 0))
    {
        warning("mmb<%s> is still in use!\n", p->mmb->name);
        p->delayed_free = 1;
        return -EBUSY;
    }

    ret = _usrdev_mmb_free(p);

    return ret;
}

static int ioctl_mmb_attr(struct file* file, unsigned int iocmd, struct mmb_info* pmi)
{
    struct mmz_userdev_info* pmu = file->private_data;
    struct mmb_info* p;

    if ( (p = get_mmbinfo_safe(pmi->phys_addr, pmu)) == NULL)
    { return -EPERM; }

    memcpy(pmi, p, sizeof(*pmi));
    return 0;
}

static int ioctl_mmb_user_remap(struct file* file, unsigned int iocmd,
                                struct mmb_info* pmi, int cached)
{
    struct mmz_userdev_info* pmu = file->private_data;
    struct mmb_info* p;

    unsigned long addr, len, prot, flags, pgoff;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
    unsigned long populate;
#endif

    if ( (p = get_mmbinfo_safe(pmi->phys_addr, pmu)) == NULL)
    { return -EPERM; }

    if ( p->mapped && p->map_ref > 0)
    {
        if (cached != p->map_cached)
        {
            error("mmb<%s> already mapped %s, can not be remap to %s.\n", p->mmb->name,
                  p->map_cached ? "cached" : "non-cached",
                  cached ? "cached" : "non-cached");
            return -EINVAL;
        }

        p->map_ref++;
        p->mmb_ref++;

        hil_mmb_get(p->mmb);

        pmi->mapped = p->mapped;

        return 0;
    }

    if (p->phys_addr & ~PAGE_MASK)
    { return -EINVAL; }

    addr = 0;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
    pgoff = p->phys_addr >> PAGE_SHIFT;
#elif LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0)
    pgoff = p->phys_addr;
#else
    pgoff = p->phys_addr >> PAGE_SHIFT;
#endif

    len = PAGE_ALIGN(p->size);

    prot = pmi->prot;
    flags = pmi->flags;

    if (prot == 0)
    { prot = p->prot; }

    if (flags == 0)
    { flags = p->flags; }

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
    down_write(&current->mm->mmap_sem);
#endif
    pmu->mmap_pid = current->pid;
    p->map_cached = cached;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
    addr = do_mmap_pgoff(file, addr, len, prot, flags, pgoff);
#elif LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0)
    addr = do_mmap(file, addr, len, prot, flags, pgoff);
#elif LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
    addr = do_mmap_pgoff(file, addr, len, prot, flags, pgoff, &populate);
#else

    pgoff = p->phys_addr;
    addr  = vm_mmap(file, addr, len, prot, flags, pgoff);
#endif

    pmu->mmap_pid = 0;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
    up_write(&current->mm->mmap_sem);
#endif

    if (IS_ERR_VALUE(addr))
    {
        error("vm_mmap(file, 0, %lu, 0x%08lX, 0x%08lX, 0x%08lX) return 0x%08lX\n",
              len, prot, flags, pgoff, addr);
        return addr;
    }

    p->mapped = (void*)addr;
    p->prot = prot;
    p->flags = flags;

    p->map_ref++;
    p->mmb_ref++;

    hil_mmb_get(p->mmb);

    pmi->mapped = p->mapped;

    return 0;
}

static int ioctl_mmb_user_unmap(struct file* file, unsigned int iocmd,
                                struct mmb_info* pmi)
{
    int ret;
    unsigned long addr, len;
    struct mmb_info* p;
    struct mmz_userdev_info* pmu = file->private_data;

    if ( (p = get_mmbinfo_safe(pmi->phys_addr, pmu)) == NULL)
    { return -EPERM; }

    if (!p->mapped)
    {
        printk(KERN_WARNING "mmb(0x%08lX) have'nt been user-mapped yet!\n", p->phys_addr);
        pmi->mapped = NULL;
        return -EIO;
    }

    if (!(p->map_ref > 0 && p->mmb_ref > 0))
    {
        error("mmb<%s> has invalid refer: map_ref = %d, mmb_ref = %d.\n", p->mmb->name, p->map_ref, p->mmb_ref);
        return -EIO;
    }

    p->map_ref--;
    p->mmb_ref--;
    hil_mmb_put(p->mmb);

    if (p->map_ref > 0)
    { return 0; }

    addr = (unsigned long)p->mapped;
    len  = PAGE_ALIGN(p->size);

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,0)
    down_write(&current->mm->mmap_sem);
#endif

    /* todo,Before lifting the map, manual brush cache*/
    if (p->map_cached)
    {
        __cpuc_flush_dcache_area((void*)addr, (size_t)len);
#if defined(CONFIG_CACHE_HIL2V200) || defined(CONFIG_CACHE_L2X0)
        /* flush without clean */
        //dmac_map_area(mmb->kvirt, mmb->length, DMA_TO_DEVICE);
        outer_flush_range(p->phys_addr, p->phys_addr + len);
#endif
    }

    //ret = vm_munmap(current->mm, addr, len);
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,0)
    ret = do_munmap(current->mm, addr, len);
#else
    ret = vm_munmap( addr, len);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,0)
    up_write(&current->mm->mmap_sem);
#endif

    if (!IS_ERR_VALUE(ret))
    {
        p->mapped = NULL;
        pmi->mapped = NULL;
    }

    if (p->delayed_free && (p->map_ref == 0) && (p->mmb_ref == 0))
    {
        _usrdev_mmb_free(p);
    }

    return ret;
}


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int mmz_userdev_ioctl_m(struct inode* inode, struct file* file, unsigned int cmd, struct mmb_info* pmi)
#else
static int mmz_userdev_ioctl_m(struct file* file, unsigned int cmd,
                               struct mmb_info* pmi)
#endif

{
    int ret = 0;

    switch (_IOC_NR(cmd))
    {
        case _IOC_NR(IOC_MMB_ALLOC):
            ret = ioctl_mmb_alloc(file, cmd, pmi);
            break;

        case _IOC_NR(IOC_MMB_ALLOC_V2):
            ret = ioctl_mmb_alloc_v2(file, cmd, pmi);
            break;

        case _IOC_NR(IOC_MMB_ATTR):
            ret = ioctl_mmb_attr(file, cmd, pmi);
            break;

        case _IOC_NR(IOC_MMB_FREE):
            ret = ioctl_mmb_free(file, cmd, pmi);
            break;

        case _IOC_NR(IOC_MMB_USER_REMAP):
            ret = ioctl_mmb_user_remap(file, cmd, pmi, 0);
            break;

        case _IOC_NR(IOC_MMB_USER_REMAP_CACHED):
            ret = ioctl_mmb_user_remap(file, cmd, pmi, 1);
            break;

        case _IOC_NR(IOC_MMB_USER_UNMAP):
            ret = ioctl_mmb_user_unmap(file, cmd, pmi);
            break;

        default:
            error("invalid ioctl cmd = %08X\n", cmd);
            ret = -EINVAL;
            break;
    }

    return ret;
}


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int mmz_userdev_ioctl_r(struct inode* inode, struct file* file, unsigned int cmd, struct mmb_info* pmi)
#else
static int mmz_userdev_ioctl_r(struct file* file, unsigned int cmd, struct mmb_info* pmi)
#endif

{
    switch (_IOC_NR(cmd))
    {
        case _IOC_NR(IOC_MMB_ADD_REF):
            pmi->mmb_ref++;
            hil_mmb_get(pmi->mmb);
            break;

        case _IOC_NR(IOC_MMB_DEC_REF):
            if (pmi->mmb_ref <= 0)
            {
                error("mmb<%s> mmb_ref is %d!\n", pmi->mmb->name, pmi->mmb_ref);
                return -EPERM;
            }

            pmi->mmb_ref--;
            hil_mmb_put(pmi->mmb);

            if (pmi->delayed_free && pmi->map_ref == 0 && pmi->mmb_ref == 0)
            {
                _usrdev_mmb_free(pmi);
            }

            break;

        default:
            return -EINVAL;
            break;
    }

    return 0;
}

/* just for test */

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int mmz_userdev_ioctl_t(struct inode* inode, struct file* file, unsigned int cmd, struct mmb_info* pmi);
#else
static int mmz_userdev_ioctl_t(struct file* file, unsigned int cmd, struct mmb_info* pmi);
#endif


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int mmz_userdev_ioctl(struct inode* inode, struct file* file, unsigned int cmd, unsigned long arg)
#else
static long mmz_userdev_ioctl(struct file* file, unsigned int cmd, unsigned long arg)
#endif

{
    int ret = 0;
    struct mmz_userdev_info* pmu = file->private_data;

    down(&pmu->sem);

    if (_IOC_TYPE(cmd) == 'm')
    {

        struct mmb_info mi;

        if (_IOC_SIZE(cmd) > sizeof(mi) || arg == 0)
        {
            error("_IOC_SIZE(cmd)=%d, arg==0x%08lX\n", _IOC_SIZE(cmd), arg);
            ret = -EINVAL;
            goto __error_exit;
        }

        memset(&mi, 0, sizeof(mi));

        if (copy_from_user(&mi, (void*)arg, _IOC_SIZE(cmd)))
        {
            printk("\nmmz_userdev_ioctl: copy_from_user error.\n");
            ret = -EFAULT;
            goto __error_exit;
        }

        mi.mmz_name[HIL_MMZ_NAME_LEN - 1] = '\0';
        mi.mmb_name[HIL_MMB_NAME_LEN - 1] = '\0';

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
        ret = mmz_userdev_ioctl_m(inode, file, cmd, &mi);
#else
        ret = mmz_userdev_ioctl_m(file, cmd, &mi);
#endif


        if (!ret && (cmd & IOC_OUT))
        {
            if (copy_to_user((void*)arg, &mi, _IOC_SIZE(cmd)))
            {
                printk("\nmmz_userdev_ioctl: copy_to_user error.\n");
                ret = -EFAULT;
                goto __error_exit;
            }
        }

    }
    else if (_IOC_TYPE(cmd) == 'r')
    {
        struct mmb_info* pmi;

        if ( (pmi = get_mmbinfo_safe(arg, pmu)) == NULL)
        {
            ret = -EPERM;
            goto __error_exit;
        }

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
        ret = mmz_userdev_ioctl_r(inode, file, cmd, pmi);
#else
        ret = mmz_userdev_ioctl_r(file, cmd, pmi);
#endif


    }
    else if (_IOC_TYPE(cmd) == 'c')
    {

        struct mmb_info* pmi;

        if (arg == 0)
        {
            mmz_flush_dcache_all();
            goto __error_exit;
        }

        if ( (pmi = get_mmbinfo_safe(arg, pmu)) == NULL)
        {
            ret = -EPERM;
            goto __error_exit;
        }

        switch (_IOC_NR(cmd))
        {
            case _IOC_NR(IOC_MMB_FLUSH_DCACHE):
                mmz_flush_dcache_mmb(pmi);
                break;

            default:
                ret = -EINVAL;
                break;
        }

    }
    else if (_IOC_TYPE(cmd) == 'd')
    {
        hil_mmb_t* mmb;
        struct mmb_info* pmi;
        struct dirty_area area;
        unsigned long offset, orig_addr;

        if (_IOC_SIZE(cmd) != sizeof(area) || arg == 0)
        {
            error("_IOC_SIZE(cmd)=%d, arg==0x%08lx\n", _IOC_SIZE(cmd), arg);
            ret = -EINVAL;
            goto __error_exit;
        }

        memset(&area, 0, sizeof(area));

        if (copy_from_user(&area, (void*)arg, _IOC_SIZE(cmd)))
        {
            printk(KERN_WARNING "\nmmz_userdev_ioctl: copy_from_user error.\n");
            ret = -EFAULT;
            goto __error_exit;
        }

        if ((mmb = hil_mmb_getby_phys_2(area.dirty_phys_start, &offset)) == NULL)
        {
            error("dirty_phys_addr=0x%08lx\n", area.dirty_phys_start);
            ret = -EFAULT;
            goto __error_exit;
        }

        pmi = get_mmbinfo_safe(mmb->phys_addr, pmu);

        if (pmi == NULL)
        {
            ret = -EPERM;
            goto __error_exit;
        }

        if (area.dirty_virt_start != (unsigned long)pmi->mapped + offset)
        {
            printk(KERN_WARNING \
                   "dirty_virt_start addr was not consistent with dirty_phys_start addr!\n");
            ret = -EFAULT;
            goto __error_exit;
        }

        if (area.dirty_phys_start + area.dirty_size > mmb->phys_addr + mmb->length)
        {
            printk(KERN_WARNING "\ndirty area overflow!\n");
            ret = -EFAULT;
            goto __error_exit;
        }

        /*cache line aligned*/
        orig_addr = area.dirty_phys_start;
        area.dirty_phys_start &= ~(CACHE_LINE_SIZE - 1);
        area.dirty_virt_start &= ~(CACHE_LINE_SIZE - 1);
        area.dirty_size = (area.dirty_size + (orig_addr - area.dirty_phys_start)
                           + (CACHE_LINE_SIZE - 1)) & ~(CACHE_LINE_SIZE - 1);

        mmz_flush_dcache_mmb_dirty(&area);

    }
    else if (_IOC_TYPE(cmd) == 't')
    {
        struct mmb_info mi;

        if (_IOC_SIZE(cmd) != sizeof(mi) || arg == 0)
        {
            error("_IOC_SIZE(cmd)=%d, arg==0x%08lx\n", _IOC_SIZE(cmd), arg);
            ret = -EINVAL;
            goto __error_exit;
        }

        memset(&mi, 0, sizeof(mi));

        if (copy_from_user(&mi, (void*)arg, sizeof(mi)))
        {
            printk("\nmmz_userdev_ioctl: copy_from_user error.\n");
            ret = -EFAULT;
            goto __error_exit;
        }

        if ( (get_mmbinfo_safe(mi.phys_addr, pmu)) == NULL)
        {
            ret = -EPERM;
            goto __error_exit;
        }

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
        ret = mmz_userdev_ioctl_t(inode, file, cmd, &mi);
#else
        ret = mmz_userdev_ioctl_t(file, cmd, &mi);
#endif

    }
    else
    {
        ret = -EINVAL;
    }

__error_exit:

    up(&pmu->sem);

    return ret;
}

int mmz_userdev_mmap(struct file* file, struct vm_area_struct* vma)
{
    struct mmb_info* p;
    struct mmz_userdev_info* pmu = file->private_data;
    unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;

    if (current->pid != pmu->mmap_pid)
    {
        error("do not call mmap() yourself!\n");
        return -EPERM;
    }

    p = get_mmbinfo(offset, pmu);

    if ( p == NULL)
    {
        error("I'm confused, mmb(0x%08lX) not found?!\n", offset);
        return -EPERM;
    }

    if (p->mapped)
    {
        error("I'm confused, mmb(0x%08lX) have been mapped yet?!\n", offset);
        return -EIO;
    }

    if (offset >= PHYS_OFFSET && offset <  __pa(high_memory) )
    {
        error("map address 0x%08lX in kernel range!\n", offset);
        return -EINVAL;
    }

#if defined(pgprot_noncached)

    if (file->f_flags & O_SYNC)
    {
        vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
    }
    else
    {
        if (p->map_cached)
            //l64467 notice: tag:ARM: pgtable: provide RDONLY page table bit rather than WRITE bit
            //vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY | L_PTE_WRITE | L_PTE_MT_DEV_CACHED);
        { vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY | L_PTE_MT_DEV_CACHED); }
        else
            /* pagetable property changes from <normal nocache> to <strong order>, for enhance ddr access performance, jira: BVTSDK-2003 */
#if 1
            /*  <strong order>  */
            vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

#else
            /*  <normal nocache> */
            vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
#endif
    }

#endif

    /* Remap-pfn-range will mark the range VM_IO and VM_RESERVED */
    if (remap_pfn_range(vma,
                        vma->vm_start,
                        vma->vm_pgoff,
                        vma->vm_end - vma->vm_start,
                        vma->vm_page_prot))
    { return -EAGAIN; }

    return 0;
}


static int mmz_userdev_release(struct inode* inode, struct file* file)
{
    struct mmz_userdev_info* pmu = file->private_data;
    struct mmb_info* p, *n;

    list_for_each_entry_safe(p, n, &pmu->list, list)
    {
        printk(KERN_ERR "MMB LEAK(pid=%d): 0x%08lX, %lu bytes, '%s'\n", \
               pmu->pid, hil_mmb_phys(p->mmb), \
               hil_mmb_length(p->mmb),
               hil_mmb_name(p->mmb));

        /* we do not need to release mapped-area here, system will do it for us */
        if (p->mapped)
        { printk(KERN_WARNING "mmz_userdev_release: mmb<0x%08lX> mapped to userspace 0x%p will be force unmaped!\n", p->phys_addr, p->mapped); }

        for (; p->mmb_ref > 0; p->mmb_ref--)
        { hil_mmb_put(p->mmb); }

        _usrdev_mmb_free(p);
    }

    file->private_data = NULL;
    kfree(pmu);

    return 0;
}


static struct file_operations mmz_userdev_fops =
{
    .owner	= THIS_MODULE,
    .open	= mmz_userdev_open,
    .release = mmz_userdev_release,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
    .ioctl  = mmz_userdev_ioctl,
#else
    .unlocked_ioctl = mmz_userdev_ioctl,
#endif
    .mmap	= mmz_userdev_mmap,
};

static struct miscdevice mmz_userdev =
{
    .minor	= MISC_DYNAMIC_MINOR,
    .fops	= &mmz_userdev_fops,
    .name	= "mmz_userdev"
};


int mmz_userdev_init(void)
{
    int ret;
    ret = misc_register(&mmz_userdev);

    if (ret)
    {
        printk("register mmz dev failure!\n");
        return -1;
    }

    return 0;
}

void mmz_userdev_exit(void)
{
    misc_deregister(&mmz_userdev);
}

/* Test func */

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int mmz_userdev_ioctl_t(struct inode* inode, struct file* file, unsigned int cmd, struct mmb_info* pmi)
#else
static int mmz_userdev_ioctl_t(struct file* file, unsigned int cmd, struct mmb_info* pmi)
#endif
{
    return 0;
}

