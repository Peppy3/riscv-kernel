/* ARCH = riscv64 */

#include <asm.h>
#include <vmem.h>

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
	csrw sscratch, zero
	la a5, arch_trap
	csrw stvec, a5

	/* 
	 * Disable floating point and vector code in the kernel and make the 
	 * operations trap.
	 */
	li a5, 0x6600
	csrw sstatus, a5

	/* set up initial stack pointer */
	la sp, __start
	addi sp, sp, -16

	// store a0 and a1 is "safe" registers
	mv s0, a0
	mv s1, a1

	// convert the dtb into our own tree (tbh, I don't know why we did this)
	mv a0, a1
	la a1, __end
	
	li a5, PAGE_SIZE
	add a1, a1, a5
	addi a5, a5, -1
	not a5, a5
	and a1, a1, a5

	call dt_convert_dtb

	//bnez a0, .Lcont_mem

	j boot_spin

.Lcont_mem:
	// Set up and enable Virtual Memory
	// returns new dtb location
	call kmem_init
	mv a1, a0
	mv a0, s0

	tail kmain
	.cfi_endproc

.global boot_spin
boot_spin:
	.cfi_startproc
	wfi
	j boot_spin
	.cfi_endproc
