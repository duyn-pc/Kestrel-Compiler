// main.cpp -- main program for Kestrel

// version Dec, 2018 by Group A
// bullet proof skeleton code from Professor Doug's Oct 3 lecture

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include"codegenerator.h"
#include "main.h"
#include "lexical.h"
#include "error.h"
#include "parser.h"

CodeGenerator* cg;

int main( int argc, char * argv[] ) {


	// first, deal with the program name
	const char * main_progname = "kestrel";
	if ((argc > 0)              // Unix/Linux shells promise this
	&&  (argv[0] != NULL)       // under Unix/Linux, above implies this
	&&  (argv[0][0] != '\0')) { // nonstandard exec could do this
		main_progname = argv[0];
	}

	// set the argument strings to indicate that they have not been set
	const char * main_infile = NULL;
	const char * main_outfile = NULL;
	FILE * infile_obj = NULL;
	FILE * outfile_obj = NULL;
	bool isinfile = false;
	bool isoutfile = false;

	// START OF COMMAND LINE PARSER
	int arg_index = 1;               // starting at the first argument
	while ((arg_index < argc) && (argv[arg_index]) != NULL ) {
		const char * arg = argv[arg_index]; // current argument
		char ch = *arg; 		    // first char of argument
		if (ch == '\0') {
			// ignore empty strings
		} else if (ch != '-') {
			// arg with no dashes are input file name
			if (isinfile) { // for case: too many input files
				error_fatal( ER_MANYINPUT, 0 );
			}
			main_infile = arg;
			isinfile = true;
		} else {
			// command line -option
			arg++; // skip the leading leading dash
			ch = *arg;

			if (ch == '\0') {       // - by itself
				// meaning read stdin
				if (isinfile) { // too many input files
					error_fatal( ER_MANYINPUT, 0 );
				}
				isinfile = true;
			} else if (ch == 'o') {  // -o option
				if (isoutfile) { // -o filename
					error_fatal( ER_MANYOUTPUT, 0 );
				}

				arg++; // strip off the letter o
				ch = *arg;
				if (ch == '\0') { // '-o filename'
					arg_index = arg_index + 1;
					if ((arg_index > argc) && (argv[arg_index] != NULL)) {
						// -o by itself
						error_fatal( ER_MISSINGFILE, 0 );
					}
					main_outfile = argv[arg_index];
					isoutfile = true;
				} else { // '-ofilename' or '-o=filename'
					if (ch == '=') {
						arg++;    // strip equal
						ch = *arg;
					}
					if (ch == '\0') { // '-o= '
						error_fatal( ER_MISSINGFILE, 0 );
					}
					main_outfile = arg;
					isoutfile = true;
				}
			} else if (!strcmp( arg, "help" )) { // -help
				error_help( ER_HELP );
			} else if (!strcmp( arg, "?" )) {    // -?, also help
				error_help( ER_HELP );
			} else {
				error_fatal( ER_BADARG, 0 );
			}
		}
		arg_index++; // onward to the next argument
	} //END OF THE COMMAND LINE PARSER

	// Create an outfile if none is provided
	// from kestrel in dwjones
	if ((main_infile != NULL) && (main_outfile == NULL)) {
		int len = strlen( main_infile ) + 3;
		char * buffer = (char *)malloc( len ); // .s
		strncpy( buffer, main_infile, len );
		char * dot = rindex( buffer, '.' );
		if (dot == NULL) dot = &(buffer[len]);
		strncpy( dot, ".s", 3 );
		main_outfile = buffer;
	}
	// Open the output file for codgen to write
	if (main_outfile != NULL) {
		outfile_obj = fopen( main_outfile, "w" );
	}
    cg = new CodeGenerator( outfile_obj );
	if (main_infile != NULL) {
		infile_obj = fopen( main_infile, "r" );
		if (infile_obj == NULL) error_fatal( ER_BADFILE, 0 );
	}
	lex_open( infile_obj );

	Block::compile(new Environment(NULL, NULL));
	cg->finalize();
}
