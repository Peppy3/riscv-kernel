#include <stddef.h>
#include <stdint.h>

#include <sbi.h>

SBIret sbicall6(uint64_t eid, uint64_t fid,
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

	return (SBIret){error, value};
}

SBIret sbicall5(uint64_t eid, uint64_t fid,
		uint64_t arg1, uint64_t arg2, uint64_t arg3, 
		uint64_t arg4, uint64_t arg5) {
	int64_t error;
	uint64_t value;

	asm volatile (
			"mv a0, %2\n\t"
			"mv a1, %3\n\t"
			"mv a2, %4\n\t"
			"mv a3, %5\n\t"
			"mv a4, %6\n\t"
			"mv a6, %7\n\t"
			"mv a7, %8\n\t"
			"ecall\n\t"
			"mv %0, a0\n\t"
			"mv %1, a1\n\t"
			: "=r" (error), "=r" (value)
			: "r" (arg1), "r" (arg2), "r" (arg3),
			  "r" (arg4), "r" (arg5),
			  "r" (fid), "r" (eid)
			);

	return (SBIret){error, value};
}

SBIret sbicall4(uint64_t eid, uint64_t fid,
		uint64_t arg1, uint64_t arg2, uint64_t arg3, 
		uint64_t arg4) {
	int64_t error;
	uint64_t value;

	asm volatile (
			"mv a0, %2\n\t"
			"mv a1, %3\n\t"
			"mv a2, %4\n\t"
			"mv a3, %5\n\t"
			"mv a6, %6\n\t"
			"mv a7, %7\n\t"
			"ecall\n\t"
			"mv %0, a0\n\t"
			"mv %1, a1\n\t"
			: "=r" (error), "=r" (value)
			: "r" (arg1), "r" (arg2), "r" (arg3),
			  "r" (arg4),
			  "r" (fid), "r" (eid)
			);

	return (SBIret){error, value};
}

SBIret sbicall3(uint64_t eid, uint64_t fid,
		uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	int64_t error;
	uint64_t value;

	asm volatile (
			"mv a0, %2\n\t"
			"mv a1, %3\n\t"
			"mv a2, %4\n\t"
			"mv a6, %5\n\t"
			"mv a7, %6\n\t"
			"ecall\n\t"
			"mv %0, a0\n\t"
			"mv %1, a1\n\t"
			: "=r" (error), "=r" (value)
			: "r" (arg1), "r" (arg2), "r" (arg3),
			  "r" (fid), "r" (eid)
			);

	return (SBIret){error, value};
}

SBIret sbicall2(uint64_t eid, uint64_t fid,
		uint64_t arg1, uint64_t arg2) {
	int64_t error;
	uint64_t value;

	asm volatile (
			"mv a0, %2\n\t"
			"mv a1, %3\n\t"
			"mv a6, %4\n\t"
			"mv a7, %5\n\t"
			"ecall\n\t"
			"mv %0, a0\n\t"
			"mv %1, a1\n\t"
			: "=r" (error), "=r" (value)
			: "r" (arg1), "r" (arg2),
			  "r" (fid), "r" (eid)
			);

	return (SBIret){error, value};
}

SBIret sbicall1(uint64_t eid, uint64_t fid,
		uint64_t arg1) {
	int64_t error;
	uint64_t value;

	asm volatile (
			"mv a0, %2\n\t"
			"mv a6, %3\n\t"
			"mv a7, %4\n\t"
			"ecall\n\t"
			"mv %0, a0\n\t"
			"mv %1, a1\n\t"
			: "=r" (error), "=r" (value)
			: "r" (arg1),
			  "r" (fid), "r" (eid)
			);

	return (SBIret){error, value};
}

SBIret sbicall0(uint64_t eid, uint64_t fid) {
	int64_t error;
	uint64_t value;

	asm volatile (
			"mv a6, %2\n\t"
			"mv a7, %3\n\t"
			"ecall\n\t"
			"mv %0, a0\n\t"
			"mv %1, a1\n\t"
			: "=r" (error), "=r" (value)
			: "r" (fid), "r" (eid)
			);

	return (SBIret){error, value};
}

// Base Extension (EID #0x10)
// note: these functions cannot return error, so it is omitted
int64_t sbi_get_spec_version() {
	return sbicall0(0x10, 0).value;
};
int64_t sbi_get_impl_id() {
	return sbicall0(0x10, 1).value;
};
int64_t sbi_get_impl_version() {
	return sbicall0(0x10, 2).value;
};
int64_t sbi_probe_extension(int64_t extension_id) {
	return sbicall1(0x10, 3, extension_id).value;
};
int64_t sbi_get_mvendorid() {
	return sbicall0(0x10, 4).value;
};
int64_t sbi_get_marchid() {
	return sbicall0(0x10, 5).value;
};
int64_t sbi_get_mimpid() {
	return sbicall0(0x10, 6).value;
};

// Timer Extension (EID #0x54494D45)
SBIret sbi_set_timer(uint64_t stime_value) {
	return sbicall1(0x54494D45, 0, stime_value);
};

// IPI Extension (EID #0x735049)
SBIret sbi_send_ipi(uint64_t hart_mask, uint64_t hart_mask_base) {
	return sbicall2(0x735049, 0, hart_mask, hart_mask_base);
};

// RFENCE Extension (EID #0x52464E43)
SBIret sbi_remote_fence_i(uint64_t hart_mask, uint64_t hart_mask_base) {
	return sbicall2(0x52464E43, 0, hart_mask, hart_mask_base);
};
SBIret sbi_remote_sfence_vma(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size) {
	return sbicall4(0x52464E43, 1, hart_mask, hart_mask_base, start_addr, size);
};
SBIret sbi_remote_sfence_vma_asid(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size, uint64_t asid) {
	return sbicall5(0x52464E43, 2, hart_mask, hart_mask_base, start_addr, size, asid);
};
SBIret sbi_remote_hfence_gvma_vmid(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size, uint64_t vmid) {
	return sbicall5(0x52464E43, 3, hart_mask, hart_mask_base, start_addr, size, vmid);
};
SBIret sbi_remote_hfence_gvma(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size) {
	return sbicall4(0x52464E43, 4, hart_mask, hart_mask_base, start_addr, size);
};
SBIret sbi_remote_hfence_vvma_asid(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size, uint64_t asid) {
	return sbicall5(0x52464E43, 5, hart_mask, hart_mask_base, start_addr, size, asid);
};
SBIret sbi_remote_hfence_vvma(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size) {
	return sbicall4(0x52464E43, 6, hart_mask, hart_mask_base, start_addr, size);
};

// Hart State Management Extension (EID #0x48534D)
SBIret sbi_hart_start(uint64_t hartid, uint64_t start_addr, uint64_t opaque) {
	return sbicall3(0x48534D, 0, hartid, start_addr, opaque);
};
SBIret sbi_hart_stop() {
	return sbicall0(0x48534D, 1);
};
SBIret sbi_hart_get_status(uint64_t hartid) {
	return sbicall1(0x48534D, 2, hartid);
};
SBIret sbi_hart_suspend(uint32_t suspend_type, uint64_t resume_addr, uint64_t opaque) {
	return sbicall3(0x48534D, 3, suspend_type, resume_addr, opaque);
};

// System Reset Extension (EID #0x53525354)
SBIret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason) {
	return sbicall2(0x53525354, 0, reset_type, reset_reason);
};

//Performance Monitoring Unit Extension (EID #0x504D55
// TODO

// Debug Console Extension (EID #0x4442434E)
SBIret sbi_debug_console_write(uint64_t num_bytes,
		uint64_t base_addr_lo, uint64_t base_addr_hi) {
	return sbicall3(0x4442434E, 0, num_bytes, base_addr_lo, base_addr_hi);
};
SBIret sbi_debug_console_read(uint64_t num_bytes,
		uint64_t base_addr_lo, uint64_t base_addr_hi) {
	return sbicall3(0x4442434E, 1, num_bytes, base_addr_lo, base_addr_hi);
};
SBIret sbi_debug_console_write_byte(uint8_t byte) {
	return sbicall1(0x4442434E, 2, byte);
};

// System Suspend Extension (EID #0x53555350)
SBIret sbi_system_suspend(uint32_t sleep_type,
		uint64_t resume_addr, uint64_t opaque) {
	return sbicall3(0x53555350, 0, sleep_type, resume_addr, opaque);
};