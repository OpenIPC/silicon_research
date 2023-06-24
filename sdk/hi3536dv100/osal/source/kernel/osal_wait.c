#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/slab.h>

unsigned long osal_msecs_to_jiffies(const unsigned int m)
{
    return msecs_to_jiffies(m);
}
EXPORT_SYMBOL(osal_msecs_to_jiffies);

int osal_wait_init(osal_wait_t * wait)
{
    wait_queue_head_t *wq;
	if(wait == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
    wq = (wait_queue_head_t *)kmalloc(sizeof(wait_queue_head_t), GFP_ATOMIC);
	if(wq == NULL){
		osal_printk("%s - kmalloc error!\n", __FUNCTION__);
		return -1;
	}
    init_waitqueue_head(wq);	
	wait->wait = wq;
	return 0;
}
EXPORT_SYMBOL(osal_wait_init);

int osal_wait(osal_wait_t * wait, osal_wait_cond_func_t func, void* param)
{
	wait_queue_head_t *wq;
	DEFINE_WAIT(__wait);     
    long ret = 0;
    int condition = 0;

	if(wait == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}

	wq = (wait_queue_head_t *)(wait->wait);
	if(wq == NULL){
		osal_printk("%s - wait->wait is NULL!\n", __FUNCTION__);
		return -1;
	}
	prepare_to_wait(wq, &__wait, TASK_INTERRUPTIBLE);
    /* if wakeup the queue brefore prepare_to_wait, the func will return true. And will not go to schedule */
    if (NULL != func)
    {
        condition = func(param);
    }

    if (!condition)
    {
    	if (!signal_pending(current)) {
    		schedule();
    	} 
    	if(signal_pending(current))
    		ret = -ERESTARTSYS;
    }
	
	finish_wait(wq, &__wait);
	return ret;
}
EXPORT_SYMBOL(osal_wait);

int osal_wait_uninterruptible(osal_wait_t * wait, osal_wait_cond_func_t func, void* param)
{
	wait_queue_head_t *wq;
	DEFINE_WAIT(__wait);     
    long ret = 0;
    int condition = 0;

	if(wait == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}

	wq = (wait_queue_head_t *)(wait->wait);
	if(wq == NULL){
		osal_printk("%s - wait->wait is NULL!\n", __FUNCTION__);
		return -1;
	}
	prepare_to_wait(wq, &__wait, TASK_UNINTERRUPTIBLE);
    /* if wakeup the queue brefore prepare_to_wait, the func will return true. And will not go to schedule */
    if (NULL != func)
    {
        condition = func(param);
    }

    if (!condition)
    {
        schedule();
    }

	finish_wait(wq, &__wait);
	return ret;
}
EXPORT_SYMBOL(osal_wait_uninterruptible);

int osal_wait_timeout_interruptible(osal_wait_t * wait, osal_wait_cond_func_t func, void* param, unsigned long ms)
{
	wait_queue_head_t *wq;
	DEFINE_WAIT(__wait);     
    long ret = ms;
    int condition = 0;

	if(wait == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
    
	wq = (wait_queue_head_t *)(wait->wait);
	if(wq == NULL){
		osal_printk("%s - wait->wait is NULL!\n", __FUNCTION__);
		return -1;
	}
	prepare_to_wait(wq, &__wait, TASK_INTERRUPTIBLE);
    /* if wakeup the queue brefore prepare_to_wait, the func will return true. And will not go to schedule */
    if (NULL != func)
    {
        condition = func(param);
    }
    
    if (!condition)
    {
    	if (!signal_pending(current)) {
    		ret = schedule_timeout(msecs_to_jiffies(ret));
            ret = jiffies_to_msecs(ret);
    	} 
    	if(signal_pending(current))
    		ret = -ERESTARTSYS;
    }
	
	finish_wait(wq, &__wait);

    return ret;
}

EXPORT_SYMBOL(osal_wait_timeout_interruptible);

int osal_wait_timeout_uninterruptible(osal_wait_t * wait, osal_wait_cond_func_t func, void* param, unsigned long ms)
{
	wait_queue_head_t *wq;
	DEFINE_WAIT(__wait);     
    long ret = ms;
    int condition = 0;

	if(wait == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
    
	wq = (wait_queue_head_t *)(wait->wait);
	if(wq == NULL){
		osal_printk("%s - wait->wait is NULL!\n", __FUNCTION__);
		return -1;
	}
	prepare_to_wait(wq, &__wait, TASK_UNINTERRUPTIBLE);
    /* if wakeup the queue brefore prepare_to_wait, the func will return true. And will not go to schedule */
    if (NULL != func)
    {
        condition = func(param);
    }
    
    if (!condition)
    {
		ret = schedule_timeout(msecs_to_jiffies(ret));
        ret = jiffies_to_msecs(ret);
    }
	
	finish_wait(wq, &__wait);

    return ret;
}
EXPORT_SYMBOL(osal_wait_timeout_uninterruptible);


int osal_wait_timeout(osal_wait_t * wait, osal_wait_cond_func_t func, void* param, unsigned long ms) {
	return osal_wait_timeout_uninterruptible(wait, func, param, ms);
}
EXPORT_SYMBOL(osal_wait_timeout);


void osal_wakeup(osal_wait_t * wait)
{
	wait_queue_head_t *wq;

	wq = (wait_queue_head_t *)(wait->wait);
	if(wq == NULL){
		osal_printk("%s - wait->wait is NULL!\n", __FUNCTION__);
		return ;
	}
	wake_up_all(wq);
}
EXPORT_SYMBOL(osal_wakeup);

void osal_wait_destory(osal_wait_t * wait)
{
	wait_queue_head_t *wq;

	wq = (wait_queue_head_t *)(wait->wait);
	if(wq == NULL){
		osal_printk("%s - wait->wait is NULL!\n", __FUNCTION__);
		return ;
	}
	kfree(wq);
	wait->wait=NULL;
}
EXPORT_SYMBOL(osal_wait_destory);

signed long osal_schedule_timeout(signed long timeout)
{
    return schedule_timeout(timeout);
}
EXPORT_SYMBOL(osal_schedule_timeout);

int hil_vma_check(void* addr) {
	return 0;
}
EXPORT_SYMBOL(hil_vma_check);

