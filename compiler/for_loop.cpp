//#include "config.h"
//#include "sets.h"
//#include "error.h"
//#include "stringpool.h"
//#include "symboltable.h"
#include "keywords.h"
//#include "lexical.h"
#include "lexsupport.h"


#ifndef EXTERN
        #define EXTERN extern
#endif

/*
#include"expression.h"
#include"assignment.h"
*/

#include"parser.h"
///Start and follow sets

///Start and follow sets
// start sets
#define START_PUNCS SET32_EMPTY
#define START_KEYS  to_set32( KEY_FOR )
#define START_LEXS  SET32_EMPTY

// follow sets
#define FOLLOW_PUNCS   to_set32_5( PT_SEMI, PT_EQUALS, PT_LPAREN, PT_LBRAKT, PT_LBRACE )                  
#define FOLLOW_KEYS  ( to_set32_6( KEY_DO, KEY_IF, KEY_SELECT, KEY_CATCH, KEY_RAISE, KEY_RETURN ) \
                     | to_set32_6( KEY_WHILE, KEY_FOR, KEY_END, KEY_ELSE, KEY_CASE, KEY_UNTIL ) )		     
#define FOLLOW_LEXS    to_set32_2( lex_id, lex_eof )



