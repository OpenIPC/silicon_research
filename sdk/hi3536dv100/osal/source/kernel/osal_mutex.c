#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/mutex.h>
#include <linux/slab.h>

int osal_mutex_init(osal_mutex_t *mutex)
{
	struct mutex *p = NULL;
	if(mutex == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
    p = kmalloc(sizeof(struct mutex), GFP_KERNEL);
	if(p == NULL){
		osal_printk("%s - kmalloc error!\n", __FUNCTION__);
		return -1;
	}
	mutex_init(p);
	mutex->mutex = p;
	return 0;
}
EXPORT_SYMBOL(osal_mutex_init);

int osal_mutex_lock(osal_mutex_t *mutex)
{
	struct mutex *p = NULL;
	if(mutex == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (struct mutex *)(mutex->mutex);
	mutex_lock(p);
    return 0;
}
EXPORT_SYMBOL(osal_mutex_lock);

int osal_mutex_lock_interruptible(osal_mutex_t *mutex)
{
	struct mutex *p = NULL;
	if(mutex == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (struct mutex *)(mutex->mutex);
	return mutex_lock_interruptible(p);
}
EXPORT_SYMBOL(osal_mutex_lock_interruptible);

int osal_mutex_trylock(osal_mutex_t *mutex)
{
	struct mutex *p = NULL;
	if(mutex == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (struct mutex *)(mutex->mutex);

	return mutex_trylock(p);
}
EXPORT_SYMBOL(osal_mutex_trylock);

void osal_mutex_unlock(osal_mutex_t *mutex)
{
	struct mutex *p = NULL;
	p = (struct mutex *)(mutex->mutex);

	mutex_unlock(p);
}
EXPORT_SYMBOL(osal_mutex_unlock);

void osal_mutex_destory(osal_mutex_t *mutex)
{
	struct mutex *p = NULL;
	p = (struct mutex *)(mutex->mutex);
	kfree(p);
	mutex->mutex=NULL;
}
EXPORT_SYMBOL(osal_mutex_destory);
