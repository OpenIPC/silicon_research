#include "main.h"

int memcpy_s(void *dest, size_t destMax, const void *src, size_t count) {
  memcpy(dest, src, count);
  return 0;
}

int memset_s(void *dest, size_t destMax, int c, size_t count) {
  memset(dest, c, count);
  return 0;
}

int memmove_s(void *dest, size_t destMax, const void *src, size_t count) {
  memmove(dest, src, count);
  return 0;
}

int strncpy_s(char *strDest, size_t destMax, const char *strSrc, size_t count) {
  strncpy(strDest, strSrc, count);
  return 0;
}

int snprintf_s(char *strDest, size_t destMax, size_t count, const char *format, ... ) {
  return snprintf(strDest, count, format);
}

#ifdef TARGET_PLATFORM_EABI_MUSL
  void* mmap(void *start, size_t len, int prot, int flags, int fd, uint32_t off) {
      return mmap64(start, len, prot, flags, fd, off);
  }
#endif