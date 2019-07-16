#!/bin/sh
# tscript  -- test script for the Kestrel Compiler
# Tests all the types of lexemes: identifiers, keywords, integers, punctuations
# strings, eof of file, and errors



# Identifier Test Module ----------------------------------------------------------
cat tests/id.txt | ./testlex > out.txt
if ! diff --strip-trailing-cr out.txt tests/idOut
	then echo "ID TEST FAILED!"
else 
	echo "ID TEST PASSED!"
fi



# String Test Module ----------------------------------------------------------
cat tests/str.txt | ./testlex > out.txt

if ! diff out.txt ./tests/strout 
	then echo "STR TEST FAILED!"
else 
	echo "STR TEST PASSED!"
fi


# Integer Test Module ---------------------------------------------------------
# Tests regular num like 0 and 8 so far. Needs base implementations, 16#3FE
cat tests/int.txt | ./testlex > out.txt
# Compares the output from our compiler to the expected output 
if ! diff out.txt ./tests/intout 
	then echo "INT TEST FAILED!"
else
	echo "INT TEST PASSED!"
fi


# Punctuation Test Module -----------------------------------------------------
cat tests/punct.txt | ./testlex > out.txt 
if ! diff --strip-trailing-cr out.txt tests/punctout 
	then echo "PUNCT TEST FAILED!"
else 
	echo "PUNCT TEST PASSED!"
fi



#rm out.txt
#rm tempout.txt
#rm expectedout.txt

