

// block.cpp -- implementation of parser/compiler for blocks

// Author, Date, etc.

// BNF
// <block> ::= { <block element> [ ; ] }
// <block element> ::= <declaration> | <statement>

#include "keywords.h"
//#include "lexical.h"
#include "lexsupport.h"
#include<iostream>
using namespace std;

#ifndef EXTERN
        #define EXTERN extern
#endif

#include"parser.h"

// start sets
#define START_PUNCS ( to_set32_2( PT_SEMI, PT_EQUALS )              \
		    | to_set32_3( PT_LPAREN, PT_LBRAKT, PT_LBRACE ) \
		    )	
#define START_KEYS  ( to_set32_4( KEY_DO, KEY_IF, KEY_SELECT, KEY_CATCH ) \
                    | to_set32_4( KEY_RAISE, KEY_WHILE, KEY_FOR, KEY_RETURN )         \
                    )
#define START_LEXS to_set32( lex_id )

// follow sets
#define FOLLOW_PUNCS SET32_EMPTY
#define FOLLOW_KEYS to_set32_4( KEY_END, KEY_ELSE, KEY_CASE, KEY_UNTIL )
#define FOLLOW_LEXS to_set32( lex_eof )


Block * Block::compile( Environment * e ) {

        lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_BLOCK );
		Declaration* d;
		Statement * s;
        while (lex_isinset( START_PUNCS, START_KEYS, START_LEXS )) {

                if ( (lex_this.type == lex_id)
                &&   lex_ispunc( lex_next, PT_COLON ) ) {
                        // all declarations begin with ident:
                        //Do not advance here, in order to save the identifier for declaration
						 d = Declaration::compile( e );
                		 
                } else {
                        // if not a declaration must be a statement
                         s = Statement::compile( e );
                }
                if (lex_ispunc( lex_this, PT_SEMI )) lex_advance();
                //Terminate block:
                if(lex_iskeyset(lex_this, to_set32_3(KEY_END, KEY_ELSE, KEY_UNTIL)))
                	break;
        }
		//////in case the follow set is not what we expected
        lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDBLOK);

        return NULL;
}


/*
#endif
*/
