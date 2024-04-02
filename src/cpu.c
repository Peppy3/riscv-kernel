#include <riscv.h>
#include <cpu.h>

void inter_push(void) {
	long old = interupts_off();
	if (cpu()->intr_npush == 0) {
		cpu()->intr_initial = old;
	}
	cpu()->intr_npush += 1;
	return;
}

void inter_pop(void) {
	/*
	if (cpu()->intr_npush < 1) 
		panic("inter_push");
*/
	cpu()->intr_npush -= 1;

	if (cpu()->intr_npush == 0 && cpu()->intr_initial)
		interupts_on(cpu()->intr_initial);
	return;
}

