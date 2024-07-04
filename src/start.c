#include <stddef.h>

#include <cpu.h>
#include <klib/bswap.h>
#include <debug.h>

#include <sbi.h>
#include <dtb.h>
#include <devicetree.h>

#include <riscv.h>

// This is getting called from src/arch/<architecture>/head.S
void kmain(Hartid boot_hartid, Dtb *dtb) {
	struct Hart hart;
	hart_init(boot_hartid, &hart);
	debug_init();
	debug_what_console();

	if (dtb_verify(dtb)) {
		debug_puts("\nDtb has a good magic");
	}
	else {
		debug_puts("\nBad dtb magic!");
	}
	
	debug_printf("DTB @ %p\n", dtb);

	DtbRsmapEntry *rsmap = dtb_rsmap_iter_get(dtb);
	if (dtb_rsmap_next(&rsmap)) do {
		debug_printf("Reserved memory: %p -> %p\n", 
				(void *)(swap64_from_be(rsmap->addr)), 
				(void *)(swap64_from_be(rsmap->addr) + swap64_from_be(rsmap->size)));
	} while (dtb_rsmap_next(&rsmap));
	else {
		debug_puts("No Reserved memory blocks\n");
	}
	debug_puts("Converting Dtb");

	Devicetree *dt = (Devicetree*)((char*)dtb + dtb_size(dtb) + 64);
	dt_convert_dtb(dtb, dt);
	dt_print(dt);
	debug_puts("going into an infinite_loop\n");
	while (1) {
		asm volatile ("wfi");
	}
}
