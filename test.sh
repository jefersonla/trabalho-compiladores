#!/bin/bash

# Execute compiled code and save the result in a test file
./compilador test.lua 2>&1

# Compara os resultados
DIFF=$(diff -q test.lua.out test.exp) 

if [ "$DIFF" != "" ]
then
    echo "ERROR RESULT IS DIFFERENT FROM WHAT IS EXPECTED"
else
    echo "COMPLETED! EVERYTHING IS CORRECT :)"	
fi