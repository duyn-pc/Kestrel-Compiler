//symboltable.c
//  implementation of the symbol table
//  (although much of it was implemented in the header file)


#include "config.h"
#include<iostream>
#include<string>
//#include "stringpool.h"
//#include "error.h"

//#define EXTERN
#include "symboltable.h"

//gratuitous definitions of extern variables
string_handle _symbol_table[ SYMBOL_SIZE ];
symbol_handle _symbol_hash;
string_handle _symbol_string;
int _symbol_line;


void symbol_init() {
        symbol_handle i;
        for (i = 0; i < SYMBOL_SIZE; i++) _symbol_table[i] = STRING_NULL;
        string_init();
}

symbol_handle symbol_lookup() {
		//Call string_done before commiting, to update the length of the stored string
        string_done();
        
        symbol_handle place = _symbol_hash;

        for (;;) { /* loop exits by embedded returns */
                if (_symbol_table[ place ] == STRING_NULL ) {
                        /* add symbol to table */
                        _symbol_table[ place ] = _symbol_string;
                        string_accept();
                        return place;
                }
                if (string_eq(_symbol_table[ place ], _symbol_string )) {
                        /* symbol is already in table */
                        string_reject();
                        return place;
                }
                /* circular increment */
                place = place + 1;
                if (place == SYMBOL_SIZE) place = 0;

                /* if we get back to where we started, trouble */
                if (place == _symbol_hash) {
                       // error_fatal( ER_SYMTAB, _symbol_line );
                       error_fatal( ER_SYMTAB, _string_line );
                }
        }
}

//print for debugging
void print_sTable(){
	 symbol_handle i;
     for (i = 0; i < SYMBOL_SIZE; i++) 
     	if(_symbol_table[i] != STRING_NULL){
     		string_print(_symbol_table[i]);
     		std::cout<<"\n";
     	}
}

string get_symbol(symbol_handle s){

 return string_get(_symbol_table[s]);
}
