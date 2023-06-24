#include <linux/module.h>
#include <linux/kernel.h>
//#include <asm/div64.h>
#include <linux/random.h>
#include <linux/version.h>
#include "hi_osal.h"

unsigned int osal_do_div(unsigned int n, unsigned int base)
{
	unsigned long long temp;
	temp = 0xffffffff & (unsigned long long)n;
	do_div(temp , base);
	return (unsigned int)temp;

}
EXPORT_SYMBOL(osal_do_div);

unsigned long long osal_do_div64(unsigned long long n, unsigned int base)
{
	do_div(n , base);
	return n;
}
EXPORT_SYMBOL(osal_do_div64);

unsigned long long osal_do_div_rem64(unsigned long long n, unsigned int base)
{
	return do_div(n , base);
}
EXPORT_SYMBOL(osal_do_div_rem64);

long long osal_do_div64_s64(long long dividend, long long divisor)
{
	return div64_s64(dividend , divisor);
}
EXPORT_SYMBOL(osal_do_div64_s64);

unsigned long long osal_do_div64_u64(unsigned long long dividend, unsigned long long divisor)
{
	return div64_u64(dividend , divisor);
}
EXPORT_SYMBOL(osal_do_div64_u64);

unsigned int osal_iter_div_u64_rem(unsigned long long  dividend, unsigned int divisor, unsigned long long  *remainder)
{
    return iter_div_u64_rem(dividend , divisor , remainder);
}
EXPORT_SYMBOL(osal_iter_div_u64_rem);


