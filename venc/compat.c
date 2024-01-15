#include "main.h"
#include <sys/syscall.h>

int __ctype_b;

int __fgetc_unlocked(FILE *stream) {
  return fgetc(stream);
}

size_t _stdlib_mb_cur_max(void) {
  return 0;
}

void* mmap(void *start, size_t len, int prot, int flags, int fd, uint32_t off) {
  return (void *)syscall(SYS_mmap2, start, len, prot, flags, fd, off >> 12);
}
