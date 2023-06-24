#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

int osal_printk(const char *fmt, ...)
{
	va_list args;
	int r;

	va_start(args, fmt);
	r = vprintk(fmt, args);
	va_end(args);

	return r;
}
EXPORT_SYMBOL(osal_printk);

void osal_panic(const char *fmt,const char * file ,const char * fun,int line, const char * cond)
{
   	panic(fmt,file,  fun, line,cond);
}
EXPORT_SYMBOL(osal_panic);
