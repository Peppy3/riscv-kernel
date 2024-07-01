#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

typedef int64_t Hartid;

struct Hart {
	Hartid id;

	long intr_npush;
	long intr_initial; // state before we pushed interupts
};

void hart_init(Hartid id, struct Hart* hart);

struct Hart *hart(void);

// hardware interupts
void inter_push(void);
void inter_pop(void);

#endif /* CPU_H */
