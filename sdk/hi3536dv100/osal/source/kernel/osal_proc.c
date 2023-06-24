#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#include "hi_osal.h"


#define OSAL_PROC_DEBUG 0

static struct osal_list_head list;
static osal_proc_entry_t *proc_entry=NULL;
static osal_proc_entry_t *proc_private=NULL;

static int osal_seq_show(struct seq_file *s, void *p)
{
    osal_proc_entry_t * sentry = s->private;
    if(sentry == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
	}
    sentry->seqfile = s;
    sentry->read(sentry);
    return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,0)
static ssize_t osal_procwrite(struct file * file, const char __user * buf, size_t count, loff_t *ppos)
{
    //char *kbuf;
    osal_proc_entry_t *sentry = ((struct seq_file *)(file->private_data))->private;
#if 0    
    kbuf = kmalloc(count, GFP_KERNEL);
	if(kbuf==NULL){
		osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
		return -1;
	}
    copy_from_user(kbuf, buf, count);
    if(OSAL_PROC_DEBUG)
        osal_printk("%s - %s!\n", __FUNCTION__, kbuf);
#endif    
    return sentry->write(sentry, (char*)buf, count, (long long *)ppos);
}
#else
static ssize_t osal_procwrite(struct file * file, const char __user * buf, 
    size_t count, loff_t *ppos)
{
    osal_proc_entry_t *item = PDE_DATA(file_inode(file));

    if (item && item->write){
        return item->write(item, buf, count, (long long *)ppos);
    }
    
    return -ENOSYS;
}
#endif

static int osal_procopen(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,0)
    osal_proc_entry_t * sentry = PDE(inode)->data;
#else
    osal_proc_entry_t * sentry = PDE_DATA(inode);
#endif
	if(sentry!=NULL && sentry->open!=NULL){
		sentry->open(sentry);
	}
    return single_open(file, osal_seq_show, sentry);
}

static struct file_operations osal_proc_ops = {
    .owner   = THIS_MODULE,
    .open    = osal_procopen,
    .read    = seq_read,
    .write   = osal_procwrite,
    .llseek  = seq_lseek, 
    .release = single_release
}; 

osal_proc_entry_t *osal_create_proc_entry(const char *name, osal_proc_entry_t *parent)
{
	struct proc_dir_entry *entry = NULL;
	osal_proc_entry_t * sentry = NULL;
	parent = proc_entry;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,0)
	if(parent == NULL){
		osal_printk("%s - parent is NULL!\n", __FUNCTION__);
		entry = create_proc_entry(name, 0, NULL);
	}else{
		osal_printk("%s - parent is not NULL! parent=%p\n", __FUNCTION__, parent->proc_dir_entry);
		entry = create_proc_entry(name, 0, parent->proc_dir_entry);
	}
	if(entry == NULL){
		osal_printk("%s - create_proc_entry failed!\n", __FUNCTION__);
		return NULL;
	}
	sentry = kmalloc(sizeof(struct osal_proc_dir_entry), GFP_KERNEL);
	if(!sentry){
		if(parent != NULL)
			remove_proc_entry(name, parent->proc_dir_entry);
		else
			remove_proc_entry(name, NULL);
		osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
		return NULL;
	}
	osal_strcpy(sentry->name, name);
	sentry->proc_dir_entry = entry;
	sentry->open=NULL;
	entry->proc_fops = &osal_proc_ops;
	entry->data = sentry; 
#else
    sentry = kmalloc(sizeof(struct osal_proc_dir_entry), GFP_KERNEL);
    if(!sentry){
        osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
        return NULL;
    }
	osal_memset(sentry, 0, sizeof(struct osal_proc_dir_entry));
	
    osal_strcpy(sentry->name, name);

	if(parent == NULL){
		//osal_printk("%s - parent is NULL!\n", __FUNCTION__);
		entry = proc_create_data(name, 0, NULL, &osal_proc_ops, sentry);
	}else{
		//osal_printk("%s - parent is not NULL! parent=%p\n", __FUNCTION__, parent->proc_dir_entry);
		entry = proc_create_data(name, 0, parent->proc_dir_entry, &osal_proc_ops, sentry);
	}
	if(entry == NULL){
		osal_printk("%s - create_proc_entry failed!\n", __FUNCTION__);
        kfree(sentry);
        sentry = NULL;
		return NULL;
	}
	sentry->proc_dir_entry = entry;
    sentry->open=NULL;
#endif
	osal_list_add_tail(&(sentry->node), &list);
	return sentry;
}
EXPORT_SYMBOL(osal_create_proc_entry);

osal_proc_entry_t *osal_create_proc_private(const char *name, osal_proc_entry_t *parent)
{
	struct proc_dir_entry *entry = NULL;
	osal_proc_entry_t * sentry = NULL;
	parent = proc_private;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,0)
	if(parent == NULL){
		osal_printk("%s - parent is NULL!\n", __FUNCTION__);
		entry = create_proc_entry(name, 0, NULL);
	}else{
		osal_printk("%s - parent is not NULL! parent=%p\n", __FUNCTION__, parent->proc_dir_entry);
		entry = create_proc_entry(name, 0, parent->proc_dir_entry);
	}
	if(entry == NULL){
		osal_printk("%s - create_proc_entry failed!\n", __FUNCTION__);
		return NULL;
	}
	sentry = kmalloc(sizeof(struct osal_proc_dir_entry), GFP_KERNEL);
	if(!sentry){
		if(parent != NULL)
			remove_proc_entry(name, parent->proc_dir_entry);
		else
			remove_proc_entry(name, NULL);
		osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
		return NULL;
	}
	osal_strcpy(sentry->name, name);
	sentry->proc_dir_entry = entry;
	sentry->open=NULL;
	entry->proc_fops = &osal_proc_ops;
	entry->data = sentry; 
#else
    sentry = kmalloc(sizeof(struct osal_proc_dir_entry), GFP_KERNEL);
    if(!sentry){
        osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
        return NULL;
    }

    osal_memset(sentry, 0, sizeof(struct osal_proc_dir_entry));
    
    osal_strcpy(sentry->name, name);

	if(parent == NULL){
		//osal_printk("%s - parent is NULL!\n", __FUNCTION__);
		entry = proc_create_data(name, 0, NULL, &osal_proc_ops, sentry);
	}else{
		//osal_printk("%s - parent is not NULL! parent=%p\n", __FUNCTION__, parent->proc_dir_entry);
		entry = proc_create_data(name, 0, parent->proc_dir_entry, &osal_proc_ops, sentry);
	}
	if(entry == NULL){
		osal_printk("%s - create_proc_entry failed!\n", __FUNCTION__);
        kfree(sentry);
        sentry = NULL;
		return NULL;
	}
	sentry->proc_dir_entry = entry;
    sentry->open=NULL;
#endif
	osal_list_add_tail(&(sentry->node), &list);
	return sentry;
}
EXPORT_SYMBOL(osal_create_proc_private);

void osal_remove_proc_private(const char *name, osal_proc_entry_t *parent)
{
	struct osal_proc_dir_entry * sproc = NULL;
	parent = proc_private;
	if(name == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return;
	}
	if(parent != NULL)
		remove_proc_entry(name, parent->proc_dir_entry);
	else
		remove_proc_entry(name, NULL); 
	osal_list_for_each_entry(sproc, &list, node){
		if(osal_strcmp(sproc->name, name) == 0){
			osal_list_del(&(sproc->node));
			break;
		}
	}
	if(sproc != NULL)
		kfree(sproc);
}
EXPORT_SYMBOL(osal_remove_proc_private);

osal_proc_entry_t *osal_proc_mkdir(const char *name, osal_proc_entry_t *parent)
{
	struct proc_dir_entry *proc = NULL;
	struct osal_proc_dir_entry * sproc = NULL;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,0)
	if(parent != NULL){
		proc = proc_mkdir(name, parent->proc_dir_entry);
		osal_printk("%s - parent is not NULL!\n", __FUNCTION__);
		//proc = create_proc_entry(name, S_IFDIR | S_IRUGO | S_IXUGO, parent->proc_dir_entry);
	}else{
		proc = proc_mkdir(name, NULL);
		osal_printk("%s - parent is NULL! proc=%p \n", __FUNCTION__, proc);
		//proc = create_proc_entry(name, S_IFDIR | S_IRUGO | S_IXUGO, NULL);
	}
	if(proc == NULL){
		osal_printk("%s - proc_mkdir failed!\n", __FUNCTION__);
		return NULL;
	}
	//osal_memset(proc, 0x00, sizeof(proc));
	sproc = kmalloc(sizeof(struct osal_proc_dir_entry), GFP_KERNEL);
	if(!sproc){
		if(parent != NULL)
			remove_proc_entry(name, parent->proc_dir_entry);
		else
			remove_proc_entry(name, NULL);
		osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
		return NULL;
	}
	osal_strcpy(sproc->name, name);
	sproc->proc_dir_entry = proc;
	//sproc->read = NULL;
	//sproc->write = NULL;
	//proc->proc_fops = NULL;
	proc->data = sproc;
#else
    sproc = kmalloc(sizeof(struct osal_proc_dir_entry), GFP_KERNEL);
    if(!sproc){
        osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
        return NULL;
    }
	osal_strcpy(sproc->name, name);

	if(parent != NULL){
		proc = proc_mkdir_data(name, 0, parent->proc_dir_entry, sproc);
		//osal_printk("%s - parent is not NULL!\n", __FUNCTION__);
	}else{
		proc = proc_mkdir_data(name, 0, NULL, sproc);
		//osal_printk("%s - parent is NULL! proc=%p \n", __FUNCTION__, proc);
	}
	if(proc == NULL){
		osal_printk("%s - proc_mkdir failed!\n", __FUNCTION__);
        kfree(sproc);
        sproc = NULL;
		return NULL;
	}
	sproc->proc_dir_entry = proc;
#endif
	osal_list_add_tail(&(sproc->node), &list);
	return sproc;
}
EXPORT_SYMBOL(osal_proc_mkdir);

void osal_remove_proc_entry(const char *name, osal_proc_entry_t *parent)
{
	struct osal_proc_dir_entry * sproc = NULL;
	parent = proc_entry;
	if(name == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return;
	}
	if(parent != NULL)
		remove_proc_entry(name, parent->proc_dir_entry);
	else
		remove_proc_entry(name, NULL); 
	osal_list_for_each_entry(sproc, &list, node){
		if(osal_strcmp(sproc->name, name) == 0){
			osal_list_del(&(sproc->node));
			kfree(sproc);
			break;
		}
	}
}
EXPORT_SYMBOL(osal_remove_proc_entry);

void osal_remove_proc_root(const char *name, osal_proc_entry_t *parent)
{
	struct osal_proc_dir_entry * sproc = NULL;
	if(name == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return;
	}
	if(parent != NULL)
		remove_proc_entry(name, parent->proc_dir_entry);
	else
		remove_proc_entry(name, NULL); 
	osal_list_for_each_entry(sproc, &list, node){
		if(osal_strcmp(sproc->name, name) == 0){
			osal_list_del(&(sproc->node));
			kfree(sproc);
			break;
		}
	}
}

int _seq_vprintf(struct seq_file *m, const char *f, va_list args)
{
        int len;

        if (m->count < m->size) {
                len = vsnprintf(m->buf + m->count, m->size - m->count, f, args);
                if (m->count + len < m->size) {
                        m->count += len;
                        return 0;
                }
        }
        m->count = m->size;
        return -1;
}

int osal_seq_printf(osal_proc_entry_t *entry, const char *fmt, ...)
{
	struct seq_file *s = (struct seq_file *)(entry->seqfile);
	va_list args;
	int r = 0;
	   
	va_start(args, fmt);
	
    #if LINUX_VERSION_CODE < KERNEL_VERSION(4,3,0)
	r = _seq_vprintf(s, fmt, args);
    #else
    _seq_vprintf(s, fmt, args);
    #endif
	
	va_end(args);
	   
	return r;
}
EXPORT_SYMBOL(osal_seq_printf);

void osal_proc_init(void)
{
	OSAL_INIT_LIST_HEAD(&list);
	proc_entry = osal_proc_mkdir("umap", OSAL_NULL);
	if(proc_entry == OSAL_NULL){
	    osal_printk("test init, proc mkdir error!\n");
	}
	 
	proc_private = osal_proc_mkdir("private", OSAL_NULL);
	if(proc_private == OSAL_NULL){
	    osal_printk("test init, proc private mkdir error!\n");		
		osal_remove_proc_root("umap", OSAL_NULL);
	}		
}
void osal_proc_exit(void)
{
	osal_remove_proc_root("umap", OSAL_NULL);
	osal_remove_proc_root("private", OSAL_NULL);
}

