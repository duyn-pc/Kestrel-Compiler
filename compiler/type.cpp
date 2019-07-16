
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
#include"expression.h"
#include"assignment.h"
*/

#include"parser.h"
///Start and follow sets


Type::Type(string name,  Declarator* type_ptr){

	this->name = name;

	this->type_ptr = type_ptr;

}


 Type* Type::compile(Environment* e, string id){


// =BUG= the following is unneeded because caller checks ident:
	//lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_TYPE );

	Type* new_type=NULL;
	cout<<"Type: --lex_this:";
	lex_put( &lex_this, stdout );
	cout<<endl;

	if(lex_this.type == lex_key){
		switch(lex_this.val){
		case KEY_ENUM:

		break;
		case KEY_ARRAY:

		break;
		case KEY_SET:

		break;
		case KEY_RECORD:

		break;
		default:
			//throw error
		break;
		}

	}
	else if(lex_this.type == lex_punc && lex_this.val == PT_ATSIGN){//pointer


	}
	else if( lex_next.type == lex_punc && lex_next.val == PT_ELIPS){ //subrange
		//assumption is the boundaries are constant valued.
		cout<<"Type: Calling Expression::Compile--lex_this:";
		lex_put( &lex_this, stdout );
		cout<<endl;
		Expression* exp1 = Expression::compile(e, false); //since it is constant, no code generation
		if(exp1 == NULL){//throw error
			return NULL;
		}
		lex_advance(); //skip the elips
		Expression* exp2 = Expression::compile(e, false);
		if(exp2 == NULL){ //throw error
			return NULL;
		}
		Declarator* d;
		d =	new Subrange(id , exp1->int_val, exp2->int_val );
		new_type = new Type(id, d);
	}
	else{ //reference: if not, then throw error


	}

	//lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDTYPE);

	return new_type;
}
