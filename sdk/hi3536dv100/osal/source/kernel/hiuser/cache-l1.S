

#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/assembler.h>



/*
 * This is the maximum size of an area which will be invalidated
 * using the single invalidate entry instructions.  Anything larger
 * than this, and we go for the whole cache.
 *
 * This value should be chosen such that we choose the cheapest
 * alternative.
 */
#define CACHE_DLIMIT	16384

/*
 * the cache line size of the I and D cache
 */
#define CACHE_DLINESIZE	32



/*
 *	hi_dma_inv_range(start, size)
 *
 *	Invalidate (discard) the specified virtual address range.
 *	May not write back any entries.  If 'start' or 'end'
 *	are not cache line aligned, those lines must be written
 *	back.
 *
 *	- start	- virtual start address
 *	- size	- length
 *
 * (same as v4wb)
 */
	.text
	
#ifdef CONFIG_CPU_V7
 /*
  * dcache_line_size - get the minimum D-cache line size from the CTR register
  * on ARMv7.
  */
         .macro  dcache_line_size, reg, tmp
         mrc     p15, 0, \tmp, c0, c0, 1         @ read ctr
         lsr     \tmp, \tmp, #16
         and     \tmp, \tmp, #0xf                @ cache line size encoding
         mov     \reg, #4                        @ bytes per word
         mov     \reg, \reg, lsl \tmp            @ actual cache line size
         .endm
#endif

	.globl hi_dma_inv_range
	.type hi_dma_inv_range, #function
	
hi_dma_inv_range:	
         add	r1, r1, r0
#ifdef CONFIG_CPU_ARM926T
#ifndef CONFIG_CPU_DCACHE_WRITETHROUGH
	       tst	r0, #CACHE_DLINESIZE - 1
	       mcrne	p15, 0, r0, c7, c10, 1		@ clean D entry
	       tst	r1, #CACHE_DLINESIZE - 1
	       mcrne	p15, 0, r1, c7, c10, 1		@ clean D entry
#endif
	       bic	r0, r0, #CACHE_DLINESIZE - 1
1:	     mcr	p15, 0, r0, c7, c6, 1		@ invalidate D entry
	       add	r0, r0, #CACHE_DLINESIZE
	       cmp	r0, r1
	       blo	1b
	       mcr	p15, 0, r0, c7, c10, 4		@ drain WB
#endif
	
#ifdef CONFIG_CPU_V7
         dcache_line_size r2, r3
         sub     r3, r2, #1
         tst     r0, r3
         bic     r0, r0, r3

         mcrne   p15, 0, r0, c7, c14, 1          @ clean & invalidate D / U line

         tst     r1, r3
         bic     r1, r1, r3
         mcrne   p15, 0, r1, c7, c14, 1          @ clean & invalidate D / U line
 1:
         mcr     p15, 0, r0, c7, c6, 1           @ invalidate D / U line
         add     r0, r0, r2
         cmp     r0, r1
         blo     1b
         dsb
#endif
	       mov	pc, lr


