// main.h -- interface for main program
#pragma once
#include "codegenerator.h"
// version Oct. 29, 2018
// skeleton code from Doug Jones' Oct 3 lecture

// In main.cpp, but nowhere else, define EXTERN first
#ifndef EXTERN
	#define EXTERN extern
#endif

EXTERN const char * main_progname; // program name
EXTERN const char * main_infile;   // input file name, NULL if stdin
EXTERN const char * main_outfile;  // output file name, NULL if stdiout



#undef EXTERN
