//#include "config.h"
//#include "sets.h"
//#include "error.h"
//#include "stringpool.h"
//#include "symboltable.h"
#include "keywords.h"
//#include "lexical.h"
#include "lexsupport.h"


#ifndef EXTERN
        #define EXTERN extern
#endif

/*
#include"assignment.h"
#include"statement.h"
*/

#include"parser.h"

///Start and follow sets

// start sets

Subrange::Subrange(){}

type_kinds Subrange::type_kind() { return ISSUBRANGE; }


declarator_kind Subrange::kind() { return ISTYPE; }

Subrange::Subrange(string id,int32_t min, int32_t max ){
	this->min = min;
	this->max = max;
	this->name = id;
}


 Declarator* Subrange::compile(Environment *e, string id){


return NULL;
}