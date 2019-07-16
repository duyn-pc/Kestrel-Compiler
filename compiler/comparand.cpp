
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

// start sets
#define START_PUNCS ( to_set32_6( PT_MINUS, PT_PLUS, PT_OR, PT_NOT, PT_TIMES, PT_DIV ) \
		    | to_set32_5( PT_MOD, PT_AND, PT_LPAREN, PT_LBRAKT, PT_LBRACE )    \
		    )
#define START_KEYS    to_set32(   KEY_NULL )
#define START_LEXS    to_set32_4( lex_int, lex_id, lex_str, lex_Cstr )

// follow sets
#define FOLLOW_PUNCS ( to_set32_6( PT_EQUALS, PT_NOTEQL, PT_GT, PT_GE, PT_LT, PT_LE )            \
		     | to_set32_6( PT_RPAREN, PT_RBRAKT, PT_RBRACE, PT_COMMA, PT_MINUS, PT_PLUS )\
	             | to_set32_6( PT_OR, PT_NOT, PT_TIMES, PT_DIV, PT_MOD, PT_AND )             \
		     | to_set32_5( PT_LPAREN, PT_LBRAKT, PT_LBRACE, PT_ELIPS, PT_SEMI )          \
		     | to_set32_2( PT_ATSIGN, PT_COLON )                                         \
		     )

#define FOLLOW_KEYS  ( to_set32_6( KEY_IN, KEY_NULL, KEY_THEN, KEY_END, KEY_DO, KEY_IF )             \
		     | to_set32_6( KEY_SELECT, KEY_CATCH, KEY_RAISE, KEY_RETURN, KEY_WHILE, KEY_FOR )\
		     | to_set32_6( KEY_ELSE, KEY_CASE, KEY_UNTIL, KEY_EXTERNAL, KEY_OF, KEY_ENUM )   \
		     | to_set32_3( KEY_ARRAY, KEY_SET, KEY_RECORD )                                  \
		     )
#define FOLLOW_LEXS    to_set32_5( lex_int, lex_id, lex_str, lex_Cstr, lex_eof )

#define ADD_OPERATOR to_set32_3(PT_PLUS, PT_MINUS, PT_OR)


Comparand::Comparand(){
	constant = false;
}

Comparand::Comparand(int32_t l){
	label = l;
	constant = false;
}

Comparand::Comparand(Type *p){
	constant = false;
	type_ptr = p;

}

Comparand::Comparand(int v, base_type b){
	this->int_val = v;
	bt = INTEGER;
		constant = true;
}


Comparand::Comparand(string v, base_type b){
	this->str_val = v;
	bt = STRING;
		constant = true;
}


Comparand * Comparand::compile( Environment * e, bool gen_code  ) {

        lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_COMP );

		punctuation op = PTX;
		int32_t label1, label2;
		int32_t term_int_val;
		string term_str_val="";
		Term* term ;
		Term* term2 = NULL;
		Comparand* comp=NULL;
		term = Term::compile(e, gen_code);

		if(term == NULL){ //error thrown in term
			return NULL;
		}

		if(!(lex_ispuncset( lex_this, ADD_OPERATOR )) ){ //We have only one term. Return it with the label of saved data
				if(term->constant == true){ //return the value computed
					if(term->bt == INTEGER)
						return  new Comparand(term->int_val, INTEGER);
					else return new Comparand(term->str_val, STRING);
				}
				else{ //return label
					return new Comparand(term->label); //No Code generation in this case. Just return the label
				}
		}
		while(lex_ispuncset( lex_this, ADD_OPERATOR )){
			//if( ){
				op = (punctuation) lex_this.val;
				lex_advance();
				term2 = Term::compile(e, gen_code);
				if(term2 == NULL){ //=BUG= We have only one term, although we expect a second
					error_warn("A second term is expected after the operator.\n", lex_this);
					return NULL;
				}

				if(gen_code){ //Generate code
				/******************* GENERATE CODE ******************/
				//Get labels
				label1 = term->label;
				label2 = term2->label;
				switch(op){
					case PT_PLUS:
						cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->add();
                        cg->saveTopAtLabel(label1);
                        cg->pop();
					break;
					case PT_MINUS:
						cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->sub();
                        cg->saveTopAtLabel(label1);
                        cg->pop();
					break;
					case PT_OR:
                        cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->Or();
                        cg->saveTopAtLabel(label1);
                        cg->pop();
					break;
					}
			/************** End of generate code ****************/
				}
				else{ //Evaluate,
					if(term->constant != true || term2->constant != true ){
						error_warn("Type mismatch between terms.\n", lex_this);
						return NULL; //Error, incompatable types
					}
					if( term->bt != term2->bt){
						error_warn("Base type mismatch between terms.\n", lex_this);
						return NULL; //Error
					}

					switch(op){
					case PT_PLUS://term_value =  + term->value;
						if(term->bt == INTEGER)
							term_int_val = term->int_val + term2->int_val;
						else term_str_val = term->str_val + term2->str_val;
					break;
					case PT_MINUS:
						if(term->bt == INTEGER)
							term_int_val = term->int_val - term2->int_val;
						else{
							error_warn("None integer type is encountered.\n", lex_this);
						 	return NULL;} // =BUG= throw Error: no such operation on strings//else term_str_val = term->str_val + term2->str_val;
					break;
					case PT_OR:
					if(term->bt == INTEGER)
							term_int_val = (term->int_val || term2->int_val);
						//else term_str_val = term->str_val + term2->str_val;
					else{	error_warn("None boolean type is encountered.\n", lex_this);
					 return NULL; //throw Error
					}
					break;
					}
				}
			if(term->bt == INTEGER)
				comp = new Comparand(term_int_val, INTEGER);
			else 	comp = new Comparand(term_str_val, STRING);
		} //End while

        lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDCOMP);

        return comp;
}


