#pragma once
#include<stdint.h>

/* sets.h a fast lightweight implementation of set operations */
typedef uint32_t set32_t;

#define SET32_EMPTY 0


/* set32_t to_set32( int i ) */
#define to_set32(i)   (((set32_t)1)<<(i))
/* construct a single-member set32_t value from one integer or enumeration */

/* bool in_set32( int i, set32_t s ) */
#define in_set32(i,s) (to_set32(i) & s)
/* test if integer in set32_t, returns nonzero if so, zero if not */


/*
Why did we need to use the cast (set32_t)1 above instead of the simple constant 1?
The reason is, the constant 1 is of type int which is not guaranteed to be 32 bits.
 It could legally be 16 bits, in which case 1<<16 is zero because the one-bit was
 shifted off the end of the number representation.

With these definitions, the set {1} is created with TO_SET32(1),
and the set {1,5} is created with TO_SET32(1)|TO_SET32(5). Because of the
way we have used integers to represent sets, we can use the integer and
or operators to perform the set union and intersection operations. The notation
is verbose, but a few extra defines for constructing 2, 3 and 4 element sets
 can help make things look nicer:
 */

/* set32_t to_set32_2( int i, int j ) */
#define to_set32_2(i,j)     (to_set32(i) | to_set32(j))
/* construct set32_t value from 2 integers */

/* set32_t to_set32_3( int i, int j, int k ) */
#define to_set32_3(i,j,k)   (to_set32(i) | to_set32(j) | to_set32(k))
/* construct set32_t value from 3 integers */

/* set32_t to_set32_4( int i, int j, int k, int l ) */
#define to_set32_4(i,j,k,l) (to_set32_2(i,j) | to_set32_2(k,l))

/* set32_t to_set32_5( int i, int j, int k, int l, int m ) */
#define to_set32_5(i,j,k,l,m) (to_set32_4(i,j,k,l) | to_set32(m))

/* set32_t to_set32_6( int i, int j, int k, int l, int m, ubt n ) */
#define to_set32_6(i,j,k,l,m,n) (to_set32_4(i,j,k,l) | to_set32_2(m,n))
