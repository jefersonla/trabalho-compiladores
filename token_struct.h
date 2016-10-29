#ifndef TOKEN_STRUCT_H
#define TOKEN_STRUCT_H

#include <stdbool.h>

/* ------------------------------------------------ */
/*                 Token Node Structure             */
/* ------------------------------------------------ */

typedef struct strToken{
    /* Root token of the given node */
    struct strToken *root_token;
    
    /* Token String */
    /* Store token string node */
    char *token_str;
    
    /* Lexical String */
    /* Store value read by lexical */
    char *lex_str;
    
    /* TokenType */
    int token_type;
    
    /* Child Nodes */
    TokenList *child_list;
} TokenNode, *ptrTokenNode;

/*------------ Token Structure Methods ------------ */

/* Creates a new token node */
TokenNode* newTokenNode(TokenNode *root_token, int token_type);

/* Add Token String */
bool nodeAddTokenStr(TokenNode *token_node, char *token_str);

/* Add Lexical String */
bool nodeAddLexStr(TokenNode *token_node, char *lex_str)

/* ------------------------------------------------ */
/*                 Token List Structure             */
/* ------------------------------------------------ */

typedef struct strTokenList{
    /* Number of childs */
    int length;
    
    /* List of Nodes */
    TokenNode *items;
} TokenList, *ptrTokenList;

/* --------------- Token List Methods ------------- */

/* Initialize a list of tokens */
bool listTokenInitialize(TokenList *token_list);

/* Add an item to a list of tokens */
bool listAddToken(TokenList *token_list, TokenNode *token);

/* Retrieve a token by it's given index */
TokenNode* listGetTokenByIndex(TokenList *token_list, int index);

/* Retrieve a token by it's given type */
TokenNode* listGetTokenByType(TokenList *token_list, int token_type);

/* ------------------------------------------------ */

#endif