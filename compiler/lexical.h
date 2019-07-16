// lexical.h -- interface spec for the lexical analyzer

// version Aug. 27, 2018 by Douglas W. Jones

// before #include "lexical.h" you must
// #include <stdio.h>
// #include <stdint.h>
#pragma once
#include <map>
#include <stdint.h>
#include <iterator>
// in lexical.h, but not elsewhere, define EXTERN before including this file
#ifndef EXTERN
	#define EXTERN extern
#endif

// all the different types of lexemes
typedef enum {
	lex_id,    // identifiers like fred or i
	lex_key,   // keywords like if while return
	lex_int,   // integers like 7 and 16#3FE
	lex_punc,  // punctuation marks like ( + ;
	lex_str,   // strings like 'this'
	lex_Cstr,  // strings like "this"
	lex_eof,   // end of file
	lex_err    // error -- not a legal punctuation
} lextypes;

typedef struct {
	lextypes type; // int, ident, punc ...
	uint32_t val;  // something specific about this lexeme
	               // interpretation depends on type
	int lineNum; //line number of lexeme
	// =BUG=RES= line number added
	// =BUG= other attributes?
} lexeme;

/* table mapping from punctuation names to text strings for lex_put */
/* WARNING:  strings must be given in the order enumerated in punct_type */
static const char * punct_name[] = {
    /* PTX */ "?WHAT?",
    /* "PT_SEMI   */ ";", /* PT_EQUALS */ "=", /* PT_COLON  */ ":",
    /* "PT_LPAREN */ "(", /* PT_LBRAKT */ "[", /* PT_LBRACE */ "{",
    /* "PT_RPAREN */ ")", /* PT_RBRAKT */ "]", /* PT_RBRACE */ "}",
    /* "PT_COMMA  */ ",", /* PT_ATSIGN */ "@", /* PT_ELIPS  */ "..",
    /* "PT_NOTEQL */ "/=", /*PT_GT     */ ">", /* PT_GE     */ ">=",
    /* "PT_LT     */ "<", /* PT_LE     */ "<=", /*PT_PLUS   */ "+",
    /* "PT_MINUS  */ "-", /* PT_TIMES  */ "*", /* PT_DIV    */ "/",
    /* "PT_MOD    */ "%", /* PT_AND    */ "&", /* PT_OR     */ "|",
    /* "PT_NOT    */ "~", /* PT_DOT    */ "."
};

typedef enum{
     PTX  /*"?WHAT?"*/,
     PT_SEMI   /* ";"*/,  PT_EQUALS /* "="*/,  PT_COLON  /* ":"*/,
     PT_LPAREN /* "("*/,  PT_LBRAKT /* "["*/,  PT_LBRACE /* "{"*/,
     PT_RPAREN /* ")"*/,  PT_RBRAKT /* "]"*/,  PT_RBRACE /* "}"*/,
     PT_COMMA  /* ","*/, PT_ATSIGN /*"@"*/,  PT_ELIPS  /* ".."*/,
     PT_NOTEQL /* "/="*/, PT_GT    /* ">"*/,  PT_GE    /*">="*/,
     PT_LT     /* "<"*/,  PT_LE     /* "<="*/, PT_PLUS   /* "+"*/,
     PT_MINUS  /* "-"*/,  PT_TIMES  /* "*"*/,  PT_DIV    /* "/"*/,
     PT_MOD    /* "%"*/,  PT_AND    /* "&"*/,  PT_OR     /* "|"*/,
     PT_NOT    /* "~"*/,  PT_DOT    /* "."*/
}punctuation;



static std::map<std::string, punctuation > char_num={
 	 {"?WHAT?", PTX},
     {";" , PT_SEMI }, { "=",  PT_EQUALS },  {":", PT_COLON} ,
      {"(", PT_LPAREN },   { "[", PT_LBRAKT},   { "{", PT_LBRACE},
      { ")", PT_RPAREN},   { "]", PT_RBRAKT},   { "}", PT_RBRACE},
       { ",", PT_COMMA},  {"@", PT_ATSIGN},    { "..", PT_ELIPS},
      { "/=", PT_NOTEQL},     { ">", PT_GT},      {">=", PT_GE},
      { "<", PT_LT},    { "<=", PT_LE},    		{ "+", PT_PLUS},
       { "-", PT_MINUS},    { "*", PT_TIMES},      { "/", PT_DIV},
         { "%", PT_MOD},      { "&", PT_AND},     { "|", PT_OR},
         { "~", PT_NOT},      { ".", PT_DOT}
};



static std::map<punctuation, std::string > num_char={
 	  {PTX, "?WHAT?"},
     {PT_SEMI, ";" }, { PT_EQUALS, "="  },   {PT_COLON, ":" } ,
       {PT_LPAREN, "(" },   {PT_LBRAKT,  "[" },   {PT_LBRACE, "{" },
      { PT_RPAREN, ")"},   { PT_RBRAKT, "]"},   { PT_RBRACE, "}"},
       { PT_COMMA, ","},  {PT_ATSIGN, "@"},    { PT_ELIPS, ".."},
      { PT_NOTEQL, "/="},     { PT_GT, ">"},      {PT_GE, ">="},
      { PT_LT, "<"},    { PT_LE, "<="},    		{ PT_PLUS, "+"},
       {PT_MINUS, "-"},    { PT_TIMES, "*"},      { PT_DIV, "/"},
         { PT_MOD, "%"},      { PT_AND, "&"},     { PT_OR, "|"},
         { PT_NOT, "~"},      { PT_DOT, "."}
};




/*
static std::map<std::string, int > char_num={
 	 {"?WHAT?", 0},
     {";" , 1 }, { "=",  2 },  {":", 3} ,
      {"(", 4 },   { "[", 5},   { "{", 6},
      { ")", 7},   { "]", 8},   { "}", 9},
       { ",", 10},  {"@", 11},    { "..", 12},
      { "/=", 13},     { ">", 14},      {">=", 16},
      { "<", 17},    { "<=", 18},    		{ "+", 19},
       { "-", 20},    { "*", 21},      { "/", 22},
         { "%", 23},      { "&", 24},     { "|", 25},
         { "~", 26},      { ".", 27}
};
*/

// the current lexeme and its successor
EXTERN lexeme lex_this;
EXTERN lexeme lex_next;

void lex_open( FILE * f );
//	open f if it exists or use stdin if not passed
//	initialize lex_this and _next to the first lexemes of the input

void lex_advance();
//	update lex_this and _next one step in lexical analysis

void lex_put( lexeme * lex, FILE * f );
//	output reconstruction of lex to file f

int _get_decimal_value(char ch);
// used for converting other bases into decimal form


/* Prints the punc  */
void put_punct( FILE * f, punctuation punc );


/* Function to return:
	1- handle to lexeme if the lexeme is str/Cstr/id
	2- value of the lexeme if it is keyword/punct/num
*/
uint32_t get_val(lexeme lex);

#undef EXTERN
