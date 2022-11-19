#include <stdio.h>
#include "bit_ops.h"

/* Returns the Nth bit of X. Assumes 0 <= N <= 31. */
unsigned get_bit(unsigned x, unsigned n) {
	return (x >> n) & 1;
}

/* Set the nth bit of the value of x to v. Assumes 0 <= N <= 31, and V is 0 or 1 */
void set_bit(unsigned *x, unsigned n, unsigned v) {
	// set nth bit to 0 in *x
	*x = *x & (~(1 << n));
	// set nth bit to v in *x
	*x = *x | (v << n);
}

/* Flips the Nth bit in X. Assumes 0 <= N <= 31.*/
void flip_bit(unsigned *x, unsigned n) {
	// get the value of nth bit in *x, then use xor(^1) to flip it
	unsigned flip_v = ((*x >> n) & 1) ^ 1;
	set_bit(x, n, flip_v);
}

