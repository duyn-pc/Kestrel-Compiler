/*codegenerator.h*/
#pragma once
/******************************************
 * Assembly Code Generator for ARM        *
 * Author:  Jon A. Stoltz                 *
 * Based off lecture notes By Prof. Doug  *
 * Jones. This is the first revision      *
 ******************************************/
#include "sets.h"
#include <stdio.h>
#include <stdint.h>
#include <string>

//=BUG= we should make these defines overidable by compiler options
/*Defines pertaining to the stack top cache. REG_CACHE_SIZE is the
number of registers used for the cache. MAX_REG is the number
identifier for the highest number register used, and MIN_REG is the
lowest number register used*/
#define REG_CACHE_SIZE 6
#define MAX_REG 9
#define MIN_REG 4

/*CodeGenerator class. Class can be initialized with a file handle to
an output file, or if not, output if directed to stdout. As currently
designed, expression compiling should use the top stack cache for
faster processing. Local variables are currently stored in the .bss data
block and a symbol in the form of P'int' is used to point to begining
of the memory for the variable, instead of being stored on the stack.
Strings local or global are stored in the .data block. Therefor all
variables are reachable from anywhere in the file, and the activation
record in the parser should keep track of which variables are reachable
in the current scope. Eventually I would like to use the .bss as a
managed memory block*/
class CodeGenerator {
    public:
        //Constructors
        CodeGenerator();
        CodeGenerator(FILE * out);

        //Codegenerator Public member functions
        //Implement as needed for compiler calls
        //See .cpp file for descriptions of the functions
        void pushi( int c );
        void add();
        int32_t pushl(int32_t x);
        int32_t pushOnStack(int32_t x);
        void popl(int32_t x);
        void pushAddress(int32_t x);
        void pushBranchAddress(int32_t x);
        void saveTopAtLabel(int32_t x);
        void pushga(int32_t a);
        void load();
        void loadhs();
        void loadhu();
        void loadbs();
        void loadbu();
        void pop();
        void pops();
        void popsh();
        void popsb();
        void dup();
        void sub();
        void mul();
        void div();
        void mod();
        void gt();
        void lt();
        void ge();
        void le();
        void eq();
        void btrue(int32_t dst);
        void bfalse(int32_t dst);
        void br(int32_t dst);
        int32_t label();
        int32_t pointerOnStack();
        void pushla(int32_t d);
        void neg();
        void Not();
        void Or();
        void And();
        void eor();
        void startActRec();
        void endActRec();
        void finalize();
        void rangecheck(int value, int Min, int Max);
        std::string allocateLable();
        std::string allocateLable(int words);
        std::string branchLable();
        int32_t putIntInMemory(int32_t value);
        void putStrChar(int Char, int label, int Offset);
        int32_t PutStringInMemory(std::string str);
        void pushPointerLabelVal(int32_t label);
        void bNotZero(int32_t dst);
        void printIntLine(int32_t label);
        void printInt(int32_t label);
        void printString(int32_t label);

    private:
        //variables for the stack top cache
        int maxReg, minReg, countReg, topReg;
        //set for the stack top registers
        set32_t stack_regs;
        //File handle for assembly output
        FILE* outfile;
        //initialize code output for arm
        void init();
        /*A counter for labels in the format B(int32_t), it only ever
        increases for an assembly file. Used to identify branch labels*/
        int32_t labelCount;
        /*A counter for labels in the format P(int32_t), it only ever
        increases for an assembly file. Used to identify labels for
        local and global variables */
        int32_t pointerCount;
};
