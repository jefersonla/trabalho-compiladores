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
/*                Token List Structure              */
/* ------------------------------------------------ */

typedef struct strTokenList{
    /* Number of childs */
    int length;
    
    /* List of Nodes */
    TokenNode **items;
} TokenList, *ptrTokenList;

/* --------------- Token List Methods ------------- */

/* Initialize a list of tokens */
TokenList* newTokenList();

/* Add an item to a list of tokens */
bool listAddToken(TokenList *token_list, TokenNode *token);

/* Retrieve a token by it's given index */
TokenNode* listGetTokenByIndex(TokenList *token_list, int index);

/* Retrieve a list token with a given type */
TokenList* listGetTokensByType(TokenList *token_list, int token_type);

/* ------------------------------------------------ */
/*               Symbol Node Structure              */
/* ------------------------------------------------ */

typedef struct SymbolNode{
    /* Name of the symbol */
    char *symbol_name;
    
    /* Value of the symbol */
    char *symbol_value
    
    /* Check if symbol is null */
    bool isNull;
} SymbolNode, *ptrSymbolNode;

/* -------------- Symbol Node Methods ------------- */



/* ------------------------------------------------ */
/*               Symbol Table Structure             */
/* ------------------------------------------------ */

typedef struct SymbolTable{
    
} SymbolTable, *ptrSymbolTable;

/* ------------- Symbol Table Methods ------------- */



/* ------------------------------------------------ */

#endif