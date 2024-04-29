#include <stddef.h>
#include <stdint.h>

#include <sbi.h>

SBIret sbicall6(unsigned long eid, unsigned long fid,
		unsigned long arg1, unsigned long arg2, unsigned long arg3, 
		unsigned long arg4, unsigned long arg5, unsigned long arg6) {
	register unsigned long a0 asm ("a0") = arg1;
	register unsigned long a1 asm ("a1") = arg2;
	register unsigned long a2 asm ("a2") = arg3;
	register unsigned long a3 asm ("a3") = arg4;
	register unsigned long a4 asm ("a4") = arg5;
	register unsigned long a5 asm ("a4") = arg6;

	register unsigned long a6 asm ("a6") = fid;
	register unsigned long a7 asm ("a7") = eid;

	asm volatile (
			"ecall"
			: "+r" (a0), "+r" (a1)
			: "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4), "r" (a5),
			"r" (a6), "r" (a7)
			: "memory"
			);

	return (SBIret){.error = a0, .value = a1};
}

SBIret sbicall5(unsigned long eid, unsigned long fid,
		unsigned long arg1, unsigned long arg2, unsigned long arg3, 
		unsigned long arg4, unsigned long arg5) {
	register unsigned long a0 asm ("a0") = arg1;
	register unsigned long a1 asm ("a1") = arg2;
	register unsigned long a2 asm ("a2") = arg3;
	register unsigned long a3 asm ("a3") = arg4;
	register unsigned long a4 asm ("a4") = arg5;

	register unsigned long a6 asm ("a6") = fid;
	register unsigned long a7 asm ("a7") = eid;

	asm volatile (
			"ecall"
			: "+r" (a0), "+r" (a1)
			: "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4),
			"r" (a6), "r" (a7)
			: "memory"
			);

	return (SBIret){.error = a0, .value = a1};
}

SBIret sbicall4(unsigned long eid, unsigned long fid,
		unsigned long arg1, unsigned long arg2, unsigned long arg3, 
		unsigned long arg4) {
	register unsigned long a0 asm ("a0") = arg1;
	register unsigned long a1 asm ("a1") = arg2;
	register unsigned long a2 asm ("a2") = arg3;
	register unsigned long a3 asm ("a3") = arg4;

	register unsigned long a6 asm ("a6") = fid;
	register unsigned long a7 asm ("a7") = eid;

	asm volatile (
			"ecall"
			: "+r" (a0), "+r" (a1)
			: "r" (a0), "r" (a1), "r" (a2), "r" (a3), 
			"r" (a6), "r" (a7)
			: "memory"
			);

	return (SBIret){.error = a0, .value = a1};
}

SBIret sbicall3(unsigned long eid, unsigned long fid,
		unsigned long arg1, unsigned long arg2, unsigned long arg3) {
	register unsigned long a0 asm ("a0") = arg1;
	register unsigned long a1 asm ("a1") = arg2;
	register unsigned long a2 asm ("a2") = arg3;
	
	register unsigned long a6 asm ("a6") = fid;
	register unsigned long a7 asm ("a7") = eid;

	asm volatile (
			"ecall"
			: "+r" (a0), "+r" (a1)
			: "r" (a0), "r" (a1), "r" (a2),
			"r" (a6), "r" (a7)
			: "memory"
			);

	return (SBIret){.error = a0, .value = a1};
}

SBIret sbicall2(unsigned long eid, unsigned long fid,
		unsigned long arg1, unsigned long arg2) {
	register unsigned long a0 asm ("a0") = arg1;
	register unsigned long a1 asm ("a1") = arg2;

	register unsigned long a6 asm ("a6") = fid;
	register unsigned long a7 asm ("a7") = eid;

	asm volatile (
			"ecall"
			: "+r" (a0), "+r" (a1)
			: "r" (a0), "r" (a1), 
			"r" (a6), "r" (a7)
			: "memory"
			);


	return (SBIret){.error = a0, .value = a1};
}

SBIret sbicall1(unsigned long eid, unsigned long fid, unsigned long arg1) {
	register unsigned long a0 asm ("a0") = arg1;
	register unsigned long a1 asm ("a1");

	register unsigned long a6 asm ("a6") = fid;
	register unsigned long a7 asm ("a7") = eid;

	asm volatile (
			"ecall"
			: "+r" (a0), "+r" (a1)
			: "r" (a0), 
			"r" (a6), "r" (a7)
			: "memory"
			);

	return (SBIret){.error = a0, .value = a1};
}

SBIret sbicall0(unsigned long eid, unsigned long fid) {
	register long a0 asm ("a0");
	register long a1 asm ("a1");

	register unsigned long a6 asm ("a6") = fid;
	register unsigned long a7 asm ("a7") = eid;

	asm volatile (
			"ecall"
			: "+r" (a0), "+r" (a1)
			: "r" (a6), "r" (a7)
			: "memory"
			);

	return (SBIret){.error = a0, .value = a1};
}

long sbicall4_legacy(unsigned long eid, 
		unsigned long arg1, unsigned long arg2, unsigned long arg3,
		unsigned long arg4) {
	register unsigned long a0 asm("a0") = arg1;
	register unsigned long a1 asm("a1") = arg1;
	register unsigned long a2 asm("a2") = arg2;
	register unsigned long a3 asm("a3") = arg3;
	register unsigned long a4 asm("a4") = arg4;
	register unsigned long a7 asm("a7") = eid;
	asm volatile ("ecall"
			: "+r" (a0)
			: "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4), "r" (a7)
			: "memory");

	return a0;
}
long sbicall3_legacy(unsigned long eid, 
		unsigned long arg1, unsigned long arg2, unsigned long arg3) {
	register unsigned long a0 asm("a0") = arg1;
	register unsigned long a1 asm("a1") = arg1;
	register unsigned long a2 asm("a2") = arg2;
	register unsigned long a3 asm("a3") = arg3;
	register unsigned long a7 asm("a7") = eid;
	asm volatile ("ecall"
			: "+r" (a0)
			: "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a7)
			: "memory");

	return a0;
}
long sbicall1_legacy(unsigned long eid, unsigned long arg1) {
	register unsigned long a0 asm("a0") = arg1;
	register unsigned long a7 asm("a7") = eid;
	asm volatile ("ecall"
			: "+r" (a0)
			: "r" (a0), "r" (a7)
			: "memory");

	return a0;
}

long sbicall0_legacy(unsigned long eid) {
	register long a0 asm("a0");
	register unsigned long a7 asm("a7") = eid;
	asm volatile ("ecall"
			: "=r" (a0)
			: "r" (a7)
			: "memory");

	return a0;
}

// DEPRECATED, use other extentions if supported
long sbi_legacy_set_timer(unsigned long long stime_val) {
	return sbicall1_legacy(0x00, (unsigned long)stime_val);
}
long sbi_legacy_putchar(int ch) {
	return sbicall1_legacy(0x01, (unsigned long)ch);
}
long sbi_legacy_getchar(void) {
	return sbicall0_legacy(0x02);
}
long sbi_legacy_clear_ipi(void) {
	return sbicall0_legacy(0x03);
}
long sbi_legacy_send_ipi(const unsigned long *hart_mask) {
	return sbicall1_legacy(0x04, (unsigned long)hart_mask);
}
long sbi_legacy_remote_fence_i(const unsigned long *hart_mask) {
	return sbicall1_legacy(0x05, (unsigned long)hart_mask);
}
long sbi_legacy_remote_sfence_vma(const unsigned long *hart_mask, 
		unsigned long start, unsigned long size) {
	return sbicall3_legacy(0x06, (unsigned long)hart_mask, start, size);
}
long sbi_legacy_remote_sfence_vma_asid(const unsigned long *hart_mask,
		unsigned long start, unsigned long size, unsigned long asid) {
	return sbicall4_legacy(0x07, (unsigned long)hart_mask, start, size, asid);
}
void sbi_legacy_shutdown(void) {
	(void)sbicall0_legacy(0x08);
	return;
}

// Base Extension (EID #0x10)
// note: these functions cannot return error, so it is omitted
int64_t sbi_get_spec_version() {
	return sbicall0(SBI_EXT_BASE, 0).value;
}
int64_t sbi_get_impl_id() {
	return sbicall0(SBI_EXT_BASE, 1).value;
}
int64_t sbi_get_impl_version() {
	return sbicall0(SBI_EXT_BASE, 2).value;
}
int64_t sbi_probe_extension(int64_t extension_id) {
	return sbicall1(SBI_EXT_BASE, 3, extension_id).value;
}
int64_t sbi_get_mvendorid() {
	return sbicall0(SBI_EXT_BASE, 4).value;
}
int64_t sbi_get_marchid() {
	return sbicall0(SBI_EXT_BASE, 5).value;
}
int64_t sbi_get_mimpid() {
	return sbicall0(SBI_EXT_BASE, 6).value;
}

// Timer Extension (EID #0x54494D45)
SBIret sbi_set_timer(uint64_t stime_value) {
	return sbicall1(SBI_EXT_TIMER, 0, stime_value);
}

// IPI Extension (EID #0x735049)
SBIret sbi_send_ipi(uint64_t hart_mask, uint64_t hart_mask_base) {
	return sbicall2(SBI_EXT_IPI, 0, hart_mask, hart_mask_base);
}

// RFENCE Extension (EID #0x52464E43)
SBIret sbi_remote_fence_i(uint64_t hart_mask, uint64_t hart_mask_base) {
	return sbicall2(SBI_EXT_RFENCE, 0, hart_mask, hart_mask_base);
}
SBIret sbi_remote_sfence_vma(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size) {
	return sbicall4(SBI_EXT_RFENCE, 1, hart_mask, hart_mask_base, start_addr, size);
}
SBIret sbi_remote_sfence_vma_asid(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size, uint64_t asid) {
	return sbicall5(SBI_EXT_RFENCE, 2, hart_mask, hart_mask_base, start_addr, size, asid);
}
SBIret sbi_remote_hfence_gvma_vmid(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size, uint64_t vmid) {
	return sbicall5(SBI_EXT_RFENCE, 3, hart_mask, hart_mask_base, start_addr, size, vmid);
}
SBIret sbi_remote_hfence_gvma(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size) {
	return sbicall4(SBI_EXT_RFENCE, 4, hart_mask, hart_mask_base, start_addr, size);
}
SBIret sbi_remote_hfence_vvma_asid(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size, uint64_t asid) {
	return sbicall5(SBI_EXT_RFENCE, 5, hart_mask, hart_mask_base, start_addr, size, asid);
}
SBIret sbi_remote_hfence_vvma(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size) {
	return sbicall4(SBI_EXT_RFENCE, 6, hart_mask, hart_mask_base, start_addr, size);
}

// Hart State Management Extension (EID #0x48534D)
SBIret sbi_hart_start(uint64_t hartid, uint64_t start_addr, uint64_t opaque) {
	return sbicall3(SBI_EXT_HSM, 0, hartid, start_addr, opaque);
}
SBIret sbi_hart_stop() {
	return sbicall0(SBI_EXT_HSM, 1);
}
SBIret sbi_hart_get_status(uint64_t hartid) {
	return sbicall1(SBI_EXT_HSM, 2, hartid);
}
SBIret sbi_hart_suspend(uint32_t suspend_type, uint64_t resume_addr, uint64_t opaque) {
	return sbicall3(SBI_EXT_HSM, 3, suspend_type, resume_addr, opaque);
}

// System Reset Extension (EID #0x53525354)
SBIret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason) {
	return sbicall2(SBI_EXT_RESET, 0, reset_type, reset_reason);
}

//Performance Monitoring Unit Extension (EID #0x504D55
// TODO

// Debug Console Extension (EID #0x4442434E)
SBIret sbi_debug_console_write(uint64_t num_bytes,
		uint64_t base_addr_lo, uint64_t base_addr_hi) {
	return sbicall3(SBI_EXT_DEBUG, 0, num_bytes, base_addr_lo, base_addr_hi);
}
SBIret sbi_debug_console_read(uint64_t num_bytes,
		uint64_t base_addr_lo, uint64_t base_addr_hi) {
	return sbicall3(SBI_EXT_DEBUG, 1, num_bytes, base_addr_lo, base_addr_hi);
}
SBIret sbi_debug_console_write_byte(uint8_t byte) {
	return sbicall1(SBI_EXT_DEBUG, 2, byte);
}

// System Suspend Extension (EID #0x53555350)
SBIret sbi_system_suspend(uint32_t sleep_type,
		uint64_t resume_addr, uint64_t opaque) {
	return sbicall3(SBI_EXT_SUSPEND, 0, sleep_type, resume_addr, opaque);
}
