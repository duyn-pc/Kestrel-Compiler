#pragma once
// Interface for Punc Map module

// version Oct 6, 2018 by Duy

#include <map>
#include <iterator>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* The map of the punctuations, enter punc get number */
using namespace std;

/*
static std::map<std::string, int>punc_val = {
	{";",  0}, {"(", 1}, {")",  2}, {"(",  3}, {",", 4},
	{"/=", 5}, {"<", 6}, {"-",  7}, {"%",  8}, {"~", 9},
	{"=", 10}, {"[",11}, {"]", 12}, {"@", 13}, {">",14},
	{"<=",15}, {"*",16}, {"&", 17}, {".", 18}, {":",19},
	{"{", 20}, {"}",21}, {"..",22}, {">=",23}, {"+",24},
	{"/", 25}, {"|",26}
};
*/

//static std::map<std::string, punctuation > char_num={
static std::map<std::string, int>punc_val = {
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

/* Same thing as above, but in reverse */
static std::map<int,std::string>punc_string = {
	{0,  ";"}, {1, "("}, {2,  ")"}, {3,  "("}, {4, ","},
	{5, "/="}, {6, "<"}, {7,  "-"}, {8,  "%"}, {9, "~"},
	{10, "="}, {11,"["}, {12, "]"}, {13, "@"}, {14,">"},
	{15,"<="}, {16,"*"}, {17, "&"}, {18, "."}, {19,":"},
	{20, "{"}, {21,"}"}, {22,".."}, {23,">="}, {24,"+"},
	{25, "/"}, {26,"|"}
};

/* Returns the map number of a punctuation */
int get_punc_val(std::string input );

/* Return the punc from the map given its value */
std::string get_punc( int input );

/* Prints the punc or return its handle */
void put_punc( FILE * f, int punc );
