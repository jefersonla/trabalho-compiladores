#ifndef UTILS_H
#define UTILS_H

#include <stdarg.h>
#include <stdbool.h>
#include "token_struct.h"

/* Convert a constant to a string */
#define QUOTE(_VAR)                         #_VAR
#define TO_STRING(VAR)                      QUOTE(VAR)

/* Max size of a 32Bit Number */
#define MAX_SIZE_32BIT_STRING               11

/* Extra size to make buffer strings more secures */
#define SECURE_BUFFER_SIZE                  10

/* Extra size to make strings more secures */
#define SECURE_STRING_TERMINATOR            5

/* Secure free memory */
#define secureFree(VAR)                     do{ free(VAR); VAR = NULL; } while(false)

/* Get a secure string size */
#define secureStringSize(VAR)               (strlen(VAR) + SECURE_STRING_TERMINATOR)

/* Secure  String Copy */
#define secureStringCopy(DEST, SRC)         do{ DEST[secureStringSize(SRC)] = 0; strncpy(DEST, SRC, secureStringSize(SRC)); }while(false)

/* Correct way of call malloc */
#define mallocIt(VAR)                       malloc(sizeof(*VAR))

/* Correct way of call malloc for arrays */
#define mallocItArray(VAR, SIZE)            malloc(sizeof(*VAR) * (SIZE))

/* Correct way of call malloc for strings */
#define mallocItString(VAR, VAR_LENGTH)     mallocItArray(VAR, secureStringSize(VAR_LENGTH))

/* Correct way of realloc an array */
#define reallocItArray(VAR, NEW_SIZE)       realloc(VAR, sizeof(*VAR) * (NEW_SIZE))

/* Utility to allocate text of tokens */
bool allocateTokenText(TokenNode *token_node, int qtd_params, ...);

/* Utility to allocate a token and token childs */
bool allocateTokenAndChilds(ptrTokenNode *token_node, int token_type, int qtd_params, ...);

/* Utility to concatenate list of tokens */
bool concatenateChildTokens(TokenNode *token_node_dest, ptrTokenNode *token_node_src);

/* Utility to allocate formatted instructions */
char* formatedInstruction(const char *format_string, ...);

#endif