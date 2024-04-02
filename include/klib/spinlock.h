#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <klib/common.h>
#include <cpu.h>

typedef struct spinlock {
	uint lock;
	cpu_t *hartid; // who's holding it
} spinlock_t;

void spinlock_init(spinlock_t *sl);
void spinlock_aquire(struct spinlock *sl);
void spinlock_release(struct spinlock *sl);

#endif /* SPINLOCK_H */
