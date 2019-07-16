
#include "keywords.h"
#include "lexsupport.h"
#include<iostream>
using namespace std;


#ifndef EXTERN
        #define EXTERN extern
#endif

#include"parser.h"
///Start and follow sets

// start sets
#define START_PUNCS to_set32_4( PT_EQUALS, PT_LPAREN, PT_LBRAKT, PT_LBRACE )
#define START_KEYS  SET32_EMPTY
#define START_LEXS  to_set32( lex_id )

// follow sets
#define FOLLOW_PUNCS  to_set32_5( PT_SEMI, PT_EQUALS, PT_LPAREN, PT_LBRAKT, PT_LBRACE )
#define FOLLOW_KEYS ( to_set32_6( KEY_DO, KEY_IF, KEY_SELECT, KEY_CATCH, KEY_RAISE, KEY_RETURN ) \
                    | to_set32_6( KEY_WHILE, KEY_FOR, KEY_END, KEY_ELSE, KEY_CASE, KEY_UNTIL )   )
#define FOLLOW_LEXS   to_set32_2( lex_id, lex_eof )

Assignment * Assignment::compile( Environment * e ) {
        // <statement> ::= <reference> [ "=" <expression> ]
        // returns NULL until we discover a need for it to return more
		int32_t label1;
		int32_t label2;

		lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_ASSIGN );

		Reference * ref = Reference::compile( e );
		if (ref == NULL) {
			//error is thrown in reference call
			return NULL;
		}
		Element * d = ref->refersto; // should be a variable; otherwise this is not an assignment
		if ( (d == NULL)  ||   (d->kind() != ISVAR) ) {
			error_warn("A variable is expected.\n", lex_this);
				return NULL;
		}

		if (lex_ispunc( lex_this, PT_EQUALS )) {
			lex_advance(); // skip =
			Expression * exp = Expression::compile( e, true ); //true means generate code. We generate code for expression,
																// except when it is called within const_declarator.
			if(exp == NULL){
				//error thrown in expression
				return NULL; 
			}
		/*********** CODE GENERATION ******************/
			// We have the rule: <reference> = <expression> -->
			//    which means we need to assign the value of expression to reference
			//get label of reference: label1 = ref->get_label()
			// if(label1 == -1) ref->assign_label( generate label)
			//get label of expression: label2 = exp->label
			/*********** Probably should range check if it is a number type ***********/
            if (ref->refersto->bt == exp->bt){
                //Types are the same assign, assign
                ref->assign_label(exp->label);
            } else {
                //types aren't the same, complain
                std::string type;
                if (ref->refersto->bt == base_type::STRING) {
                    type = "String.";
                } else {
                    type = "Integer.";
                }
                //throw (ref->refersto->name + ": type assignment missmatch, expected " +type);
                error_warn("Assignment types mismatch.\n", lex_this);
            }
		/******* END OF CODE GENERATION ****************/

		}
		else { /* No need to check for this case, where this should have been checked by <statement>;
			but will keep it in case there are mistakes could lead to this situation.*/
			return NULL; //=BUG Throw error
		}

		lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDASSIGN);
}
