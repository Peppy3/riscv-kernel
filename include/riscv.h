#ifndef RISCV_H
#define RISCV_H

// Supervisor Status Register, sstatus

#define SSTATUS_SPP (1L << 8)  // Previous mode, 1=Supervisor, 0=User
#define SSTATUS_SPIE (1L << 5) // Supervisor Previous Interrupt Enable
#define SSTATUS_UPIE (1L << 4) // User Previous Interrupt Enable
#define SSTATUS_SIE (1L << 1)  // Supervisor Interrupt Enable
#define SSTATUS_UIE (1L << 0)  // User Interrupt Enable

// returns what was last in SSTATUS.SIE
inline long interupts_off(void) {
	long status;
	// control register read and set immediate
	asm volatile ("csrrsi %0, sstatus, 0b00010" : "=r" (status));
	return (status & SSTATUS_SIE) >> 1;
}

inline void interupts_on(long status) {
	register long clobber;
	// control register read and clear
	asm volatile ("csrrc %0, sstatus, %1" 
			: "=r" (clobber)
			: "r" ((status << 1) & SSTATUS_SIE));
}

#endif /* RISCV_H */
