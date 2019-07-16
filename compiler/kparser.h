#ifndef EXTERN
	#define EXTERN extern
#endif
#pragma once
#include "lexical.h"

typedef enum {
	p_exp,    // expression
	p_fact,   // factor
	p_term,   // term
	p_num,    // number
    p_assign, // assignment
    p_eof     // EOF
} ptype;

typedef struct {
	lextypes type;
	int value;
	int line;
	int column;
} parseme;

extern parseme parse_this;
extern parseme parse_next;
//extern ptype current_type;

void parse_advance();

void get_lex(FILE * f);

void parse_table();

void parse_open(FILE * f);

#undef EXTERN
