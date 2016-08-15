#!/bin/bash

# Check if theres a variable
if [ -z $1 ]
then
	echo "NOT SPECIFIED EXECUTABLE FILES!"
	exit 1
fi

# Check if file exists and can be executed
if [ -e $1 ] && [ -s $1 ] && [ -x $1 ]
then
	# Execute compiled code and save the result in a test file
	./$1 test.lua 2>&1	
else
	echo "FILE NOT EXIST, IS EMPTY OR CAN'T BE EXECUTABLE!"
	exit 1
fi

OUPUT_FILE=test.lua.out

# Check if the other file exist
# Check if file exists and can be executed
if [ -e $OUPUT_FILE ]
then
	# File EXIST
	echo
else
	echo "OUTPUT FILE NOT EXIST!"
	exit 1
fi


# Compara os resultados
DIFF=$(diff -q test.lua.out test.exp) 

if [ "$DIFF" != "" ]
then
    echo "ERROR RESULT IS DIFFERENT FROM WHAT IS EXPECTED"
    exit 1
else
    echo "COMPLETED! EVERYTHING IS CORRECT :)"	
    exit 0
fi