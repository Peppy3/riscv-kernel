#ifndef CPU_H
#define CPU_H

#include <stdbool.h>

typedef long hartid_t;

typedef struct cpu {
	hartid_t id;

	long intr_npush;
	long intr_initial; // state before we pushed interupts
} cpu_t;

// poitner to the cpu struct for the hart is in register 'tp' which
// we're unable to get to in C
static inline cpu_t *cpu(void) {
	register cpu_t *hart asm("tp");
	return hart;
}

// hardware interupts
void inter_push(void);
void inter_pop(void);

#endif /* CPU_H */
