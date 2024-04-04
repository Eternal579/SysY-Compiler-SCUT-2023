	.file "test.txt"
	.option pic
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.data
	.float1: .word 0x0
	.data
a: .word 0
sum: .word 0
count: .word 0
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
	.globl getA
	.type func, @function
getA:
	addi sp, sp, -352
	sd ra, 344(sp)
	sd s0, 336(sp)
	lla s0, count
	lw t4, 0(s0)
	lla s0, count
	sw t4, 0(s0)
	addiw t6, t4, 1
	mv a0, t6
	lla s0, count
	sw t6, 0(s0)
	j .mylabel1
.mylabel1:
	ld ra, 344(sp)
	ld s0, 336(sp)
	addi sp, sp, 352
	jr ra
	.size getA, .-getA
	.globl f1
	.type func, @function
f1:
	addi sp, sp, -352
	sd ra, 344(sp)
	sd s0, 336(sp)
	lla s0, sum
	lw t6, 0(s0)
	lla s0, sum
	sw t6, 0(s0)
	mv t5, a0
	sw t5, 332(sp)
	addw t3, t6, t5
	lla s0, sum
	sw t3, 0(s0)
	call getA
	mv t2, a0
	lla s0, sum
	lw t1, 0(s0)
	lla s0, sum
	sw t1, 0(s0)
	sw t2, 332(sp)
	addw t0, t1, t2
	lla s0, sum
	sw t0, 0(s0)
	call getA
	mv t2, a0
	lla s0, sum
	lw t4, 0(s0)
	lla s0, sum
	sw t4, 0(s0)
	sw t2, 328(sp)
	addw t6, t4, t2
	lla s0, sum
	sw t6, 0(s0)
	lw t2, 332(sp)
	sw t2, 332(sp)
	addw t4, t6, t2
	lla s0, sum
	sw t4, 0(s0)
	lw t6, 332(sp)
	sw t6, 332(sp)
	addw t1, t4, t6
.mylabel2:
	lla s0, sum
	sw t1, 0(s0)
	ld ra, 344(sp)
	ld s0, 336(sp)
	addi sp, sp, 352
	jr ra
	.size f1, .-f1
	.globl f2
	.type func, @function
f2:
	addi sp, sp, -352
	sd ra, 344(sp)
	sd s0, 336(sp)
	lla s0, sum
	lw t5, 0(s0)
	lla s0, sum
	sw t5, 0(s0)
	lla s0, a
	lw t4, 0(s0)
	lla s0, a
	sw t4, 0(s0)
	addw t6, t5, t4
	lla s0, sum
	sw t6, 0(s0)
	call getA
	mv t2, a0
	lla s0, sum
	lw t5, 0(s0)
	lla s0, sum
	sw t5, 0(s0)
	lla s0, a
	lw t2, 0(s0)
	lla s0, a
	sw t2, 0(s0)
	addw t4, t5, t2
.mylabel3:
	lla s0, sum
	sw t4, 0(s0)
	ld ra, 344(sp)
	ld s0, 336(sp)
	addi sp, sp, 352
	jr ra
	.size f2, .-f2
	.globl f3
	.type func, @function
f3:
	addi sp, sp, -352
	sd ra, 344(sp)
	sd s0, 336(sp)
	call getA
	mv t4, a0
	lla s0, sum
	lw t5, 0(s0)
	lla s0, sum
	sw t5, 0(s0)
	sw t4, 332(sp)
	addw t2, t5, t4
	lla s0, sum
	sw t2, 0(s0)
	call getA
	mv t5, a0
	lla s0, sum
	lw t4, 0(s0)
	lla s0, sum
	sw t4, 0(s0)
	sw t5, 332(sp)
	addw t3, t4, t5
	lla s0, sum
	sw t3, 0(s0)
	call getA
	mv t4, a0
	lla s0, sum
	lw t4, 0(s0)
	lla s0, sum
	sw t4, 0(s0)
	lw t5, 332(sp)
	sw t5, 332(sp)
	addw t3, t4, t5
.mylabel4:
	lla s0, sum
	sw t3, 0(s0)
	ld ra, 344(sp)
	ld s0, 336(sp)
	addi sp, sp, 352
	jr ra
	.size f3, .-f3
	.globl main
	.type func, @function
main:
	addi sp, sp, -368
	sd ra, 360(sp)
	sd s0, 352(sp)
	li t4, 0
	lla s0, sum
	sw t4, 0(s0)
	call getA
	mv t4, a0
	lla s0, a
	sw t4, 0(s0)
	lla s0, sum
	lw t4, 0(s0)
	lla s0, sum
	sw t4, 0(s0)
	lla s0, a
	lw t5, 0(s0)
	lla s0, a
	sw t5, 0(s0)
	addw t1, t4, t5
	lla s0, sum
	sw t1, 0(s0)
	call getA
	mv t4, a0
	sw t4, 348(sp)
	sw t4, 344(sp)
	lw a0, 344(sp)
	call f1
	call f2
	call f3
	lw t5, 348(sp)
	sw t5, 348(sp)
	sw t5, 340(sp)
	lw a0, 340(sp)
	call f1
	call f2
	call f3
	call getA
	mv t4, a0
	lw t5, 348(sp)
	sw t5, 348(sp)
	sw t5, 332(sp)
	lw a0, 332(sp)
	call f1
	call f2
	call f3
	call getA
	mv t5, a0
	sw t5, 328(sp)
	call getA
	mv t4, a0
	sw t4, 324(sp)
	sw t4, 320(sp)
	lw a0, 320(sp)
	call f1
	call f2
	call f3
	call getA
	mv t5, a0
	sw t5, 316(sp)
	sw t5, 312(sp)
	lw a0, 312(sp)
	call f1
	call f2
	call f3
	call getA
	mv t5, a0
	sw t5, 316(sp)
	sw t5, 308(sp)
	lw a0, 308(sp)
	call f1
	call f2
	call f3
	lw t4, 348(sp)
	sw t4, 348(sp)
	sw t4, 304(sp)
	lw a0, 304(sp)
	call f1
	call f2
	call f3
.mylabel6:
	li t4, 1
	beqz t4, .mylabel7
.mylabel8:
	li t5, 1
	beqz t5, .mylabel9
	li t5, 0
	sw t5, 300(sp)
.mylabel10:
	lw t4, 300(sp)
	sw t4, 300(sp)
	li t0, 3
	slt t5, t4, t0
	beqz t5, .mylabel11
.mylabel12:
	li t5, 1
	beqz t5, .mylabel13
	lw t4, 348(sp)
	sw t4, 348(sp)
	sw t4, 296(sp)
	lw a0, 296(sp)
	call f1
	call f2
	call f3
	j .mylabel13
	call getA
	mv t4, a0
	sw t4, 348(sp)
	j .mylabel12
.mylabel13:
	lw t6, 300(sp)
	sw t6, 300(sp)
	li t4, 1
	subw t0, t6, t4
	seqz t0, t0
	beqz t0, .mylabel15
	call getA
	mv t4, a0
	sw t4, 292(sp)
	sw t4, 288(sp)
	lw a0, 288(sp)
	call f1
	call f2
	call f3
	lw t5, 300(sp)
	sw t5, 300(sp)
	addiw t2, t5, 1
	sw t2, 300(sp)
	j .mylabel10
	j .mylabel14
.mylabel15:
	lw t5, 348(sp)
	sw t5, 348(sp)
	sw t5, 284(sp)
	lw a0, 284(sp)
	call f1
	call f2
	call f3
.mylabel14:
	call getA
	mv t2, a0
	lw t5, 300(sp)
	sw t5, 300(sp)
	addiw t6, t5, 1
	sw t2, 348(sp)
	sw t6, 300(sp)
	j .mylabel10
.mylabel11:
	j .mylabel9
	j .mylabel8
.mylabel9:
	j .mylabel7
	j .mylabel6
.mylabel7:
	lla s0, sum
	lw t5, 0(s0)
	lla s0, sum
	sw t5, 0(s0)
	sw t5, 280(sp)
	lw a0, 280(sp)
	call putint
	li t0, 0
	mv a0, t0
	j .mylabel5
.mylabel5:
	ld ra, 360(sp)
	ld s0, 352(sp)
	addi sp, sp, 368
	jr ra
	.size main, .-main
	.ident  "GCC: (Debian 12.2.0 - 10) 12.2.0"
	.section  .note.GNU-stack,"",@progbits
