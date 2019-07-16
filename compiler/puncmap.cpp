// puncmap.cpp -- mapping for punctuation module

// version Oct 6, 2018 by Duy

#include <map>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "puncmap.h"

/* Returns the map number of a punctuation */
int get_punc_val(std::string input ){
	std::map<std::string, int>::iterator it = punc_val.find( input );
	if (it == punc_val.end()){
		return -1;
	} else {
		return it->second;
	}
}

/* Return the punc from the map given its value */
std::string get_punc_string( int input ){
	std::map<int,std::string>::iterator it = punc_string.find( input );
	if (it == punc_string.end()) {
		return NULL;
	} else {
		return it->second;
	}
}

/* Prints the punc or return its handle */
void put_punc( FILE * f, int punc ){
	std::string str = get_punc_string( punc );
	fprintf(f, "%s ", str.c_str());
}
