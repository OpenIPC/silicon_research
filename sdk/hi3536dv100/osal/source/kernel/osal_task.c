#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/interrupt.h>

osal_task_t *osal_kthread_create(threadfn_t thread,void *data, char *name)
{
	struct task_struct *k;
	osal_task_t *p = (osal_task_t *)kmalloc(sizeof(osal_task_t), GFP_KERNEL);
	if(p == NULL){
		osal_printk("%s - kmalloc error!\n", __FUNCTION__);
		return NULL;
	}

	k = kthread_run(thread, data, name);
    if (IS_ERR(k))
	{
		osal_printk("%s - kthread create error!\n", __FUNCTION__);
		kfree(p);
		return NULL;
	}
	p->task_struct = k;
	return p;
}
EXPORT_SYMBOL(osal_kthread_create);

void osal_kthread_destory(osal_task_t *task, unsigned int stop_flag){
	if(task == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return;
	}
    /* note: When you call the Kthread_stop function, the thread function cannot be finished, otherwise it will oops. */
    if(0 != stop_flag)
    {
	    kthread_stop((struct task_struct *)(task->task_struct));
    }
	task->task_struct = NULL;
	kfree(task);
}
EXPORT_SYMBOL(osal_kthread_destory);

void osal_kthread_bind(osal_task_t *task, unsigned int cpu)
{
	kthread_bind((struct task_struct *)(task->task_struct), cpu);
}
EXPORT_SYMBOL(osal_kthread_bind);

int osal_kthread_should_stop(void)
{
    return kthread_should_stop();
}
EXPORT_SYMBOL(osal_kthread_should_stop);

int osal_tasklet_init(osal_tasklet_t *tasklet, void (*func)(unsigned long), unsigned long data)
{
	struct tasklet_struct *p = NULL;
	if(tasklet == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
    p = (struct tasklet_struct *)kmalloc(sizeof(struct tasklet_struct), GFP_KERNEL);
	if(p == NULL){
		osal_printk("%s - kmalloc error!\n", __FUNCTION__);
		return -1;
	}
    tasklet_init(p, func, data);
	tasklet->tasklet_struct = p;
	return 0;
}
EXPORT_SYMBOL(osal_tasklet_init);

void osal_tasklet_kill(osal_tasklet_t *tasklet)
{
	struct tasklet_struct *p = NULL;
	if(tasklet == NULL){
    	osal_printk("%s - parameter invalid!\n", __FUNCTION__);
    	return;
	}
	p = (struct tasklet_struct *)(tasklet->tasklet_struct);
	tasklet_kill(p);
	kfree(p);
	tasklet->tasklet_struct=NULL;
}
EXPORT_SYMBOL(osal_tasklet_kill);

void osal_tasklet_schedule(osal_tasklet_t *tasklet)
{
 	struct tasklet_struct *p = NULL;
	if(tasklet == NULL){
    	osal_printk("%s - parameter invalid!\n", __FUNCTION__);
    	return;
	}
    p = (struct tasklet_struct *)tasklet->tasklet_struct;
    tasklet_schedule(p);
}
EXPORT_SYMBOL(osal_tasklet_schedule);

int osal_get_current_tgid(void)
{
    return get_current()->tgid;
}
EXPORT_SYMBOL(osal_get_current_tgid);

int osal_get_current_pid(void)
{
    return get_current()->pid;
}
EXPORT_SYMBOL(osal_get_current_pid);

