.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
	addi t0, x0, 1	
	blt a1, t0, error_length
	# Prologue
	addi sp, sp, -12
	sw ra, 0(sp)
	sw s0, 4(sp)
	sw s1, 8(sp)
	add s0, x0, a0 # store the pointer of arrary to s0
	add s1, x0, a1 # store the count of elements to s1
	jal x0, loop_start

loop_start:
	lw t0, 0(s0) # store the value of current node
	blt x0, t0, loop_continue # if current value greater 0 loop continue, otherwise, set value to 0, then loop continue
	sw x0, 0(s0)
	jal x0, loop_continue

loop_continue:
	addi s0, s0, 4
	addi s1, s1, -1
	beq s1, x0, loop_end # if count equals zero, loop end
	jal x0, loop_start

loop_end:
	# Epilogue
	lw ra, 0(sp)
	lw s0, 4(sp)
	lw s1, 8(sp)
	addi sp, sp, 12
	ret

error_length:
	li a0, 36
	j exit # immdiately terminate program, 'exit' is a special label that we can directly use it
