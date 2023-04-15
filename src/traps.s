 .section .text.trap


.macro kernel_entry
	addi sp, sp, -288

	sd x1,  8(sp)
	sd x3,  24(sp)
	sd x5,  40(sp)
	sd x6,  48(sp)
	sd x7,  56(sp)
	sd x8,  64(sp)
	sd x9,  72(sp)
	sd x10, 80(sp)
	sd x11, 88(sp)
	sd x12, 96(sp)
	sd x13, 104(sp)
	sd x14, 112(sp)
	sd x15, 120(sp)
	sd x16, 128(sp)
	sd x17, 136(sp)
	sd x18, 144(sp)
	sd x19, 152(sp)
	sd x20, 160(sp)
	sd x21, 168(sp)
	sd x22, 176(sp)
	sd x23, 184(sp)
	sd x24, 192(sp)
	sd x25, 200(sp)
	sd x26, 208(sp)
	sd x27, 216(sp)
	sd x28, 224(sp)
	sd x29, 232(sp)
	sd x30, 240(sp)
	sd x31, 248(sp)

	csrr s1, sstatus
	sd s1, 256(sp)

	/*保存sepc*/
	csrr s2, sepc
	sd s2, 0(sp)
	
	/*保存sbadaddr*/
	csrr s3, sbadaddr
	sd s3, 264(sp)

	/*保存scause*/
	csrr s4, scause
	sd s4, 272(sp)

	/*保存ssratch*/
	csrr s5, sscratch
	sd s5, 32(sp)

	/*保存SP*/
	addi s0, sp, 288 
	sd s0, 16(sp)
.endm

.macro kernel_exit
	ld a0, 272(sp)
	csrw sstatus, a0

	ld a2, 0(sp)
	csrw sepc, a2

	ld x1,  8(sp)
	ld x3,  24(sp)
	ld x4,  32(sp)
	ld x5,  40(sp)
	ld x6,  40(sp)
	ld x7,  56(sp)
	ld x8,  64(sp)
	ld x9,  72(sp)
	ld x10, 80(sp)
	ld x11, 88(sp)
	ld x12, 96(sp)
	ld x13, 104(sp)
	ld x14, 112(sp)
	ld x15, 120(sp)
	ld x16, 128(sp)
	ld x17, 136(sp)
	ld x18, 144(sp)
	ld x19, 152(sp)
	ld x20, 160(sp)
	ld x21, 168(sp)
	ld x22, 176(sp)
	ld x23, 184(sp)
	ld x24, 192(sp)
	ld x25, 200(sp)
	ld x26, 208(sp)
	ld x27, 216(sp)
	ld x28, 224(sp)
	ld x29, 232(sp)
	ld x30, 240(sp)
	ld x31, 248(sp)

	ld x2,  16(sp)
.endm

/*
   do_exception_vector必须4字节对齐
   否则写入stvec寄存器会不成功
 */
.align 2
.global do_exception_vector
do_exception_vector:
	kernel_entry

	csrw sscratch, x0

	la ra, ret_from_exception

	mv a0, sp /* pt_regs */
	mv a1, s4
	tail do_exception

ret_from_exception:

restore_all:
	kernel_exit
	sret