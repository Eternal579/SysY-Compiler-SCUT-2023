	.file "test.txt"
	.option pic
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.data
	.float1: .word 0x0
	.data
N: .word 5
	.text
	.globl scut_memset_int
	.type func, @function
scut_memset_int:
	ble a1,zero,.L1
	slli a2,a1,2
	li a1,0
	tail memset
.L1:
	ret
	.size scut_memset_int, .-scut_memset_int
	.globl scut_memset_float
	.type func, @function
scut_memset_float:
	ble a1,zero,.L2
	slli a2,a1,2
	li a1,0
	tail memset
.L2:
	ret
	.size scut_memset_float, .-scut_memset_float
	.globl main
	.type func, @function
main:
	addi sp, sp, -368
	sd ra, 360(sp)
	sd s0, 352(sp)
	sd s1, 344(sp)
	sd s2, 336(sp)
	sd s3, 328(sp)
	sd s4, 320(sp)
	sd s5, 312(sp)
	sd s6, 304(sp)
	li t0, 0
	lla s0, N
	lw t3, 0(s0)
	slt t2, t0, t3
	lla s0, N
	sw t3, 0(s0)
	sw t0, 280(sp)
	beqz t2, .mylabel2
	li t4, 0
	lw t6, 280(sp)
	mv t2, t6
	slli t0, t2, 2
	li s0, 284
	add t0, t0, s0
	add t0, t0, sp
	sw t4, 0(t0)
	sd t4, 268(sp)
	lla s0, N
	lw t3, 0(s0)
	sw t0, 264(sp)
	ld a0, 268(sp)
	lw a1, 264(sp)
	lla s0, N
	sw t3, 0(s0)
	sw t6, 280(sp)
	call scut_memset_int
.mylabel2:
	li t0, 5
	sw t0, 260(sp)
	li t4, 284
	add t4, t4, sp
	sd t4, 252(sp)
	lw a0, 260(sp)
	ld a1, 252(sp)
	call putarray
	li t6, 0
	mv a0, t6
	j .mylabel1
.mylabel1:
	ld ra, 360(sp)
	ld s0, 352(sp)
	ld s1, 344(sp)
	ld s2, 336(sp)
	ld s3, 328(sp)
	ld s4, 320(sp)
	ld s5, 312(sp)
	ld s6, 304(sp)
	addi sp, sp, 368
	jr ra
	.size main, .-main
	.ident  "GCC: (Debian 12.2.0 - 10) 12.2.0"
	.section  .note.GNU-stack,"",@progbits
