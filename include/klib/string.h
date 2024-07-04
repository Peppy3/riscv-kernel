#ifndef KLIB_STRING_H
#define KLIB_STRING_H

#include <stddef.h>

// returns the index at the first char ch found in string s
size_t stridx(const char *s, int ch);

size_t strlen(const char *s);
size_t strnlen(const char *s, size_t maxlen);
char *strchr(const char *str, int ch);
int strcmp(const char *lhs, const char *rhs);
int strncmp(const char *lhs, const char *rhs, size_t count);


#endif /* KLIB_STRING_H */
