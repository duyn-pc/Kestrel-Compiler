
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
#define START_KEYS  to_set32( KEY_DO )
#define START_LEXS  SET32_EMPTY

// follow sets
#define FOLLOW_PUNCS   to_set32_5( PT_SEMI, PT_EQUALS, PT_LPAREN, PT_LBRAKT, PT_LBRACE )
#define FOLLOW_KEYS  ( to_set32_6( KEY_DO, KEY_IF, KEY_SELECT, KEY_CATCH, KEY_RAISE, KEY_RETURN ) \
                     | to_set32_6( KEY_WHILE, KEY_FOR, KEY_END, KEY_ELSE, KEY_CASE, KEY_UNTIL ) )
#define FOLLOW_LEXS    to_set32_2( lex_id, lex_eof )

void Do_until::compile( Environment * e ) {

	lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_DO );
		/////code-generation:
		//// -may need to make a label for fllowing block
		/////code-generation:
			//// -may need to make a label for fllowing block
			int32_t top = cg->label();
			//end of code-generation
			Block::compile(new Environment(e->types, e->elements));
			//Another note: when the block finishes execution, it returns without advancing from (end/else/until)
			if(lex_this.type == lex_key && lex_this.val == KEY_UNTIL){
				lex_advance();//skip do
				//code-generation: may need a label for this expression
				Expression* exp = Expression::compile(e, true); //true: we need to generate code here
				if(exp == NULL) //error thrown in expression
					return;
				//code-generation
				//Expression now has a label for the produced value
				// if zero--> continue; otherwise --> break ***IMPORTANT: this is opposite of while in behavior
				cg->pushAddress(exp->label);
				cg->load();
				cg->bNotZero(top);
				cg->pop();

				//end of code-generation
			}
		else if(lex_this.type == lex_key && lex_this.val != KEY_END){
			return ; //throw error
		}

	lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDDO);
	return ;
}
