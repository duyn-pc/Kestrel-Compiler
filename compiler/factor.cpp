
#include "keywords.h"
//#include "lexical.h"
#include "lexsupport.h"
#include<iostream>
using namespace std;

#ifndef EXTERN
        #define EXTERN extern
#endif

#include"parser.h"
///Start and follow sets

///Start and follow sets
// start sets
#define START_PUNCS to_set32_5( PT_MINUS, PT_NOT, PT_LPAREN, PT_LBRAKT, PT_LBRACE )
#define START_KEYS  to_set32( KEY_NULL )
#define START_LEXS  to_set32_4( lex_int, lex_id, lex_str, lex_Cstr )

// follow sets
#define FOLLOW_PUNCS ( to_set32_6( PT_TIMES , PT_DIV, PT_MOD, PT_AND, PT_MINUS , PT_PLUS )        \
                     | to_set32_6( PT_OR,  PT_EQUALS, PT_NOTEQL, PT_GT, PT_GE, PT_LT )            \
                     | to_set32_6( PT_LE, PT_RPAREN, PT_RBRAKT, PT_RBRACE, PT_COMMA, PT_NOT )     \
                     | to_set32_6( PT_LPAREN, PT_LBRAKT, PT_LBRACE, PT_ELIPS, PT_SEMI, PT_ATSIGN )\
                     | to_set32( PT_COLON ) )
#define FOLLOW_KEYS  ( to_set32_6( KEY_IN, KEY_NULL, KEY_THEN, KEY_END, KEY_DO, KEY_IF )             \
                     | to_set32_6( KEY_SELECT, KEY_CATCH, KEY_RAISE, KEY_RETURN, KEY_WHILE, KEY_FOR )\
		     | to_set32_6( KEY_ELSE, KEY_CASE, KEY_UNTIL, KEY_EXTERNAL, KEY_OF, KEY_ENUM )   \
		     | to_set32_3( KEY_ARRAY, KEY_SET, KEY_RECORD ) )
#define FOLLOW_LEXS    to_set32_5( lex_int, lex_id, lex_str, lex_Cstr, lex_eof)

Factor::Factor(){
	constant = false;
}

Factor::Factor(int32_t l){
	label = l;
	constant = false;
}

Factor::Factor(Type *p){
	constant = false;
	type_ptr = p;
}

Factor::Factor(int v, base_type b){
	this->int_val = v;
	bt = INTEGER;
		constant = true;
}

Factor::Factor(string v, base_type b){
	this->str_val = v;
	bt = STRING;
		constant = true;
}

Factor * Factor::compile( Environment * e, bool gen_code ) {

        lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_FACTOR );

		punctuation op = PTX;
		int32_t label1;
		int32_t val_int_val;
		string val_str_val="";
		Value* val = NULL ;
		Factor* fact=NULL;

		if(lex_ispuncset( lex_this, to_set32_2(PT_MINUS, PT_NOT) )){
			op = (punctuation) lex_this.val;
			lex_advance();
		}
		val = Value::compile(e, gen_code);
		if(val == NULL){ //error thrown in value
			return NULL;
		}

		if(gen_code){
		/************ Generate code *********************/
		label1 = val->label;

            if (op == PT_MINUS) {
                //cg->neg();
                cg->pushAddress(label1);
                cg->load();
                cg->neg();
                cg->saveTopAtLabel(label1);
                cg->pop();
            } else if (op == PT_NOT){
                //cg->Not();
                cg->pushAddress(label1);
                cg->load();
                cg->Not();
                cg->saveTopAtLabel(label1);
                cg->pop();
            } else {
                //do nothing the value is already on the stack
            }
            fact = new Factor(label1);

       /*********** END of GENERATE CODE ************/
		}
		else{ //evaluate
			if(val->constant != true){
				error_warn( "Constant type os expected.", lex_this);
				return NULL; //ERROR
			}
			if(val->bt != INTEGER){
				error_warn( "Integer type os expected.", lex_this);
				return NULL;
				}
			if(op == PT_MINUS){
				val_int_val = -1 * val->int_val;
			}
			else if(op == PT_NOT)
					if(val->int_val != 0 && val->int_val != 1) {///=BUG= what to do if it is logical
						error_warn( "Type mismatch of terms.", lex_this);
						return NULL; ///ERROR: not of boolean type
						}
					else val_int_val = (val->int_val +1) % 2;
			else
				val_int_val = val->int_val;
			fact = new Factor(val_int_val, INTEGER);
		}

        lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDFACTOR);

        return fact;
}


