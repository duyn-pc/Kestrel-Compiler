
#include "keywords.h"
#include "lexsupport.h"

#ifndef EXTERN
        #define EXTERN extern
#endif
#include"parser.h"

///Start and follow sets

// start sets
#define START_PUNCS  to_set32_4( PT_EQUALS, PT_LPAREN, PT_LBRAKT, PT_LBRACE )
#define START_KEYS ( to_set32_4( KEY_DO, KEY_IF, KEY_SELECT, KEY_CATCH )      \
                   | to_set32_4( KEY_RAISE, KEY_WHILE, KEY_FOR, KEY_RETURN )  )
#define START_LEXS to_set32( lex_id )

// follow sets
#define FOLLOW_PUNCS ( to_set32_4( PT_SEMI, PT_EQUALS, PT_LPAREN, PT_LBRAKT ) \
                     | to_set32(   PT_LBRACE )                                \
		     )
#define FOLLOW_KEYS  ( to_set32_4( KEY_DO, KEY_IF, KEY_SELECT, KEY_CATCH )    \
                     | to_set32_4( KEY_RAISE, KEY_WHILE, KEY_FOR, KEY_RETURN )\
		     | to_set32_4( KEY_END, KEY_ELSE, KEY_CASE, KEY_UNTIL )   \
		     )
#define FOLLOW_LEXS    to_set32_2( lex_id, lex_eof )

//Statement::Statement(){}
Statement * Statement::compile( Environment * e ) {

        lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_STMNT );

		if(lex_iskeyset(lex_this, to_set32(KEY_DO) ) ){ ///Keyword DO may be a start for do until loop
										   //// or <do end> --> both can be handeled by the same code
			lex_advance(); //skip do
			 Do_until::compile( e ); 
		}
		else if(lex_iskeyset(lex_this, to_set32(KEY_WHILE ))){ /////No need to have a class for
												 ///loops, since each loop is distinct
													 /// and can be distinguished from teh keyword
				lex_advance(); //skip while
				While::compile(  e );
		}
		else if(lex_iskeyset(lex_this, to_set32(KEY_FOR ))){

		}
		else if(lex_iskeyset(lex_this, to_set32(KEY_IF ))){
			lex_advance(); //skip if
			If::compile( e );
		}

		else if(lex_iskeyset(lex_this, to_set32(KEY_SELECT ))){

		}
		else if(lex_iskeyset(lex_this, to_set32(KEY_CATCH) )){

		}
		else if(lex_iskeyset(lex_this, to_set32( KEY_RAISE ))){

		}
		else if(lex_iskeyset(lex_this, to_set32(KEY_RETURN ))){ //"return"

		}
		else if( lex_this.type != lex_key && lex_ispunc(lex_next, PT_EQUALS) ){ //This is an assignment

			Assignment* a = Assignment::compile(e);

		}
		else if(lex_ispunc(lex_this,to_set32_3(PT_LPAREN, PT_LBRAKT, PT_LBRACE) )){ //This is a procedure call

		}
		else{ //ERROR

		}

        lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDSTMT);

        // =BUG= does the enclosing block need anything about this statement?
        return NULL;
}

