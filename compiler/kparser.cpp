#include <stdio.h>
#include "kparser.h"


static FILE * infile; // the input file
static ptype current_type;

void get_lex(parseme* target)
/*Read lex_type, value, line, and column from input file into parse_this or
  next. Expect format from lexer: x,x(x)*,x(x)*,x(x)*; where x = 0 - 9
  and (x)* is the regex *. */
{
    int tmp_int = 0;
    char ch = getc(infile);
    //get lex_type
    while (ch != ','){
        if (ch == EOF) {
            current_type = p_eof;
            return;
        } else if (ch < '0' || ch > '9') {
            //=BUG= make error message for invalid character in parser
            target->type = lex_err;
            return;
        } else {
            tmp_int = tmp_int * 10 + (ch - '0');
            ch = getc(infile);
        }
    }
    //check to make sure it is a valid lex type
    if (tmp_int > 7) {
        target->type = lex_err;
        return;
    }
    target->type = (lextypes)tmp_int;
    ch = getc(infile);
    //get lex_val
    tmp_int = 0;
    while (ch != ','){
        if (ch == EOF) {
            current_type = p_eof;
            return;
        } else if (ch < '0' || ch > '9') {
            //=BUG= make error message for invalid character in parser
            target->type = lex_err;
            return;
        } else {
            tmp_int = tmp_int * 10 + (ch - '0');
            ch = getc(infile);
        }
    }
    target->value = tmp_int;
    //get the line number for error messages
    ch = getc(infile);
    tmp_int = 0;
    while (ch != ','){
        if (ch == EOF) {
            current_type = p_eof;
            return;
        } else if (ch < '0' || ch > '9') {
            //=BUG= make error message for invalid character in parser
            target->type = lex_err;
            return;
        } else {
            tmp_int = tmp_int * 10 + (ch - '0');
            ch = getc(infile);
        }
    }
    target->line = tmp_int;
    ch = getc(infile);
    //get the column number for error messages
    tmp_int = 0;
    while (ch != ';'){
        if (ch == EOF) {
            current_type = p_eof;
            return;
        } else if (ch < '0' || ch > '9') {
            //=BUG= make error message for invalid character in parser
            target->type = lex_err;
            return;
        } else {
            tmp_int = tmp_int * 10 + (ch - '0');
            ch = getc(infile);
        }
    }
    target->column = tmp_int;
    return;
}

void parse_advance()
//=BUG= Fill in code
//Advance the parse elements in parse_this and next
{
}

void parse_table()
//=BUG= fill in code
//Parse table logic goes here, including calls to build the parse tree
{
}

void parse_open(FILE * f)
//=BUG= Fill in code
//Open the file and do intialization logic here
{

}
