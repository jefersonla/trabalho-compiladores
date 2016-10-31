#ifndef UTILS_H
#define UTILS_H

#include <stdarg.h>
#include <stdbool.h>
#include "token_struct.h"

/* Utility to allocate text of tokens */
bool allocateTokenText(TokenNode *token_node, const char *token_format, int qtd_params, ...);

/* Utility to allocate a token and token childs */
bool allocateTokenAndChilds(ptrTokenNode *token_node, int token_type, int qtd_params, ...);

/* Utility to concatenate list of tokens */
bool concatenateChildTokens(TokenNode *token_node_dest, ptrTokenNode *token_node_src);

#endif