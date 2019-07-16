#include<iostream>
#include"keywords.h"

hash_t key_hash[KEY_HASH_SIZE];

void key_put( key_handle k, FILE * f ) {
        /* output the keyword to the human readable file */
        fputs( key_names[ k ], f );
}

/* Changed hash_table[] to key_hash[] as in key_init()	*/
key_handle key_lookup( symbol_handle s ) {
        /* return the keyword handle corresponding to the given symbol handle
         * if the symbol is not a keyword, returns KEY_INVALID
         */

        int hash = s % KEY_HASH_SIZE;
        //if (key_hash[i].sym == s) {
        do{
         	if (key_hash[hash].sym == s) {
                //return key_hash[i].key;
                return key_hash[hash].key;
        } else if(key_hash[hash].key == KEY_INVALID) {
                return KEY_INVALID;
        		}
        hash++;
        if(hash == KEY_HASH_SIZE)
        	hash = 0;
        }while(true );
}

symbol_handle symbol_add( const char * s ) {
        symbol_start( 0 );
        while ( *s != '\0' ) {
                symbol_append( *s );
                s++;
        }
        return symbol_lookup();
}

void key_init() {
        /* initializer for keyword lookup mechanism */
        int i;
        int trouble = 0;

        /* first, put default values in table */
        for (i = 0; i < KEY_HASH_SIZE; i++) {
                key_hash[i].key = KEY_INVALID;
                /* =BUG= not sure what to replace SYMBOL_HANDLE with????
                   though I think we will not need it anyway. */
               // key_hash[i].sym = SYMBOL_INVALID;
               key_hash[i].sym = STRING_NULL;
        }

        for (i = KEY_INVALID + 1; i <= KEY_NULL; i++) {
                symbol_handle s = symbol_add( key_names[i] );
                int hash = s % KEY_HASH_SIZE;
                if (key_hash[hash].key == KEY_INVALID) {
                        key_hash[hash].key = (key_handle)i;
                        key_hash[hash].sym = s;
                } else {
                        trouble = trouble + 1;
                        //Add code to ensure key insertion key_hash
                        do{
                        	hash++;
                        	if(hash == KEY_HASH_SIZE) 
                        		hash=0;
                        	if (key_hash[hash].key == KEY_INVALID) {
                        		key_hash[hash].key = (key_handle)i;
                        		key_hash[hash].sym = s;
                        		break;
                			}
                		}while(true);
        		}
        }

        if (trouble > 0) {
                /* =BUG= this should probably be a call to error_fatal() */
        }
}