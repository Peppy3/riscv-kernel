/* ARCH = riscv64 */

#include <asm.h>

#ifdef DEBUG
#define BSS_FILL 0x0A
#else
#define BSS_FILL 0x00
#endif

.section ".boot.header"

// linux kernel image header for booting with u-boot's 'booti' command
// read more at: https://www.kernel.org/doc/html/latest/arch/riscv/boot-image-header.html
linux_header:

.option push
.option norvc
	j _start // u32 code 0
	j _start // u32 code 1
.option pop

.dword 0x400000 		// text offset (above where SBI and U-Boot are loaded)

.dword __end - __start  // effective image size
.dword 0 				// flags (0 = Little Endian)
.word 2					// version
.dword 0   				// res1
.word 0 				  // res2
.ascii RISCV_IMAGE_MAGIC  // magic1 (deprecated)
.ascii RISCV_IMAGE_MAGIC2 // magic2 
.word 0 				// res3 reserved for PE COFF offset

.align 2

/* 
 * Initial entry point. 
 * a0 = boot hartid
 * a1 = dtb file
 */
.section ".boot.entry"
.type _start, @function
.global _start
_start:
	.cfi_startproc
	csrw sie, zero
	csrw sip, zero

	/* 
	 * Disable floating point and vector code in the kernel and make the 
	 * operations trap.
	 */
	//li t0, 0x6600
	//csrw sstatus,t0 

	/* set up initial stack pointer */
	la sp, __boot_core_stack_bottom
	addi sp, sp, -16

	// store a0 and a1 is "safe" registers
	mv s0, a0
	mv s1, a1

	/* Set up and enable VM */
	call kmem_init
	
	//j boot_spin
	mv a0, s0
	mv a1, s1

	tail kmain
	.cfi_endproc

boot_spin:
	.cfi_startproc
	wfi
	j boot_spin
	.cfi_endproc
