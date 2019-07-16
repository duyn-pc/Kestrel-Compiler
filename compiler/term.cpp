
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
#define START_PUNCS ( to_set32_6( PT_MINUS, PT_NOT, PT_TIMES, PT_DIV, PT_MOD, PT_AND ) \
					| to_set32_3( PT_LPAREN, PT_LBRAKT, PT_LBRACE ) )
#define START_KEYS  to_set32( KEY_NULL )
#define START_LEXS  to_set32_4( lex_int, lex_str, lex_Cstr, lex_id )

// follow sets
#define FOLLOW_PUNCS ( to_set32_6( PT_MINUS , PT_PLUS, PT_OR,  PT_EQUALS, PT_NOTEQL, PT_GT )      \
		     | to_set32_6( PT_LT, PT_LE, PT_RPAREN, PT_RBRAKT, PT_RBRACE, PT_COMMA )      \
	             | to_set32_6( PT_NOT, PT_TIMES, PT_DIV, PT_MOD, PT_AND, PT_LPAREN )          \
 		     | to_set32_6( PT_LBRAKT, PT_LBRACE, PT_ELIPS, PT_SEMI, PT_ATSIGN, PT_COLON ) )
#define FOLLOW_KEYS  ( to_set32_6( KEY_IN, KEY_NULL, KEY_THEN, KEY_END, KEY_DO, KEY_IF )             \
		     | to_set32_6( KEY_SELECT, KEY_CATCH, KEY_RAISE, KEY_RETURN, KEY_WHILE, KEY_FOR )\
		     | to_set32_6( KEY_ELSE, KEY_CASE, KEY_UNTIL, KEY_EXTERNAL, KEY_OF, KEY_ENUM )   \
		     | to_set32_3( KEY_ARRAY, KEY_SET, KEY_RECORD ) )
#define FOLLOW_LEXS    to_set32_5( lex_int, lex_id, lex_str, lex_Cstr, lex_eof )

#define MULT_OPERATOR to_set32_4(PT_TIMES, PT_DIV, PT_MOD, PT_AND)


Term::Term(){
	constant = false;
}

Term::Term(int32_t l){
	label = l;
	constant = false;
}

Term::Term(Type* p){
	constant = false;
	type_ptr = p;
}

Term::Term(int v, base_type b){
	this->int_val = v;
	bt = INTEGER;
		constant = true;
}

Term::Term(string v, base_type b){
	this->str_val = v;
	bt = STRING;
		constant = true;
}

Term * Term::compile( Environment * e, bool gen_code ) {

        lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_TERM );

		punctuation op = PTX;
		int32_t label1, label2;
		int32_t fact_int_val;
		string fact_str_val="";
		Factor* fact=NULL;
		Factor* fact2=NULL;
		Term* term;
		fact = Factor::compile(e, gen_code);
		if(!(lex_ispuncset( lex_this, MULT_OPERATOR )) ){ //We have only one factor. Return it with the label of saved data
				if(fact->constant == true){ //return the value computed
					if(fact->bt == INTEGER)
						return  new Term(fact->int_val, INTEGER);
					else return new Term(fact->str_val, STRING);
				}
				else{ //return label
					return new Term(fact->label); //No Code generation in this case. Just return the label
				}
		}
		while(lex_ispuncset( lex_this, MULT_OPERATOR )){
				op = (punctuation) lex_this.val;
				fact2 = Factor::compile(e, gen_code);
				if(fact2 == NULL){ //=BUG= We have only one term, although we expect a second
					error_warn("A second term is expected after the operator.\n", lex_this);
					error_warn("A second factor is expected.\n", lex_this);
					return NULL;
				}
		if(gen_code){ //Generate code
				/******************* GENERATE CODE ******************/
				//Get labels
            label1 = fact->label;
            label2 = fact2->label;
            switch(op){
                    case PT_TIMES:
                        cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->mul();
                        cg->saveTopAtLabel(label1);
                        cg->pop();
                        break;
                    case PT_DIV:
                        cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->div();
                        cg->saveTopAtLabel(label1);
                        cg->pop();
                        break;
                    case PT_MOD:
                        cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->mod();
                        cg->saveTopAtLabel(label1);
                        cg->pop();
                        break;
                    case PT_AND:
                        cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->And();
                        cg->saveTopAtLabel(label1);
                        cg->pop();
                        break;
                     default:
                        //=BUG= Shouldn't be possible to reach here
                        break;
			/************** End of generate code ****************/
				}
		}
		else{ //Evaluate,
			if(fact->constant != true || fact2->constant != true )
						return NULL; //Error, incompatable types
					if( fact->bt != fact2->bt){
						error_warn("Base type mismatch.\n", lex_this);
						return NULL; //Error
					}
			switch(op){
					case PT_TIMES:
					if(fact->bt == INTEGER)
							fact_int_val = fact->int_val * fact2->int_val;
					else{
					error_warn("Non integer type is encountered.\n", lex_this);
					 return NULL; //=BUG= throw Error: no mult operation on strings
					}
					break;
					case PT_DIV:
					if(fact->bt == INTEGER)
							fact_int_val = fact->int_val / fact2->int_val;
					else{
					error_warn("Non integer type is encountered.\n", lex_this);
					 return NULL; //=BUG= throw Error: no mult operation on strings
					}
					break;
					case PT_MOD:
					/////=BUG= how do we check for valid terms (int vs boolean)
					if(fact->bt == INTEGER)
							fact_int_val = fact->int_val % fact2->int_val;
					else{
					 error_warn("Non integer type is encountered.\n", lex_this);
					 return NULL; //=BUG= throw Error: no mult operation on strings
					}
					break;
					case PT_AND:
					/////=BUG= how do we check for valid terms (int vs boolean)
					if(fact->bt == INTEGER)
							fact_int_val = (fact->int_val && fact2->int_val);
					else{
					 error_warn("Non integer type is encountered.\n", lex_this);
					 return NULL; //=BUG= throw Error: no mult operation on strings
					}
					break;
			}
		}

		if(fact->bt == INTEGER)
				term = new Term(fact_int_val, INTEGER);
			//else 	term = new Term(fact_str_val, STRING); //None of the operator works on Strings.
		}
		lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDTERM);

        return term;
}


/*

		if(!gen_code){
			//factor_value = fact->value;
			if(fact->constant){ //it should be constant anyway
				if(fact->bt == INTEGER || fact->bt == BOOL){
					int_val = fact->int_val;
					ter = new Term(int_val, INTEGER);
					}
				else if(fact->bt == STRING){ //string
					str_val = fact->str_val;
					ter = new Term(str_val, STRING);
				}

			}
		}
		else{ //generate code




			//make a term.
			ter = new Term(fact->type_ptr);
		}

		while(lex_ispuncset( lex_this, MULT_OPERATOR )){
			//if( ){
				op = (punctuation) lex_this.val;
				lex_advance();
				fact = Factor::compile(e, gen_code);

				if(!gen_code){  /////Important-if it is of type string, then the switch will not be executed??
					switch(op){
					case PT_TIMES:
						int_val = int_val * fact->int_val;
					break;
					case PT_DIV:
						int_val = int_val / fact->int_val;
					break;
					case PT_MOD:
					/////=BUG= how do we check for valid terms (int vs boolean)
						int_val = (int_val % fact->int_val);
					break;
					case PT_AND:
					/////=BUG= how do we check for valid terms (int vs boolean)
						int_val = (int_val && fact->int_val);
					break;
					}

				ter = new Term(int_val, INTEGER); //if there is more than one term, this will replace the object created above.
				}
				else{ //generate code
					switch(op){
					case PT_TIMES:
						cg->mul();
					break;
					case PT_DIV:
						cg->div();
					break;
					case PT_MOD:
					/////=BUG= how do we check for valid terms (int vs boolean)
						cg->mod();
					break;
					case PT_AND:
					/////=BUG= how do we check for valid terms (int vs boolean)
						cg->And();
					break;
					} //End of switch
					ter = new Term(fact->type_ptr);
				}

			//}//End if
		} //End while

        lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDTERM);
*/

