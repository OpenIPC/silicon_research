/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef SECUREC_H_5D13A042_DC3F_4ED9_A8D1_882811274C27
#define SECUREC_H_5D13A042_DC3F_4ED9_A8D1_882811274C27

#include "securectype.h"
#ifndef SECUREC_HAVE_STDARG_H
#define SECUREC_HAVE_STDARG_H 1
#endif

#if SECUREC_HAVE_STDARG_H
#include <stdarg.h>
#endif

#ifndef SECUREC_HAVE_ERRNO_H
#define SECUREC_HAVE_ERRNO_H 1
#endif

#if SECUREC_HAVE_ERRNO_H
#if SECUREC_IN_KERNEL
#include <linux/errno.h>
#else
#include <errno.h>
#endif
#endif

#if defined(SECUREC_NEED_ERRNO_TYPE) || !defined(__STDC_WANT_LIB_EXT1__) || \
    (defined(__STDC_WANT_LIB_EXT1__) && (!__STDC_WANT_LIB_EXT1__))
#ifndef SECUREC_DEFINED_ERRNO_TYPE
#define SECUREC_DEFINED_ERRNO_TYPE
#ifndef errno_t
typedef int errno_t;
#endif
#endif
#endif

#ifndef EOK
#define EOK 0
#endif

#ifndef EINVAL
#define EINVAL 22
#endif

#ifndef EINVAL_AND_RESET
#define EINVAL_AND_RESET 150
#endif

#ifndef ERANGE
#define ERANGE 34
#endif

#ifndef ERANGE_AND_RESET
#define ERANGE_AND_RESET  162
#endif

#ifndef EOVERLAP_AND_RESET
#define EOVERLAP_AND_RESET 182
#endif

#ifndef SECUREC_API
#if defined(SECUREC_DLL_EXPORT)
#define SECUREC_API __declspec(dllexport)
#elif defined(SECUREC_DLL_IMPORT)
#define SECUREC_API __declspec(dllimport)
#else
#if defined(_MSC_VER)
#define SECUREC_API
#else
#define SECUREC_API extern
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
    SECUREC_API const char *GetHwSecureCVersion(unsigned short *verNumber);

#if SECUREC_ENABLE_MEMSET
    SECUREC_API errno_t memset_s(void *dest, size_t destMax, int c, size_t count);
#endif

#ifndef SECUREC_ONLY_DECLARE_MEMSET
#define SECUREC_ONLY_DECLARE_MEMSET     0
#endif

#if !SECUREC_ONLY_DECLARE_MEMSET

#if SECUREC_ENABLE_MEMMOVE
    SECUREC_API errno_t memmove_s(void *dest, size_t destMax, const void *src, size_t count);
#endif

#if SECUREC_ENABLE_MEMCPY
    SECUREC_API errno_t memcpy_s(void *dest, size_t destMax, const void *src, size_t count);
#endif

#if SECUREC_ENABLE_STRCPY
    SECUREC_API errno_t strcpy_s(char *strDest, size_t destMax, const char *strSrc);
#endif

#if SECUREC_ENABLE_STRNCPY
    SECUREC_API errno_t strncpy_s(char *strDest, size_t destMax, const char *strSrc, size_t count);
#endif

#if SECUREC_ENABLE_STRCAT
    SECUREC_API errno_t strcat_s(char *strDest, size_t destMax, const char *strSrc);
#endif

#if SECUREC_ENABLE_STRNCAT
    SECUREC_API errno_t strncat_s(char *strDest, size_t destMax, const char *strSrc, size_t count);
#endif

#if SECUREC_ENABLE_VSPRINTF
    SECUREC_API int vsprintf_s(char *strDest, size_t destMax, const char *format,
                               va_list argList) SECUREC_ATTRIBUTE(3, 0);
#endif

#if SECUREC_ENABLE_SPRINTF
    SECUREC_API int sprintf_s(char *strDest, size_t destMax, const char *format, ...) SECUREC_ATTRIBUTE(3, 4);
#endif

#if SECUREC_ENABLE_VSNPRINTF
    SECUREC_API int vsnprintf_s(char *strDest, size_t destMax, size_t count, const char *format,
                                va_list argList) SECUREC_ATTRIBUTE(4, 0);
#endif

#if SECUREC_ENABLE_SNPRINTF
    SECUREC_API int snprintf_s(char *strDest, size_t destMax, size_t count, const char *format,
                               ...) SECUREC_ATTRIBUTE(4, 5);
#endif

#if SECUREC_SNPRINTF_TRUNCATED
    SECUREC_API int vsnprintf_truncated_s(char *strDest, size_t destMax, const char *format,
                                          va_list argList) SECUREC_ATTRIBUTE(3, 0);

    SECUREC_API int snprintf_truncated_s(char *strDest, size_t destMax,
                                         const char *format, ...) SECUREC_ATTRIBUTE(3, 4);
#endif

#if SECUREC_ENABLE_SCANF
    SECUREC_API int scanf_s(const char *format, ...);
#endif

#if SECUREC_ENABLE_VSCANF
    SECUREC_API int vscanf_s(const char *format, va_list argList);
#endif

#if SECUREC_ENABLE_SSCANF
    SECUREC_API int sscanf_s(const char *buffer, const char *format, ...);
#endif

#if SECUREC_ENABLE_VSSCANF
    SECUREC_API int vsscanf_s(const char *buffer, const char *format, va_list argList);
#endif

#if SECUREC_ENABLE_FSCANF
    SECUREC_API int fscanf_s(FILE *stream, const char *format, ...);
#endif

#if SECUREC_ENABLE_VFSCANF
    SECUREC_API int vfscanf_s(FILE *stream, const char *format, va_list argList);
#endif

#if SECUREC_ENABLE_STRTOK
    SECUREC_API char *strtok_s(char *strToken, const char *strDelimit, char **context);
#endif

#if SECUREC_ENABLE_GETS && !SECUREC_IN_KERNEL
    SECUREC_API char *gets_s(char *buffer, size_t destMax);
#endif

#if SECUREC_ENABLE_WCHAR_FUNC
#if SECUREC_ENABLE_MEMCPY
    SECUREC_API errno_t wmemcpy_s(wchar_t *dest, size_t destMax, const wchar_t *src, size_t count);
#endif

#if SECUREC_ENABLE_MEMMOVE
    SECUREC_API errno_t wmemmove_s(wchar_t *dest, size_t destMax, const wchar_t *src, size_t count);
#endif

#if SECUREC_ENABLE_STRCPY
    SECUREC_API errno_t wcscpy_s(wchar_t *strDest, size_t destMax, const wchar_t *strSrc);
#endif

#if SECUREC_ENABLE_STRNCPY
    SECUREC_API errno_t wcsncpy_s(wchar_t *strDest, size_t destMax, const wchar_t *strSrc, size_t count);
#endif

#if SECUREC_ENABLE_STRCAT
    SECUREC_API errno_t wcscat_s(wchar_t *strDest, size_t destMax, const wchar_t *strSrc);
#endif

#if SECUREC_ENABLE_STRNCAT
    SECUREC_API errno_t wcsncat_s(wchar_t *strDest, size_t destMax, const wchar_t *strSrc, size_t count);
#endif

#if SECUREC_ENABLE_STRTOK
    SECUREC_API wchar_t *wcstok_s(wchar_t *strToken, const wchar_t *strDelimit, wchar_t **context);
#endif

#if SECUREC_ENABLE_VSPRINTF
    SECUREC_API int vswprintf_s(wchar_t *strDest, size_t destMax, const wchar_t *format, va_list argList);
#endif

#if SECUREC_ENABLE_SPRINTF

    SECUREC_API int swprintf_s(wchar_t *strDest, size_t destMax, const wchar_t *format, ...);
#endif

#if SECUREC_ENABLE_FSCANF
    SECUREC_API int fwscanf_s(FILE *stream, const wchar_t *format, ...);
#endif

#if SECUREC_ENABLE_VFSCANF
    SECUREC_API int vfwscanf_s(FILE *stream, const wchar_t *format, va_list argList);
#endif

#if SECUREC_ENABLE_SCANF
    SECUREC_API int wscanf_s(const wchar_t *format, ...);
#endif

#if SECUREC_ENABLE_VSCANF
    SECUREC_API int vwscanf_s(const wchar_t *format, va_list argList);
#endif

#if SECUREC_ENABLE_SSCANF
    SECUREC_API int swscanf_s(const wchar_t *buffer, const wchar_t *format, ...);
#endif

#if SECUREC_ENABLE_VSSCANF
    SECUREC_API int vswscanf_s(const wchar_t *buffer, const wchar_t *format, va_list argList);
#endif
#endif
#endif

    extern errno_t strncpy_error(char *strDest, size_t destMax, const char *strSrc, size_t count);
    extern errno_t strcpy_error(char *strDest, size_t destMax, const char *strSrc);

#if SECUREC_WITH_PERFORMANCE_ADDONS
    extern errno_t memset_sOptAsm(void *dest, size_t destMax, int c, size_t count);
    extern errno_t memset_sOptTc(void *dest, size_t destMax, int c, size_t count);
    extern errno_t memcpy_sOptAsm(void *dest, size_t destMax, const void *src, size_t count);
    extern errno_t memcpy_sOptTc(void *dest, size_t destMax, const void *src, size_t count);

#define strcpy_sp(dest, destMax, src)  ((__builtin_constant_p((destMax)) && \
    __builtin_constant_p((src))) ?  \
    SECUREC_STRCPY_SM((dest), (destMax), (src)) : \
    strcpy_s((dest), (destMax), (src)))

#define strncpy_sp(dest, destMax, src, count)  ((__builtin_constant_p((count)) && \
    __builtin_constant_p((destMax)) && \
    __builtin_constant_p((src))) ?  \
    SECUREC_STRNCPY_SM((dest), (destMax), (src), (count)) : \
    strncpy_s((dest), (destMax), (src), (count)))

#define strcat_sp(dest, destMax, src) ((__builtin_constant_p((destMax)) && \
    __builtin_constant_p((src))) ?  \
    SECUREC_STRCAT_SM((dest), (destMax), (src)) : \
    strcat_s((dest), (destMax), (src)))

#define strncat_sp(dest, destMax, src, count) ((__builtin_constant_p((count)) &&  \
    __builtin_constant_p((destMax)) && \
    __builtin_constant_p((src))) ?  \
    SECUREC_STRNCAT_SM((dest), (destMax), (src), (count)) : \
    strncat_s((dest), (destMax), (src), (count)))

#define memcpy_sp(dest, destMax, src, count)  (__builtin_constant_p((count)) ? \
    (SECUREC_MEMCPY_SM((dest), (destMax), (src), (count))) : \
    (__builtin_constant_p((destMax)) ? \
    (((size_t)(destMax) > 0 && \
    (((unsigned long long)(destMax) & (unsigned long long)(-2)) < SECUREC_MEM_MAX_LEN)) ? \
    memcpy_sOptTc((dest), (destMax), (src), (count)) : ERANGE) : \
    memcpy_sOptAsm((dest), (destMax), (src), (count))))

#define memset_sp(dest, destMax, c, count)  (__builtin_constant_p((count)) ? \
    (SECUREC_MEMSET_SM((dest), (destMax), (c), (count))) : \
    (__builtin_constant_p((destMax)) ? \
    (((((unsigned long long)(destMax) & (unsigned long long)(-2)) < SECUREC_MEM_MAX_LEN)) ? \
    memset_sOptTc((dest), (destMax), (c), (count)) : ERANGE) : \
    memset_sOptAsm((dest), (destMax), (c), (count))))

#endif

#ifdef __cplusplus
}
#endif
#endif

