/* ARCH = riscv64 */

#ifdef DEBUG
#define BSS_FILL 0x0A
#else
#define BSS_FILL 0x00
#endif

.section ".text.header"

// linux kernel image header for booting with u-boot's 'booti' command
// read more at: https://www.kernel.org/doc/html/latest/arch/riscv/boot-image-header.html
// and: https://www.kernel.org/doc/html/latest/arch/riscv/boot.html
linux_header:

.option push
.option norvc
	j _entry	// u32 code 0
	j _entry	// u32 code 1
.option pop

.dword 0x400000 		// text offset (above where SBI and U-Boot is loaded)

.dword __end - __start 	// effective image size
.dword 0 				// flags (0 = Little Endian)
.word 2					// version
.word 0 				// res1
.dword 0 				// res2
.ascii "RISCV\0\0\0"	// magic1 (deprecated)
.ascii "RSC\x5"			// magic2 
.word 0 				// res3 reserved for PE COFF offset



.section ".text"
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
