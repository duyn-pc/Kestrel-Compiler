/* errors.h -- error reporting package interface */
#pragma once
/* an enumeration type to encode the error messages */
typedef enum {
        /* intended for use in calls to error_fatal */
        ER_BADFILE,
        ER_SYMTAB,
        ER_POOLOVF,
	ER_MANYINPUT,
	ER_MANYOUTPUT,
	ER_MISSINGFILE,
	ER_BADARG,
        /* intended for use in calls to error_warn */
	ER_TOOLONG,
        ER_TOOBIG,
        ER_IDFORMAT,
        ER_BADSTR,
        ER_WANT_BLOCK,
        ER_WANT_ENDBLOK,
        ER_WANT_STMNT,
        ER_WANT_ENDSTMT,
        ER_WANT_ASSIGN,
        ER_WANT_ENDASSIGN,
        ER_WANT_EXP,
        ER_WANT_ENDEXP,
        ER_WANT_COMP,
        ER_WANT_ENDCOMP,
         ER_WANT_TERM,
        ER_WANT_ENDTERM,
        ER_WANT_FACTOR,
        ER_WANT_ENDFACTOR,
        ER_WANT_VAL,
        ER_WANT_ENDVAL,
        ER_WANT_TYPE,
        ER_WANT_ENDTYPE,
        ER_WANT_REFER,
        ER_WANT_ENDREFER,
        ER_WANT_DO,
        ER_WANT_ENDDO,
        ER_WANT_WHILE,
        ER_WANT_ENDWHILE,
        ER_WANT_IF,
		ER_WANT_ENDIF,
        


        //...
	/* intended for use in calls to error_help */
	ER_HELP
} error_message;

void error_fatal( error_message er, int line );
        /* output message er and exit the program indicating failure */

void error_warn( error_message er, int line );
        /* output message er and allow processing to continue */

void error_help( error_message er );
	/* output the help message */
