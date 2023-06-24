#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/semaphore.h>
#include <linux/slab.h>

int osal_sema_init(osal_semaphore_t *sem, int val)
{
	struct semaphore *p = NULL;
	if(sem == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
    p = kmalloc(sizeof(struct semaphore), GFP_KERNEL);
	if(p == NULL){
		osal_printk("%s - kmalloc error!\n", __FUNCTION__);
		return -1;
	}
    sema_init(p, val);	
	sem->sem = p;
	return 0;
}
EXPORT_SYMBOL(osal_sema_init);

int osal_down(osal_semaphore_t *sem)
{
	struct semaphore *p = NULL;
	if(sem == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (struct semaphore *)(sem->sem);
	if(p == NULL){
		osal_printk("%s - sem->sem is NULL!\n", __FUNCTION__);
		return -1;
	}
	down(p);
    return 0;
}
EXPORT_SYMBOL(osal_down);

int osal_down_interruptible(osal_semaphore_t *sem)
{
	struct semaphore *p = NULL;
	if(sem == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (struct semaphore *)(sem->sem);
	if(p == NULL){
		osal_printk("%s - sem->sem is NULL!\n", __FUNCTION__);
		return -1;
	}
	return down_interruptible(p);
}
EXPORT_SYMBOL(osal_down_interruptible);

int osal_down_trylock(osal_semaphore_t *sem)
{
	struct semaphore *p = NULL;
	if(sem == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (struct semaphore *)(sem->sem);
	if(p == NULL){
		osal_printk("%s - sem->sem is NULL!\n", __FUNCTION__);
		return -1;
	}
	return down_trylock(p);
}
EXPORT_SYMBOL(osal_down_trylock);

int osal_down_timeout(osal_semaphore_t *sem, long ms)
{
	struct semaphore *p = NULL;
	if(sem == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (struct semaphore *)(sem->sem);
	if(p == NULL){
		osal_printk("%s - sem->sem is NULL!\n", __FUNCTION__);
		return -1;
	}
	return down_timeout(p, msecs_to_jiffies(ms));
}
EXPORT_SYMBOL(osal_down_timeout);

void osal_up(osal_semaphore_t *sem)
{
	struct semaphore *p = NULL;
	p = (struct semaphore *)(sem->sem);
	if(p == NULL){
		osal_printk("%s - sem->sem is NULL!\n", __FUNCTION__);
		return ;
	}
	up(p);
}
EXPORT_SYMBOL(osal_up);

void osal_sema_destory(osal_semaphore_t *sem)
{
	struct semaphore *p = NULL;
	p = (struct semaphore *)(sem->sem);
	if(p == NULL){
		osal_printk("%s - sem->sem is NULL!\n", __FUNCTION__);
		return ;
	}
	kfree(p);
	sem->sem=NULL;
}
EXPORT_SYMBOL(osal_sema_destory);
