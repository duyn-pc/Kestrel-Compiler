
#include "keywords.h"
//#include "lexical.h"
#include "lexsupport.h"
#ifndef EXTERN
        #define EXTERN extern
#endif

#include"parser.h"
///Start and follow sets

///Start and follow sets
// start sets
#define START_PUNCS SET32_EMPTY
#define START_KEYS  to_set32( KEY_IF )
#define START_LEXS  SET32_EMPTY

// follow sets
#define FOLLOW_PUNCS   to_set32_5( PT_SEMI, PT_EQUALS, PT_LPAREN, PT_LBRAKT, PT_LBRACE )                  
#define FOLLOW_KEYS  ( to_set32_6( KEY_DO, KEY_IF, KEY_SELECT, KEY_CATCH, KEY_RAISE, KEY_RETURN ) \
                     | to_set32_6( KEY_WHILE, KEY_FOR, KEY_END, KEY_ELSE, KEY_CASE, KEY_UNTIL ) )		     
#define FOLLOW_LEXS    to_set32_2( lex_id, lex_eof )

void If::compile( Environment * e ) {

	lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_IF);


				//code-generation:
				//may need label for this expression to return to it
				
				Expression* exp = Expression::compile(e, true); //true: we need to generate code here
				if(exp == NULL) //throw error
					return; 
				if(lex_this.type == lex_key && lex_this.val == KEY_THEN)
					lex_advance();//skip then
				//if expression evaluates to non-zero, execute the folowing block
				//May need label
				Block::compile(new Environment(e->types, e->elements)); 
				
				if(lex_this.type == lex_key && lex_this.val == KEY_ELSE){
					lex_advance(); //skip else
					//else, execute this block; also may need label
					Block::compile(new Environment(e->types, e->elements));
				}
				if(lex_this.type == lex_key && lex_this.val == KEY_END)
					lex_advance();
				else{
					error_warn( "End symbol is expected.", lex_this);
				 return;//throw error
				 }
		lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDIF);
		
	return;
			
}
