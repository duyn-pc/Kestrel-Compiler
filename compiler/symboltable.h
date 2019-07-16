/* symboltable.h  interface specifications for the symbol table */

/* users of this header file should first include
 *   <stdio.h>
 *   <stdint.h>
 *   stringpool.h
 * users of this header file should define (probably from a global
 * configuration file)
 *   SYMBOL_SIZE -- the maximum number of symbols
 *       values on the order of POOL_SIZE/6 make good sense
 *   SYMBOL_HASH -- used in the hash function
 *       must be less than and relatively prime to SYMBOL_SIZE
 *       wrong values degrade performance but do not break anything
 * when this file is used from stringpool.c (but nowhere else)
 *   the user must first define EXTERN as nothing or blank
 */

 #pragma once

#ifndef EXTERN
    #define EXTERN extern
#endif


 #include <stdio.h>
 #include <stdint.h>
 #include   "stringpool.h"
 #include<string>
using namespace std;
#define SYMBOL_SIZE 100

#define SYMBOL_HASH 79



typedef uint32_t symbol_handle;
/* the symbol handle type.  C and C++ don't let us be more specific,
 * but it is actually values between 0 and SYMBOL_SIZE-1, inclusive.
 */
extern string_handle _symbol_table[ SYMBOL_SIZE ];
/* private:  The symbol table maps symbol handles to string handles */

extern symbol_handle _symbol_hash;
/* private:  The hash code of the symbol currently being accumulated */

extern string_handle _symbol_string;
/* the tentative string handle for the symbol being accumulated */

extern int _symbol_line;
/* the line number on which this string was found, for error reporting */

void symbol_init();
/* initializer for the symbol table package */

/* void symbol_start( line ) */
/*
#define symbol_start(line) {                           \
        _symbol_hash = 0;                              \
        _symbol_string = string_start(line);           \
        _symbol_line = line;                           \
}
*/

#define symbol_start(line) {                           \
        _symbol_hash = 0;                              \
        _symbol_string = _string_limit;                          \
         string_start(line);           \
        _symbol_line = line;                           \
}
/* setup to accumulate one string into the symbol table
 * the line parameter is given so that symbol-table errors can be reported
 */

/* void symbol_append( char ch ) */
#define symbol_append(ch) {                            \
        _symbol_hash = ((_symbol_hash * SYMBOL_HASH )  \
                        + (ch)) % SYMBOL_SIZE;         \
        string_append(ch);                             \
}
/* append one character to the symbol currently being accumulated */


symbol_handle symbol_lookup();
/* finish the lookup of the symbol currently being accumulated
 * this looks it up in the hashed symbol table and returns its handle
 */

/* note:
 * to add a symbol to the table
 *   symbol_start()
 *   for each character ch in the string { symbol_append(ch) }
 *   handle = symbol_lookup()
 */
/* void symbol_put( symbol_handle s, FILE * f ) */
#define symbol_put( s, f ) { string_put( _symbol_table[ s ], f ); }
/* output the symbol to the human-readable file */

/* print symbol table for DEBUGGING purposes */
void print_sTable();

/* return the string at given string handle */
string get_symbol(symbol_handle s );

/* =BUG= the code generator will need a way to put symbols into object files */
/* =BUG= could symbol_chars(s,thunk()) call thunk(ch) for each char of s? */

#undef EXTERN

