
#include <stdint.h>
#include <math.h>
#include<iostream>
#include<string>

#define EXTERN

#include "stringpool.h"

#define STR_DIG_SIZE 2


/* the following interfaces could have been implemented here but
 * they are implemented as macros in the header file instead
 *   void string_init();
 *   string_handle string_start( int line );
 *   void string_append( char ch );
 *   void string_done();
 *   void string_accept();
 *   void string_reject();
 */



void string_put( string_handle h, FILE * f ) {
        /* output the string to the human-readable file */
        int limit = h + 2 + _string_pool[h] + (_string_pool[h + 1] << 8);
        h = h + 2;
        while (h < limit) {
                putc( _string_pool[h], f );
                h = h + 1;
        }
}

bool string_eq( string_handle h1, string_handle h2 ){
        /* compare the strings h1 and h2 and return true if the are identical */
       
        int limit1 = h1 + 2 + _string_pool[h1] + (_string_pool[h1 + 1] << 8);
        int limit2 = h2 + 2 + _string_pool[h2] + (_string_pool[h2 + 1] << 8);
        
        //if their length is not equal, return false.
        if( (_string_pool[h1] + (_string_pool[h1 + 1] << 8)) !=  (_string_pool[h2] + (_string_pool[h2 + 1] << 8)) )
        	return false;
        	
        h1 = h1+2;
        h2=h2+2;
        //
        while (h1 < limit1) {
                if (_string_pool[h1] != _string_pool[h2]) return false;
                h1 = h1 + 1;
                h2 = h2 + 1;
        }
        /* Tricky code: since the string lengths are encoded in the first 2
         * characters, the above loop exits early if the string lengths differ
         */
        return true;
}



void string_print( string_handle h) {
        /* output the string to the human-readable file */
        int limit = h + 2 + _string_pool[h] + (_string_pool[h + 1] << 8);
        h = h + 2;
        
        while (h < limit) {
				std::cout<<(char)_string_pool[h];
                h = h + 1;
        }
}


string string_get( string_handle h) {
        /* output the string to the human-readable file */
        int limit = h + 2 + _string_pool[h] + (_string_pool[h + 1] << 8);
        char temp[limit - h];
        h = h + 2;
		int i=0;
		
        while (h < limit) {
				//std::cout<<(char)_string_pool[h];
				temp[i++] = (char) _string_pool[h];
                h = h + 1;
        }
        temp[i]='\0';
        return string(temp);
}
