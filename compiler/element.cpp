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

#include"parser.h"
///Start and follow sets


// code generator code to generate a label for any element--to be used in machine code
// void CodeGenerator::label(int32_t dst) 
Element::Element(string name, Type* ptr , accessibility access, int32_t label ) {
	this->name = name;
	this->type_ptr = ptr;
	this->access = access;
	this->label = label;

}


Element::Element(string name, Type* ptr , accessibility access,  int32_t label, base_type bt, const_d_type const_t) { //if it is a constant type, then we need a value
	this->name = name;
	this->type_ptr = ptr;
	this->access = access;
	this->label = label;
	this->bt = bt;
	this->const_t = const_t; //const or final
																						  
																						  
}

int32_t Element::get_label(){
	return label;
}

declarator_kind Element::kind(){

	return this->type_ptr->type_ptr->kind();
}
	