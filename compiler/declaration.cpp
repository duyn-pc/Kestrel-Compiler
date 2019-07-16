
#include "keywords.h"
#include "lexsupport.h"
#include<string>
#include<iostream>
using namespace std;

#ifndef EXTERN
        #define EXTERN extern
#endif
#include"parser.h"


// start sets
#define START_PUNCS SET32_EMPTY
#define START_KEYS SET32_EMPTY
#define START_LEXS to_set32( lex_id )

// follow sets
#define FOLLOW_PUNCS ( to_set32_4( PT_SEMI, PT_EQUALS, PT_LPAREN, PT_LBRAKT ) \
		     | to_set32(   PT_LBRACE )                                \
		     )
#define FOLLOW_KEYS  ( to_set32_4( KEY_DO, KEY_IF, KEY_SELECT, KEY_CATCH )    \
		     | to_set32_4( KEY_RAISE, KEY_RETURN, KEY_WHILE, KEY_FOR )\
	             | to_set32_4( KEY_END, KEY_ELSE, KEY_CASE, KEY_UNTIL )   \
	     	     )
#define FOLLOW_LEXS    to_set32_2( lex_id, lex_eof )


Declaration* Declaration::compile( Environment * e ) {
		accessibility access;
        lex_wantinset( START_PUNCS, START_KEYS, START_LEXS, ER_WANT_STMNT );

        //At this point: lex_this = id; lex_next = ':'; advance lex, but keep identifier
        string id = get_symbol(lex_this.val);
        lex_advance();
        if(lex_ispunc(lex_this, PT_COLON)){
			lex_advance();
			if(lex_iskeyset(lex_this, to_set32_2(KEY_PRIVATE, KEY_RESTRICTED))){ //if private or restricted
				if(lex_this.val == KEY_PRIVATE)
					access = ISPRIVATE;
				else access = ISRESTRICTED;
				lex_advance(); //lex is pointing to keywords: const or final
			}
			else //accessability not specified
				access = ISPUBLIC;
        }
		else{ //Not needed since this was checked by block before calling declaration
		}
		
		Type* t=NULL;
		Const_decl* d=NULL;
		Type* new_t = NULL;
		Element* elem= NULL;

        if(lex_this.type == lex_key ){ //lex this must determine whcih declaration function to call
			key_handle key = (key_handle) lex_this.val;
			 	//lex_advance(); //+extra

			switch(key){
				case KEY_CONST:
					d = (Const_decl*) Const_decl::compile(e,  id, access, CONST);
					break;

				case KEY_FINAL:
				 //after advancing, lex_this is the keyword const or final
					//check for errors in the compile of const
					d = (Const_decl*) Const_decl::compile(e,  id, access, FINAL);
				break;

				case KEY_TYPE:
					lex_advance();
					new_t = Type::compile(e, id);
				break;

				case KEY_EXCEPTION :

				break;

				case KEY_VAR: // "var" <type>
					{
					//<type> should be an Id of the type associated with variable
				 	new_t = e->type_lookup(get_symbol(lex_this.val)); //lookup the type of var
					if(t == NULL){ //type not found: throw error
						string str = "Type "+get_symbol(lex_this.val)+" is is not found.\n";
						error_warn( str, lex_this);
						return NULL;
					}
					//else make a new element
			 		elem = new Element(id, new_t, ISPUBLIC, -1 ); //Note: this neds to be replaced later
                    }
				break;

				case KEY_PROCEDURE:

				break;

				case KEY_FUNCTION:

				break;
			}
		}
		else{
			error_warn("Declaration type expected.\n", lex_this);
			return NULL; //throw error
		}
        lex_wantinset( FOLLOW_PUNCS, FOLLOW_KEYS, FOLLOW_LEXS, ER_WANT_ENDSTMT);

        // =BUG= does the enclosing block need anything about this statement?
        return NULL;
}

