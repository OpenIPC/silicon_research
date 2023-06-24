#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include "himedia.h"
#include <linux/fs.h>
//#include <asm/ioctl.h>
#include <asm/io.h>
#include <asm/page.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/kmod.h>
#include "hi_osal.h"

#define DRVAL_DEBUG 0

typedef struct osal_coat_dev{
    struct osal_dev osal_dev;
    struct himedia_device himedia_dev;
}osal_coat_dev_t;


struct osal_private_data{
     struct osal_dev *dev;
     void *data;
     struct osal_poll table;
};

static int osal_open(struct inode *inode, struct file *file)
{
    struct himedia_device *himedia;
    osal_coat_dev_t *coat_dev;
    struct osal_private_data *pdata = NULL;

    if (!capable(CAP_SYS_RAWIO))
            return -EPERM;

    //himedia = gethimedia(inode);
    himedia = (struct himedia_device *)file->private_data;
    if(himedia == NULL){
        osal_printk("%s - get himedia device error!\n", __FUNCTION__);
        return -1;
    }
    coat_dev = osal_container_of(himedia, struct osal_coat_dev, himedia_dev);
    pdata = (struct osal_private_data *)kmalloc(sizeof(struct osal_private_data), GFP_KERNEL);
    if(pdata == NULL){
        osal_printk("%s - kmalloc error!\n", __FUNCTION__);
        return -1;
    }
    if(DRVAL_DEBUG)
        osal_printk("%s - file->private_data=%p!\n", __FUNCTION__, pdata);

    memset(pdata, 0, sizeof(struct osal_private_data));

    file->private_data = pdata;
    pdata->dev = &(coat_dev->osal_dev);
    if(coat_dev->osal_dev.fops->open != NULL)
        return coat_dev->osal_dev.fops->open((void *)&(pdata->data));
    return 0;
}

static int osal_read(struct file *file, char __user *buf, size_t size, loff_t *offset)
{
     struct osal_private_data *pdata = file->private_data;
     int ret = 0;

     if(pdata->dev->fops->read != NULL){
         ret = pdata->dev->fops->read(buf, (int)size, (long *)offset, (void *)&(pdata->data));
     }
     return ret;
}

static int osal_write(struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
     struct osal_private_data *pdata = file->private_data;
     int ret = 0;

     if(pdata->dev->fops->write != NULL){
         ret = pdata->dev->fops->write(buf, (int)size, (long *)offset, (void *)&(pdata->data));
     }

     return ret;
}

static loff_t osal_llseek(struct file *file, loff_t offset, int whence)
{
     struct osal_private_data *pdata = file->private_data;
     int ret = 0;
     if(DRVAL_DEBUG)
        osal_printk("%s - file->private_data=%p!\n", __FUNCTION__, pdata);

     if(whence == SEEK_SET){
         if(pdata->dev->fops->llseek != NULL){
               ret = pdata->dev->fops->llseek((long)offset, OSAL_SEEK_SET, (void *)&(pdata->data));
         }
     }else if(whence == SEEK_CUR){
         if(pdata->dev->fops->llseek != NULL){
               ret = pdata->dev->fops->llseek((long)offset, OSAL_SEEK_CUR, (void *)&(pdata->data));
         }
     }else if(whence == SEEK_END){
         if(pdata->dev->fops->llseek != NULL){
               ret = pdata->dev->fops->llseek((long)offset, OSAL_SEEK_END, (void *)&(pdata->data));
         }
     }
     return (loff_t)ret;
}

static int osal_release(struct inode *inode, struct file *file)
{
     int ret = 0;
     struct osal_private_data *pdata = file->private_data;

     if(DRVAL_DEBUG)
        osal_printk("%s - file->private_data=%p!\n", __FUNCTION__, pdata);

     if(pdata->dev->fops->release != NULL)
         ret = pdata->dev->fops->release((void *)&(pdata->data));
     if(ret != 0){
         osal_printk("%s - release failed!\n", __FUNCTION__);
         return ret;
     }
     kfree(file->private_data);
     return 0;
}

static long osal_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret = -1;
    struct osal_private_data *pdata = file->private_data;
    char *kbuf = NULL;

    if(DRVAL_DEBUG)
        osal_printk("%s - file->private_data=%p!\n", __FUNCTION__, pdata);

    if (8192 < _IOC_SIZE(cmd) || ((0 == _IOC_SIZE(cmd)) && (_IOC_NONE != _IOC_DIR(cmd))))
    {
        return -1;
    }

    if(_IOC_DIR(cmd) == _IOC_NONE){
        if(pdata->dev->fops->unlocked_ioctl == NULL)
            return -1;
        else{

            ret = pdata->dev->fops->unlocked_ioctl(cmd, arg, (void *)&(pdata->data));
        }
    }else if(_IOC_DIR(cmd) == _IOC_WRITE){
        kbuf = (char *)vmalloc(_IOC_SIZE(cmd));
        if(kbuf == NULL){
            osal_printk("%s - vmalloc failed!\n", __FUNCTION__);
        return -1;
        }

        if(copy_from_user(kbuf, (char *)arg, _IOC_SIZE(cmd)))
        {
            vfree(kbuf);
            return -1;
        }

        if(pdata->dev->fops->unlocked_ioctl == NULL){
            vfree(kbuf);
            return -1;
        }else{
            ret = pdata->dev->fops->unlocked_ioctl(cmd, (unsigned long)kbuf, (void *)&(pdata->data));
        }
    }else if(_IOC_DIR(cmd) == _IOC_READ){
        kbuf = vmalloc(_IOC_SIZE(cmd));
        if(kbuf == NULL){
            osal_printk("%s - vmalloc failed!\n", __FUNCTION__);
            return -1;
        }
        if(pdata->dev->fops->unlocked_ioctl == NULL){
            vfree(kbuf);
            return -1;
        }else{
            ret = pdata->dev->fops->unlocked_ioctl(cmd, (unsigned long)kbuf, (void *)&(pdata->data));
            if(ret == 0)
            {
                if(copy_to_user((char *)arg, kbuf, _IOC_SIZE(cmd)))
                {
                    vfree(kbuf);
                    return -1;
                }
            }
        }
    }else if(_IOC_DIR(cmd) == (_IOC_READ + _IOC_WRITE)){
        kbuf = vmalloc(_IOC_SIZE(cmd));
        if(kbuf == NULL){
            osal_printk("%s - vmalloc failed!\n", __FUNCTION__);
            return -1;
        }

        if(copy_from_user(kbuf, (char *)arg, _IOC_SIZE(cmd)))
        {
            vfree(kbuf);
            return -1;
        }

        if(pdata->dev->fops->unlocked_ioctl == NULL){
            vfree(kbuf);
            return -1;
        }else{
            ret = pdata->dev->fops->unlocked_ioctl(cmd, (unsigned long)kbuf, (void *)&(pdata->data));
            if(ret == 0)
            {
                if(copy_to_user((char *)arg, kbuf, _IOC_SIZE(cmd)))
                {
                    vfree(kbuf);
                    return -1;
                }
            }
        }
    }

    if(kbuf != NULL)
        vfree(kbuf);

    return ret;
}

static unsigned int osal_poll(struct file *file, struct poll_table_struct *table)
{
    struct osal_private_data *pdata = file->private_data;
    //osal_coat_dev_t *coat_dev = container_of(pdata->dev, struct osal_coat_dev, osal_dev);
    struct osal_poll *t = &(pdata->table);

    if(DRVAL_DEBUG)
        osal_printk("%s - table=%p, file=%p!\n", __FUNCTION__, table, file);
    t->poll_table = table;
    t->data = file;
    if(pdata->dev->fops->poll!= NULL)
        return pdata->dev->fops->poll(t, (void *)&(pdata->data));
    return 0;
}

int valid_mmap_phys_addr_range(unsigned long pfn, size_t size)
{
    /*check physical addr greater than 4G*/
    if ((pfn + (size >> PAGE_SHIFT)) > (1 + ((~0UL) >> PAGE_SHIFT)))
    {
        return 0;
    }

    /* CMA need to alloc ram region, so annotate the code as follows */
#if 1
    /*check physical addr is ram region*/
    if (pfn_valid(pfn) || pfn_valid(pfn + (size >> PAGE_SHIFT)))
    {
        return 0;
    }
#endif

    /*It's necessary for the variable "size" to align 4k(page_size).*/
    #define PAGE_SIZE_MASK 0xfffff000
    if (size & (~PAGE_SIZE_MASK))
    {
        return 0;
    }
    #undef PAGE_SIZE_MASK

    return 1;
}

static int osal_mmap(struct file *file, struct vm_area_struct *vm)
{
    struct osal_vm osal_vm;
    struct osal_private_data *pdata = file->private_data;
    osal_vm.vm = vm;

    //osal_printk("\n%s - start=%x, end=%x!, off=%x\n", __FUNCTION__, vm->vm_start, vm->vm_end, vm->vm_pgoff);
    if (!valid_mmap_phys_addr_range(vm->vm_pgoff, vm->vm_end - vm->vm_start))
    {
        osal_printk("\n%s - invalid argument!!!\n", __FUNCTION__);
        return -EINVAL;
    }

    if(DRVAL_DEBUG)
        osal_printk("%s - start=%x, end=%x!, off=%x\n", __FUNCTION__, vm->vm_start, vm->vm_end, vm->vm_pgoff);
    if(pdata->dev->fops->mmap!= NULL)
        return pdata->dev->fops->mmap(&osal_vm, vm->vm_start, vm->vm_end, vm->vm_pgoff, (void *)&(pdata->data));
    return 0;
}

static struct file_operations s_osal_fops = {
    .owner = THIS_MODULE,
    .open = osal_open,
    .read = osal_read,
    .write = osal_write,
    .llseek = osal_llseek,
    .unlocked_ioctl = osal_unlocked_ioctl,
    .release = osal_release,
    .poll = osal_poll,
    .mmap = osal_mmap,
};

static int osal_pm_prepare(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_prepare)
        return coat_dev->osal_dev.osal_pmops->pm_prepare(&(coat_dev->osal_dev));
    return 0;
}

static void osal_pm_complete(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_complete)
        coat_dev->osal_dev.osal_pmops->pm_complete(&(coat_dev->osal_dev));
}

static int osal_pm_suspend(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_suspend)
        return coat_dev->osal_dev.osal_pmops->pm_suspend(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_resume(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_resume)
        return coat_dev->osal_dev.osal_pmops->pm_resume(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_freeze(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_freeze)
        return coat_dev->osal_dev.osal_pmops->pm_freeze(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_thaw(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_thaw)
        return coat_dev->osal_dev.osal_pmops->pm_thaw(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_poweroff(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_poweroff)
        return coat_dev->osal_dev.osal_pmops->pm_poweroff(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_restore(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_restore)
        return coat_dev->osal_dev.osal_pmops->pm_restore(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_suspend_late(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_suspend_late)
        return coat_dev->osal_dev.osal_pmops->pm_suspend_late(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_resume_early(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_resume_early)
        return coat_dev->osal_dev.osal_pmops->pm_resume_early(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_freeze_late(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_freeze_late)
        return coat_dev->osal_dev.osal_pmops->pm_freeze_late(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_thaw_early(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_thaw_early)
        return coat_dev->osal_dev.osal_pmops->pm_thaw_early(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_poweroff_late(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_poweroff_late)
        return coat_dev->osal_dev.osal_pmops->pm_poweroff_late(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_restore_early(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_restore_early)
        return coat_dev->osal_dev.osal_pmops->pm_restore_early(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_suspend_noirq(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_suspend_noirq)
        return coat_dev->osal_dev.osal_pmops->pm_suspend_noirq(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_resume_noirq(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_resume_noirq)
        return coat_dev->osal_dev.osal_pmops->pm_resume_noirq(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_freeze_noirq(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_freeze_noirq)
        return coat_dev->osal_dev.osal_pmops->pm_freeze_noirq(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_thaw_noirq(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_thaw_noirq)
        return coat_dev->osal_dev.osal_pmops->pm_thaw_noirq(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_poweroff_noirq(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_poweroff_noirq)
        return coat_dev->osal_dev.osal_pmops->pm_poweroff_noirq(&(coat_dev->osal_dev));
    return 0;
}

static int osal_pm_restore_noirq(struct himedia_device *himedia)
{
    osal_coat_dev_t *coat_dev = container_of(himedia, struct osal_coat_dev, himedia_dev);
    if(coat_dev->osal_dev.osal_pmops && coat_dev->osal_dev.osal_pmops->pm_restore_noirq)
        return coat_dev->osal_dev.osal_pmops->pm_restore_noirq(&(coat_dev->osal_dev));
    return 0;
}

static struct himedia_ops s_osal_pmops ={
    .pm_prepare = osal_pm_prepare,
    .pm_complete = osal_pm_complete,
    .pm_suspend = osal_pm_suspend,
    .pm_resume = osal_pm_resume,
    .pm_freeze = osal_pm_freeze,
    .pm_thaw = osal_pm_thaw,
    .pm_poweroff = osal_pm_poweroff,
    .pm_restore = osal_pm_restore,
    .pm_suspend_late = osal_pm_suspend_late,
    .pm_resume_early = osal_pm_resume_early,
    .pm_freeze_late = osal_pm_freeze_late,
    .pm_thaw_early = osal_pm_thaw_early,
    .pm_poweroff_late = osal_pm_poweroff_late,
    .pm_restore_early = osal_pm_restore_early,
    .pm_suspend_noirq = osal_pm_suspend_noirq,
    .pm_resume_noirq = osal_pm_resume_noirq,
    .pm_freeze_noirq = osal_pm_freeze_noirq,
    .pm_thaw_noirq = osal_pm_thaw_noirq,
    .pm_poweroff_noirq = osal_pm_poweroff_noirq,
    .pm_restore_noirq = osal_pm_restore_noirq,
};

osal_dev_t *osal_createdev(char *name)
{
    osal_coat_dev_t *pdev;
    if(name == NULL){
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return NULL;
    }
    pdev = (osal_coat_dev_t *)kmalloc(sizeof(osal_coat_dev_t), GFP_KERNEL);
    if(pdev == NULL){
        osal_printk("%s - kmalloc error!\n", __FUNCTION__);
        return NULL;
    }
    memset(pdev, 0, sizeof(osal_coat_dev_t));
    osal_strncpy(pdev->osal_dev.name, name, 48);
    pdev->osal_dev.dev = pdev;
    return &(pdev->osal_dev);
}
EXPORT_SYMBOL(osal_createdev);

int osal_destroydev(osal_dev_t *osal_dev)
{
    osal_coat_dev_t *pdev;
    if(osal_dev == NULL){
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    pdev = osal_dev->dev;
    if(pdev == NULL){
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    kfree(pdev);
    return 0;
}
EXPORT_SYMBOL(osal_destroydev);

int osal_registerdevice(osal_dev_t *osal_dev)
{
    struct himedia_device *himedia;
    if(osal_dev == NULL || osal_dev->fops == NULL){
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    himedia = &(((osal_coat_dev_t *)(osal_dev->dev))->himedia_dev);
    if(osal_dev->minor != 0)
        himedia->minor = osal_dev->minor;
    else
        himedia->minor = HIMEDIA_DYNAMIC_MINOR;
    himedia->owner = THIS_MODULE;
    himedia->fops = &s_osal_fops;
    himedia->drvops = &s_osal_pmops;
    osal_strcpy(himedia->devfs_name, osal_dev->name);
    return himedia_register(himedia);
}
EXPORT_SYMBOL(osal_registerdevice);

void osal_deregisterdevice(osal_dev_t *pdev)
{
    if(pdev == NULL){
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }
    himedia_unregister((struct himedia_device *)&(((osal_coat_dev_t *)(pdev->dev))->himedia_dev));
}
EXPORT_SYMBOL(osal_deregisterdevice);

void osal_poll_wait(osal_poll_t *table, osal_wait_t * wait)
{
    if(DRVAL_DEBUG)
        osal_printk("%s - call poll_wait +!, table=%p, file=%p\n", __FUNCTION__, table->poll_table, table->data);

    poll_wait((struct file *)table->data, (wait_queue_head_t *)(wait->wait), table->poll_table);

    if(DRVAL_DEBUG)
        osal_printk("%s - call poll_wait -!\n", __FUNCTION__);
}
EXPORT_SYMBOL(osal_poll_wait);

void osal_pgprot_noncached(osal_vm_t *vm)
{
    struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);
    v->vm_page_prot = pgprot_noncached(v->vm_page_prot);
}
EXPORT_SYMBOL(osal_pgprot_noncached);

void osal_pgprot_cached(osal_vm_t *vm)
{
    struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);
    v->vm_page_prot = __pgprot(pgprot_val(v->vm_page_prot) | L_PTE_PRESENT
                               | L_PTE_YOUNG | L_PTE_DIRTY | L_PTE_MT_DEV_CACHED);
}
EXPORT_SYMBOL(osal_pgprot_cached);

void osal_pgprot_writecombine(osal_vm_t *vm)
{
    struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);
    v->vm_page_prot = pgprot_writecombine(v->vm_page_prot);
}
EXPORT_SYMBOL(osal_pgprot_writecombine);

void osal_pgprot_stronglyordered(osal_vm_t *vm)
{
    struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);
    v->vm_page_prot = pgprot_stronglyordered(v->vm_page_prot);
}
EXPORT_SYMBOL(osal_pgprot_stronglyordered);

int osal_remap_pfn_range(osal_vm_t *vm, unsigned long addr, unsigned long pfn, unsigned long size)
{
    struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);
    if(0 == size)
    {
        return -EPERM;
    }
    return remap_pfn_range(v, addr, pfn, size, v->vm_page_prot);
}
EXPORT_SYMBOL(osal_remap_pfn_range);

int osal_io_remap_pfn_range(osal_vm_t *vm, unsigned long addr, unsigned long pfn, unsigned long size)
{
    struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);
    v->vm_flags |= VM_IO;
    if(0 == size)
    {
        return -EPERM;
    }
    return io_remap_pfn_range(v, addr, pfn, size, v->vm_page_prot);
}
EXPORT_SYMBOL(osal_io_remap_pfn_range);

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
int osal_call_usermodehelper_force(char *path, char **argv, char **envp, int wait)
{
    return call_usermodehelper_force(path, argv, envp, wait);
}
EXPORT_SYMBOL(osal_call_usermodehelper_force);
#endif

int osal_pfn_valid(unsigned int pfn)
{
    return pfn_valid(pfn);
}
EXPORT_SYMBOL(osal_pfn_valid);

