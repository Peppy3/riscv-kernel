#include <stddef.h>
#include <stdint.h>

#define SBI_DEBUG_CONSOLE 0x4442434E

struct sbiret {
	int64_t error;
	int64_t value;
}

inline struct sbiret sbicall6(uint64_t eid, uint64_t fid,
		uint64_t arg1, uint64_t arg2, uint64_t arg3, 
		uint64_t arg4, uint64_t arg5, uint64_t arg6) {
	int64_t error;
	uint64_t value;

	asm volatile (
			"mv a0, %2\n\t"
			"mv a1, %3\n\t"
			"mv a2, %4\n\t"
			"mv a3, %5\n\t"
			"mv a4, %6\n\t"
			"mv a5, %7\n\t"
			"mv a6, %8\n\t"
			"mv a7, %9\n\t"
			"ecall\n\t"
			"mv %0, a0\n\t"
			"mv %1, a1\n\t"
			: "=r" (error), "=r" (value)
			: "r" (arg1), "r" (arg2), "r" (arg3),
			  "r" (arg4), "r" (arg5), "r" (arg6),
			  "r" (fid), "r" (eid)
			);

	return (struct sbiret){error, value};
}

void start(void) {



	while (1);
}
