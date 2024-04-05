#include <stddef.h>

int strncmp(const char *lhs, const char *rhs, size_t count) {
	int i = 0;

	while (lhs[i] == rhs[i] && --count && lhs[i]) {
		i++;
	}

	return lhs[i] - rhs[i];
}
