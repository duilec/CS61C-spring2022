.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   d = matmul(m0, m1)
# Arguments:
#   a0 (int*)  is the pointer to the start of m0
#   a1 (int)   is the # of rows (height) of m0
#   a2 (int)   is the # of columns (width) of m0
#   a3 (int*)  is the pointer to the start of m1
#   a4 (int)   is the # of rows (height) of m1
#   a5 (int)   is the # of columns (width) of m1
#   a6 (int*)  is the pointer to the the start of d
# Returns:
#   None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 38
# =======================================================
matmul:
	# Error checks
	addi t0, x0, 1
	blt a1, t0, error_matrix
	blt a2, t0, error_matrix
	blt a4, t0, error_matrix
	blt a5, t0, error_matrix
	bne a2, a4, error_matrix
	# Prologue
	addi sp, sp, -20
	sw, ra, 0(sp)	
	sw, s0, 4(sp)
	sw, s1, 8(sp)
	sw, s2, 12(sp)
	sw, s3, 16(sp)
	# prepare loop
	add s0, x0, x0 # set m0 count to 0
	add s1, x0, x0 # set m1 count to 0
	add s2, x0, x0 # set md count to 0
	# after calling dot, t0-t6 and a1-a7 will contain garbage even function of dot NOT use some registers,
	# so, a6(address of md) need to store to s3 in order to access md
	add s3, x0, a6 # store the address of md to s3
	jal x0, outer_loop_start

outer_loop_start:
	beq s0, a1, outer_loop_end
	slli t0, a2, 2 # get bytes offset step1 
	mul t0, t0, s0 # get bytes offset step2 (m0_count * m0_width * 4bytes)
	add t1, a0, t0 # get address + bytes offset
	jal x0, inner_loop_start

inner_loop_start:
	beq s1, a5, inner_loop_end
	slli t2, s1, 2 # get bytes offset (m1_count * 1 * 4bytes)
	add t3, a3, t2 # get address + bytes offset
	#a0-a4, t0-t2 storing
	addi sp, sp -40
	sw a0, 0(sp)
	sw a1, 4(sp)
	sw a2, 8(sp)
	sw a3, 12(sp)
	sw a4, 16(sp)
	sw a5, 20(sp)
	sw t0, 24(sp)
	sw t1, 28(sp)
	sw t2, 32(sp)
	sw t3, 36(sp)

	# prepare calling dot
	add a0, x0, t1 # A pointer to the start of the first array
	add a1, x0, t3 # A pointer to the start of the second array
	add a2, x0, a2 # The number of elements to use in the calculation
	addi a3, x0, 1 # The stride of the first array
	add a4, x0, a5 # The stride of the second array
	# call dot
	jal ra, dot
	# store return value(a0) to destination matrix
	slli t4, s2, 2 # get bytes offset (md_count * 4bytes)
	add t5, s3, t4 # get address + bytes offset
	sw a0, 0(t5)
	# increment md count
	addi s2, s2, 1 

	#a0-a4, t0-t2 restoring
	lw a0, 0(sp)
	lw a1, 4(sp)
	lw a2, 8(sp)
	lw a3, 12(sp)
	lw a4, 16(sp)
	lw a5, 20(sp)
	lw t0, 24(sp)
	lw t1, 28(sp)
	lw t2, 32(sp)
	lw t3, 36(sp)
	addi sp, sp 40

	# increment m1 count
	addi s1, s1, 1
	jal x0, inner_loop_start

inner_loop_end:
	# reset m1 count
	add s1, x0, x0 	
	# increment m0 count
	addi s0, s0, 1 
	jal x0, outer_loop_start

outer_loop_end:
	# Epilogue
	lw, ra, 0(sp)	
	lw, s0, 4(sp)
	lw, s1, 8(sp)
	lw, s2, 12(sp)
	lw, s3, 16(sp)
	addi sp, sp, 20
	ret

error_matrix:
	li a0, 38
	j exit # immdiately terminate program, 'exit' is a special label that we can directly use it
