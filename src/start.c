#include <stddef.h>

#include <cpu.h>
#include <klib/common.h>
#include <klib/mem.h>
#include <spinlock.h>

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

static uint a[41];
static uint b[41];

void start(hartid_t boot_hartid, dtb_t* dtb_location) {
	cpu_t hart = { 0 };
	hart.id = boot_hartid;
	asm volatile ("mv tp, %0" : : "r" (&hart));

	a[0] = 0xa8b92fec;
	a[1] = 0xdeadbeef;
	a[2] = 0x18c0de;
	a[3] = 0xbadf00d;

	memset(&a[4], 0xc0, (41 - 4) * sizeof(uint));
	memmove(b, a, 41 * sizeof(uint));

	while (1);
}
