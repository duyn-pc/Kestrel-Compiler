// charclass.h -- character classifier
//   there is no charclass.c
//   this defines char_type, an enumeration of character classes
//   plus, at the end, ISCLASS( ch, class ) to test for a character in a class
//   or set of classes

// version Sept 3, 2018 by Douglas W. Jones, ripped from course notes
#pragma once

// the following are the public names of character classes
typedef enum {
	OTHER=0, WHITESPACE=1, LETTER=2, DIGIT=4, PUNCTUATION=8
} char_type;

// short definitions of character types used in the classifier table
#define OTH OTHER
#define WIT WHITESPACE
#define LET LETTER
#define DIG DIGIT
#define PUN PUNCTUATION

// character classifier table
//   note, this table should never be referenced outside of this header file
static const char_type _char_class[256] = {
     // NUL SOH STX ETX EOT ENQ ACK BEL BS  HT  LF  VT  FF  CR  SO  SI
        OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,WIT,WIT,WIT,WIT,WIT,OTH,OTH,
     // DLE DC1 DC2 DC3 DC4 NAK SYN ETB CAN EM  SUB ESC FS  GS  RS  US
        OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,
     //      !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /
        WIT,OTH,OTH,OTH,OTH,PUN,PUN,OTH,PUN,PUN,PUN,PUN,PUN,PUN,PUN,PUN,
     //  0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?
        DIG,DIG,DIG,DIG,DIG,DIG,DIG,DIG,DIG,DIG,PUN,PUN,PUN,PUN,PUN,OTH,
     //  @   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O
        PUN,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,
     //  P   Q   R   S   T   U   V   W   X   Y   Z   [   \   ]   ^   _
        LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,PUN,OTH,PUN,OTH,OTH,
     //  `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o
        OTH,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,
     //  p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~  DEL
        LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,LET,PUN,PUN,PUN,PUN,OTH,
     // beyond ASCII
        OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,
        OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,
        OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,
        OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,
        OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,
        OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,
        OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,
        OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH,OTH
};

// undefine the short forms used above
#undef OTH
#undef WIT
#undef LET
#undef DIG
#undef PUN

// check whether a character is in a particular class other than OTH
#define ISCLASS(ch,class) (_char_class[ch]&(class))

// because the public names of character classes are powers of two it is
// legal to write things like  ISCLASS( ch, LETTER|DIGIT )
