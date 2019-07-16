// lexical.cpp -- lexical analyzer

// version Aug. 27, 2018 by Douglas W. Jones

#include <stdio.h>
#include <stdint.h>
#include<iostream>
#include<string>
//#include "puncmap.h"
#include <iterator>

//#include "symboltable.h"
#include "charclass.h"

#include "keywords.h"
//#include<sets.h>
//#include "error.h"

#define EXTERN
#include "lexical.h"

int ch; // the current character not yet classified as part of a lexeme
        // this must be int because EOF is outside the range of unsigned char
static FILE * infile; // the input file
static int currLine;

void idents(){

		symbol_start(currLine);
				// identifier or possibly a keyword 
		lex_next.type = lex_id;
		do {
			// =BUG= must remember ch as part of the identifier's text 
			symbol_append( ch );
			// get the next letter 
			ch = getc( infile );

		} while ((ch != EOF) && ISCLASS(ch,LETTER|DIGIT));
		lex_next.val = symbol_lookup();  //we save handle to it
		
		key_handle key_h = key_lookup( lex_next.val );
		// check to see if it is a keyword 
		if (key_h != KEY_INVALID) {
			lex_next.type = lex_key;
			lex_next.val = key_h;  //We get the number of 
		}
}


void digits(){
		uint32_t acc = 0;
		uint32_t base = 0;

		do { // accumulate in base 10

			if (acc > (UINT32_MAX - (ch - '0')) / 10) {
				error_warn(ER_TOOBIG, currLine);

				lex_next.type = lex_err;
				//=BUG= should this be error type or DIGIT
				lex_next.val = 0;
				//advance to end of current lex
				do {
			 		ch = fgetc( infile );
		 		} while ((ch != EOF) && ISCLASS(ch, DIGIT));
				return;
			}
			acc = (acc * 10) + (ch - '0');
			ch = fgetc( infile );
		} while ((ch != EOF) && ISCLASS( ch, DIGIT ));

			if (ch == '#'){
				ch = fgetc( infile );

				if ( acc > 32 ) {
					//=BUG= need error message too large of base error

					lex_next.type = lex_err;
					lex_next.val = 0;
					//=BUG= should this be error type or DIGIT
					do {
					 	ch = fgetc( infile );
					} while ( ch != EOF && ISCLASS(ch, DIGIT) && ISCLASS(ch,LETTER));
					return;
				}

				base = acc;
				acc = 0;
				int digit = 0;

				while (ch != EOF && !ISCLASS(ch, WHITESPACE)) {

					if ( (digit = _get_decimal_value(ch)) == -1 ||
					 digit > base ) {
					//=BUG= Need invalid char in number for base error

					lex_next.type = lex_err;
					//advance to end of current lex
					do {
						ch = fgetc( infile );
					} while ( !ISCLASS(ch, WHITESPACE) || ch != EOF );
					return;
					}
					if (acc > (UINT32_MAX - digit) / base) {
						//=BUG= Handle overflow error

						lex_next.type = lex_err;
						//advance to end of current lex
						do {
					 		ch = fgetc( infile );
				 		} while ( !ISCLASS(ch, WHITESPACE) || ch != EOF );
					return;
					}
					acc = acc * base + digit;
					ch = fgetc( infile );
				}
				lex_next.type = lex_int;
				lex_next.val = acc;

			} else {
				lex_next.type = lex_int;
				lex_next.val = acc;
			}
}

void put_punct( FILE * f, punctuation punc ){
	std::string str = num_char[punc];
	fprintf(f, "%s ", str.c_str());
}

void puncs(){
	lex_next.type = lex_punc;
	char temp = ch;
	ch = fgetc( infile );
	string temp_str="";
	temp_str.push_back(temp); //push only one, and keep the other for later
	
	std::map<std::string, punctuation>::iterator it;
	it = char_num.map::find(temp_str);
	if(it != char_num.end()){
		lex_next.val = it->second;
	}
	
	//if the two compined chars is punc, replace the value with the new punc
	if(ISCLASS( ch, PUNCTUATION )){
		temp_str.push_back(ch);
		it = char_num.find(temp_str);
		if(it != char_num.end()){
			lex_next.val = it->second;
			//read in the next char since we consumed the one read above
			ch = fgetc( infile ); 
		}
	}
}


void strings(){
 /* string */
	if(ch == '"')
		lex_next.type = lex_Cstr;
	else
		lex_next.type = lex_str;
	symbol_start(currLine);

	char quote = ch; /* remember what quote mark to use */
    /* get the first letter after the opening quote */
	ch = getc( infile );
	while ((ch != EOF) && (ch != '\n') && (ch != quote)) {
        /* =BUG= must remember ch as part of the string's text */
		symbol_append( ch );
		/* get the next letter within the string */
		ch = getc( infile );
        }

        if (ch == quote) {
			lex_next.val = symbol_lookup();
			/* get the next letter after the closing quote */
			ch = getc( infile );
        } else {
                error_warn( ER_BADSTR, currLine );
        }
}

void lex_open( FILE * f ) {
	// open f if it exists or use stdin if not passed
	// initialize lex_this and _next to the first lexemes of the input
	if (f == NULL) {
		infile = stdin;
	} else {
		infile = f;
	}
	// setup to start reading the file
	ch = fgetc( infile );

	//initialize line number
	currLine = 1;

	// gratuitous initializations of values the user will never see
	lex_this.type = lex_err;
	lex_this.val = 0;
	lex_this.lineNum = currLine;
	lex_next.type = lex_err;
	lex_next.val = 0;
	lex_next.lineNum = currLine;
	//initialize symbol table and keywords table
	// order is important: symbol must be initialzed first
	symbol_init();
	key_init();

	// gratuitous initializations of values the user will never see
	lex_advance();
	lex_advance();
}

void lex_advance() {
	// update lex_this and _next one step in lexical analysis
	lex_this = lex_next;
	while ( (ch != EOF) && ISCLASS( ch, WHITESPACE ) ) {
		//if newline is encountered, update currLine
		if(ch == '\n'){
			//symbol_put('\n', f);
			currLine++;
			}
		ch = fgetc( infile );
	}

	if (ch == EOF) {
		lex_next.type = lex_eof;
		lex_next.val = 0;

	} else if (ISCLASS( ch, LETTER )) { // identifier
		idents();

	} else if (ISCLASS( ch, DIGIT )) { // number
		digits();

	} else if ((ch == '\'') || (ch == '"')) { // string
		strings();

	} else if (ISCLASS( ch, PUNCTUATION )) { // punctuation
		puncs();

	} else { // error!
		//=BUG= Handle invalid starting character error
		lex_next.type = lex_err;
		//advance to end of current lex
		do {
	 		ch = fgetc( infile );
		} while ( !ISCLASS(ch, WHITESPACE) || ch != EOF );
	}
}





void lex_put( lexeme * lex, FILE * f ) {
	// output reconstruction of lex to file f
	// =BUG= all of the following is scaffolding that needs rewrite
	switch (lex->type) {
		case lex_id: // identifiers like fred or i
		symbol_put(lex->val, f);
		putc(' ', f);
		break;

        case lex_key:   // keywords like if while return
		//symbol_put(lex->val, f);
		key_put(  (key_handle)lex->val , f);
		putc(' ', f);
		break;
		
        case lex_int:   // integers like 7 and 16#3FE
		fprintf( f, "%d ", lex->val );
		break;
		
        case lex_punc:  // punctuation marks like ( + ;
		//fprintf(f, "%d ", lex->val );
		put_punct( f, (punctuation) lex->val );
		//symbol_put(lex->val, f);
		putc(' ', f);
		break;
        case lex_str:   // strings like 'this'
		/*fputc( '\'' , f );
		string_put( lex->val, f );
		fputc( '\'' , f );
		*/
		symbol_put(lex->val, f);
		putc(' ', f);
		break;
		case lex_Cstr:   // strings like "this"
		/*fputc( '\'' , f );
		string_put( lex->val, f );
		fputc( '\'' , f );
		*/
		symbol_put(lex->val, f);
		putc(' ', f);
		break;
        case lex_eof:   // end of file
		fputs( "eof", f );
		break;
        case lex_err:    // error -- not a legal punctuation
		fputs( "error", f );
		break;
	}
}





int _get_decimal_value(char ch){
	switch (ch) {
		case '0':
		return 0;
		case '1':
		return 1;
		case '2':
		return 2;
		case '3':
		return 3;
		case '4':
		return 4;
		case '5':
		return 5;
		case '6':
		return 6;
		case '7':
		return 7;
		case '8':
		return 8;
		case '9':
		return 9;
		case 'A':
		case 'a':
		return 10;
		case 'B':
		case 'b':
		return 11;
		case 'C':
		case 'c':
		return 12;
		case 'D':
		case 'd':
		return 13;
		case 'E':
		case 'e':
		return 14;
		case 'F':
		case 'f':
		return 15;
		case 'G':
		case 'g':
		return 16;
		case 'H':
		case 'h':
		return 17;
		case 'J':
		case 'j':
		return 18;
		case 'K':
		case 'k':
		return 19;
		case 'M':
		case 'm':
		return 20;
		case 'N':
		case 'n':
		return 21;
		case 'P':
		case 'p':
		return 22;
		case 'Q':
		case 'q':
		return 23;
		case 'R':
		case 'r':
		return 24;
		case 'S':
		case 's':
		return 25;
		case 'T':
		case 't':
		return 26;
		case 'V':
		case 'v':
		return 27;
		case 'W':
		case 'w':
		return 28;
		case 'X':
		case 'x':
		return 29;
		case 'Y':
		case 'y':
		return 30;
		case 'Z':
		case 'z':
		return 31;
		default:
		return -1;
	}
}
