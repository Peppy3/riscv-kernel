#include <stddef.h>

#include <cpu.h>
#include <klib/common.h>
#include <klib/bswap.h>
#include <debug.h>

#include <sbi.h>
#include <dtb.h>

#include <riscv.h>


void start(Hartid boot_hartid, Dtb *dtb) {
	Hart hart;
	hart_init(boot_hartid, &hart);
	debug_init();
	debug_what_console();
	
	DtbRsmapEntry *rsmap = dtb_rsmap_iter_get(dtb);
	if (dtb_rsmap_next(&rsmap)) do {
		debug_printf("Reserved memory: %p -> %p\n", 
				(void *)(swap64_from_be(rsmap->addr)), 
				(void *)(swap64_from_be(rsmap->addr) + swap64_from_be(rsmap->size)));
	} while (dtb_rsmap_next(&rsmap));
	else {
		debug_printf("No Reserved memory blocks\n");
	}


	while (1);
}
