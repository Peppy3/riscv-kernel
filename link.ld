
OUTPUT_ARCH(riscv)
ENTRY(_entry)


SECTIONS {
	/* start address */
	. = 0x80200000;

	__start = .;
	__text_start = .;
	.text : {
		KEEP(*(.text.entry))
		*(.text)
		
		/* trampoline */
		. = ALIGN(4096);
		__trampoline_start = .;
		*(.trampoline)
		/*ASSERT(. - __trampoline_start == 4096, 
			"error: trampoline larger than one page");
		*/
	}
	. = ALIGN(4096);
	__text_end = .;

	__rodata_start = .;
	.rodata : {
		*(.rodata)
	}
	. = ALIGN(4096);
	__rodata_end = .;

	__data_start = .;
	.data : {
		*(.data)
	}
	. = ALIGN(4096);
	__data_end = .;

	__bss_start = .;
	.bss : {
		bss = .;
		*(.bss)

		/* boot core stack */
		. = ALIGN(0x1000);
		KEEP(*(.bss.boot_stack))
		__boot_core_stack_start = .;
		. += 0x2000;
		__boot_core_stack_end = .;
	}
	. = ALIGN(4096);
	__bss_end = .;
	__bss_size = __bss_end - __bss_start;
	
	__end = .;
}
