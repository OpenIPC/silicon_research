#include "main.h"

int __ctype_b;

int __fgetc_unlocked(FILE *stream) {
	return fgetc(stream);
}

size_t _stdlib_mb_cur_max(void) {
	return 0;
}

void *mmap64(void *start, size_t len, int prot, int flags, int fd, off_t off);

void* mmap(void *start, size_t len, int prot, int flags, int fd, uint32_t off) {
	return mmap64(start, len, prot, flags, fd, off);
}
