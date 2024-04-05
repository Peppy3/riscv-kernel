#include <stddef.h>
#include <cpu.h>

#include <klib/common.h>

#include <spinlock.h>

void spinlock_init(spinlock_t *sl) {
	sl->lock = 0;
	sl->hartid = NULL;
}

void spinlock_aquire(struct spinlock *sl) {
	// am I holding the lock?
	if (sl->hartid == cpu())
		return;
	
	inter_push();

	// atomic swap
	while (__sync_lock_test_and_set(&sl->lock, 1) != 0)
		;

	// fence for atomic swap
	__sync_synchronize();
	
	sl->hartid = cpu();
	return;
}

void spinlock_release(struct spinlock *sl) {
	/*
	if (!(sl->hartid == cpu()))
		panic();
	*/
	
	sl->hartid = NULL;
	
	// fence
	__sync_synchronize();

	// release lock
	__sync_lock_release(&sl->lock);
	
	inter_pop();
	return;
}
