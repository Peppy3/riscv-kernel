#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <klib/common.h>
#include <cpu.h>

typedef struct Spinlock {
	uint lock;
	Hartid hartid; // who's holding it
} Spinlock;

void spinlock_init(Spinlock *sl);
void spinlock_aquire(Spinlock *sl);
void spinlock_release(Spinlock *sl);

#endif /* SPINLOCK_H */
