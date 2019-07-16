#pragma once
//#include"codegenerator.h"
#include "main.h"
#include<string>
#include<iostream>

#ifndef EXTERN
        #define EXTERN extern
#endif
#include "codegenerator.h"

using namespace std;
//Forward declaration.
class Environment;
class Type;
//static Type* Type::compile(Environment* e, string id);

//class CodeGenerator;
EXTERN CodeGenerator *cg;

//#include "environment.h"


typedef enum {ISPUBLIC, ISPRIVATE, ISRESTRICTED} accessibility;

typedef enum {ISCONST, ISTYPE, ISEXCEPT, ISVAR, ISPROC, ISFUNC} declarator_kind;

typedef enum {ISSUBRANGE, ISREF, ISENUM, ISPOINTER, ISARRAY, ISSET, ISREC } type_kinds;

typedef enum{STRING, INTEGER} base_type;

typedef enum{FINAL, CONST} const_d_type;



/////// DECLARATION CLASSES

class Declarator {
public:
	Declarator * next;
	virtual declarator_kind kind()=0;
};


class Declaration{
public:
//Attributes

static Declaration* compile(Environment * e);

};



class Const_decl: public Declarator {
public:

//We only construct an object of const_decl after evaluating the expressions
 int int_val;
  string str_val;
  base_type bt;
	Type* type_ptr;
	accessibility access;
	const_d_type const_t;

Const_decl(int v);
Const_decl(string s);

declarator_kind kind();

static Declarator* compile(Environment * e, string id, accessibility access, const_d_type cdt);

};





class Type_decl: public Declarator {
public:
	//declarator_kind kind();
	virtual type_kinds type_kind() = 0;

	static Declarator * compile(Declarator * e, string id);
};
//declarator_kind Type_decl::kind() { return ISTYPE; }

////Derived Types classes


class Subrange: public Type_decl {
public:
	type_kinds type_kind() ;
	string name;
	int32_t min;
	int32_t max;
	declarator_kind kind();
	Subrange();
	Subrange(string id,int32_t min, int32_t max );

	static Declarator * compile(Environment *e, string id);
};



class  Except_decl: public Declarator {
public:
	declarator_kind kind();
static void  compile(Environment * e, string id);

};
//declarator_kind Except_decl::kind() { return ISEXCEPT ; }


class Var_decl: public Declarator {
public:
	declarator_kind kind();
static void compile(Environment * e, string id);

};
//declarator_kind Var_decl::kind() { return ISVAR; }


class Proc_decl: public Declarator {
public:
	declarator_kind kind();
static void compile(Environment * e, string id);


};
//declarator_kind Proc_decl::kind() { return ISPROC ; }


class Func_decl: public Declarator {
public:
	declarator_kind kind();
static void compile(Environment * e, string id);

};
//declarator_kind Func_decl::kind() { return ISFUNC ; }

//////////////////////////////////////////////////////////////
/////// NON Declaration classes /////////////////////////////
////////////////////////////////////////////////////////


class Type{ //Needed to be encapsulated within  Environment
public:
	string name;
	Declarator* type_ptr; //Points to type declarator
	Type* next;
	Type(string name, Declarator* type_ptr);
	static Type* compile(Environment* e, string id);
};

class Element{
public:
	string name;//what if pointer?
	Type* type_ptr;
	accessibility access;
	const_d_type const_t;
	Element* next;
	int32_t label; //generated label by code generator.

	base_type bt;
	int int_val;
	string str_val;

	Element(string name, Type* ptr ,  accessibility access, int32_t label ) ;
	Element(string name, Type* ptr , accessibility access, int32_t label, base_type bt, const_d_type const_t) ; //if it is a constant type, then we need a value
	// the value could be int or a string. Need base_tybe
																						  // to know which container use: int_val or str_val
	int32_t get_label();
	declarator_kind kind();

};

class Block {
public:
////attributes
	Declarator * head;
    static Block * compile( Environment * e );
        // factory method

        // =BUG= blocks certainly have attributes, they are missing here
};

class Reference{

public:
///attributes
Element* refersto;
Reference();
Reference(Element* refto); //Takes a pointer ot element to return it for caller.
void assign_label(int32_t l);
int32_t get_label();
static Reference *compile( Environment * e );

};


class Statement{

public:
///attributes

static Statement * compile( Environment * e );

};

class Assignment{

public:
///attributes

static Assignment *compile( Environment * e );
};


class Expression{

public:
///attributes
  //int value --> needed if we compute the value at compile time
  int int_val;
  string str_val;

  bool constant;

  Type* type_ptr;

  int32_t label; //this will be the return label for place where the result of the exp is saved in memory

  base_type bt;

  Expression(int v, base_type b);
  Expression(string s, base_type b);
  Expression(Type *p);
    Expression(int32_t label); //used to return the label of the result
  Expression();


  static Expression *compile( Environment * e, bool gen_code );
};




class Comparand{
public:
///attributes
  //int value --> needed if we compute the value at compile time
  int int_val;
  string str_val;
  base_type bt;
    bool constant;
    Type* type_ptr;
     int32_t label; //this will be the return label for place where the result of the comparand is saved in memory

  Comparand(int v, base_type b);
  Comparand(string v, base_type b);
  Comparand();
  Comparand(int32_t l);
  Comparand(Type *p);
static Comparand *compile( Environment * e , bool gen_code );

};



class Term{
public:
///attributes
  //int value --> needed if we compute the value at compile time
 int int_val;
  string str_val;
  base_type bt;
    bool constant;
      Type* type_ptr;
        int32_t label; //this will be the return label for place where the result of the term is saved in memory
  Term(int v, base_type b);
    Term(string v, base_type b);
  Term();
  Term(int32_t l);
  Term(Type *p);
static Term *compile( Environment * e , bool gen_code );

};



class Factor{
public:
///attributes
  //int value --> needed if we compute the value at compile time
 int int_val;
  string str_val;
  base_type bt;
    bool constant;
      Type* type_ptr;
       int32_t label; //this will be the return label for place where the result of the factor is saved in memory
  Factor(int v, base_type b);
  Factor(string v, base_type b);
  Factor();
  Factor(int32_t l);
  Factor(Type *p);
static Factor * compile( Environment * e , bool gen_code );
};



class Value{
public:
///attributes
  //int value --> needed if we compute the value at compile time
  int int_val;
  string str_val;
  base_type bt;
  	bool constant;
  	  Type* type_ptr;
  	  int32_t label; //this will be the return label for place where the result of the value is saved in memory
  Value(int32_t l);
  Value(int v, base_type b);
   Value(string v, base_type b);
  Value();
  Value(Type *p);
static Value * compile( Environment * e , bool gen_code );
};


class Environment{
	public:
	Type* types; //Points to the list of types--may not be needed if each element points to its type
	Element* elements; /*points to elements in the environment; it points to last declared element in the environment
						or, if no elements declared within this environment, it points to the last declared
						*/
	Environment(Type* t, Element* e);
	Type* type_lookup(string name); //
	void type_add(Type* t);
	Element* element_lookup(string name);
	void element_add(Element* e);
};



class Do_until{
	public:
	static void compile(Environment* e);

};

class If{
public:
static void compile( Environment * e );
};



class While{
public:
static void compile( Environment * e );
};



static void error_warn( string message, lexeme lex ) {
        /* output message er and allow processing to continue */
        cout<<"Error on line "<< lex.lineNum<<": "<<message<<endl;
}
