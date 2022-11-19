/* Include the system headers we need */
#include <stdlib.h>
#include <stdio.h>

/* Include our header */
#include "vector.h"

/* Define what our struct is */
struct vector_t {
    size_t size;
    int *data;
};

/* Utility function to handle allocation failures. In this
   case we print a message and exit. */
static void allocation_failed() {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
}

/* Bad example of how to create a new vector */
/* return a pointer(address) of vector_t in stack that maybe be overwritted by other progress */
/* Answer: The vector is created on the stack, instead of the heap. 
   All memory stored on the stack gets freed as soon as that function finishes running, 
   so when the function returns, we lose the vector we constructed. */
vector_t *bad_vector_new() {
    /* Create the vector and a pointer to it */
    vector_t *retval, v;
    retval = &v;

    /* Initialize attributes */
    retval->size = 1;
    retval->data = malloc(sizeof(int));
    if (retval->data == NULL) {
        allocation_failed();
    }

    retval->data[0] = 0;
    return retval;
}

/* Another suboptimal way of creating a vector */
/* Answer: While this does work, it is rather inefficient.
   The vector object is a "Big" thing, so when we return it,
   we need to copy a lot of data from the return value to the calling frame.
   Instead, it is generally better practice to store structs on the heap, 
   and pass around pointers, as pointers are a fixed, "Small" size. */
vector_t also_bad_vector_new() {
    /* Create the vector */
    vector_t v;

    /* Initialize attributes */
    v.size = 1;
    v.data = malloc(sizeof(int));
    if (v.data == NULL) {
        allocation_failed();
    }
    v.data[0] = 0;
    return v;
}

/* Create a new vector with a size (length) of 1 and set its single component to zero... the
   right way */
vector_t *vector_new() {
	/* Declare what this function will return */
	vector_t *retval;

    /* First, we need to allocate memory on the heap for the struct */
	/* note: malloc() will return a address */
    /* note: use sizeof(vector_t) NOT sizeof(vector_t*),
	   becasue sizeof(vector_t*) == 4 BUT sizeof(vector_t) Maybe != 4 */
	retval = (vector_t*)malloc(sizeof(vector_t));

    /* Check our return value to make sure we got memory */
	if (retval == NULL) {
		allocation_failed();
    }

    /* Now we need to initialize our data.
       Since retval->data should be able to dynamically grow,
       what do you need to do? */
    retval->size = 1;
    retval->data = (int*)malloc(sizeof(int));

    /* Check the data attribute of our vector to make sure we got memory */
    if (retval->data == NULL) {
		free(retval);				//Why is this line necessary? because we already allocate a block to retval
        allocation_failed();
    }

    /* Complete the initialization by setting the single component to zero */
    retval->data[0] = 0;

    /* and return... */
    return retval;
}

/* Return the value at the specified location/component "loc" of the vector */
int vector_get(vector_t *v, size_t loc) {

    /* If we are passed a NULL pointer for our vector, complain about it and exit. */
    if(v == NULL) {
        fprintf(stderr, "vector_get: passed a NULL vector.\n");
        abort();
    }

    /* If the requested location is higher than we have allocated, return 0.
     * Otherwise, return what is in the passed location.
     */
	if (loc < v->size) {
		return v->data[loc];	
	}
    return 0;
}

/* Free up the memory allocated for the passed vector.
   Remember, you need to free up ALL the memory that was allocated. */
void vector_delete(vector_t *v) {
    if(v == NULL) {
		return;
	}
	// we should free all box in v->data of array
	free(v->data);
	free(v);
}

/* Set a value in the vector. If the extra memory allocation fails, call
   allocation_failed(). */
void vector_set(vector_t *v, size_t loc, int value) {
    /* What do you need to do if the location is greater than the size we have
     * allocated?  Remember that unset locations should contain a value of 0.
     */
	/* if loc greater(equals) than size, reallocate memory of data to size of (loc + 1) */	
	if (loc >= v->size) {
		v->size = loc + 1;
		/* note: use "sizeof(int) * (v->size)" NOT "v->size" */
		v->data = (int*)realloc(v->data, (v->size) * sizeof(int));
	}
	/* set value in loc, finally, return */
	v->data[loc] = value;	
}
