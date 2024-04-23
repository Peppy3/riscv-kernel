#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

typedef int64_t Hartid;

typedef struct Hart {
	Hartid id;

	long intr_npush;
	long intr_initial; // state before we pushed interupts
} Hart;

void hart_init(Hartid id, Hart* hart);

Hart *hart(void);

// hardware interupts
void inter_push(void);
void inter_pop(void);

#endif /* CPU_H */
