#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"
#include "token_struct.h"
#include "codegen_functions.h"

/*------------ Token Structure Methods ------------ */

/**
 * Creates a new token node.
 * 
 * @param token_type Type of the Token;
 * @return A new token node.
 */
TokenNode* newTokenNode(int token_type){
    TokenNode *_new_token_node;
    
    /* Malloc new token node */
    _new_token_node = mallocIt(_new_token_node);
    
    /* Give a fatal error if malloc has errors */
    if(_new_token_node == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE A NEW TOKEN!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Initialize new token node */
    _new_token_node->root_token = NULL;
    _new_token_node->token_type = token_type;
    _new_token_node->token_str = NULL;
    _new_token_node->lex_str = NULL;
    _new_token_node->child_list = newTokenList();
    
    /* Return pointer to the new token */
    return _new_token_node;
}

/** 
 * Destroy an already allocated token node.
 * 
 * @param token_node that will be destroyed.
 * @param deleteChilds if this flag is true it will destroy all childs related to this token.
 * @return true if there's no error on execution and false otherwise.
 */
bool deleteTokenNode(ptrTokenNode *token_node, bool deleteChilds){
    /* Check if token node exists */
    if(token_node == NULL){
        fprintf(stderr, "[ERROR] CANNOT DESTROY A NULL TOKEN VARIABLE NODE!\n");
        return false;
    }
    
    /* Check if token node exists */
    if((*token_node) == NULL){
        fprintf(stderr, "[ERROR] CANNOT DESTROY A NULL TOKEN NODE!\n");
        return false;
    }
    
    /* Free token_str if it exists */
    if((*token_node)->token_str != NULL){
        //secureFree((*token_node)->token_str);
    }
    
    /* Free lex_str if it exists */
    if((*token_node)->lex_str != NULL){
        //secureFree((*token_node)->lex_str);
    }
    
    /* Delete child list, and childs if destroyChild is true */
    deleteTokenList(&(*token_node)->child_list, deleteChilds);
    
    /* Free tokne_node pointer */
    //secureFree((*token_node));
    
    /* Return success */
    return true;
}

/**
 * Add Token String.
 *
 * @param token_node Token which lex_str will be appended.
 * @param token_str Token string value to be copied to TokenNode.
 * @return true if there's no error on execution and false otherwise.
 */
bool nodeAddTokenStr(TokenNode *token_node, char *token_str){
    char *_new_token_str;
    
    /* Check if token_node isn't null */
    if(token_node == NULL){
        fprintf(stderr, "[ERROR] TOKEN NODE IS NULL!\n");
        return false;
    }
    
    /* Allocate new token_str */
    _new_token_str = mallocItString(_new_token_str, token_str);
    
    /* Check if malloc has succeed */
    if(_new_token_str == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE STRING FOR TOKEN!\n");
        return false;
    }
    
    /* Check if this token already has token str */
    if(token_node->token_str != NULL){
        //secureFree(token_node->token_str);
    }
    
    /* Store the new token_str array pointer */
    token_node->token_str = _new_token_str;
    
    /* Copy content from token_str to the new token */
    secureStringCopy(token_node->token_str, token_str);
    
    /* Return success */
    return true;
}

/**
 * Add Lexical String.
 * 
 * @param token_node Token which lex_str will be appended.
 * @param lex_str Lex string value to be copied to TokenNode.
 * @return true if there's no error on execution and false otherwise.
 */
bool nodeAddLexStr(TokenNode *token_node, char *lex_str){
    char *_new_lex_str;
    
    /* Check if token_node isn't null */
    if(token_node == NULL){
        fprintf(stderr, "[ERROR] TOKEN NODE IS NULL!\n");
        return false;
    }
    
    /* Allocate new lex_str */
    _new_lex_str = mallocItString(_new_lex_str, lex_str);
    
    /* Check if malloc has succeed */
    if(_new_lex_str == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE STRING FOR TOKEN!\n");
        return false;
    }
    
    /* Check if this token already has token str */
    if(token_node->lex_str != NULL){
        //secureFree(token_node->lex_str);
    }
    
    /* Store the new lex_str array pointer */
    token_node->lex_str = _new_lex_str;
    
    /* Copy content from lex_str to the new token */
    secureStringCopy(token_node->lex_str, lex_str);
    
    /* Return success */
    return true;
}

/**
 * Add Root Token.
 *
 * @param token_node Token which lex_str will be appended.
 * @param token_root Token root pointer value to be copied to TokenNode.
 * @return true if there's no error on execution and false otherwise.
 */
bool nodeAddRootToken(TokenNode *token_node, TokenNode *root_token){
    /* Check if token_node isn't null */
    if(token_node == NULL){
        fprintf(stderr, "[ERROR] TOKEN NODE IS NULL!\n");
        return false;
    }
    
    /* Check if token_node isn't null */
    if(root_token == NULL){
        fprintf(stderr, "[INFO] ROOT TOKEN IS NULL!\n");
    }
    
    /* Store the new token_str array pointer */
    token_node->root_token = root_token;
    
    /* Return success */
    return true;
}

/* --------------- Token List Methods ------------- */

/**
 * Initialize a list of tokens.
 * 
 * @return Return a new token list.
 */
TokenList* newTokenList(){
    TokenList *_new_token_list;
    TokenNode **_items_list;
    
    /* Try allocate a new token list */
    _new_token_list = mallocIt(_new_token_list);
    
    /* Give a fatal error if malloc has errors */
    if(_new_token_list == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE TOKEN LIST!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Try to allocate an array of tokens */
    _items_list = mallocItArray(_items_list, DEFAULT_BLOCK_SIZE);
    
    /* Give a fatal error if malloc has errors */
    if(_items_list == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE ITEMS OF THE TOKEN LIST!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Initialize token list */
    _new_token_list->length = 0;
    _new_token_list->size = DEFAULT_BLOCK_SIZE;
    _new_token_list->items = _items_list;
    
    /* Return the new token list */
    return _new_token_list;
}

/** 
 * Destroy an already allocated token list.
 * 
 * @param token_list pointer to the token list that will be destroyed.
 * @param deleteChilds if this flag is true it will delete childs recursively.
 * @return true if there's no error on execution and false otherwise.
 */
bool deleteTokenList(ptrTokenList *token_list, bool deleteChilds){
    int i;
    TokenNode *token_child;
   
    /* Check if token node exists */
    if(token_list == NULL){
        fprintf(stderr, "[ERROR] CANNOT DESTROY A NULL TOKEN LIST VARIABLE NODE!\n");
        return false;
    }
    
    /* Check if token node exists */
    if((*token_list) == NULL){
        fprintf(stderr, "[ERROR] CANNOT DESTROY AN ALREADY NULL TOKEN LIST NODE!\n");
        return false;
    }
    
    /* If deleteChild is set to true it will delete all token childs related to this token */
    if(deleteChilds){
        for(i = 1; i <= (*token_list)->length; i++){
            
            /* Get the actual node */
            token_child = listGetTokenByIndex((*token_list), i);
            
            /* Check if for some reason token child is null */
            if(token_child == NULL){
                fprintf(stderr, "[WARNING] CHILD TOKEN IS NULL!\n");
                continue;
            }
            
            /* Delete all childs recursively */
            deleteTokenNode(&token_child, deleteChilds);
        }
    }
    
    /* Free array */
    //secureFree((*token_list)->items);

    /* Free array */
    //secureFree((*token_list));

    /* Null the pointer apponted by token_list */
    (*token_list) = NULL;
    
    /* Return success */
    return true;
}

/**
 * Add an item to a list of tokens.
 * 
 * @param token_list A pointer to a token list structure.
 * @param token Token to be added.
 * @return true if there's no error on execution and false otherwise.
 */
bool listAddToken(TokenList *token_list, TokenNode *token){
    TokenNode **_reallocated_items;
    
    /* Check if token isn't null */
    if(token_list == NULL){
        fprintf(stderr, "[ERROR] TOKEN LIST IS NULL!\n");
        return false;
    }
    
    /* Check if token isn't null */
    if(token == NULL){
        fprintf(stderr, "[ERROR] TOKEN IS NULL!\n");
        return false;
    }
    
    /* If array is full, add more empty spaces */
    if((token_list->length + 1) == token_list->size){
        /* Increase structure size */
        token_list->size += DEFAULT_BLOCK_SIZE;
        
        /* Try reallocate array of token_list items */
        _reallocated_items = reallocItArray(token_list->items, token_list->size);
        
        /* Give a fatal error if malloc has errors */
        if(_reallocated_items == NULL){
            fprintf(stderr, "[ERROR] FATAL ERROR CANNOT REALLOCATE TOKEN LIST ITEMS!\n");
            exit(EXIT_FAILURE);
        }
        
        /* Store the pointer to the new items list array */
        token_list->items = _reallocated_items;
    }
    
    /* Store pointer of the new token on items list */
    token_list->items[token_list->length] = token;
    
    /* Increment list length */
    token_list->length += 1;
    
    /* Return success */
    return true;
}

/**
 * Retrieve a token by it's given index, this function assume that valid index is on range from 1 to n.
 * 
 * @param token_list A pointer to a token list structure.
 * @param index Index of the item retrieved.
 * @return Pointer to the token at the given index.
 */
TokenNode* listGetTokenByIndex(TokenList *token_list, int index){
    /* Check if either token or token_list isn't null */
    if(token_list == NULL){
        fprintf(stderr, "[ERROR] TOKEN LIST IS NULL!\n");
        return NULL;
    }
    
    /* Invalid index used */
    if(index <= 0){
        fprintf(stderr, "[ERROR] INVALID INDEX!\n");
        return NULL;
    }
    
    /* Out of bounds access */
    if(index > token_list->length){
        fprintf(stderr, "[ERROR] INDEX OUT OF BOUNDS!\n");
        return NULL;
    }
    
    /* return Token pointer */
    return token_list->items[index - 1];
}

/**
 * Retrieve a list of tokens by it's given type.
 * 
 * @param token_list A pointer to a token list structure.
 * @param token_type Type of the wanted tokens.
 * @return Pointer to a generated list of tokens of a given type.
 */
TokenList* listGetTokensByType(TokenList *token_list, int token_type){
    int i;
    TokenList *_tokens_found;
    TokenNode *_token_i;
    
    /* Create a new token list */
    _tokens_found = newTokenList();
    
    /* Search for the given types */
    for(i = 1; i <= token_list->length; i++){
        
        /* Pick the token at index i */
        _token_i = listGetTokenByIndex(token_list, i);
        
        /* Check if token_list item has the type wanted, and add this item to the found list */
        if(_token_i->token_type == token_type){
            listAddToken(_tokens_found, _token_i);
        }
    }
    
    /* Return token founds */
    return _tokens_found;
}

/* -------------- Symbol Node Methods ------------- */

/**
 * Create a new symbol node, if symbol value it's null the symbol is of type nil.
 * 
 * @param root_symbol_table Root symbol table of the given node.
 * @param symbol_name Name of the given symbol.
 * @param symbol_value Value of the symbol, use NULL to store nil value.
 * @return A new symbol, or fatal erro cause it fails.
 */
SymbolNode* newSymbolNode(SymbolTable *root_symbol_table, char *symbol_name, int symbol_address, int symbol_type){
    SymbolNode *_new_symbol_node;
    char *_new_symbol_name;
    
    /* Check if root symbol table is valid */
    if(root_symbol_table == NULL){
        fprintf(stderr, "[ERROR] INVALID ROOT SYMBOL TABLE!\n");
        return NULL;
    }
    
    /* Try allocate this new symbol */
    _new_symbol_node = mallocIt(_new_symbol_node);
    
    /* Give a fatal error if malloc has errors */
    if(_new_symbol_node == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW SYMBOL!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Try allocate the name of the new symbol */
    _new_symbol_name = mallocItString(_new_symbol_name, symbol_name); 
    
    /* Give a fatal error if malloc has errors */
    if(_new_symbol_name == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW SYMBOL!\n");
        exit(EXIT_FAILURE);
    }

    /* Copy content of the symbol name to this new structure */
    secureStringCopy(_new_symbol_name, symbol_name);

    /* Set the new symbol node */
    _new_symbol_node->symbol_name = _new_symbol_name;
    _new_symbol_node->symbol_address = symbol_address;
    _new_symbol_node->symbol_type = symbol_type;
    _new_symbol_node->symbol_size = DEFAULT_SYMBOL_SIZE;
    _new_symbol_node->symbol_type_size = DEFAULT_SYMBOL_TYPE_SIZE;
    _new_symbol_node->root_symbol_table = root_symbol_table;
    
    /* Return pointer to this new node */
    return _new_symbol_node;
}

/** 
 * Destroy an already allocated symbol node.
 * 
 * @param symbol_node pointer to the symbol node that will be destroyed.
 * @return true if there's no error on execution and false otherwise.
 */
bool deleteSymbolNode(ptrSymbolNode *symbol_node){
    /* Check if token node exists */
    if(symbol_node == NULL){
        fprintf(stderr, "[ERROR] CANNOT DESTROY A NULL TOKEN LIST VARIABLE NODE!\n");
        return false;
    }
    
    /* Check if token node exists */
    if((*symbol_node) == NULL){
        fprintf(stderr, "[ERROR] CANNOT DESTROY AN ALREADY NULL TOKEN LIST NODE!\n");
        return false;
    }
    
    /* Check if there are a symbol name allocated and remove it */
    if((*symbol_node)->symbol_name != NULL){
        //secureFree((*symbol_node)->symbol_name);
    }
    
    /* Free token list  */
    //secureFree((*symbol_node));
    
    /* Return success */
    return true;
}

/**
 * Compare symbol with a symbol name.
 * 
 * @param symbol Symbol which will be compared.
 * @param symbol_name Name of the symbol to check.
 * @return true if symbol's are equal and false otherwise.
 */
bool symbolEqualsName(SymbolNode *symbol, char *symbol_name){
    /* Check if symbol is not NULL */
    if(symbol == NULL){
        fprintf(stderr, "[ERROR] SYMBOL IS NULL!\n");
        return false;
    }
    
    /* Check if symbol_name is not NULL */
    if(symbol_name == NULL){
        fprintf(stderr, "[ERROR] SYMBOL NAME IS NULL!\n");
        return false;
    }
    
    /* Compare symbol name of symbol with symbol_name */
    return (strcmp(symbol->symbol_name, symbol_name) == 0);
}

/** 
 * Get relative address of a symbol node.
 * 
 * @param symbol_node Symbol node pointer structure.
 * @return Integer relative stack address pointer;
 */
int symbolNodeGetSymbolAdress(SymbolNode *symbol_node){
    /* Check if symbol node is not NULL */
    if(symbol_node == NULL){
        fprintf(stderr, "[ERROR] ERROR SYMBOL NODE IS NULL!\n");
        return -1;
    }
    
    /* Return symbol relative address */
    return symbol_node->symbol_address;
}

/** 
 * Get symbol size of a given symbol node.
 * 
 * @param symbol_node The node which size is wanted.
 * @return Size in bytes of a given symbol node.
 */
int symbolNodeGetSymbolSize(SymbolNode *symbol_node){
    /* Check if symbol node is not NULL */
    if(symbol_node == NULL){
        fprintf(stderr, "[ERROR] ERROR SYMBOL NODE IS NULL!\n");
        return -1;
    }
    
    /* Return symbol relative address */
    return symbol_node->symbol_size;
}

/**
 * Get define a symbol type into a register instruction.
 * 
 * @param symbol_node The node which instruction is wanted.
 * @return Pointer to a instruction node with the apropriated load instruction.
 */
InstructionNode* symbolNodeGetDefineInstruction(SymbolNode *symbol_node){
    /* Check if symbol node is not NULL */
    if(symbol_node == NULL){
        fprintf(stderr, "[ERROR] ERROR SYMBOL NODE IS NULL!\n");
        return NULL;
    }

    /* If this node belong toa global symbol table */
    if(symbol_node->root_symbol_table->start_address == GLOBAL_START_ADRESS){
        return newInstructionNode(formatedInstruction(mips_global_define, symbol_node->symbol_name), false);
    }

    /* Return the formated instruction */
    return newInstructionNode((char *) mips_local_define, false);
}

/**
 * Get load a symbol into a register instruction.
 * 
 * @param symbol_node The node which instruction is wanted.
 * @return Pointer to a instruction node with the apropriated load instruction.
 */
InstructionNode* symbolNodeGetLoadInstruction(SymbolNode *symbol_node){
    /* Check if symbol node is not NULL */
    if(symbol_node == NULL){
        fprintf(stderr, "[ERROR] ERROR SYMBOL NODE IS NULL!\n");
        return NULL;
    }

    /* If this node belong toa global symbol table */
    if(symbol_node->root_symbol_table->start_address == GLOBAL_START_ADRESS){
        return newInstructionNode(formatedInstruction(mips_global_load, symbol_node->symbol_name), false);
    }

    /* Return the formated instruction */
    return newInstructionNode(formatedInstruction(mips_local_load, symbol_node->symbol_address), false);
}

/** 
 * Get store a symbol into a register instruction.
 *  
 * @param symbol_node The node which instruction is wanted.
 * @return Pointer to a instruction node with the apropriated store instruction.
 */
InstructionNode* symbolNodeGetStoreInstruction(SymbolNode *symbol_node){
    /* Check if symbol node is not NULL */
    if(symbol_node == NULL){
        fprintf(stderr, "[ERROR] ERROR SYMBOL NODE IS NULL!\n");
        return NULL;
    }
    
    /* If this node belong toa global symbol table */
    if(symbol_node->root_symbol_table->start_address == GLOBAL_START_ADRESS){
        return newInstructionNode(formatedInstruction(mips_global_store, symbol_node->symbol_name), false);
    }

    /* Return the formated instruction */
    return newInstructionNode(formatedInstruction(mips_local_store, symbol_node->symbol_address), false);
}

/**
 * Get load a symbol type into a register instruction.
 * 
 * @param symbol_node The node which instruction is wanted.
 * @return Pointer to a instruction node with the apropriated load instruction.
 */
InstructionNode* symbolNodeGetLoadTypeInstruction(SymbolNode *symbol_node){
    /* Check if symbol node is not NULL */
    if(symbol_node == NULL){
        fprintf(stderr, "[ERROR] ERROR SYMBOL NODE IS NULL!\n");
        return NULL;
    }
    
    /* Return the formated instruction */
    return newInstructionNode(formatedInstruction("# TODO - LOAD TYPE\n"), false);
}

/**
 * Get store a symbol type into a register instruction.
 *
 * @param symbol_node The node which instruction is wanted.
 * @return Pointer to a instruction node with the apropriated store instruction.
 */
InstructionNode* symbolNodeGetStoreTypeInstruction(SymbolNode *symbol_node){
    /* Check if symbol node is not NULL */
    if(symbol_node == NULL){
        fprintf(stderr, "[ERROR] ERROR SYMBOL NODE IS NULL!\n");
        return NULL;
    }
    
    /* Return the formated instruction */
    return newInstructionNode(formatedInstruction("# TODO - STORE TYPE\n"), false);
}

/* ------------- Symbol Table Methods ------------- */

/**
 * Create a new SymbolTable.
 * 
 * @param previous_scope Previous symbol table, define symbol scope.
 * @return An empty symbol table.
 */
SymbolTable* newSymbolTable(SymbolTable *previous_scope){
    SymbolTable *_new_symbol_table;
    SymbolNode **_items_table;
    
    /* Allocate a new symbol table */
    _new_symbol_table = mallocIt(_new_symbol_table);
    
    /* Give a fatal error if cannot allocate new symbol table */
    if(_new_symbol_table == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW SYMBOL TABLE!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Allocate item symbols */
    _items_table = mallocItArray(_items_table, DEFAULT_BLOCK_SIZE);
    
     /* Give a fatal error if cannot allocate items of the table */
    if(_items_table == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE ARRAY OF SYMBOLS!\n");
        exit(EXIT_FAILURE);
    }
    
    /* If there are no previous scope */
    if(previous_scope == NULL){
        _new_symbol_table->start_address = 0;
    }
    else{
        _new_symbol_table->start_address = previous_scope->start_address + previous_scope->shift_address;
    }
    
    /* Set the new symbol table */
    _new_symbol_table->size = DEFAULT_BLOCK_SIZE;
    _new_symbol_table->length = 0;
    _new_symbol_table->shift_address = 0;
    _new_symbol_table->previous_scope = previous_scope;
    _new_symbol_table->items = _items_table;
    
    /* Return pointer of the new symbol table */
    return _new_symbol_table;
}

/** 
 * Create a new global Symbol Table.
 * 
 * @return A new global symbol table.
 */
SymbolTable* newGlobalSymbolTable(){
    SymbolTable *_new_global_symbol_table;

    /* Allocate a new symbol table */
    _new_global_symbol_table = newSymbolTable(NULL);
    
    /* Give a fatal error if cannot allocate new symbol table */
    if(_new_global_symbol_table == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW GLOBAL SYMBOL TABLE!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Global Symbol Table Start Address */
    _new_global_symbol_table->start_address = GLOBAL_START_ADRESS;
    
    /* Return pointer of the new symbol table */
    return _new_global_symbol_table;
}

/** 
 * Destroy an already allocated symbol table.
 * 
 * @param symbol_table pointer to the symbol table that will be destroyed.
 * @return true if there's no error on execution and false otherwise.
 */
bool deleteSymbolTable(ptrSymbolTable *symbol_table){
    int i;
    
    /* Check if token node exists */
    if(symbol_table == NULL){
        fprintf(stderr, "[ERROR] CANNOT DESTROY A NULL SYMBOL TABLE VARIABLE!\n");
        return false;
    }
    
    /* Check if token node exists */
    if((*symbol_table) == NULL){
        fprintf(stderr, "[ERROR] CANNOT DESTROY AN ALREADY NULL SYMBOL TABLE!\n");
        return false;
    }
    
    /* Check if there are items on this symbol table */
    if((*symbol_table)->items != NULL){
        
        /* Delete every symbol node on this table */
        for(i = 0; i < (*symbol_table)->length; i++){
            deleteSymbolNode(&(*symbol_table)->items[i]);
        }
        
        /* Remove this array */
        //secureFree((*symbol_table)->items);
    }
    
    /* Free symbol_table  */
    //secureFree((*symbol_table));
    
    /* Return success */
    return true;
}

/**
 * Add a new symbol to symbol table.
 * 
 * @param symbol_table Table of symbols.
 * @param symbol_name Name of the symbol which will be appended to symbol table.
 * @param symbol_type Type of the symbol which will be appended to symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool symbolTableAddSymbol(SymbolTable *symbol_table, char *symbol_name, int symbol_type){
    SymbolNode *_new_symbol_node;
    SymbolNode **_reallocated_items;
    int i;
    
    /* Check if symbol name is null or incorrect */
    if((symbol_name == NULL) || (strlen(symbol_name) == 0)){
        fprintf(stderr, "[ERROR] SYMBOL NAME EMPTY OR INVALID !\n");
        return false;
    }
    
    /* Check if the symbol is already on symbol table */
    if(symbol_table->length != 0 && symbolTableContains(symbol_table, symbol_name)){
        return false;
    }
    
    /* Check if there are space on symbol table */
    if((symbol_table->length + 1) == symbol_table->size){
        /* Increase structure size */
        symbol_table->size += DEFAULT_BLOCK_SIZE;
        
        /* Try reallocate array of symbol_table items */
        _reallocated_items = reallocItArray(symbol_table->items, symbol_table->size);
        
        /* Give a fatal error if malloc has errors */
        if(_reallocated_items == NULL){
            fprintf(stderr, "[ERROR] FATAL ERROR CANNOT REALLOCATE SYMBOL TABLE ITEMS!\n");
            exit(EXIT_FAILURE);
        }
        
        /* Store the pointer to the new items list array */
        symbol_table->items = _reallocated_items;
    }
    
    /* Try allocate the new symbol node */
    _new_symbol_node = newSymbolNode(symbol_table, symbol_name, symbol_table->start_address + BYTE_VARIABLE_SIZE, symbol_type);
    
    /* Check if we can't allocate a new symbol node has returned no errors */
    if(_new_symbol_node == NULL){
        fprintf(stderr, "[ERROR] CANNOT CREATE NEW SYMBOL NODE!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Increase address of previous nodes */
    for(i = 0; i < symbol_table->length; i++){
        symbol_table->items[i]->symbol_address += BYTE_VARIABLE_SIZE;
    }
    
    /* Increase shift */
    symbol_table->shift_address += BYTE_VARIABLE_SIZE;
    
    /* Add the new symbol */
    symbol_table->items[symbol_table->length] = _new_symbol_node;
    
    /* Increase number of symbols */
    symbol_table->length += 1;
    
    /* Return success */
    return true;
}

/**
 * Check if a symbol of a given name is present on the table.
 * 
 * @param symbol_table Table of symbols.
 * @param symbol_name symbol which is being searched.
 * @return Return true if symbol name exist on table and false otherwise.
 */
bool symbolTableContains(SymbolTable *symbol_table, char *symbol_name){
    int i;
    SymbolTable *_actual_symbol_table;
    
    /* Check if symbol table is not NULL */
    if(symbol_table == NULL){
        fprintf(stderr, "[ERROR] SYMBOL TABLE IS NULL!\n");
        return false;
    }
    
    /* Check if symbol_name is not NULL */
    if(symbol_name == NULL){
        fprintf(stderr, "[ERROR] SYMBOL NAME IS NULL!\n");
        return false;
    }
    
    /* Get the actual symbl table */
    _actual_symbol_table = symbol_table;
    
    /* Search for the variable in the actual symbol table */
    while(_actual_symbol_table != NULL){
    
        /* Check if the symbol table have elements and them search for */ 
        if(symbol_table->length != 0){
            
            /* Check if symbol table already has a symbol with this name */
            for(i = 0; i < symbol_table->length; i++){
                if(symbolEqualsName(symbol_table->items[i], symbol_name)){
                    return true;
                }
            }
        }
        
        /* Get the previous symbol table */
        _actual_symbol_table = _actual_symbol_table->previous_scope; 
    }
    
    /* The element is not on symbol table */
    return false;
}

/**
 * Get symbol node by name.
 * 
 * @param symbol_table Table of symbols.
 * @param symbol_name Name of the symbol wanted.
 * @return A pointer to the symbol with the given name
 */
SymbolNode* symbolTableGetSymbolNodeByName(SymbolTable *symbol_table, char *symbol_name){
    int i;
    SymbolTable *_actual_symbol_table;
    
    /* Check if symbol table is not NULL */
    if(symbol_table == NULL){
        fprintf(stderr, "[ERROR] SYMBOL TABLE IS NULL!\n");
        return false;
    }
    
    /* Check if symbol_name is not NULL */
    if(symbol_name == NULL){
        fprintf(stderr, "[ERROR] SYMBOL NAME IS NULL!\n");
        return false;
    }

    /* Get the actual symbl table */
    _actual_symbol_table = symbol_table;
    
    /* Search for the variable in the actual symbol table */
    while(_actual_symbol_table != NULL){
        
        /* Check if the symbol table have elements and them search for */ 
        if(symbol_table->length != 0){
            
            /* Check if symbol table already has a symbol with this name */
            for(i = 0; i < symbol_table->length; i++){
                if(symbolEqualsName(symbol_table->items[i], symbol_name)){
                    return symbol_table->items[i];
                }
            }
        }
        
        /* Get the previous symbol table */
        _actual_symbol_table = _actual_symbol_table->previous_scope; 
    }
    
    /* The element is not on symbol table */
    return NULL;
}

/* ---------- Instruction Node Methods ----------- */

/**
 * Return a new Instruction Node.
 * 
 * @param instruction_string string of the instruction.
 * @param copyInstruction Flag that ask if the user want to copy new instruction or just store the pointer
 * @return a newly created instruction node.
 */
InstructionNode *newInstructionNode(char* instruction_string, bool copyInstruction){
    InstructionNode *_new_instruction_node;
    char *_new_instruction_string;
    
    /* Try allocate this new instruction */
    _new_instruction_node = mallocIt(_new_instruction_node);
    
    /* Give a fatal error if malloc has errors */
    if(_new_instruction_node == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW SYMBOL!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Check if the user wants to copy the instruction or use old pointer */
    if(copyInstruction){
        /* Try allocate the name of the new instruction */
        _new_instruction_string = mallocItString(_new_instruction_string, instruction_string);
        
        /* Give a fatal error if malloc has errors */
        if(_new_instruction_string == NULL){
            fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW SYMBOL!\n");
            exit(EXIT_FAILURE);
        }
        
        /* Copy content to the new instruction */
        secureStringCopy(_new_instruction_string, instruction_string);
    }
    else{
        /* Store the new pointer into parameter */
        _new_instruction_string = instruction_string;
    }
    
    /* Set the new instrunction */
    _new_instruction_node->instruction = _new_instruction_string;
    _new_instruction_node->length = (int) strlen(_new_instruction_string);

    /* Return pointer to the new instruction node */
    return _new_instruction_node;
}

/** 
 * Destroy an already allocated instruction node.
 * 
 * @param instruction_node pointer to the instruction node that will be destroyed.
 * @return true if there's no error on execution and false otherwise.
 */
bool deleteInstructionNode(ptrInstructionNode *instruction_node){
    
    /* Check if token node exists */
    if(instruction_node == NULL){
        fprintf(stderr, "[ERROR] CANNOT DESTROY A NULL INSTRUCTION NODE VARIABLE!\n");
        return false;
    }
    
    /* Check if token node exists */
    if((*instruction_node) == NULL){
        fprintf(stderr, "[ERROR] CANNOT DESTROY AN ALREADY NULL INSTRUCTION NODE!\n");
        return false;
    }
    
    /* Check if there are a instruction string on this node */
    if((*instruction_node)->instruction != NULL){
        
        /* Remove this string */
        //secureFree((*instruction_node)->instruction);
    }
    
    /* Free instruction node  */
    //secureFree((*instruction_node));
    
    /* Return success */
    return true;
}

/**
 * Print a instruction node.
 * 
 * @param _output_file Output stream file.
 * @param instruction The instruction that will be printed.
 * @return true if there's no error on execution and false otherwise.
 */
bool instructionNodeFilePrint(FILE *_output_file, InstructionNode *instruction_node){
    int _fprintf_status;
    
    /* Check if instruction node is not NULL */
    if(instruction_node == NULL){
        fprintf(stderr, "[ERROR] INSTRUCTION IS NULL!\n");
        return false;
    }
    
    /* Check if _output_file is not NULL */
    if(_output_file == NULL){
        fprintf(stderr, "[ERROR] OUTPUT FILE IS NULL!\n");
        return false;
    }
    
    /* Print the instruction */
    _fprintf_status = fprintf(_output_file, "%s", instruction_node->instruction);
    
    /* Check if fprintf has worked */
    if(_fprintf_status < 0){
        fprintf(stderr, "[ERROR] CANNOT WRITE TO OUTPUT FILE, ERROR %d!\n", _fprintf_status);
        return false;
    }
    
    /* Return Sucess */
    return true;
}

/**
 * Get instruction length.
 * 
 * @param instruction The instruction.
 * @return a positivie number if instruction is valid and a negative number if instruction or instruction value is null;
 */
int instructionNodeLength(InstructionNode *instruction){
    /* Check if instruction node is not NULL */
    if(instruction == NULL){
        fprintf(stderr, "[ERROR] INSTRUCTION IS NULL!\n");
        return -1;
    }
    
    /* Check if instruction value is not NULL */
    if(instruction->instruction == NULL){
        fprintf(stderr, "[ERROR] INSTRUCTION VALUE IS NULL!\n");
        return -1;
    }
    
    /* Return size of the given instruction node */
    return instruction->length;
}

/* ---------- Instruction Queue Methods ----------- */

/**
 * Return a new Instruction Queue.
 * 
 * @return a newly created instruction queue.
 */
InstructionQueue *newInstructionQueue(){
    InstructionQueue *_new_instruction_queue;
    InstructionNode **_new_instruction_nodes;
    
    /* Try to malloc a new Instruction queue */
    _new_instruction_queue = mallocIt(_new_instruction_queue);
    
    /* Give a fatal error if malloc failled */
    if(_new_instruction_queue == NULL){
        fprintf(stderr, "[ERROR] CANNOT ALLOCATE NEW INSTRUCTION QUEUE\n");
        exit(EXIT_FAILURE);
    }
    
    /* Mallocate array of instruction nodes */
    _new_instruction_nodes = mallocItArray(_new_instruction_nodes, DEFAULT_BLOCK_SIZE);
    
    /* Give a fatal error if malloc failed */
    if(_new_instruction_nodes == NULL){
        fprintf(stderr, "[ERROR] CANNOT ALLOCATE ARRAY OF INSTRUCTIONS NODE\n");
        exit(EXIT_FAILURE);
    }
    
    /* Set the new Instruction Queue */
    _new_instruction_queue->size = DEFAULT_BLOCK_SIZE;
    _new_instruction_queue->length = 0;
    _new_instruction_queue->instructions = _new_instruction_nodes;
    
    /* Return pointer to the new instruction queue object */
    return _new_instruction_queue;
}

/** 
 * Destroy an already allocated instruction queue.
 * 
 * @param instruction_queue pointer to the symbol table that will be destroyed.
 * @return true if there's no error on execution and false otherwise.
 */
bool deleteInstructionQueue(ptrInstructionQueue *instruction_queue){
    int i;
    InstructionNode *tmp_node;
    
    /* Check if token node exists */
    if(instruction_queue == NULL){
        fprintf(stderr, "[ERROR] CANNOT DESTROY A NULL INSTRUCTION QUEUE VARIABLE!\n");
        return false;
    }
    
    /* Check if token node exists */
    if((*instruction_queue) == NULL){
        fprintf(stderr, "[ERROR] CANNOT DESTROY AN ALREADY NULL INSTRUCTION QUEUE!\n");
        return false;
    }
    
    /* Check if there are instructions on this instruction queue */
    if((*instruction_queue)->instructions != NULL){
        
        /* Delete every instruction on this instruction queue */
        for(i = 0; i < (*instruction_queue)->length; i++){
            tmp_node = (*instruction_queue)->instructions[i];
            deleteInstructionNode(&tmp_node);
        }
        
        /* Remove this array */
        //secureFree((*instruction_queue)->instructions);
    }
    
    /* Free instruction queue  */
    //secureFree((*instruction_queue));
    
    /* Return success */
    return true;
}

/**
 * Add a new instruction to Instruction Queue.
 * 
 * @param instruction_queue Instruction Queue.
 * @param instruction_string Instruction String.
 * @param copyInstruction check if user wants to copy instruction or just store the pointer.
 * @return true if there's no error on execution and false otherwise. 
 */
bool instructionQueueEnqueueInstruction(InstructionQueue *instruction_queue, char *instruction_string, bool copyInstruction){
    InstructionNode *_new_instruction_node;
    InstructionNode **_reallocated_instructions;
    
    /* Try to create a new instruction node */
    _new_instruction_node = newInstructionNode(instruction_string, copyInstruction);
    
    /* Check if for some reason new instruction node is null */
    if(_new_instruction_node == NULL){
        fprintf(stderr, "[ERROR] RECEIVED A NULL NEW INSTRUCTION NODE!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Check if there are left spaces on instruction queue */
    if((instruction_queue->length + 2) >= instruction_queue->size){
        /* Increase block size */
        instruction_queue->size += DEFAULT_BLOCK_SIZE;
        
        /* Try to reallocate instructions array */
        _reallocated_instructions = reallocItArray(instruction_queue->instructions, instruction_queue->size);
        
        /* Give fatal error if malloc has failled */
        if(_reallocated_instructions == NULL){
            fprintf(stderr, "[ERROR] WHEN TRY TO REALLOCATE NEW INSTRUCTION NODE ARRAY!\n");
            exit(EXIT_FAILURE);
        }
        
        /**
         * Thanks for Roderick Gibson (GH @kniteli) from https://hackhands.com
         * for helping with found the bug on instruction queue enqueue,
         * now I'l never forget to replace the reallocated pointer after a reallocation
         * process. 
         */
        instruction_queue->instructions = _reallocated_instructions;
    }
    
    /* Store the new instruction node */
    instruction_queue->instructions[instruction_queue->length] = _new_instruction_node;
    instruction_queue->length += 1;
    
    /* Return success */
    return true;
}

/* Add a new instruction node structure to instruction queue */
bool instructionQueueEnqueueInstructionNode(InstructionQueue *instruction_queue, InstructionNode *instruction_node){
    InstructionNode **_reallocated_instructions;
    
    /* Check if instruction queue is null */
    if(instruction_queue == NULL){
        fprintf(stderr, "[ERROR] INSTRUCTION QUEUE IS NULL!\n");
        return false;
    }
    
    /* Check if instruction node is null */
    if(instruction_node == NULL){
        fprintf(stderr, "[ERROR] INSTRUCTION NODE IS NULL!\n");
        return false;
    }
    
    /* Check if there are left spaces on instruction queue */
    if((instruction_queue->length + 1) == instruction_queue->size){
        /* Increase block size */
        instruction_queue->size += DEFAULT_BLOCK_SIZE;
        
        /* Try to reallocate instructions array */
        _reallocated_instructions = reallocItArray(instruction_queue->instructions, instruction_queue->size);
        
        /* Give fatal error if malloc has failled */
        if(_reallocated_instructions == NULL){
            fprintf(stderr, "[ERROR] WHEN TRY TO REALLOCATE NEW INSTRUCTION NODE ARRAY!\n");
            exit(EXIT_FAILURE);
        }
        
        /* Assign the reallocated pointer array */
        instruction_queue->instructions = _reallocated_instructions;
    }
    
    /* Store the new instruction node */
    instruction_queue->instructions[instruction_queue->length] = instruction_node;
    instruction_queue->length += 1;

    /* Return success */
    return true;
}


/** 
 * Print a instruction queue on a given file.
 * 
 * @param _output_file output file, use stdout or stderr for system files, and a pointer to a valid file.
 * @param instruction Instruction Queue that will be printed.
 * @return true if there's no error on execution and false otherwise. 
 */
bool instructionQueueFilePrint(FILE *_output_file, InstructionQueue *instruction_queue){
    int i;
    
    /* Check if _output_file is not null */
    if(_output_file == NULL){
        fprintf(stderr,"[ERROR] OUTPUT FILE IS NULL!\n");
        return false;
    }
    
    /* Check if instruction_queue is not null */
    if(instruction_queue == NULL){
        fprintf(stderr, "[ERROR] INSTRUCTION QUEUE IS NULL!\n");
        return false;
    }
    
    /* Print all instructions in instruction queue to _output_file */
    for(i = 0; i < instruction_queue->length; i++){
        /* Execute fprintf, and return ex */
        if(!instructionNodeFilePrint(_output_file, instruction_queue->instructions[i])){
            return false;
        }
    }
    
    /* Return success */
    return true;
}

/* ------------------------------------------------ */

/** 
 * After write this code for three long weaks, every day and night and morning and another time
 * that I had forgot, I need to say that write this was a pain, C is good for processor, but not
 * for programming, after this job, I wish at least one weak of rest, because I'm too near of
 * being insane. Congratulations dude, you fixed a big code, never forget this...
 * 
 * NEVER FORGET THAT DAY DUDE!
 * 
 *                                                      Jeferson Lima de Almeida - 03/11/2016
 */
 
/* ------------------------------------------------ */