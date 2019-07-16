#pragma once



#ifndef EXTERN
        #define EXTERN extern
#endif


typedef enum{

  REF, ENUM, SUBR, POINTER, ARRAY, SET, RECORD
} decl_type;

class Environment{
	public:
		Environment * next;
		decl_type d_type;


};
