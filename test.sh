#!/bin/bash

# Execute compiled code and save the result in a test file
./compilador test.lua > test.out 2>&1

# Compara os resultados
diff -q test.out test.expected.out