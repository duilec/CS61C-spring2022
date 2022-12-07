.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:
	addi t0, x0, 1
	blt a2, t0, error_length
	blt a3, t0, error_stride
	blt a4, t0, error_stride
	# Prologue
	addi sp, sp, -24
	sw ra, 0(sp)
	sw s0, 4(sp)
	sw s1, 8(sp)
	sw s2, 12(sp)
	sw s3, 16(sp)
	sw s4, 20(sp)
	add s0, x0, a0 # store the pointer of arr0 to s0
	add s1, x0, a1 # store the pointer of arr1 to s1
	add s2, x0, x0 # store the count of array to s2, we firstly set 0
	slli s3, a3, 2 # convert the stride of arr0 to stride of bytes offset, then store to s3
	slli s4, a4, 2 # convert the stride of arr1 to stride of bytes offset, then store to s4
	add a0, x0, x0 # set result to 0
	jal x0, loop_start

loop_start:
	# if count euqals the number of array, loop end, otherwise, loop continue
	beq s2, a2, loop_end
	# get the value0 in arr0
	mul t0, s3, s2 # get the bytes offset
	add t0, t0, s0 # get address + byte offset
	lw t0, 0(t0) # get the value0
	# get the value1 in arr1
	mul t1, s4, s2 # get the bytes offset
	add t1, t1, s1 # get address + byte offset
	lw t1, 0(t1) # get the value1
	# value = value0 * value2
	mul t2, t0, t1
	# add value to result
	add a0, a0, t2
	# goto next loop
	addi s2, s2, 1
	jal x0, loop_start

loop_end:
	# Epilogue
	lw ra, 0(sp)
	lw s0, 4(sp)
	lw s1, 8(sp)
	lw s2, 12(sp)
	lw s3, 16(sp)
	lw s4, 20(sp)
	addi sp, sp, 24
	ret

error_length:
	li a0, 36
	j exit # immdiately terminate program, 'exit' is a special label that we can directly use it
	
error_stride:
	li a0, 37
	j exit # immdiately terminate program, 'exit' is a special label that we can directly use it
