#ifndef TOKEN_STRUCT_H
#define TOKEN_STRUCT_H

#include <stdio.h>
#include <stdbool.h>

/* ------------------------------------------------ */
/*                Prototype definitions             */
/* ------------------------------------------------ */

/* Token Node */
typedef struct strToken TokenNode;

/* TokenList */
typedef struct strTokenList TokenList;

/* Symbol Node */
typedef struct strSymbolNode SymbolNode;

/* Symbol Table */
typedef struct strSymbolTable SymbolTable;

/* Instruction Node */
typedef struct strInstructionNode InstructionNode;

/* Instruction Queue */
typedef struct strInstructionQueue InstructionQueue;

/* ------------------------------------------------ */
/*                 Token Node Structure             */
/* ------------------------------------------------ */

typedef struct strToken{
    /* Root token of the given node */
    TokenNode *root_token;
    
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
TokenNode* newTokenNode(int token_type);

/* Free a token */
bool deleteTokenNode(ptrTokenNode *token_node);

/* Add Token String */
bool nodeAddTokenStr(TokenNode *token_node, char *token_str);

/* Add Lexical String */
bool nodeAddLexStr(TokenNode *token_node, char *lex_str);

/* Add Root Token */
bool nodeAddRootToken(TokenNode *token_node, TokenNode *root_token);

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

typedef struct strSymbolNode{
    /* Name of the symbol */
    char *symbol_name;
    
    /* Start adress */
    int symbol_address;
    
    /* Symbol Size in bytes */
    int symbol_size;
    
    /* Type of the symbol */
    int symbol_type;
    
    /* Symbol type byte size */
    int symbol_type_size;
    
    /* Root SymbolTable */
    SymbolTable *root_symbol_table;
} SymbolNode, *ptrSymbolNode;

/* -------------- Symbol Node Methods ------------- */

/* Create a new symbol node. */
SymbolNode* newSymbolNode(  SymbolTable *root_symbol_table, char *symbol_name, 
                            int symbol_address, int symbol_type);

/* Compare symbol name */
bool symbolEqualsName(SymbolNode *symbol, char *symbol_name);

/* Get symbol address of a symbol node */
int symbolNodeGetSymbolAdress(SymbolNode *symbol_node);

/* Get symbol size of a given symbol node */
int symbolNodeGetSymbolSize(SymbolNode *symbol_node);

/* Get load a symbol into a register instruction */
InstructionNode* symbolNodeGetLoadInstruction(SymbolNode *symbol_node);

/* Get store a symbol into a register instruction */
InstructionNode* symbolNodeGetStoreInstruction(SymbolNode *symbol_node);

/* Get load a symbol type into a register instruction */
InstructionNode* symbolNodeGetLoadTypeInstruction(SymbolNode *symbol_node);

/* Get store a symbol type into a register instruction */
InstructionNode* symbolNodeGetStoreTypeInstruction(SymbolNode *symbol_node);

/* ------------------------------------------------ */
/*               Symbol Table Structure             */
/* ------------------------------------------------ */

typedef struct strSymbolTable{
    /* Size of the structure */
    int size;
    
    /* Qunantity of elements present on this table */
    int length;
    
    /* Start Address */
    int start_address;
    
    /* Address Shift */
    int shift_address;
    
    /* Previous Scope */
    SymbolTable *previous_scope;
    
    /* Items present on the table */
    SymbolNode **items;
} SymbolTable, *ptrSymbolTable;

/* ------------- Symbol Table Methods ------------- */

/* Create a new global Symbol Table */
SymbolTable* newGlobalSymbolTable();

/* Create a new SymbolTable */
SymbolTable* newSymbolTable(SymbolTable *symbol_table);

/* Add a new symbol to symbol table */
bool symbolTableAddSymbol(  SymbolTable *symbol_table, char *symbol_name, 
                            int symbol_type);

/* Check if a symbol of a given name is present on the table */
bool symbolTableContains(SymbolTable *symbol_table, char *symbol_name);

/* Get symbol node by name */
SymbolNode* symbolTableGetSymbolNodeByName( SymbolTable *symbol_table, 
                                            char *symbol_name);

/* ------------------------------------------------ */
/*            Instruction Node Structure            */
/* ------------------------------------------------ */

typedef struct strInstructionNode{
    /* Instruction String */
    char *instruction;

    /* Length of the instruction string */
    int length;
} InstructionNode, *ptrInstructionNode;

/* ---------- Instruction Node Methods ----------- */

/* Return a new Instruction Node  */
InstructionNode *newInstructionNode(char* instruction_string, bool copyInstruction);

/* Print a instruction node on a given file */
bool instructionNodeFilePrint(FILE *_output_file, InstructionNode *instruction);

/* Get instruction length */
int instructionNodeLength(InstructionNode *instruction);

/* ------------------------------------------------ */
/*            Instruction Queue Structure           */
/* ------------------------------------------------ */

typedef struct strInstructionQueue{
    /* Instructions Array */
    InstructionNode **instructions;
    
    /* Structure size */
    int size;
    
    /* Quantity of elements */
    int length;
} InstructionQueue, *ptrInstructionQueue;

/* ---------- Instruction Queue Methods ----------- */

/* Return a new Instruction Queue */
InstructionQueue* newInstructionQueue();

/* Add a new instruction to instruction queue */
bool instructionQueueEnqueueInstruction(InstructionQueue *instruction_queue,
                                        char *instruction_string, 
                                        bool copyInstruction);
                                        
/* Add a new instruction node structure to instruction queue */
bool instructionQueueEnqueueInstructionNode(InstructionQueue *instruction_queue,
                                            InstructionNode *instruction_node);

/* Print a instruction queue on a given file */
bool instructionQueueFilePrint(FILE *_output_file, InstructionQueue *instruction);

/* ------------------------------------------------ */
/*                     Utilities                    */
/* ------------------------------------------------ */

/* Size of blocks in structures */
#define DEFAULT_BLOCK_SIZE          10

/* Standard size of the type and size of the type descriptor */
#define DEFAULT_SYMBOL_SIZE         4
#define DEFAULT_SYMBOL_TYPE_SIZE    0

/* Newline char */
#define NEWLINE_CHAR                "\n"

/* Tab char */
#define TAB_CHAR                    "\n"

/* Empty String */
#define EMPTY_STRING                ""

/* Variable Size in Bytes */
#define BYTE_VARIABLE_SIZE          4

/* ------------------------------------------------ */

#endif