#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <type.h>
#include <cpu.h>

typedef struct Spinlock {
	atomic_t lock;
	Hartid hartid; // who's holding it
} Spinlock;

void spinlock_init(Spinlock *sl);
void spinlock_aquire(Spinlock *sl);
void spinlock_release(Spinlock *sl);

#endif /* SPINLOCK_H */
