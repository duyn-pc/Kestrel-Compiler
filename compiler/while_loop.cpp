
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
#define START_KEYS  to_set32( KEY_WHILE )
#define START_LEXS  SET32_EMPTY

// follow sets
#define FOLLOW_PUNCS   to_set32_5( PT_SEMI, PT_EQUALS, PT_LPAREN, PT_LBRAKT, PT_LBRACE )
#define FOLLOW_KEYS  ( to_set32_6( KEY_DO, KEY_IF, KEY_SELECT, KEY_CATCH, KEY_RAISE, KEY_RETURN ) \
                     | to_set32_6( KEY_WHILE, KEY_FOR, KEY_END, KEY_ELSE, KEY_CASE, KEY_UNTIL ) )
#define FOLLOW_LEXS    to_set32_2( lex_id, lex_eof )


void While::compile( Environment * e ) {

	lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_WHILE );

				//code-generation:
				//may need label for this expression to return to it
				int32_t check = cg->label();
				Expression* exp = Expression::compile(e, true); //true: we need to generate code here
				if(exp == NULL){ //error thrown in expression
					return ;
					}
				if(lex_this.type == lex_key && lex_this.val == KEY_DO)
					lex_advance();//skip do
				//code-generation
				//Expression now has a label for the produced value
				// if zero--> break; otherwise --> loop
				cg->pushAddress(exp->label);
				cg->load();
				cg->bNotZero(check + 1);
				cg->pop();
				//May need to generate label for next block
			//end of code-generation
			Block::compile(new Environment(e->types, e->elements));
			 if(lex_this.type == lex_key && lex_this.val != KEY_END){
				return ; //throw error
			}
			else
			{
                cg->br(check);
                cg->label();
			}    //need to return to expression label above to evaluate again

		lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDWHILE);

	return;

}
