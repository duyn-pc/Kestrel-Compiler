

//#include "config.h"
//#include "sets.h"
//#include "error.h"
//#include "stringpool.h"
//#include "symboltable.h"
#include "keywords.h"
//#include "lexical.h"
#include "lexsupport.h"
#include<iostream>
using namespace std;


#ifndef EXTERN
        #define EXTERN extern
#endif

/*
#include"assignment.h"
#include"statement.h"
*/

#include"parser.h"

///Start and follow sets
/*# start set:   "const" "final"
# follow set:  ";" <identifier> "=" "do" "if" "select" "catch" "raise" "@" "."
#              "return" "while" "for" "(" "[" "{" <end of file> "end" "else"
#              "case" "until"
*/
// start sets
#define START_PUNCS SET32_EMPTY
#define START_KEYS to_set32_2( KEY_CONST, KEY_FINAL )
#define START_LEXS SET32_EMPTY

// follow sets
#define FOLLOW_PUNCS ( to_set32_4( PT_SEMI, PT_EQUALS, PT_LPAREN, PT_LBRAKT )  \
		     | to_set32(   PT_LBRACE )                                 \
		     )
#define FOLLOW_KEYS  ( to_set32_4( KEY_DO, KEY_IF, KEY_SELECT, KEY_CATCH )     \
		     | to_set32_4( KEY_RAISE, KEY_RETURN, KEY_WHILE, KEY_FOR ) \
		     | to_set32_4( KEY_END, KEY_ELSE, KEY_CASE, KEY_UNTIL)     \
		     )
#define FOLLOW_LEXS    to_set32_2( lex_id, lex_eof )


 Const_decl::Const_decl(int exp_val){

	int_val = exp_val;
	this->bt = INTEGER;

}


 Const_decl::Const_decl(string exp_val){

	str_val = exp_val;
	this->bt = STRING;

}

declarator_kind Const_decl::kind() { return ISCONST; }

//return a pointer to this object
//Const_decl* Const_decl::get_ptr(){ return this;}

 Declarator * Const_decl::compile(Environment * e, string id, accessibility access, const_d_type cdt ){ //Maybe we return void, since
		int32_t label;
	lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_STMNT );
	//if(lex_this.type == lex_key && lex_this.val == KEY_CONST  ) cout<<"Key is "<<key_get( (key_handle) lex_this.val)<<endl;
	if(lex_this.type == lex_key && lex_this.val == KEY_CONST  ){ //lex_this is "const"
		//get the value of the expression--must be computed at compile time

		lex_advance(); //advance from "const"
		Expression* exp = Expression::compile( e , false ); //need to get the value of exp
														//second attribute: 0 compute
														// value at compile time and do not generate code;
														//if 1, generate code and evaluate at run time
		//Now we make a new element and a new type
		if(exp->constant != true) ///error
			return NULL;
		Type* t;
		Const_decl* decl = NULL;
		if(exp->bt == INTEGER){
			decl =  new Const_decl(exp->int_val);
		}
		else{
			decl= new Const_decl(exp->str_val);
		}
		
		//Generate label using code generator and reserve memory
		//Element* elem = new Element(id, t, access,cg->label(), t->bt, CONST);
		
		t = new Type(id, decl);
		e->type_add(t);
		if(decl->bt == INTEGER) {
            //decl->int_val
            label = (int) (cg->putIntInMemory(decl->int_val));
        }
		else {
            //decl->str_val
            label = cg->PutStringInMemory(decl->str_val);
        }
		Element* elem = new Element(id, t, access, label, decl->bt, CONST);
		e->element_add(elem);
	}
	else if(lex_this.type == lex_key && lex_this.val == KEY_FINAL  ){ //lex_this is "final"
		lex_advance(); //advance from "final
		Expression* exp = Expression::compile( e , true );
	}
	else { //Error
		error_warn("Keyword const or final was expected.\n", lex_this);
	}

	lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDSTMT);



return NULL;
}


