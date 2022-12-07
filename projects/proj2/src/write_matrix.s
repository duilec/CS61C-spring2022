.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fwrite error or eof,
#     this function terminates the program with error code 30
# ==============================================================================
write_matrix:

	# Prologue
	addi sp sp -20
	sw ra 0(sp)
	sw s0 4(sp)
	sw s1 8(sp)
	sw s2 12(sp)
	sw s3 16(sp)

	
	# prepare calling fopen
	add s1 x0 a1
	add s2 x0 a2
	add s3 x0 a3
	addi a1 x0 1
	
	# open file with write-only(1)
	jal ra fopen
	# if ret -1 fopen with error
	addi t0 x0 -1
	beq a0 t0 error_fopen

	# store file descriptor to s0
	add s0 x0 a0

	# store the first value of matrix in stack in order use pointer in fwrite
	lw t0 0(s1)
	addi sp sp -4
	sw t0 0(sp)

	# store row to file
	# prepare calling fwrite
	add a0 x0 s0
	sw s2 0(s1) 
	add a1 x0 s1
	addi a2 x0 1
	addi a3 x0 4
	# call fwrite to write row
	addi sp sp -4
	sw a2 0(sp)
	jal ra fwrite
	lw a2 0(sp)
	addi sp sp 4
	# check result(a0)
	bne a0 a2 error_fwrite 

	# store column to file
	# prepare calling fwrite
	add a0 x0 s0
	sw s3 0(s1) 
	add a1 x0 s1
	addi a2 x0 1
	addi a3 x0 4
	# call fwrite to write column
	addi sp sp -4
	sw a2 0(sp)
	jal ra fwrite
	lw a2 0(sp)
	addi sp sp 4
	# check result(a0)
	bne a0 a2 error_fwrite 

	# restore the first value in matrix from stack
	lw t0 0(sp)
	addi sp sp 4
	sw t0 0(s1)

	# store matrix to file
	# prepare calling fwrite
	add a0 x0 s0 
	add a1 x0 s1
	mul a2 s2 s3
	addi a3 x0 4
	# call fwrite
	jal ra fwrite
	# check result(a0)
	mul t0 s2 s3
	bne a0 t0 error_fwrite

	# call fclose
	add a0 x0 s0
	jal ra fclose
	bne a0 x0 error_fclose
	
	# Epilogue
	lw ra 0(sp)
	lw s0 4(sp)
	lw s1 8(sp)
	lw s2 12(sp)
	lw s3 16(sp)
	addi sp sp 20

	ret

error_fopen:
	li a0 27
	j exit

error_fclose:
	li a0 28
	j exit

error_fwrite:
	li a0 30
	j exit
