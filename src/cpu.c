#include <riscv.h>
#include <cpu.h>

// poitner to the cpu struct for the hart is in register 'tp' which
// we're unable to get to in C
Hart *hart(void) {
	register Hart *hart asm("tp");
	return hart;
}

void hart_init(Hartid id, Hart *hart) {
	asm volatile ("mv tp, %0" : : "r" (hart));
	hart->id = id;
	hart->intr_npush = 0;
	hart->intr_initial = 0;
}

void inter_push(void) {
	long old = interupts_off();
	if (hart()->intr_npush == 0) {
		hart()->intr_initial = old;
	}
	hart()->intr_npush += 1;
	return;
}

void inter_pop(void) {
	/*
	if (cpu()->intr_npush < 1) 
		panic("inter_push");
*/
	hart()->intr_npush -= 1;

	if (hart()->intr_npush == 0 && hart()->intr_initial)
		interupts_on(hart()->intr_initial);
	return;
}

