#include <stddef.h>
#include <cpu.h>

#include <spinlock.h>

void spinlock_init(struct Spinlock *sl) {
	sl->lock = 0;
	sl->hartid = -1;
}

void spinlock_aquire(struct Spinlock *sl) {
	// am I holding the lock?
	if (sl->hartid == hart()->id)
		return;
	
	inter_push();

	// atomic swap
	while (__sync_lock_test_and_set(&sl->lock, 1) != 0)
		;

	// fence for atomic swap
	__sync_synchronize();
	
	sl->hartid = hart()->id;
	return;
}

void spinlock_release(struct Spinlock *sl) {
	/*
	if (!(sl->hartid == cpu()))
		panic();
	*/
	
	sl->hartid = -1;
	
	// fence
	__sync_synchronize();

	// release lock
	__sync_lock_release(&sl->lock);
	
	inter_pop();
	return;
}
