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

.dword 0x400000 		// text offset (above where SBI and U-Boot are loaded)

.dword __end - __start 	// effective image size
.dword 0 				// flags (0 = Little Endian)
.word 2					// version
.word 0 				// res1
.dword 0 				// res2
.dword 0x5643534952		// magic1 (deprecated)
.word 0x05435352		// magic2 
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

	// zero all registers exept for a0 and a1
	
	li ra, 0
	/* li sp, 0 */
	li gp, 0
	li tp, 0
	li t0, 0
	li t1, 0
	li t2, 0
	li s0, 0 /* s0/fp */
	li s1, 0
	/* li a0, 0 */
	/* li a1, 0 */
	li a2, 0
	li a3, 0
	li a4, 0
	li a5, 0
	li a6, 0
	li a7, 0
	li s2, 0
	li s3, 0
	li s4, 0
	li s5, 0
	li s6, 0
	li s7, 0
	li s8, 0
	li s9, 0
	li s10, 0
	li s11, 0
	li t3, 0
	li t4, 0
	li t5, 0
	li t6, 0

	tail start

spin:
	j spin

.end
