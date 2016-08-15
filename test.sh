#!/bin/bash

# Execute compiled code and save the result in a test file
./compilador test.lua 2>&1

# Compara os resultados
diff -q test.lua.out test.exp