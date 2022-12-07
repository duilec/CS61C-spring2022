.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
#   - If malloc returns an error,
#     this function terminates the program with error code 26
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fread error or eof,
#     this function terminates the program with error code 29
# ==============================================================================
read_matrix:

	# Prologue
	addi sp sp -16
	sw ra, 0(sp)
	sw s0, 4(sp)
	sw s1, 8(sp)
	sw s2, 12(sp)
	
	# prepare calling fopen
	add s0, x0, a0
	add s1, x0, a1
	add s2, x0, a2
	add a1, x0, x0
	
	# open file with read-only(0)
	jal ra, fopen
	# if ret -1, fopen with error
	addi t0, x0, -1
	beq a0, t0, error_fopen

	# qestion: how to know the position of begnning when calling fread?
	# answer: the position of begnning changing with fread calling 
	# fread: Read bytes from a file to a buffer in memory. 
	# note: Subsequent reads will read from later parts of the file.

	# store length of row to memory
	# prepare calling fread
	add s0, x0, a0
	add a1, x0, s1
	addi a2, x0, 4
	# call fread: store length of row to memory
	addi sp, sp, -4
	sw a2, 0(sp)
	jal ra, fread	
	lw a2, 0(sp)
	addi sp, sp, 4
	bne a0, a2, error_fread

	# store length of column to memory
	# prepare calling fread
	add a0, x0, s0 
	add a1, x0, s2
	addi a2, x0, 4
	# call fread: store length of column to memory
	addi sp, sp, -4
	sw a2, 0(sp)
	jal ra, fread	
	lw a2, 0(sp)
	addi sp, sp, 4
	bne a0, a2, error_fread

	# allocate memory to store matrix	
	lw t1, 0(s1)	
	lw t2, 0(s2)	
	mul a0, t1, t2
    slli a0 a0 2 # get how many bytes to allocate
    jal malloc
    beqz a0 error_malloc # exit if malloc failed

	# store matrix to memory
	# prepare calling fread
	add a1, x0, a0
	add a0, x0, s0 
	lw t1, 0(s1)	
	lw t2, 0(s2)	
	mul a2, t1, t2
    slli a2, a2, 2 # get how many bytes to read
	# store result pointer to s1
	add s1, x0, a1
	# call fread: store matrix to memory
	addi sp, sp, -4
	sw a2, 0(sp)
	jal ra, fread	
	lw a2, 0(sp)
	addi sp, sp, 4
	bne a0, a2, error_fread

	# close file
	add a0, x0, s0
	jal ra, fclose
	bne a0, x0, error_fclose

	# store result pointer to a0
	add a0, x0, s1
	# Epilogue
	lw ra, 0(sp)
	lw s0, 4(sp)
	lw s1, 8(sp)
	lw s2, 12(sp)
	addi sp sp 16
	ret

error_malloc:
	li a0, 26
	j exit

error_fopen:
	li a0, 27
	j exit

error_fclose:
	li a0, 28
	j exit

error_fread:
	li a0, 29
	j exit
