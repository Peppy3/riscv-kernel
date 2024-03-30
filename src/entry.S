/* ARCH = riscv64 */

#ifdef DEBUG
#define BSS_FILL 0x0A
#else
#define BSS_FILL 0x00
#endif

.section ".text.entry"

.option norvc

.type _entry, @function
.global _entry
_entry:
	csrw satp, zero
	
	li t0, BSS_FILL
	la t5, __bss_start
	la t6, __bss_end
1:
	sb t0, (t5)
	addi t5, t5, 1
	bltu t5, t6, 1b

	/* set up initial stack pointer */
	la sp, __boot_core_stack_end
	
	tail start

spin:
	j spin

.end
