OUTPUT_ARCH(riscv)
ENTRY(_start)

PHDRS {
	/* flag bits: 0 = executable, 1 = writable, 2 = readable */
	text PT_LOAD FLAGS( (1 << 0) | (1 << 2) );
	rodata PT_LOAD FLAGS( (1 << 2) );
	data PT_LOAD FLAGS( (1 << 1) | (1 << 2) );
}

SECTIONS {
	/* 
	 * Boot location of the kernel.
	 * We want to move up to the negative space as quickly as possible
	 */
	. = 0x80200000;
	PROVIDE(__start = .);
	.text : {
		PROVIDE(__text_start = .);
		/* Header for linux kernel image */
		KEEP(*(.boot.header))
		/* Our boot code before we jump to higher half */
		KEEP(*(.boot.entry))
		__boot_end = .;

		/* trampoline */
		. = ALIGN(4096);
		__trampoline_start = .;
		*(.trampoline)
		. = ALIGN(4096);
		ASSERT(. - __trampoline_start != 4096, 
			"error: trampoline larger than one page");

		KEEP(*(.text))
	} :text
	. = ALIGN(4096);
	PROVIDE(__text_end = .);

	PROVIDE(__rodata_start = .);
	.rodata : {
		*(.rodata)
	} :rodata
	. = ALIGN(4096);
	PROVIDE(__rodata_end = .);

	PROVIDE(__data_start = .);
	.data : {
		*(.data)
	} :data
	. = ALIGN(4096);
	PROVIDE(__data_end = .);

	PROVIDE(__bss_start = .);
	.bss : {
		bss = .;
		/* Compiler stuff */
		*(COMMON)

		*(.bss)

		/* boot core stack */
		. = ALIGN(0x1000);
		KEEP(*(.bss.boot_stack))
		__boot_core_stack_top = .;
		. += 0x1000;
		__boot_core_stack_bottom = .;
		. = ALIGN(4096);
	} :data
	. = ALIGN(4096);
	PROVIDE(__bss_end = .);
	
	PROVIDE(__end = .);
}
