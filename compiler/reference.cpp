//#include "config.h"
//#include "sets.h"
//#include "error.h"
//#include "stringpool.h"
//#include "symboltable.h"
#include "keywords.h"
//#include "lexical.h"
#include "lexsupport.h"


#ifndef EXTERN
        #define EXTERN extern
#endif

/*
#include"expression.h"
#include"assignment.h"
*/

#include"parser.h"
///Start and follow sets

// start sets
#define START_PUNCS ( to_set32_4(PT_ATSIGN, PT_LPAREN, PT_LBRAKT, PT_LBRACE ) \
						 | to_set32(PT_DOT))
#define START_KEYS SET32_EMPTY
#define START_LEXS to_set32( lex_id )

// follow sets
#define FOLLOW_PUNCS ( to_set32_6( PT_LPAREN, PT_LBRAKT, PT_LBRACE , PT_EQUALS, PT_COMMA, PT_TIMES)\
                     | to_set32_6( PT_DIV, PT_MOD, PT_AND, PT_MINUS , PT_PLUS, PT_OR)              \
		     | to_set32_6( PT_NOTEQL, PT_GT, PT_GE, PT_LT, PT_LE, PT_RPAREN )              \
                     | to_set32_6( PT_RBRAKT, PT_RBRACE, PT_NOT, PT_ELIPS, PT_SEMI, PT_ATSIGN )    \
	             | to_set32( PT_COLON ) )

#define FOLLOW_KEYS  ( to_set32_4(KEY_IN, KEY_NULL, KEY_THEN, KEY_END)        \
		     | to_set32_4(KEY_DO, KEY_IF, KEY_SELECT, KEY_CATCH)      \
		     | to_set32_4(KEY_RAISE, KEY_RETURN, KEY_WHILE, KEY_FOR)  \
		     | to_set32_4(KEY_ELSE, KEY_CASE, KEY_UNTIL, KEY_EXTERNAL)\
		     | to_set32_4(KEY_OF, KEY_ENUM, KEY_ARRAY, KEY_SET)       \
		     | to_set32(KEY_RECORD))
#define FOLLOW_LEXS  ( to_set32(lex_eof) \
		     | to_set32_4(lex_id, lex_int, lex_str, lex_Cstr) )


//Code taken from lecture notes lecture 33 (compiling reference)

/*

<reference> ::= <identifier> { <refinement> }
<refinement> ::= "@"
              |  "." <identifier>
              |  <expression list>
*/
#define REFIN_PUNCS ( to_set32_2( PT_ATSIGN, PT_DOT )                   \
                    | to_set32_3( PT_LPAREN, PT_LBRAKT, PT_LBRACE ) )


Reference::Reference(){}

Reference::Reference(Element* d){
	this->refersto = d;
}

int32_t Reference::get_label(){
	return this->refersto->label;

}
void  Reference::assign_label(int32_t l){
	this->refersto->label = l;
}

 Reference* Reference::compile( Environment * e ) {
        // factory method, but returns NULL
	lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_REFER );
	if (lex_this.type != lex_id) return NULL; //This is the case because the reference is rewritten to have <refine>,
									// and the only rule in reference is <reference> ::= <identifier> { <refinement> }

	Element * d = e->element_lookup( get_symbol(lex_this.val) );
	// assume lookup complained and returned NULL if undefined
	if(d == NULL){ //=BUG= throw error because element is not found, and the reference points to nothing
		return NULL;
	}

	lex_advance(); // skip the identifier

	//Do not think this switch is really needed.
	/*
	if (d != NULL) switch (d->kind()) {
		case DECL_CONST:
		case DECL_TYPE:
		case DECL_EXCEPT:
		// =BUG= need to understand more, perhaps nothing to do here?
		break;


		case DECL_VAR:

        	cg->pushOnStack();

		break;
		case DECL_PROC:
		case DECL_FUNC:
		// =BUG= must push the mark and the up-link for it
		// =BUG= when do we do the call?  Depends on whether parameters

		break;
	}
	*/


	/*******************************
	<refinement> ::= "@"
              |  "." <identifier>
              |  <expression list>
    *******************************/
    /*
	while (lex_ispuncset( lex_this, REFIN_PUNCS )) {

		if (lex_ispunc( lex_this, PT_ATSIGN )) { //Case : "@"

			if ((d != NULL) &&  (d->kind() == DECL_VAR) &&  (d->type->kind == TYPE_POINTER)) {
				//cg_load(); // generate LOAD abstract instruction

				d = d->type->referent;
			} else {

				error_warn( ER_WANTPTR, lex_this.line );
			}

			lex_advance(); // skip @
		}//end of @ case
		else if (lex_ispunc( LEX_THIS, PT_DOT )) { // Case: "."

			if ((d != NULL) &&  (d->kind() == DECL_VAR) &&  (d->type->kind() == TYPE_RECORD)) {
				// nothing to do here

			} else {

				error_warn( ER_WANTREC, lex_this.line );
			}

		lex_advance(); // skip the "."

		if (lex_is( IDENT )) {
			Type * f = NULL;
			if (d != NULL) {
				f = d->type_ptr; ///How to send type??
			}
			if (f != NULL) cg_addi( f->offset );

				// =BUG= forgot some special cases
				//       specifically, type.min, type.max, etc.

			lex_advance(); // skip identifier

			} else {
				lex_gotbutwant( &lex_this, ER_WANT_IDENT );
			}

		}//End of "." case
		else { // assert lex_ispuncset( BEGINPARENS ))
			Expressionlist::compile
			// =BUG= if it was a parameter list, generate call here
		} // there are no other alternatives
	}
	*/

	lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDREFER );

	Reference * v = new Reference(d); //return ponter to the element

	return v;
}



/*
lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_REFER );
	if (!lex_is(IDENT)) return NULL;

	Element * d = e->element_lookup( get_symbol(lex_this.val) );
	// assume lookup complained and returned NULL if undefined
	lex_advance(); // skip the identifier

	if (d != NULL) switch (d->kind()) {
		case DECL_CONST:
		case DECL_TYPE:
		case DECL_EXCEPT:
		// =BUG= need to understand more, perhaps nothing to do here?
		break;
		case DECL_VAR:
        	cg->pushOnStack()
		break;
		case DECL_PROC:
		case DECL_FUNC:
		// =BUG= must push the mark and the up-link for it
		// =BUG= when do we do the call?  Depends on whether parameters
		break;
	}

	while (lex_ispuncset( LEX_THIS, REFIN_PUNCS )) {
		if (lex_ispunc( LEX_THIS, PT_ATSIGN )) {
			if ((d != NULL) &&  (d->kind == DECL_VAR) &&  (d->type->kind == TYPE_POINTER)) {
				//cg_load(); // generate LOAD abstract instruction


				d = d->type->referent;
			} else {

				error_warn( ER_WANTPTR, lex_this.line );
			}

			lex_advance(); // skip @
		} else if (lex_ispunc( LEX_THIS, PT_DOT )) {
			if ((d != NULL) &&  (d->kind() == DECL_VAR) &&  (d->type->kind() == TYPE_RECORD)) {
				// nothing to do here

			} else {

				error_warn( ER_WANTREC, lex_this.line );
			}

		lex_advance(); // skip .

		if (lex_is( IDENT ) {
			Type * f = NULL;
			if (d != NULL) {
				f = d->type_ptr; ///How to send type??
			}
			if (f != NULL) cg_addi( f->offset );

				// =BUG= forgot some special cases
				//       specifically, type.min, type.max, etc.

			lex_advance(); // skip identifier

			} else {
				lex_gotbutwant( &lex_this, ER_WANT_IDENT );
			}

		} else { // assert lex_ispuncset( BEGINPARENS ))
			Expressionlist::compile
			// =BUG= if it was a parameter list, generate call here
		} // there are no other alternatives
	}

	Reference * v = new Reference();
	v->refersto = d;
	lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_EQSEMI );
	*/
