#ifndef UTILS_H
#define UTILS_H

#include <stdarg.h>
#include <stdbool.h>
#include "token_struct.h"

/* Convert a constant to a string */
#define QOUTE(_VAR)                 #_VAR
#define TO_STRING(VAR)              QOUTE(VAR)

/* Max size of a 32Bit Number */
#define MAX_SIZE_32BIT_STRING       11

/* Utility to allocate text of tokens */
bool allocateTokenText(TokenNode *token_node, int qtd_params, ...);

/* Utility to allocate a token and token childs */
bool allocateTokenAndChilds(ptrTokenNode *token_node, int token_type, int qtd_params, ...);

/* Utility to concatenate list of tokens */
bool concatenateChildTokens(TokenNode *token_node_dest, ptrTokenNode *token_node_src);

/* Utility to allocate formatted instructions */
char* formatedInstruction(const char *format_string, ...);

#endif