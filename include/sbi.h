#ifndef SBI_H
#define SBI_H

#include <stddef.h>
#include <stdint.h>

#define SBI_DEBUG_CONSOLE 0x4442434E

#define SBI_IMPL_BBL 0L
#define SBI_IMPL_OPENSBI 1L
#define SBI_IMPL_XVISOR 2L
#define SBI_IMPL_KVM 3L
#define SBI_IMPL_RUSTSBI 4L
#define SBI_IMPL_DIOSIX 5L
#define SBI_IMPL_COFFER 6L
#define SBI_IMPL_XEN 7L
#define SBI_IMPL_POLARFIRE 8L

#define SBI_SUCCESS 0					// Completed successfully
#define SBI_ERR_FAILED -1				// Failed
#define SBI_ERR_NOT_SUPPORTED -2		// Not supported
#define SBI_ERR_INVALID_PARAM -3		// Invalid parameter(s)
#define SBI_ERR_DENIED -4				// Denied or not allowed
#define SBI_ERR_INVALID_ADDRESS -5		// Invalid address(s)
#define SBI_ERR_ALREADY_AVAILABLE -6	// Already available
#define SBI_ERR_ALREADY_STARTED -7		// Already started
#define SBI_ERR_ALREADY_STOPPED -8		// Already stopped
#define SBI_ERR_NO_SHMEM -9				// Shared memory not available

#define SBI_RESET_SHUTDOWN 0L
#define SBI_RESET_COLD_REBOOT 1L
#define SBI_RESET_WART_REBOOT 2L

#define SBI_RESET_NO_REASON 0L
#define SBI_RESET_SYSTEM_FAILURE 1L

#define HART_STARTED 0L
#define HART_STOPPED 1L
#define HART_START_PENDING 2L
#define HART_STOP_PENDING 3L
#define HART_SUSPENDED 4L
#define HART_SUSPEND_PENDING 5L
#define HART_RESUME_PENDING 6L

typedef struct {
	int64_t error;
	int64_t value;
} SBIret;

SBIret sbicall6(uint64_t eid, uint64_t fid,
		uint64_t arg1, uint64_t arg2, uint64_t arg3, 
		uint64_t arg4, uint64_t arg5, uint64_t arg6);
SBIret sbicall5(uint64_t eid, uint64_t fid,
		uint64_t arg1, uint64_t arg2, uint64_t arg3, 
		uint64_t arg4, uint64_t arg5);
SBIret sbicall4(uint64_t eid, uint64_t fid,
		uint64_t arg1, uint64_t arg2, uint64_t arg3, 
		uint64_t arg4);
SBIret sbicall3(uint64_t eid, uint64_t fid,
		uint64_t arg1, uint64_t arg2, uint64_t arg3);
SBIret sbicall2(uint64_t eid, uint64_t fid,
		uint64_t arg1, uint64_t arg2);
SBIret sbicall1(uint64_t eid, uint64_t fid,
		uint64_t arg1);
SBIret sbicall0(uint64_t eid, uint64_t fid);

// Base Extension (EID #0x10)
int64_t sbi_get_spec_version();
int64_t sbi_get_impl_id();
int64_t sbi_get_impl_version();
int64_t sbi_probe_extension(int64_t extension_id);
int64_t sbi_get_mvendorid();
int64_t sbi_get_marchid();
int64_t sbi_get_mimpid();

// Timer Extension (EID #0x54494D45)
SBIret sbi_set_timer(uint64_t stime_value);

// IPI Extension (EID #0x735049)
SBIret sbi_send_ipi(uint64_t hart_mask, uint64_t hart_mask_base);

// RFENCE Extension (EID #0x52464E43)
SBIret sbi_remote_fence_i(uint64_t hart_mask, uint64_t hart_mask_base);
SBIret sbi_remote_sfence_vma(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size);
SBIret sbi_remote_sfence_vma_asid(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size, uint64_t asid);
SBIret sbi_remote_hfence_gvma_vmid(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size, uint64_t vmid);
SBIret sbi_remote_hfence_gvma(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size);
SBIret sbi_remote_hfence_vvma_asid(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size, uint64_t asid);
SBIret sbi_remote_hfence_vvma(uint64_t hart_mask, uint64_t hart_mask_base,
		uint64_t start_addr, uint64_t size);

// Hart State Management Extension (EID #0x48534D)
SBIret sbi_hart_start(uint64_t hartid, uint64_t start_addr, uint64_t opaque);
SBIret sbi_hart_stop();
SBIret sbi_hart_get_status(uint64_t hartid);
SBIret sbi_hart_suspend(uint32_t suspend_type, uint64_t resume_addr, uint64_t opaque);

// System Reset Extension (EID #0x53525354)
SBIret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason);

//Performance Monitoring Unit Extension (EID #0x504D55
// TODO

// Debug Console Extension (EID #0x4442434E)
SBIret sbi_debug_console_write(uint64_t num_bytes,
		uint64_t base_addr_lo, uint64_t base_addr_hi);
SBIret sbi_debug_console_read(uint64_t num_bytes,
		uint64_t base_addr_lo, uint64_t base_addr_hi);
SBIret sbi_debug_console_write_byte(uint8_t byte);

// System Suspend Extension (EID #0x53555350)
SBIret sbi_system_suspend(uint32_t sleep_type,
		uint64_t resume_addr, uint64_t opaque);


#endif /* SBI_H */