#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <type.h>
#include <cpu.h>

struct Spinlock {
	atomic_t lock;
	Hartid hartid; // who's holding it
};

void spinlock_init(struct Spinlock *sl);
void spinlock_aquire(struct Spinlock *sl);
void spinlock_release(struct Spinlock *sl);

#endif /* SPINLOCK_H */
