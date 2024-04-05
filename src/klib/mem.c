#include <stddef.h>

#include <klib/common.h>

void *memmove(void *dest, const void *src, size_t n) {
	for (size_t i = 0; i < n; i++) {
		((char *)dest)[i] = ((char *)src)[i];
	}
	return dest;
}

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
	for (size_t i = 0; i < n; i++) {
		((char *)dest)[i] = ((char *)src)[i];
	}
	return dest;
}

void *memset(void *s, int c, size_t n) {
	for (size_t i = 0; i < n; i++) {
		((char *)s)[i] = (char)c;
	}

	return s;
}
