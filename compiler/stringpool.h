// stringpool.h
// manage string storage

// version 9/7/2018 by Douglas Jones -- just enough to make, not to run

// before #include "stringpool.h" you must
// #include <stdint.h>

// in stringpool.h, but not elsewhere, define EXTERN before including this file
#pragma once
#ifndef EXTERN
        #define EXTERN extern
#endif

#include <stdint.h>
#include <stdio.h>
#include<string>
//#include "stringpool.h"
#include "error.h"

using namespace std;



typedef uint32_t string_handle;
/* the string handle type.  C and C++ don't let us be more specific,
 * but it is actually values between 0 and POOL_SIZE-1, inclusive.
 */

////////////////////////////////////////////
// configuration constants
// =BUG= configuration constants should come from somewhere else
#define POOL_SIZE 10000



EXTERN unsigned char _string_pool[ POOL_SIZE ];
/* private, the actual location where the text of strings is stored */

EXTERN string_handle _string_limit;
/* private, index of the first unused location in _string_pool */

EXTERN string_handle _string_pos;
/* private, position to store new characters added to _string_pool */

EXTERN int _string_line;
/* private, line number on which this string started, for error reporting */

#define STRING_NULL 0
/* the null string handle, never used to index an actual string
 * this serves as a null pointer to a string
 */

/* void string_init(); */
#define string_init() { _string_limit = 1; }
/* initializer for the string pool package
 * initial value 1 guarantees that STRING_NULL won't accidentally be used
 */

/* string_handle string_start( int line ); */
#define string_start(line) { _string_line = line;      \
        _string_pos = _string_limit + 2;                \
}
/* setup to accumulate a new string and return its handle
   we have reserved 2 bytes for the string length, allowing up to 65535
 */

/* void string_append( char ch ); */
#define string_append(ch) {                             \
        if (_string_pos > (POOL_SIZE - 1)) {            \
                error_fatal( ER_POOLOVF, _string_line); \
        }                                               \
        _string_pool[_string_pos] = ch;                 \
        _string_pos++;                                  \
}
/* append one character to the string */

/* void string_done(); */
#define string_done() {                                 \
        int length = _string_pos - (_string_limit + 2); \
        if (length > 65535) {                           \
                error_warn( ER_TOOLONG, _string_line);  \
                length = 65535;                         \
        }                                               \
        _string_pool[_string_limit] = length & 0xFF;    \
        _string_pool[_string_limit + 1] = length >> 8;  \
}
/* mark the end of the string */

/* void string_accept(); */
#define string_accept() { 	_string_limit = _string_pos; }
/* accept the new string as a permanent part of the string pool */

/* void string_reject(); */
#define string_reject() {  }
/* reject the new string, it will not be included in the string pool */

void string_put( string_handle h, FILE * f );
/* output the string to the human-readable file */

/* =BUG= the code generator will need a way to put strings into object files */
/* =BUG= could string_chars(s,thunk()) call thunk(ch) for each char of s? */

bool string_eq( string_handle h1, string_handle h2 );
/* compare the strings h1 and h2 and return true if they are identical */

/* string_print() is meant to print the atring with handle h--for debugging only */
void string_print( string_handle h);

string string_get( string_handle h );
/* Rteurn the string to caller */


#undef EXTERN
