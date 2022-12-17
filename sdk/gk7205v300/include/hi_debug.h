/*
 * 
 */
#ifndef __HI_DEBUG_H__
#define __HI_DEBUG_H__

#ifndef __KERNEL__
#include <stdio.h>
#include <stdarg.h>
#endif

#include "hi_type.h"
#include "hi_common.h"
#include "mpp_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_DBG_EMERG      0   /* system is unusable                   */
#define HI_DBG_ALERT      1   /* action must be taken immediately     */
#define HI_DBG_CRIT       2   /* critical conditions                  */
#define HI_DBG_ERR        3   /* error conditions                     */
#define HI_DBG_WARN       4   /* warning conditions                   */
#define HI_DBG_NOTICE     5   /* normal but significant condition     */
#define HI_DBG_INFO       6   /* informational                        */
#define HI_DBG_DEBUG      7   /* debug-level messages                 */

#ifndef __KERNEL__
/******************************************************************************
** For User Mode : HI_PRINT, HI_ASSERT, HI_TRACE
******************************************************************************/

#define HI_PRINT GK_PRINT
#define HI_ASSERT GK_ASSERT

#define HI_TRACE MODULE_TRACE
/* #ifdef HI_DEBUG */


#else
/******************************************************************************
** For Linux Kernel : HI_PRINT, HI_ASSERT, HI_TRACE
******************************************************************************/
//#include "hi_osal.h"
#define HI_PRINT GK_PRINT
#ifdef CONFIG_LOG_TRACE_SUPPORT
	#define HI_ASSERT GK_ASSERT

	#define HI_TRACE LOG
#else
    #define HI_ASSERT(expr)
    #define HI_TRACE(level, enModId, fmt...)
#endif


#endif  /* end of __KERNEL__ */


#define HI_PRINT_BLOCK GK_PRINT_BLOCK



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_DEBUG_H__ */

