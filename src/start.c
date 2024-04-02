#include <stddef.h>

#include <cpu.h>
#include <klib/common.h>
#include <klib/spinlock.h>

#include <sbi.h>
#include <dtb.h>

#include <riscv.h>

long legacy_print(const char *s) {
	long err = 0;
	for (; *(s) != '\0'; s++) {
		err = sbi_legacy_putchar(*s);
		if (err < 0) break;
	}
	return err;
}

void start(hartid_t boot_hartid, dtb_t* dtb_location) {
	cpu_t hart = { 0 };
	hart.id = boot_hartid;
	asm volatile ("mv tp, %0" : : "r" (&hart));
	
	spinlock_t sl;
	spinlock_init(&sl);

	spinlock_aquire(&sl);
	
	legacy_print("I'm in an infinite spinlock\r\n");

	while (1);
}
