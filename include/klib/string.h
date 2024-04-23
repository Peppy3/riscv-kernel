#ifndef KLIB_STRING_H
#define KLIB_STRING_H

#include <stddef.h>

size_t strlen(const char *s);
size_t strnlen(const char *s, size_t maxlen);
char *strchr(const char *str, int ch);
int strncmp(const char *lhs, const char *rhs, size_t count);

#endif /* KLIB_STRING_H */
