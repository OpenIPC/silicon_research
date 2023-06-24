#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/atomic.h>
#include <linux/printk.h>
#include <linux/slab.h>

int osal_atomic_init(osal_atomic_t *atomic)
{
	atomic_t *p;
	if(atomic == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (atomic_t *)kmalloc(sizeof(atomic_t), GFP_KERNEL);
	if(p == NULL){
		osal_printk("%s - kmalloc error!\n", __FUNCTION__);
		return -1;
	}
	atomic->atomic = p;
	return 0;
}
EXPORT_SYMBOL(osal_atomic_init);

void osal_atomic_destory(osal_atomic_t *atomic)
{
	kfree(atomic->atomic);
	atomic->atomic=NULL;
}
EXPORT_SYMBOL(osal_atomic_destory);

int osal_atomic_read(osal_atomic_t *atomic)
{
	atomic_t *p;
	if(atomic == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (atomic_t *)(atomic->atomic);
	if(NULL == p){
		osal_printk("%s - atomic->atomic is NULL!\n", __FUNCTION__);
		return -1;
	}
	return atomic_read(p);
}
EXPORT_SYMBOL(osal_atomic_read);

void osal_atomic_set(osal_atomic_t *atomic, int i)
{
	atomic_t *p;
	p = (atomic_t *)(atomic->atomic);
	if(NULL == p){
		osal_printk("%s - atomic->atomic is NULL!\n", __FUNCTION__);
		return ;
	}
	atomic_set(p, i);
}
EXPORT_SYMBOL(osal_atomic_set);

int osal_atomic_inc_return(osal_atomic_t *atomic)
{
	atomic_t *p;
	if(atomic == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (atomic_t *)(atomic->atomic);
	if(NULL == p){
		osal_printk("%s - atomic->atomic is NULL!\n", __FUNCTION__);
		return -1;
	}
	return atomic_inc_return(p);
}
EXPORT_SYMBOL(osal_atomic_inc_return);

int osal_atomic_dec_return(osal_atomic_t *atomic)
{
	atomic_t *p;
	if(atomic == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (atomic_t *)(atomic->atomic);
	if(NULL == p){
		osal_printk("%s - atomic->atomic is NULL!\n", __FUNCTION__);
		return -1;
	}
	return atomic_dec_return(p);
}
EXPORT_SYMBOL(osal_atomic_dec_return);

int osal_atomic_dec_and_test(osal_atomic_t *atomic){
	atomic_t *p;
	if(atomic == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (atomic_t *)(atomic->atomic);
	if(NULL == p){
		osal_printk("%s - atomic->atomic is NULL!\n", __FUNCTION__);
		return -1;
	}
	return atomic_dec_and_test(p);
}
EXPORT_SYMBOL(osal_atomic_dec_and_test);

