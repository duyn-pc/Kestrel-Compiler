//#include "config.h"
//#include "sets.h"
//#include "error.h"
//#include "stringpool.h"
//#include "symboltable.h"
#include "keywords.h"
//#include "lexical.h"
#include "lexsupport.h"

/*
#ifndef EXTERN
#define EXTERN
*/
#ifndef EXTERN
        #define EXTERN extern
#endif

#include"parser.h"

///Start and follow sets


Environment::Environment(Type* t, Element* e){
	this->types = t;
	this->elements = e;
	
}


Type* Environment::type_lookup(string name){
	
	if(types == NULL)
		return NULL;
		
	Type* temp = this->types;
	while(temp != NULL){
		if(temp->name == name)
			return temp;
		temp = temp->next;
	}
	return NULL;
	
} //

//Assumption is the type is already constructed.
void Environment::type_add(Type* t){
	if(t!=NULL){ //add to front
		t->next = types;
		types = t;
	}
}

Element* Environment::element_lookup(string name){
	
	if(elements == NULL)
		return NULL;
	
	Element* temp = this->elements;
	
	while(temp != NULL){
		if(temp->name == name)
			return temp;
		temp = temp->next;
	}
	return NULL;
}

//Assumption is the element is already constructed.
void Environment::element_add(Element* e){
	if(e != NULL){
		e->next = elements;
		elements = e;
	}
	
}