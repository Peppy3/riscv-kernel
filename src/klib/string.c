#include <stddef.h>

size_t stridx(const char *s, int ch) {
	size_t i = 0;
	while (*s != (ch & 0xff) && *s) {
		s++; i++;
	}
	return i;
}

size_t strlen(const char *s) {
	if (s == NULL) return 0;
	size_t len = 0;

	while(s[len] != 0) {
		len++;
	}
	return len;
}

size_t strnlen(const char *s, size_t maxlen) {
	if (s == NULL) return 0;

	size_t len = 0;
	for (;s[len] != 0 || len < maxlen; len++);
	return len;
}

char *strchr(char *str, int ch) {
	size_t i = 0;
	while (str[i] != 0) {
		if (str[i] == (ch & 0xff)) {
			return &str[i];
		}
	}
	return NULL;
}

int strcmp(const char *lhs, const char *rhs) {
	size_t i = 0;

	while (lhs[i] == rhs[i] && lhs[i]) {
		i++;
	}

	return lhs[i] - rhs[i];
}

int strncmp(const char *lhs, const char *rhs, size_t count) {
	size_t i = 0;

	while (lhs[i] == rhs[i] && lhs[i] && --count) {
		i++;
	}

	return lhs[i] - rhs[i];
}
