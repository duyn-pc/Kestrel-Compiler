
#include "keywords.h"
//#include "lexical.h"
#include "lexsupport.h"

#ifndef EXTERN
        #define EXTERN extern
#endif
#include"parser.h"


// start sets
#define START_PUNCS ( to_set32_4( PT_MINUS, PT_EQUALS, PT_NOTEQL, PT_GT ) \
		    | to_set32_4( PT_GE, PT_LT, PT_LE, PT_PLUS )          \
		    | to_set32_4( PT_OR, PT_NOT, PT_TIMES, PT_DIV )        \
		    | to_set32_4( PT_MOD, PT_AND, PT_LPAREN, PT_LBRAKT )  \
		    | to_set32( PT_LBRACE )                                 \
	            )
#define START_KEYS    to_set32_2( KEY_IN, KEY_NULL )
#define START_LEXS    to_set32_4( lex_int, lex_id, lex_str, lex_Cstr )

// follow sets
#define FOLLOW_PUNCS ( to_set32_4( PT_RPAREN, PT_RBRAKT, PT_RBRACE, PT_COMMA ) \
		     | to_set32_4( PT_MINUS, PT_EQUALS, PT_NOTEQL, PT_GT )     \
		     | to_set32_4( PT_GE, PT_LT, PT_LE, PT_PLUS )              \
                     | to_set32_4( PT_OR, PT_NOT, PT_TIMES, PT_DIV )            \
                     | to_set32_4( PT_MOD, PT_AND, PT_LPAREN, PT_LBRAKT )      \
		     | to_set32_4( PT_LBRACE, PT_ELIPS, PT_SEMI,  PT_ATSIGN )  \
		     | to_set32(   PT_COLON )                                  \
		     )
#define FOLLOW_KEYS  ( to_set32_4( KEY_IN, KEY_NULL, KEY_THEN, KEY_END )       \
		     | to_set32_4( KEY_DO, KEY_IF, KEY_SELECT, KEY_CATCH )     \
		     | to_set32_4( KEY_RAISE, KEY_RETURN, KEY_WHILE, KEY_FOR ) \
		     | to_set32_4( KEY_ELSE, KEY_CASE, KEY_UNTIL, KEY_EXTERNAL)\
		     | to_set32_4( KEY_OF, KEY_ENUM, KEY_ARRAY, KEY_SET )      \
		     | to_set32(   KEY_RECORD ) )
#define FOLLOW_LEXS    to_set32_5( lex_int, lex_id, lex_str, lex_Cstr, lex_eof )

//Define here the comparing operands as a set
//"=", "/=", ">", ">=", "<", "<=", "in"
#define COMP_OPERATOR ( to_set32_4(PT_EQUALS, PT_NOTEQL, PT_GT, PT_GE) \
						| to_set32_3(PT_LT, PT_LE, KEY_IN) )


Expression::Expression(){
	constant = false;
}


Expression::Expression(int32_t l){
	label = l;
	constant = false;
}

Expression::Expression(	Type* p){
	constant = false;
		type_ptr = p;

}

Expression::Expression(int v, base_type b){
	this->int_val = v;
	bt = INTEGER;
	constant = true;
}


Expression::Expression(string s, base_type b){
	this->str_val = s;
	bt = STRING;
		constant = true;
}

Expression * Expression::compile( Environment * e, bool gen_code ) {

        lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_EXP );

		Comparand * comp = NULL;
		Comparand * comp2 = NULL;
		punctuation op = PTX;

		int exp_value = -1; //indicate we never accumolated any value yet
		Expression * exp = NULL;
		//We keep compiling exp as long as the lex_this (MYABE lex_next) is comp_operator
		//do{
			comp = Comparand::compile(e, gen_code );
			if(comp == NULL){//There was an error, so exit
				//=BUG= Nothing to be done, or return null instantly???
				return NULL;
			}
			////At this point, lex_this is the lexeme after the expression;
			// check if it is a comparand or not.
			if(!(lex_ispuncset( lex_this, COMP_OPERATOR )) ){ //We have only one comp. Return it with the label of saved data
				if(comp->constant == true){ //return the value computed
					if(comp->bt == INTEGER)
						return  new Expression(comp->int_val, INTEGER);
					else return new Expression(comp->str_val, STRING);
				}
				else{ //return label
					return new Expression(comp->label); //No Code generation in this case. Just return the label
				}
			}
			else{ // we have '='
				op = (punctuation) lex_this.val;
				lex_advance();
				comp2 = Comparand::compile(e, gen_code );
				if(comp2 == NULL) //=BUG= Error: We have one comp to be compared with comp2(=NULL). What to return?
					return NULL;
			}
		//****************generate code here ******************************************
			if(gen_code){
				//If we reach this point, we have two comp objects, and we need to execute the op on them
				//May need to check for types compatability and/or range?????

				//Get label of comp and comp2: comp->label; comp2->label
				int32_t label1 = comp->label;
				int32_t  label2 = comp2->label;

				switch(op){
					case PT_EQUALS :
						cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->eq();
                        cg->saveTopAtLabel(label1);
                        cg->pop();
					break;
					case PT_NOTEQL :
						cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->eq();
                        cg->Not();
                        cg->saveTopAtLabel(label1);
                        cg->pop();
					break;
					case PT_GT :
						cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->gt();
                        cg->saveTopAtLabel(label1);
                        cg->pop();
					break;

					case PT_GE:
						cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->ge();
                        cg->saveTopAtLabel(label1);
                        cg->pop();
					break;

					case PT_LT:
						cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->lt();
                        cg->saveTopAtLabel(label1);
                        cg->pop();
					break;

					case  PT_LE:
						cg->pushAddress(label1);
                        cg->load();
                        cg->pushAddress(label2);
                        cg->load();
                        cg->le();
                        cg->saveTopAtLabel(label1);
                        cg->pop();;
					break;

					case KEY_IN:
					//=BUG= Not sure what to do here.
					////MAYBE error
					break;
				}

				exp = new Expression(label1); ///////I assumed we use label of comp to save the data
				///// May need to be changed ////////////
			}
	/******************************* ENd of code generation ****************************/
			else{ //evaluate exp.
			//mAY NEED TO CHECK IF THE VALUES ARE INTEGERS
			if(comp->constant != true || comp2->constant != true )
				return NULL; //Error
			if(comp->bt != INTEGER || comp2->bt != INTEGER)
				return NULL; //ERROR

			switch(op){
				case PT_EQUALS :
					exp_value = (comp->int_val == comp2->int_val);
				break;
				case PT_NOTEQL :
					exp_value = (comp->int_val != comp2->int_val);
				break;
				case PT_GT :
					exp_value = (comp->int_val > comp2->int_val);
				break;

				case PT_GE:
					exp_value = (comp->int_val >= comp2->int_val);
				break;

				case PT_LT:
					exp_value = (comp->int_val < comp2->int_val);
				break;

				case  PT_LE:
					exp_value = (comp->int_val <= comp2->int_val);
				break;

				case KEY_IN:
				//=BUG= I think this case does not take place when we evaluate at compile time
				////MAYBE error
				break;
				}
				exp = new Expression(exp_value);
			}
        lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_EXP );
return exp;
}


