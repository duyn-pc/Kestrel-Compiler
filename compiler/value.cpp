
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
#define START_PUNCS to_set32_3( PT_LPAREN, PT_LBRAKT, PT_LBRACE )
#define START_KEYS  to_set32( KEY_NULL )
#define START_LEXS  to_set32_4( lex_int, lex_id, lex_str, lex_Cstr )

// follow sets
#define FOLLOW_PUNCS ( to_set32_6( PT_TIMES, PT_DIV, PT_MOD, PT_AND, PT_MINUS , PT_PLUS )      \
		     | to_set32_6( PT_OR,  PT_EQUALS, PT_NOTEQL, PT_GT, PT_GE, PT_LT )         \
		     | to_set32_6( PT_LT, PT_LPAREN, PT_LBRAKT, PT_LBRACE, PT_ELIPS, PT_SEMI  )\
                     | to_set32_2( PT_ATSIGN, PT_COLON ) )
#define FOLLOW_KEYS  ( to_set32_6( KEY_IN, KEY_NULL, KEY_THEN, KEY_END, KEY_DO, KEY_IF )             \
                     | to_set32_6( KEY_SELECT, KEY_CATCH, KEY_RAISE, KEY_RETURN, KEY_WHILE, KEY_FOR )\
		     | to_set32_6( KEY_ELSE, KEY_CASE, KEY_UNTIL, KEY_EXTERNAL, KEY_OF, KEY_ENUM )   \
		     | to_set32_3( KEY_ARRAY, KEY_SET, KEY_RECORD ) )
#define FOLLOW_LEXS    to_set32_5( lex_int, lex_id, lex_str, lex_Cstr, lex_eof )


Value::Value(){
	constant = false;
}

Value::Value(int32_t l){
	label = l;
	constant = false;
}


Value::Value(Type* p){
	constant = false;
	type_ptr = p;
}

Value::Value(int v, base_type b){
	this->int_val = v;
	bt = INTEGER;
		constant = true;
	//cg->pushi(v);
}

Value::Value(string v, base_type b){
	this->str_val = v;
	bt = STRING;
		constant = true;
}

Value * Value::compile(Environment * e, bool gen_code){

	Expression * exp = NULL;
	Reference * ref = NULL;
	Value* val = NULL;

	lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_VAL );

	if(!gen_code){ //Evaluate value
		if(lex_this.type != lex_int && lex_this.type != lex_Cstr && lex_this.type != lex_str ){
			error_warn( "Constant type is expected.", lex_this);
			return NULL; //=BUG= Throw an error
	}
	if(lex_this.type == lex_int)
		val = new Value(lex_this.val, INTEGER);
	else val = new Value(get_symbol(lex_this.val), STRING); //if string, need to get its value from symbol table
	lex_advance(); // this is a terminal, need to skip this lexeme
	}
	else{ //Generate code
		if(lex_this.type == lex_int || lex_this.type == lex_str || lex_this.type == lex_Cstr){
			/******** Generate code:
						1- generate a new label for the constant
						2- assign enough memory: one word for int, or chars equal to size of string and save it there
						3- Maybe pop from stack any thing need to be poped in case you pushed things in it
						4- make a new value object with the new label: val = new Value(label);
						5- also, you may need to advance on lexeme
						*/
            if (lex_this.type == lex_int) {
                val = new Value(cg->putIntInMemory(lex_this.val));
            } else {
                string str = get_symbol(lex_this.val);
                //=BUG need to write a string translater to escape the "\"
                //to conform with kestrel strings
                val = new Value(cg->PutStringInMemory(str));
            }
            lex_advance();
            return val;///CHECK
		/**************** end of generate code for this case *******/
		}

		//what left is either a subexpression or a reference
		// start looking into subexpression
		if(lex_ispunc(lex_this, to_set32_3(PT_LPAREN, PT_LBRAKT, PT_LBRACE))){
			punctuation pre= (punctuation)lex_this.val;
			lex_advance();
			exp = Expression::compile(e, false); //do not generate code
			if(exp == NULL){
				return NULL;//throw error thrown in expression
			}
			if(pre == PT_LPAREN && lex_ispunc(lex_this,PT_RPAREN )){
				lex_advance();
			}
			else if(pre == PT_LBRACE && lex_ispunc(lex_this,PT_RBRACE )){
				lex_advance();
			}
			else if(pre == PT_LBRAKT && lex_ispunc(lex_this,PT_RBRAKT )){
				lex_advance();
			}
			else{
				error_warn( "Enclosing symbol is expected.", lex_this);
				return NULL;
			}
			val = new Value(exp->label);
		}
		else{ //reference
			ref = Reference::compile(e);
			if(ref == NULL)
				return NULL; //error
			val = new Value(ref->get_label());
		}
	}

	lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDVAL);

	return val;
}

