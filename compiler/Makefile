# Makefile for kestrel compiler project

# Aug 27, 2018 -- initial version by Douglas W. Jones
# Dec 13, 2018 -- current version by Group A

# instructions:
#   make kestrel -- make kestrel
#   make testlex -- makes lexical analyzer test
#   make clean -- remove all object and executable files created by make

# internal kestrel compiler components
CXXFLAGS += -std=c++11

# make kestrel

lexstuff = lexical.o symboltable.o stringpool.o puncmap.o error.o keywords.o
par1 =  lexsupport.o term.o factor.o comparand.o subrange.o expression.o if.o while_loop.o do_end.o assignment.o statement.o block.o value.o const_decl.o declaration.o environment.o type.o reference.o element.o codegenerator.o
kestrel: main.o $(lexstuff) $(par1) $(codgen)
	g++ -std=c++11 -o kestrel main.o $(lexstuff) $(par1) $(codgen)

# Lexical stuff
stringpool.o: stringpool.cpp stringpool.h
	g++ -std=c++11 -c stringpool.cpp
	
symboltable.o: symboltable.cpp symboltable.h stringpool.h
	g++ -std=c++11 -c symboltable.cpp

puncmap.o: puncmap.cpp puncmap.h
	g++ -std=c++11 -c puncmap.cpp

lexsupport.o: lexsupport.cpp lexsupport.h
	g++ -std=c++11 -c lexsupport.cpp

lexical.o: lexical.cpp lexical.h charclass.h symboltable.h stringpool.h puncmap.h
	g++ -std=c++11 -c lexical.cpp

error.o: error.cpp error.h
	g++ -std=c++11 -c error.cpp
	
keywords.o: keywords.cpp keywords.h 
	g++ -std=c++11 -c keywords.cpp
	

factor.o: factor.cpp parser.h
	g++  -std=c++11 -c factor.cpp 
	
term.o: term.cpp parser.h
	g++  -std=c++11 -c term.cpp 


comparand.o: comparand.cpp parser.h
	g++ -std=c++11 -c comparand.cpp 


expression.o: expression.cpp parser.h 
	g++ -std=c++11 -c expression.cpp 

assignment.o: assignment.cpp parser.h
	g++ -std=c++11 -c assignment.cpp 
	
	
statement.o: statement.cpp parser.h
	g++ -std=c++11 -c statement.cpp 

block.o: block.cpp parser.h
	g++   -std=c++11 -c block.cpp 
	
	
value.o: value.cpp parser.h
	g++   -std=c++11 -c value.cpp 
	
type.o: type.cpp parser.h
	g++ -std=c++11 -c type.cpp

declaration.o: declaration.cpp parser.h
	g++ -std=c++11 -c declaration.cpp
	
environment.o: environment.cpp parser.h
	g++ -std=c++11 -c environment.cpp

reference.o: reference.cpp parser.h
	g++ -std=c++11 -c reference.cpp 

const_decl.o: const_decl.cpp parser.h
	g++ -std=c++11 -c const_decl.cpp 

element.o: element.cpp parser.h
	g++ -std=c++11 -c element.cpp

subrange.o: subrange.cpp parser.h
	g++ -std=c++11 -c subrange.cpp

if.o: if.cpp parser.h
	g++ -std=c++11 -c if.cpp


while_loop.o: while_loop.cpp parser.h
	g++ -std=c++11 -c while_loop.cpp


do_end.o: do_end.cpp parser.h
	g++ -std=c++11 -c do_end.cpp

# code generator
codegenerator.o: codegenerator.cpp codegenerator.h
	g++ -std=c++11 -c codegenerator.cpp

# make testlex -- test scaffolding
testlex.o: testlex.cpp lexical.h 
	g++ -std=c++11 -c testlex.cpp

testlex: testlex.o $(lexstuff)
	g++ -std=c++11 -o testlex testlex.o $(lexstuff)

# make clean
clean: 
	rm *.o
	rm -f testlex
	rm -f kestrel
