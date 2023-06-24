#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/version.h>

char * osal_strcpy(char *dest,const char *src)
{
	return strcpy(dest, src);
}
EXPORT_SYMBOL(osal_strcpy);

char * osal_strncpy(char *dest,const char *src, int count)
{
	return strncpy(dest, src, count);
}
EXPORT_SYMBOL(osal_strncpy);

int osal_strlcpy(char *dest, const char *src, int size)
{
	return strlcpy(dest, src, size);
}
EXPORT_SYMBOL(osal_strlcpy);

char * osal_strcat(char *dest, const char *src)
{
	return strcat(dest, src);
}
EXPORT_SYMBOL(osal_strcat);

char * osal_strncat(char *dest, const char *src, int count)
{
	return strncat(dest, src, count);
}
EXPORT_SYMBOL(osal_strncat);

int osal_strlcat(char *dest, const char *src, int count)
{
	return strlcat(dest, src, count);
}
EXPORT_SYMBOL(osal_strlcat);

int osal_strcmp(const char *cs,const char *ct)
{
	return strcmp(cs, ct);
}
EXPORT_SYMBOL(osal_strcmp);

int osal_strncmp(const char *cs,const char *ct,int count)
{
	return strncmp(cs, ct, count);
}
EXPORT_SYMBOL(osal_strncmp);

int osal_strnicmp(const char *s1, const char *s2, int len)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0)
	return strnicmp(s1, s2, len);
#endif
	return 0;
}
EXPORT_SYMBOL(osal_strnicmp);

int osal_strcasecmp(const char *s1, const char *s2)
{
	return strcasecmp(s1, s2);
}
EXPORT_SYMBOL(osal_strcasecmp);

int osal_strncasecmp(const char *s1, const char *s2, int n)
{
	return strncasecmp(s1, s2, n);
}
EXPORT_SYMBOL(osal_strncasecmp);

char * osal_strchr(const char *s,int c)
{
	return strchr(s, c);
}
EXPORT_SYMBOL(osal_strchr);

char * osal_strnchr(const char *s , int count, int c)
{
	return strnchr(s, count, c);
}
EXPORT_SYMBOL(osal_strnchr);

char * osal_strrchr(const char *s,int c)
{
	return strrchr(s, c);
}
EXPORT_SYMBOL(osal_strrchr);

char * osal_skip_spaces(const char *str)
{
	return skip_spaces(str);
}
EXPORT_SYMBOL(osal_skip_spaces);

char *osal_strim(char *s)
{
	return strim(s);
}
EXPORT_SYMBOL(osal_strim);

char *osal_strstrip(char *str)
{
	return strstrip(str);
}
EXPORT_SYMBOL(osal_strstrip);

char * osal_strstr(const char *s1, const char *s2)
{
	return strstr(s1, s2);
}
EXPORT_SYMBOL(osal_strstr);

char * osal_strnstr(const char *s1, const char *s2, int len)
{
	return strnstr(s1, s2, len);
}
EXPORT_SYMBOL(osal_strnstr);

int osal_strlen(const char *s)
{
	return strlen(s);
}
EXPORT_SYMBOL(osal_strlen);

int osal_strnlen(const char *s,int count)
{
	return strnlen(s, count);
}
EXPORT_SYMBOL(osal_strnlen);

char * osal_strpbrk(const char *cs,const char *ct)
{
	return strpbrk(cs, ct);
}
EXPORT_SYMBOL(osal_strpbrk);

char * osal_strsep(char **s,const char *ct)
{
	return strsep(s, ct);
}
EXPORT_SYMBOL(osal_strsep);

int osal_strspn(const char *s,const char *accept)
{
	return strspn(s, accept);
}
EXPORT_SYMBOL(osal_strspn);

int osal_strcspn(const char *s,const char *reject)
{
	return strcspn(s, reject);
}
EXPORT_SYMBOL(osal_strcspn);

void * osal_memset(void *str,int c,int count)
{
	return memset(str, c, count);
}
EXPORT_SYMBOL(osal_memset);

void * osal_memcpy(void *dest,const void *src,int count)
{
	return memcpy(dest, src, count);
}
EXPORT_SYMBOL(osal_memcpy);

void * osal_memmove(void *dest,const void *src,int count)
{
	return memmove(dest, src, count);
}
EXPORT_SYMBOL(osal_memmove);

void * osal_memscan(void *addr, int c, int size)
{
	return memscan(addr, c, size);
}
EXPORT_SYMBOL(osal_memscan);

int osal_memcmp(const void *cs,const void *ct,int count)
{
	return memcmp(cs, ct, count);
}
EXPORT_SYMBOL(osal_memcmp);

void * osal_memchr(const void *s,int c,int n)
{
	return memchr(s, c, n);
}
EXPORT_SYMBOL(osal_memchr);

void * osal_memchr_inv(const void *start, int c, int bytes)
{
	return memchr_inv(start, c, bytes);
}
EXPORT_SYMBOL(osal_memchr_inv);

unsigned long long osal_strtoull(const char *cp, char **endp, unsigned int base)
{
	return simple_strtoull(cp, endp, base);
}
EXPORT_SYMBOL(osal_strtoull);

unsigned long osal_strtoul(const char *cp, char **endp, unsigned int base)
{
	return simple_strtoul(cp, endp, base);
}
EXPORT_SYMBOL(osal_strtoul);

long osal_strtol(const char *cp, char **endp, unsigned int base)
{
	return simple_strtol(cp, endp, base);
}
EXPORT_SYMBOL(osal_strtol);

long long osal_strtoll(const char *cp, char **endp, unsigned int base)
{
	return simple_strtoll(cp, endp, base);
}
EXPORT_SYMBOL(osal_strtoll);

int osal_snprintf(char *buf, int size, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsnprintf(buf, size, fmt, args);
	va_end(args);

	return i;
}
EXPORT_SYMBOL(osal_snprintf);

int osal_scnprintf(char *buf, int size, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vscnprintf(buf, size, fmt, args);
	va_end(args);

	return i;
}
EXPORT_SYMBOL(osal_scnprintf);

int osal_sprintf(char *buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsnprintf(buf, INT_MAX, fmt, args);
	va_end(args);

	return i;
}
EXPORT_SYMBOL(osal_sprintf);

int osal_sscanf(const char *buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsscanf(buf, fmt, args);
	va_end(args);

	return i;
}
EXPORT_SYMBOL(osal_sscanf);

char *osal_llitoa(long long int n, char * buf)
{
	int i,j;
	long long int sign;
	unsigned long long temp;
	char tmp[20];
	if((sign=n)<0)
		n=-n;
	i = 0;
	j = 0;
	temp = (unsigned long long)n;
	do{
	    tmp[i++]=do_div(temp,10)+'0';
	}while ((temp)>0);
	if(sign<0)
		tmp[i]='-';
	else
		i--;
	for(j=0;j<=i;j++)
		buf[j] = tmp[i-j];
	buf[j]='\0';
	return buf;
} 

EXPORT_SYMBOL(osal_llitoa);

char *osal_lluitoa(long long unsigned int n, char * buf)
{
	int i,j;
	char tmp[20];

	i = 0;
	do{
	    tmp[i++]=do_div(n,10)+'0';
	}while ((n)>0);
    i--;
	for(j=0;j<=i;j++)
		buf[j] = tmp[i-j];
	buf[j]='\0';
	return buf;
} 

EXPORT_SYMBOL(osal_lluitoa);

char *osal_itoa(int n, char * buf)
{
	int i,j,sign;
	char tmp[12];
	if((sign=n)<0)
		n=-n;
	i = 0;
	j = 0;
	do{
	    tmp[i++]=n%10+'0';
	}while ((n/=10)>0);
	if(sign<0)
		tmp[i]='-';
	else
		i--;
	for(j=0;j<=i;j++)
		buf[j] = tmp[i-j];
	buf[j]='\0';
	return buf;
} 
EXPORT_SYMBOL(osal_itoa);

char *osal_uitoa(unsigned int n, char * buf)
{
	int i,j;
	char tmp[10];

	i = 0;
	do{
	    tmp[i++]=n%10+'0';
	}while ((n/=10)>0);
    i--;
	for(j=0;j<=i;j++)
		buf[j] = tmp[i-j];
	buf[j]='\0';
	return buf;
} 

EXPORT_SYMBOL(osal_uitoa);

char *osal_uitoxa(unsigned int n, char * buf)
{
	int i,j,num;    
	char tmp[10];

	i = 0;
	do{  
        num = n & 0xf;
        if(num < 10){
	         tmp[i++]= num + '0';
        } else {
            tmp[i++]= num - 10 + 'A';
        }
	}while ((n >>= 4)>0);
    i--;
	for(j=0;j<=i;j++)
		buf[j] = tmp[i-j];
	buf[j]='\0';
	return buf;
} 

EXPORT_SYMBOL(osal_uitoxa);

int osal_vsnprintf(char *str, int size, const char *fmt, osal_va_list args)
{
    return vsnprintf(str, size, fmt, args);
}
EXPORT_SYMBOL(osal_vsnprintf);
