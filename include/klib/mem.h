#ifndef KLIB_MEM_H
#define KLIB_MEM_H

#include <stddef.h>

void *memmove(void *dest, const void *src, size_t n);
void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memset(void *s, int c, size_t n);

#endif /* KLIB_MEM_H */
