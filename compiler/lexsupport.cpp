
#include"lexsupport.h"





bool lex_isinset( set32_t ps, set32_t ks, set32_t ls ) {
        /* test if lex.this is punct in ps, keyword in ks, or lexeme in ls */
        return lex_ispuncset( lex_this, ps )
        ||     lex_iskeyset( lex_this, ks )
        ||     lex_isset( lex_this, ls );
}


 /* complain about error */
 void       lex_gotbutwant( lexeme lex, error_message e ){


 }

void lex_wantinset( set32_t ps, set32_t ks, set32_t ls, error_message e ) {
        /* test if lex.this is punct in ps, keyword in ks, or lexeme in ls */
        if (lex_isinset( ps, ks, ls )) return;

        /* complain about error */
        lex_gotbutwant( lex_this, e );

        /* =BUG= should probably skip ahead until we find something in set */
        /*       or take some other action to recover from the error       */
}

bool lex_iskeyset( lexeme lex, set32_t s ) {
        if (lex.type == lex_key) {
                return in_set32(key_lookup(lex.val), s);
        }
        else return false;
}

bool lex_isset( lexeme lex, set32_t s ) {
        return in_set32(lex.type, s);
}
