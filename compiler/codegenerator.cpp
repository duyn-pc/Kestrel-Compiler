#include "codegenerator.h"
#include <stdio.h>
#include <iostream>
#include <exception>
#include <stdint.h>
#include <string>


CodeGenerator::CodeGenerator() {
/*Constructor for the code generator using stdout for assembly
  output.
  Global:
  MAX_REG, used to set the highest register used in cache
  MIN_REG, used to set the lowest register used in cache */
    this->outfile = stdout;
    maxReg = MAX_REG;
    minReg = MIN_REG;
    countReg = 0;
    topReg = 4;
    labelCount = 0;
    pointerCount = 0;
    //=BUG= we should really find a way to dynamically set this
    stack_regs = to_set32_6(4,5,6,7,8,9);
    this->init();
}


CodeGenerator::CodeGenerator(FILE * out) {
/*Constructor for code generator
  Given:
  out, a file handle with write privilages
  Global:
  MAX_REG, used to set the highest register used in cache
  MIN_REG, used to set the lowest register used in cache */
    this->outfile = out;
    maxReg = MAX_REG;
    minReg = MIN_REG;
    countReg = 0;
    topReg = 4;
    labelCount = 0;
    pointerCount = 0;
    //=BUG= we should really find a way to dynamically set this
    stack_regs = to_set32_6(4,5,6,7,8,9);
    this->init();
}

void CodeGenerator::init() {
    fprintf(outfile,"\t.arch armv6\n");
	fprintf(outfile, "\t.eabi_attribute 27, 3\n");
	fprintf(outfile, "\t.eabi_attribute 28, 1\n");
	fprintf(outfile, "\t.fpu vfp\n");
	fprintf(outfile, "\t.eabi_attribute 20, 1\n");
	fprintf(outfile, "\t.eabi_attribute 21, 1\n");
	fprintf(outfile, "\t.eabi_attribute 23, 3\n");
	fprintf(outfile, "\t.eabi_attribute 24, 1\n");
	fprintf(outfile, "\t.eabi_attribute 25, 1\n");
	fprintf(outfile, "\t.eabi_attribute 26, 2\n");
	fprintf(outfile, "\t.eabi_attribute 30, 6\n");
	fprintf(outfile, "\t.eabi_attribute 18, 4\n");
	fprintf(outfile, "\t.file\t\"test.c\"\n");
	fprintf(outfile, ".data\nINTSTR:\t.asciz\t\"%%d\"\n");
	fprintf(outfile, "INTSTRL:\t.asciz\t\"%%d\\n\"\n");
	fprintf(outfile, "\t.text\n");
	fprintf(outfile, "\t.align	2\n");
	fprintf(outfile, "\t.extern printf\n");
	fprintf(outfile, "\t.global\tmain\n");
	fprintf(outfile, "\t.type\tmain, %%function\n");
    fprintf(outfile, "main:\n");
}

void CodeGenerator::finalize() {
    fprintf(outfile, "\t.size\tmain, .-main\n");
	fprintf(outfile, "\t.ident\t\"GCC:\t(Debian 4.6.3-14+rpi1) 4.6.3\"\n");
	fprintf(outfile, "\t.section\t.note.GNU-stack,\"\",%%progbits\n\n");
}
void CodeGenerator::pushAddress(int32_t x) {
    //like pushi but we load an address from a label in the top reg
    int r;
    if (countReg == 0) r = topReg;
    else {
        r = topReg + 1;
        if (r > maxReg) r = minReg;
    }
    if (countReg == REG_CACHE_SIZE) { /* we need to clear a register */
        //=BUG= someone confirm I am doing this right
        fprintf(outfile, "\tsub\tsp, sp, #4\n\tstr\tr%d, [sp]\n", r);
        countReg = countReg - 1;
    }
    //=BUG= someone confirm this
    fprintf(outfile, "\tldr\tr%d, =P%d\n", r, x);
    topReg = r;
    countReg = countReg + 1;
}

void CodeGenerator::pushBranchAddress(int32_t x) {
    //like pushi but we load an address from a label in the top reg
    int r;
    if (countReg == 0) r = topReg;
    else {
        r = topReg + 1;
        if (r > maxReg) r = minReg;
    }
    if (countReg == REG_CACHE_SIZE) { /* we need to clear a register */
        //=BUG= someone confirm I am doing this right
        fprintf(outfile, "\tsub\tsp, sp, #4\n\tstr\tr%d, [sp]\n", r);
        countReg = countReg - 1;
    }
    //=BUG= someone confirm this
    fprintf(outfile, "\tldr\tr%d, P%d\n", r, x);
    topReg = r;
    countReg = countReg + 1;
}

void CodeGenerator::pushi( int c ){
    //used for expression parsing to push
    int r;
    if (countReg == 0) r = topReg;
    else {
        r = topReg + 1;
        if (r > maxReg) r = minReg;
    }
    if (countReg == REG_CACHE_SIZE) { /* we need to clear a register */
       //=BUG= someone confirm I am doing this right
        fprintf(outfile, "\tsub\tsp, sp, #4\n\tstr\tr%d, [sp]\n", r);
        countReg = countReg - 1;
    }
    //=BUG= someone confirm this
    fprintf(outfile, "\tldr\tr%d, #%d\n", r, c);
    topReg = r;
    countReg = countReg + 1;
}

int32_t CodeGenerator::putIntInMemory(int32_t val) {
    //put an integer in memory return a lable
    fprintf(outfile, ".data\n");
    fprintf(outfile, "P%d:\n\t.word %d\n", pointerCount, val);
    fprintf(outfile, ".text\n");
    return pointerCount++;
}

int32_t CodeGenerator::pushOnStack(int32_t value) {
    //push one word of space on the stack
    fprintf(outfile, "\tsub\tsp, sp, #4\n");
    fprintf(outfile, "\tsub\tsp, sp, #4\n");
    fprintf(outfile, "\tstr\tr0, [sp]\n");
    fprintf(outfile, "\tldr\tr0, #%d\n", value);
    fprintf(outfile, "\tstr\tr0, [sp, #4]\n");
    fprintf(outfile, "\tldr\tr0, [sp]\n");
    fprintf(outfile, "\tadd\tsp, sp, #4\n");
    int32_t temp;
    temp = pointerOnStack();
    return temp;
}

void CodeGenerator::add(){
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= emit code to add R[r] plus R[top] and put the result in R[r]
    fprintf(outfile, "\tadd\tr%d, r%d, r%d\n", r, r, topReg);
    topReg = r;
    countReg = countReg - 1;
}

int32_t CodeGenerator::pushl(int32_t x){
    /*PUSH Local; x words of uninitialized memory are pushed onto the stack to reserve space for a local variable. Used for storage allocation.
    sp = sp - x; Returns an interger representing a label in assemble associated with the start of the stack memory block.
    (L(int32_t)) is the lable */
    if (x < 2) return -1;
    int32_t labelint = labelCount;
    labelCount++;
    fprintf(outfile, "\tsub\tsp, sp, #4\n");
    fprintf(outfile, "p%d equ sp\n", labelint);
    //=BUG= should have some overflow protection, probably ok
    int32_t words = (x - 1) * 4;
    fprintf(outfile, "\tsub\tsp, sp, #%d\n", words);
    return labelint;
}

void CodeGenerator::pop() {
    if (countReg == 0) {
        fprintf(outfile, "\tadd\tsp, sp, #4\n");
    } else if (countReg == 1) {
        countReg --;
    }else {
        topReg--;
        if (topReg < MIN_REG) topReg = MAX_REG;
        countReg--;
    }
}

void CodeGenerator::popl(int32_t x) {
    /*POP Local; x words of memory are popped from the stack. Used for local storage deallocation.
    sp = sp + x;*/
    //=BUG= we should really have some overflow protection here but
    //it probably won't matter
    if (x < 0) return;
    x *= 4;
    fprintf(outfile, "\tadd\tsp, sp, #%d\n", x);
}

void CodeGenerator::pushga(int32_t a) {
    //PUSH Global Address; the one-word address a is pushed onto the stack, where A is the symbolic name of a global variable. In a real machine, the loader would convert PUSHGA to PUSHI after substituting the actual memory address of the global variable for its name.
    int r;
    if (countReg == 0) r = topReg;
    else r = topReg + 1;
    if (r > maxReg) r = minReg;
    if (countReg == REG_CACHE_SIZE) { /* we need to clear a register */
        //=BUG= someone confirm I am doing this right
        fprintf(outfile, "\tsub\tsp, sp, #4\n\tstr\tr%d, [sp]\n", r);
        countReg = countReg - 1;
    }
    fprintf(outfile, "\tldr\tr%d,=P%d\n", r, a);
    topReg = r;
    countReg = countReg + 1;
}

void CodeGenerator::load() {
    /*LOAD onto the stack top the contents of the one-word memory location addressed by the stack top. The stack pointer is unchanged.
    M[sp] = M[M[sp]];*/
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    fprintf(outfile, "\tldr\tr%d, [r%d]\n", topReg, topReg);
}

void CodeGenerator::loadhs() {
    //=BUG= Stub for loadhs
    throw ("Not Implemented");
}

void CodeGenerator::loadhu() {
    //=BUG= Stub for loadhu
    throw ("Not Implemented");
}

void CodeGenerator::loadbs() {
    //=BUG= Stub for loadbs
    throw ("Not Implemented");
}

void CodeGenerator::loadbu() {
    //=BUG= Stub for loadbu
    throw ("Not Implemented");
}
//Later, when thinking about operands in memory that occupy fractional words,
//we may want to introduce halfword and byte load operands. Each of these may require a signed and an unsigned version. For now, however, we assume that we are only using one-word variables. The difference between signed and unsigned load operations lies in whether they sign-extend their operand to place it into a full-word stack top, or alternatively, pad the operand with zero in its high bits.

void CodeGenerator::pops() {
    /*POP the data on the stack top and Store it in the memory address
    referenced by the address popped from the stack top. A total of two words
    are popped from the stack; the data was on top of the address.
    temp = M[sp++];
    M[M[sp++]] = temp;*/
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    fprintf(outfile, "\tstr\tr%d, [r%d]\n", topReg, r);
    topReg--;
    countReg--;
    if (topReg < minReg) topReg = maxReg;
    if (countReg != 1) {
        topReg--;
        if (topReg < minReg) topReg = maxReg;
    }
    countReg--;
}

void CodeGenerator::popsh() {
    //=BUG= Stub for popsh
    throw ("Not Implemented");
}

void CodeGenerator::popsb() {
    /*Later, when thinking about operands in memory that occupy fractional
    words, we may want to introduce pop operations that store bytes or
    halfwords, discarding the high bits of the operand on the stack. The store
     operations for signed and unsigned operands do not differ.*/
    //=BUG= Stub for popsb
    throw ("Not Implemented");
}

void CodeGenerator::dup() {
    /*DUPlicate the item on the stack top. Don't include this in your
    intermediate high-level architecture unless you find you need it, but in
    some cases, it may be useful. M[--sp] = M[sp];*/
    //=BUG= Stub for dup
    throw ("Not Implemented");
}

void CodeGenerator::sub() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= emit code to add R[r] plus R[top] and put the result in R[r]
    fprintf(outfile, "\tsub\tr%d, r%d, r%d\n", r, r, topReg);
    topReg = r;
    countReg = countReg - 1;
}

void CodeGenerator::mul() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= emit code to add R[r] plus R[top] and put the result in R[r]
    fprintf(outfile, "\tmul\tr%d, r%d, r%d\n", r, r, topReg);
    topReg = r;
    countReg = countReg - 1;
    //=BUG= Stub for mul
    //throw new exception("Not Implemented");
}

void CodeGenerator::div() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= emit code to add R[r] plus R[top] and put the result in R[r]
    fprintf(outfile, "\tdiv\tr%d, r%d, r%d\n", r, r, topReg);
    topReg = r;
    countReg = countReg - 1;
    //=BUG= Stub for div
    //throw ("Not Implemented");
}

void CodeGenerator::mod() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= emit code to add R[r] plus R[top] and put the result in R[r]
    fprintf(outfile, "\tmod\tr%d, r%d, r%d\n", r, r, topReg);
    topReg = r;
    countReg = countReg - 1;
    //=BUG= Stub for mod
    //throw ("Not Implemented");
}
//Other arithmetic operators are similar to ADD.

void CodeGenerator::gt() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= check comparison
    fprintf(outfile, "\tcmp\tr%d, r%d\n\tmovgt\tr%d, #1\n\tmovle\tr%d, #0\n", r, topReg, r, r);
    topReg = r;
    countReg = countReg - 1;
}

void CodeGenerator::lt() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= check comparison
    fprintf(outfile, "\tcmp\tr%d, r%d\n\tmovlt\tr%d, #1\n\tmovge\tr%d, #0\n", r, topReg, r, r);
    topReg = r;
    countReg = countReg - 1;
}


void CodeGenerator::ge() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= check comparison
    fprintf(outfile, "\tcmp\tr%d, r%d\n\tmovge\tr%d, #1\n\tmovlt\tr%d, #0\n", r, topReg, r, r);
    topReg = r;
    countReg = countReg - 1;
}


void CodeGenerator::le() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= check comparison
    fprintf(outfile, "\tcmp\tr%d, r%d\n\tmovle\tr%d, #1\n\tmovgt\tr%d, #0\n", r, topReg, r, r);
    topReg = r;
    countReg = countReg - 1;
}


void CodeGenerator::eq() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= emit code to add R[r] plus R[top] and put the result in R[r]
    fprintf(outfile, "\tcmp\tr%d, r%d\n\tmoveq\tr%d, #1\n\tmovne\tr%d, #0\n", r, topReg, r, r);
    topReg = r;
    countReg = countReg - 1;
}

//Other comparison operators are similar to GT.

void CodeGenerator::btrue(int32_t dst) {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    fprintf(outfile, "\tcmp\tr%d, #1\n\tbeq\tB%d\n", topReg, dst);
}

void CodeGenerator::bNotZero(int32_t dst) {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    fprintf(outfile, "\tcmp\tr%d, #0\n\tbne\tB%d\n", topReg, dst);
}

void CodeGenerator::bfalse(int32_t dst) {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    fprintf(outfile, "\tcmp\tr%d, #0\n\tbeq\tl%d\n", topReg, dst);
}

void CodeGenerator::br(int32_t dst) {
    fprintf(outfile, "\tb\tB%d\n", dst);
}

int32_t CodeGenerator::label() {
    /*Mark a point that will be the destination for branches. In machine code, branch destinations are just memory addresses, but in a compiler, we typically generate symbolic labels by incrementing a counter. In this case, argument would be an integer, say 25, and if the code generator generates symbolic assembly language, then LABEL 25 translates to something like the label L25 in the actual assembly language. Labels of this form are used only as branch destinations.
    The above instruction set is sufficient to allow reference to global variables at static addresses, but for languages such as ALGOL 60 and its descendants, including C, C++ and Java, we need a way to load the addresses of local variables. The easiest way to do this is with a special register, the frame pointer fp. Within any procedure or function, this always points to the base of the stack frame (activation record) for the current context.
    */
    fprintf(outfile, "B%d:\n", labelCount);
    labelCount++;
    return labelCount - 1;
}

int32_t CodeGenerator::pointerOnStack(){
    //return int of a P(int) label to the current top of the stack
    fprintf(outfile, "\tP%d\tequ\tsp\n", pointerCount);
    return pointerCount++;
}

void CodeGenerator::pushla(int d) {
    /*PUSH Local Address; the address of a local variable at displacement d in the activation record is pushed onto the stack.
    M[--sp] = d + fp;
    There are numerous alternatives to the above. You could have an instruction to push the frame pointer itself, then push the displacement, then add, if you wanted to eliminate the combination of arithmetic with a constnt.
    */
    //=BUG= Stub for pushla
    throw ("Not Implemented");
}


void CodeGenerator::Not() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    fprintf(outfile, " \tmvn\tr%d, r%d\n", topReg, topReg);
}

void CodeGenerator::neg() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    fprintf(outfile, "\tneg\tr%d, r%d\n", topReg, topReg);
}

void CodeGenerator::Or() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= emit code to add R[r] plus R[top] and put the result in R[r]
    fprintf(outfile, "\torr\tr%d, r%d, r%d\n", r, r, topReg);
    topReg = r;
    countReg = countReg - 1;
}

void CodeGenerator::And() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= emit code to add R[r] plus R[top] and put the result in R[r]
    fprintf(outfile, "\tand\tr%d, r%d, r%d\n", r, r, topReg);
    topReg = r;
    countReg = countReg - 1;
}

void CodeGenerator::eor() {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    int r = topReg - 1;
    if (r < minReg) r = maxReg;
    if (countReg == 1) { /* we to get the stack top into a register */
        //=Bug= someone confirm emit code to pop the RAM stack into R[r] --
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", r);
        countReg = countReg + 1;
    }
    //=BUG= emit code to add R[r] plus R[top] and put the result in R[r]
    fprintf(outfile, "\teor\tr%d, r%d, r%d\n", r, r, topReg);
    topReg = r;
    countReg = countReg - 1;
}

void CodeGenerator::startActRec(){
    /*SP--,set [SP] = FP, set FP = SP */
    fprintf(outfile, "\tsub\tsp, sp, #4\n\tstr\tr11, [sp]\n\tmov\tr11, sp\n");
}

void CodeGenerator::endActRec(){
    /* Set SP = FP, FP = [SP], SP++ */
    fprintf(outfile, "\tmov\tsp, r11\n\tldr\tr11, [sp]\n\tadd\tsp, sp, #4\n");
}

void rangecheck(int value, int Min, int Max) {
    if (value < Min || value > Max) throw "Out of range";
}

std::string CodeGenerator::allocateLable(){
    int lab = pushOnStack(1);
    return "P" +lab;
}

std::string CodeGenerator::allocateLable(int words) {
    //Allocate space for 'words' number of words and create a label
    int lab = pushl(words);
    return "P" +lab;
}

std::string CodeGenerator::branchLable(){
    int lab = label();
    return "B" +lab;
    }

int32_t CodeGenerator::PutStringInMemory(std::string str) {
    fprintf(outfile, ".data\n");
    fprintf(outfile, "P%d:\n\t.ascii \"%s\"\n", pointerCount, str.c_str());
    fprintf(outfile, ".text\n");
    return pointerCount++;
}

void CodeGenerator::saveTopAtLabel(int32_t x) {
    if (countReg == 0) { /* we to get the stack top into a register */
        //=BUG= someone confirm this
        fprintf(outfile, "\tldr\tr%d, [sp]\n\tadd\tsp, sp, #4\n", topReg);
        countReg = countReg + 1;
    }
    fprintf(outfile, "\tldr\tr0, =P%d\n", x);
    fprintf(outfile, "\tstr\tr%d, [r0]\n", topReg);
}


void CodeGenerator::putStrChar(int Char, int label, int Offset) {
    if (Offset == 0) {
        fprintf(outfile, "\tsub\tsp, sp, #4\n");
        fprintf(outfile, "\tstr\tr0, [sp]\n");
        fprintf(outfile, "\tsub\tsp, sp, #4\n");
        fprintf(outfile, "\tstr\tr1, [sp]\n");
        fprintf(outfile, "\tldr\tr1, #=p%d\n", label);
        fprintf(outfile, "\tldr\tr0, #%d\n", Char);
        fprintf(outfile, "\tstr\tr0, [r1]\n");
        fprintf(outfile, "\tldr\tr1, [sp]\n");
        fprintf(outfile, "\tadd\tsp, sp, #4\n");
        fprintf(outfile, "\tldr\tr0, [sp]\n");
        fprintf(outfile, "\tadd\tsp, sp, #4\n");
    } else if (Offset > 0) {
        Offset = Offset * 4;
        fprintf(outfile, "\tsub\tsp, sp, #4\n");
        fprintf(outfile, "\tstr\tr0, [sp]\n");
        fprintf(outfile, "\tsub\tsp, sp, #4\n");
        fprintf(outfile, "\tstr\tr1, [sp]\n");
        fprintf(outfile, "\tldr\tr1, #=p%d - %d\n", label, Offset);
        fprintf(outfile, "\tldr\tr0, #%d\n", Char);
        fprintf(outfile, "\tstr\tr0, [r1]\n");
        fprintf(outfile, "\tldr\tr1, [sp]\n");
        fprintf(outfile, "\tadd\tsp, sp, #4\n");
        fprintf(outfile, "\tldr\tr0, [sp]\n");
        fprintf(outfile, "\tadd\tsp, sp, #4\n");
    }
}

void CodeGenerator::pushPointerLabelVal(int32_t label) {
    topReg++;
    if (topReg > maxReg) topReg = minReg;
    if (countReg == REG_CACHE_SIZE) {
        //have to make room for new value
        fprintf(outfile, "\tsub\tsp, sp, #4\n");
        fprintf(outfile, "\tstr\tr%d, [sp]\n", topReg);
    } else {
        countReg++;
    }
    fprintf(outfile, "\tldr\tr%d, =P%d\n", topReg, label);
}

void CodeGenerator::printInt(int32_t label) {
    //load register r0 with the "%d" string address
    fprintf(outfile, "\tldr\tr0, =INTSTR\n");
    //load r1 with address of integer;
    fprintf(outfile, "\tldr\tr1, =P%d\n", label);
    //call printf
    fprintf(outfile, "\tbl\tprintf\n");
}

void CodeGenerator::printIntLine(int32_t label) {
    //load register r0 with the "%d\n" string address
    fprintf(outfile, "\tldr\tr0, =INTSTRL\n");
    //load r1 with address of integer;
    fprintf(outfile, "\tldr\tr1, =P%d\n", label);
    //call printf
    fprintf(outfile, "\tbl\tprintf\n");
}

void CodeGenerator::printString(int32_t label){
    //load string address in r0
    fprintf(outfile,"\tldr\tr0, =P%d\n", label);
    //call printf
    fprintf(outfile, "\tbl\tprinf\n");
}
