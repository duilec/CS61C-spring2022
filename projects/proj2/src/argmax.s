.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
	addi t0, x0, 1	
	blt a1, t0, error_length
	# Prologue
	addi sp, sp, -12
	sw ra, 0(sp)
	sw s0, 4(sp)
	sw s1, 8(sp)
	add s0, x0, a0 # store the pointer of arrary to s0
	add s1, x0, x0 # store the count of elements to s1(but set 0)
	add t0, x0, x0 # the first index of greatest element: store 0 the index to t0
	jal x0, loop_start

loop_start:
	lw t1, 0(s0) # store the value of current node to t1
	slli t2, t0, 2 # get the bytes offset by index(t0)
	add t2, t2, a0 # get address + bytes offset
	lw, t2, 0(t2) # load the greatest value to t2
	bge t2, t1 loop_continue # if the greatest vlaue greater than or equals the current value, loop continue, otherwise, change the index, then loop continue
	add t0, x0, s1
	jal x0, loop_continue

loop_continue:
	addi s0, s0, 4
	addi s1, s1, 1
	beq s1, a1, loop_end # if count equals zero, loop end
	jal x0, loop_start
	
loop_end:
	add a0, x0, t0 # set the index as the return value
	# Epilogue
	lw ra, 0(sp)
	lw s0, 4(sp)
	lw s1, 8(sp)
	addi sp, sp, 12	
	ret

error_length:
	li a0, 36
	j exit # immdiately terminate program, 'exit' is a special label that we can directly use it
