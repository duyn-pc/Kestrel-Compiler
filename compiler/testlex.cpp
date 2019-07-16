// testlex.cpp -- test program for the Kestrel lexical analyzer

// version Aug. 27, 2018 by Douglas W. Jones

#include <stdio.h>
#include<iostream>
#include <stdint.h>
#include "lexical.h"
//#include "stringpool.h"
#include"symboltable.h"


// string_handle _symbol_table[ SYMBOL_SIZE ];
/* private:  The symbol table maps symbol handles to string handles */

 //symbol_handle _symbol_hash;
/* private:  The hash code of the symbol currently being accumulated */

 //string_handle _symbol_string;
/* the tentative string handle for the symbol being accumulated */


//int main( int argc, char * argv[] ) {
//
//	if(argc>1)
//		lex_open( (FILE *) argv[1]);
//	else
//		lex_open( NULL );
//
//
//	while (lex_next.type != lex_eof) {
//		lex_put( &lex_this, stdout );
//		lex_advance();
//	}
//	lex_put( &lex_this, stdout );
//
//	//cout<<">>>>>>>>>>>>>>>\n\n";
//	//print_sTable();
//
//}
