

/* errors.c -- error reporting package implementation */

#include <stdlib.h>
#include <stdio.h>
#include "error.h"

/* the error messages.
 *  NOTE:  this array must have one entry for each
 *  member of the error_message enumeration, in exactly the same order
 */
static const char * message[] = {
        /* intended for use in calls to error_fatal */
        /* ER_BADFILE */ "Cannot open input file.",
        /* ER_SYMTAB */ "Symbol table is full.",
	/* ER_POOLOVF */ "String pool is full.",
	/* ER_MANYINPUT */ "Why you givin me so many input files?\n",
	/* ER_MANYOUTPUT */ "You got too many output files, I think.\n",
	/* ER_MISSINGFILE */ "We can't find any output files??\n",
	/* ER_BADARG */ "Your argument is bad.\n",

        /* intended for use in calls to error_warn */
        /* ER_TOOLONG,*/ "String is too large.",
        /* ER_TOOBIG  */ "Value too large.",
        /* ER_IDFORMAT*/ "Format of identifier is not correct.",
        /* ER_BADSTR */ "String is not terminated.",
        /* ER_WANT_BLOCK */ "Start of block is not correct.",/////////  =BUG=
        /* ER_WANT_ENDBLOK */ "End of block is not correct.",//////////  =BUG= change error message to contain useful info
   		/* ER_WANT_STMNT */ "Start of Statement is not correct.",
   		/*ER_WANT_ENDSTMT */ "End of statement is not correct.",
   		/* ER_WANT_ASSIGN */ "Start of Assignment is not correct.",
   		/* ER_WANT_ENDASSIGN */ "End of Assignment is not correct.",
   		/* ER_WANT_EXP */ "Start of expression is not correct.",
   		/* ER_WANT_ENDEXP */ "End of expression is not correct.", 
   		/* ER_WANT_COMP */ "Start of comparand is not correct.",
   		/* ER_WANT_ENDCOMP */ "End of comparand is not correct.",
   		/* ER_WANT_TERM */ "Start of term is not correct.",
   		/* ER_WANT_ENDTERM */ "End of term is not correct.",
   		/* ER_WANT_FACTOR */ "Start of factor is not correct.",
   		/* ER_WANT_ENDFACTOR */ "End of factor is not correct.",
   		/* ER_WANT_VAL */ "Start of value is not correct.",
   		/* ER_WANT_ENDVAL */ "End of value is not correct.",
   		/* ER_WANT_TYPE) */ "Start of Type is not correct.",
   		/* ER_WANT_ENDTYPE */ "End of Type is not correct.",
   		/* ER_WANT_REFER */ "Start of reference is not correct.",
   		/* ER_WANT_ENDREFER */ "ENd of reference is not correct",
   		/* ER_WANT_DO */ "Start of Do/loop is not correct.",
   		/* ER_WANT_ENDDO */ "End of D0/loop is not correct",
   		/* ER_WANT_WHILE */ "Start of While loop is not correct.",
   		/* ER_WANT_ENDWHILE */ "End of While loop is not correct.",
		/* ER_WANT_IF */ "Start of If statement is not correct.",
   		/* ER_WANT_ENDIF */ "End of If statement is not correct.",

	/* ER_HELP */ "Help we give:\n"
		      "./kestrel infile.k -o outfile.s   run with inputfile and outputfile\n"
	      	      "./kestrel                         run with stdin and stdout\n"
		      "./kestrel infile.k                run with stdin and output to infile.s\n"
		      "./kestrel -o outfile.s            run with stdin and output to outfile.s\n"
		      "./kestrel infile.k -o -           run with inputfile and stdout\n"	
		      "./kestrel - -o -                  run with stdin and stdout\n"      
	//...
};

void error_fatal( error_message er, int line ) {
        /* output the message er and exit the program indicating failure */
	if (line == 0) {
        	fprintf( stderr, message[ er ] );
	} else { 
        	fprintf( stderr, "Fatal error on line %d: %s\n", line, message[ er ] );
	}
        exit( EXIT_FAILURE );
}

void error_warn( error_message er, int line ) {
        /* output message er and allow processing to continue */
        fprintf( stderr, "Error on line %d: %s\n", line, message[ er ] );
}

void error_help( error_message er ) {
	/* outputs helpful mesages for users */
	fprintf( stderr, message[ er ] );
	exit( EXIT_FAILURE );
}
