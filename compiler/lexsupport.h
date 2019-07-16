#pragma once
#include"lexical.h"
#include"sets.h"
#include"error.h"
#include"keywords.h"
///////=BUG= DEBUG: needs rewriting and adding some of the
///////

/* bool lex_ispuncset( lexeme lex, set32_t s ); */
#define lex_ispuncset( lex, s ) (                               \
        (lex.type == lex_punc) && in_set32(lex.val, s )       \
)

/* bool lex_ispunc( lexeme lex, punct_type t ); */
#define lex_ispunc( lex, t ) (                               \
        (lex.type == lex_punc) && (lex.val == t)              \
)


bool lex_isset( lexeme lex, set32_t s );


bool lex_iskeyset( lexeme lex, set32_t s );

bool lex_isinset( set32_t ps, set32_t ks, set32_t ls );

 /* complain about error */
void       lex_gotbutwant( lexeme lex, error_message e );

void lex_wantinset( set32_t ps, set32_t ks, set32_t ls, error_message e );
        /* test if lex.this is punct in ps, keyword in ks, or lexeme in ls*/
