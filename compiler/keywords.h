/* keywords.h -- keyword recognition mechanism interface */

#pragma once

#ifndef EXTERN
    #define EXTERN extern
#endif


 //#include"stringpool.h"
 #include"symboltable.h"


 #define KEY_HASH_SIZE  113


/* list of all the keywords in the language */
typedef enum {
        KEY_INVALID, /* the null keyword handle */
        KEY_END,        KEY_CONST,      KEY_FINAL,      KEY_TYPE,
        KEY_EXCEPTION,  KEY_VAR,        KEY_PROCEDURE,  KEY_FUNCTION,
        KEY_PRIVATE,    KEY_RESTRICTED, KEY_EXTERNAL,   KEY_ARRAY,
        KEY_SET,        KEY_OF,         KEY_RECORD,     KEY_IF,
        KEY_THEN,       KEY_ELSE,       KEY_SELECT,     KEY_CASE,
        KEY_WHILE,      KEY_DO,         KEY_UNTIL,      KEY_FOR,
        KEY_IN,         KEY_CATCH,      KEY_RAISE,      KEY_RETURN,
        KEY_ENUM,
        KEY_NULL
} key_handle;
/* the range of valid key_handle values is KEY_INVALID + 1 to KEY_NULL
 * always keep KEY_NULL at the end!
 */


struct hash_t{
	key_handle key;
	symbol_handle sym;
};


/* list of all the keywords in the language */
const char * const key_names[] = {
        "???", /* the null keyword handle */
        "end",          "const",        "final",        "type",
        "exception",    "var",          "procedure",    "function",
        "private",      "restricted",   "external",     "array",
        "set",          "of",           "record",       "if",
        "then",         "else",         "select",       "case",
        "while",        "do",           "until",        "for",
        "in",           "catch",        "raise",        "return",
        "enum",
        "null"
};
/* IT IS CRUCIAL that the order of the strings listed in this table
 * exactly match the order of the keyword names declared in
 * key_handle in file keywords.h
 */


extern hash_t key_hash[KEY_HASH_SIZE];

void key_init();
/* initializer for keyword mechanism */

void key_put( key_handle k, FILE * f );
/* output the indicated keyword to the human readable file */

key_handle key_lookup( symbol_handle s );
/* return the keyword handle corresponding to the given symbol handle
 * if the symbol is not a keyword, returns KEY_INVALID
 */

/* key_get(key_handle) return the key name. Ex: key_handle 16-> return "if"
*/
#define key_get( k ){  \
	return key_names[k];  \
	}


#undef EXTERN
