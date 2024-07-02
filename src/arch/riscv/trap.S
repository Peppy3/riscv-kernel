// System trap vector. All traps go to here.
// sscratch CSR should be zero when comming from supervisor mode

.section ".text"

.align 4 
.global arch_trap
arch_trap:
	// swap sscratch and tp so we can safetly store away all of the other registers
	csrrw tp, sscratch, tp 

	//TODO: check if we're comming from supervisor or user mode
	//bneq zero, tp, ...

	sd zero, 0(tp) // for completeness
	sd ra, 8(tp)
	sd sp, 16(tp)
	sd gp, 24(tp)
	//sd tp, 32(tp)
	sd t0, 40(tp)
	sd t1, 48(tp)
	sd t2, 56(tp)
	sd s0, 64(tp)
	sd s1, 72(tp)
	sd a0, 80(tp)
	sd a1, 88(tp)
	sd a2, 96(tp)
	sd a3, 104(tp)
	sd a4, 112(tp)
	sd a5, 120(tp)
	sd a6, 128(tp)
	sd a7, 136(tp)
	sd s2, 144(tp)
	sd s3, 152(tp)
	sd s4, 160(tp)
	sd s5, 168(tp)
	sd s6, 176(tp)
	sd s7, 184(tp)
	sd s8, 192(tp)
	sd s9, 200(tp)
	sd s10, 208(tp)
	sd s11, 216(tp)
	sd t3, 224(tp)
	sd t4, 232(tp)
	sd t5, 240(tp)
	sd t6, 248(tp)

	csrrw tp, sscratch, tp
	sd tp, 32(tp)
	
	j boot_spin

