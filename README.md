# CS61C

## introduce

This is my solution about labs and projects in CS61C spring2022

## course website

- [CS61C spring2022](https://inst.eecs.berkeley.edu/~cs61c/sp22/)
- [CS61C the newest](https://cs61c.org/)

## course progress

- lab
  - [x] lab00
  - [x] lab01
  - [x] lab02
  - [ ] lab03
  - [ ] lab04
  - [ ] lab05
  - [ ] lab06
  - [ ] lab07
  - [ ] lab08
  - [ ] lab09
  
- project
  - [ ] proj1
  - [ ] proj2
  
  - [ ] proj3
  - [ ] proj4

## helper link

- [cs61c teach](https://github.com/61c-teach)



## labs

### lab00

start time: *22.10.28*

end time: *22.10.29*

**keeping patience~**

### lab01

start time: *22.11.14*

end time: *22.11.15*

In C, if we would change the value of the original something (ie. already existing)
we should use it's pointer/address in called function

- eg1. if we would change the value of the `int` we should pass `int*`
- eg2. if we would change the value(pointer) of the `void*` we should pass `void**`

### lab02

start time: *22.11.19*

end time:*22.11.19*

- about `realloc()`

  ```c
  /* note: malloc() will return a address */
  /* note: realloc() will return a address */
  int *p
  /* allocate memory, you should use '20 * sizeof(int)' NOT '20' */
  p = (int*)malloc(10 * sizeof(int));
  ...
  /* reallocate memory, you should use '20 * sizeof(int)' NOT '20' */
  p = (int*)realloc(p, 20 * sizeof(int));
  ```
  
- about `malloc()`

  ```c
  int* int_ptr;
  int_ptr = (int*)malloc(sizeof(int));
  
  /* note: use sizeof(node) NOT sizeof(node*),
  	 becasue sizeof(node*) == 4 BUT sizeof(node) maybe != 4 */
  node* node_ptr;
  node_ptr = (node*)malloc(sizeof(node));
  ```

  

## projects

### proj1

Conditional Inclusion

- `#ifndef`

  ```c
  /* 
   * frame
   */
  #ifndef HDR 
  #define HDR 
  /* contents of hdr.h go here */ 
  #endif
  
  /* 
   * example
   */
  #define YEARS_OLD 12
  #ifndef YEARS_OLD
  #define YEARS_OLD 10
  #endif
  ```

- `#ifdef`

  ```c
  /* 
   * frame
   */
  #ifdef macro_definition
  
  /* 
   * example
   */
  #include <stdio.h>
  #define UNIX 1
  
  int main(){
     #ifdef UNIX
     printf("UNIX specific function calls go here.\n");
     #endif
     return 0;
  }
  ```
  
- `#if`, `#else`, `#elif`

  ```c
  #if SYSTEM == SYSV 
  	#define HDR "sysv.h" 
  #elif SYSTEM == BSD 
  	#define HDR "bsd.h" 
  #elif SYSTEM == MSDOS 
  	#define HDR "msdos.h" 
  #else 
  	#define HDR "default.h" 
  #endif 
  #include HDR
  ```