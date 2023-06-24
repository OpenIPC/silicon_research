#include "hi_osal.h"
#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/sched.h>

void osal_yield(void)
{
	yield();
}
EXPORT_SYMBOL(osal_yield);
