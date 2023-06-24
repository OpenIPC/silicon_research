#include "hi_osal.h"
#include <linux/fs.h>
#include <asm/uaccess.h>



struct file *klib_fopen(const char *filename, int flags, int mode)
{
    struct file *filp = filp_open(filename, flags, mode);
    return (IS_ERR(filp)) ? NULL : filp;
}


void klib_fclose(struct file *filp)
{
    if (filp){
        filp_close(filp, NULL);
    }
	return;
}


int klib_fwrite(char *buf, int len, struct file *filp)
{
    int writelen;

    if (filp == NULL){ 
		return -ENOENT; 
    }

	writelen = __kernel_write(filp, buf, len, &filp->f_pos);
	return writelen;
}


int klib_fread(char *buf, unsigned int len, struct file *filp)
{
	mm_segment_t old_fs;
	int readlen;

	if (filp == NULL){ 
	    return -ENOENT;
    }

	old_fs = get_fs();
	set_fs(get_ds());
	/* The cast to a user pointer is valid due to the set_fs() */
	readlen = vfs_read(filp, (void __user*)buf, len, &filp->f_pos);
	set_fs(old_fs);
	return readlen;
}



void *osal_klib_fopen(const char *filename, int flags, int mode)
{
	return (void *)klib_fopen(filename, flags, mode);	
}
EXPORT_SYMBOL(osal_klib_fopen);

void osal_klib_fclose(void *filp)
{
	klib_fclose((struct file *)filp);
}
EXPORT_SYMBOL(osal_klib_fclose);

int osal_klib_fwrite(char *buf, int len, void *filp)
{	
	return klib_fwrite(buf, len, (struct file *)filp);
}
EXPORT_SYMBOL(osal_klib_fwrite);

int osal_klib_fread(char *buf, unsigned int len, void *filp)
{
	return klib_fread(buf, len, (struct file *)filp);	
}
EXPORT_SYMBOL(osal_klib_fread);


