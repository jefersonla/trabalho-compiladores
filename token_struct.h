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
bool nodeAddLexStr(TokenNode *token_node, char *lex_str);

/* ------------------------------------------------ */
/*                Token List Structure              */
/* ------------------------------------------------ */

typedef struct strTokenList{
    /* Number of childs */
    int length;
    
    /* Size of the list */
    int size;
    
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

/* Create a new symbol node. */
SymbolNode* newSymbolNode(char *symbol_name, char *symbol_value);

/* Compare symbol value */
bool symbolEqualsName(SymbolNode *symbol, char *symbol_name);

/* Set value of symbol */
bool setSymbolValue(SymbolNode *symbol, char *symbol_value);

/* ------------------------------------------------ */
/*               Symbol Table Structure             */
/* ------------------------------------------------ */

typedef struct SymbolTable{
    /* Size of the structure */
    int size;
    
    /* Qunantity of elements present on this table */
    int length;
    
    /* Start Address */
    int start_address;
    
    /* Items present on the table */
    SymbolNode **items;
} SymbolTable, *ptrSymbolTable;

/* ------------- Symbol Table Methods ------------- */

/* Create a new SymbolTable */
SymbolTable* newSymbolTable(int start_address);

/* Add a new symbol to symbol table */
bool symbolTableAddSymbol(SymbolTable *symbol_table, SymbolNode *symbol);

/* Check if a symbol of a given name is present on the table */
bool symbolTableContains(SymbolTable *symbol_table, char *symbol_name);

/* Get symbol node by name */
SymbolNode* symbolTableGetSymbolNodeByName(SymbolTable *symbol_table, char *symbol_name);

/* Set symbol value by name */
bool symbolTableSetSymbolNodeValue(SymbolTable *symbol_table, char *symbol_name, char *symbol_value);

/* Get index of a given symbol_name */
int symbolTableGetSymbolNodeIndex(SymbolTable *symbol_table, char *symbol_name);

/*

** TAD **

FIFO

QUEUE === FILA

1 INS ^
2 INS |
3 INS |
4 INS |
5 INS |
6 INS |
7 INS | <----

------------------------------------------

X
.
X

const char z

Y
&z
&z
&z

li $%s, 4
...
...
.
.
.
Y

CGEN(exp)

CGEN(T_NUMBER + T_NUMBER);

vetor de instrucoes

ins[] = &z
        &z
        ins3
        ...

*/

/* ------------------------------------------------ */
/*            Instruction Node Structure            */
/* ------------------------------------------------ */

typedef struct strInstructionNode{
    /* Instruction String */
    char *instruction;
    
    /* With tab or not */
    bool useTab;
    
    /* Length of the instruction string */
    int length;
} InstructionNode, *ptrInstructionQueue;

/* ---------- Instruction Node Methods ----------- */

/* Return a new Instruction Node  */
InstructionNode *newInstructionNode(char* instruction_string, bool use_tab, bool copyInstruction);

/* Print a instruction node */
printInstructionNode(FILE *_output_file, InstructionNode *instruction);

/* ------------------------------------------------ */
/*            Instruction Queue Structure           */
/* ------------------------------------------------ */

typedef struct strInstructionQueue{
    
} InstructionQueue, *ptrInstructionQueue;

/* ---------- Instruction Queue Methods ----------- */



/* ------------------------------------------------ */
/*                     Utilities                    */
/* ------------------------------------------------ */

/* Size of blocks in structures */
#define DEFAULT_BLOCK_SIZE      10

/* Newline char */
#define NEWLINE_CHAR            ""\n"

/* Tab char */
#define TAB_CHAR                "\n"

/* Empty String */
#define EMPTY_STRING            ""

/* ------------------------------------------------ */

#endif