#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "token_struct.h"

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
    _new_token_node = (TokenNode*) malloc(sizeof(TokenNode));
    
    /* Give a falta error if malloc has errors */
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
 * Destroy a already allocated token node.
 * 
 * @param token that will be destroyed.
 * @return true if there's no error on execution and false otherwise.
 */
bool deleteTokenNode(ptrTokenNode *token_node){
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
        free((*token_node)->token_str);
    }
    
    /* Free lex_str if it exists */
    if((*token_node)->lex_str != NULL){
        free((*token_node)->lex_str);
    }
    
    /* Free toke_node pointer */
    free((*token_node));

    /* Null the pointer apponted by token_node */
    (*token_node) = NULL;
    
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
    _new_token_str = (char *) malloc(sizeof(char) * (strlen(token_str) + 1));
    
    /* Check if malloc has succeed */
    if(_new_token_str == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE STRING FOR TOKEN!\n");
        return false;
    }
    
    /* Store the new token_str array pointer */
    token_node->token_str = _new_token_str;
    
    /* Copy content from token_str to the new token */
    strcpy(token_node->token_str, token_str);
    
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
    _new_lex_str = (char *) malloc(sizeof(char) * (strlen(lex_str) + 1));
    
    /* Check if malloc has succeed */
    if(_new_lex_str == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE STRING FOR TOKEN!\n");
        return false;
    }
    
    /* Store the new lex_str array pointer */
    token_node->lex_str = _new_lex_str;
    
    /* Copy content from lex_str to the new token */
    strcpy(token_node->lex_str, lex_str);
    
    /* Return success */
    return true;
}

/**
 * Add Token String.
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
    _new_token_list = (TokenList*) malloc(sizeof(TokenList));
    
    /* Give a falta error if malloc has errors */
    if(_new_token_list == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE TOKEN LIST!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Try to allocate an array of tokens */
    _items_list = (TokenNode**) malloc(sizeof(TokenNode*) * DEFAULT_BLOCK_SIZE);
    
    /* Give a falta error if malloc has errors */
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
    if(token_list->length == token_list->size){
        /* Increase structure size */
        token_list->size += DEFAULT_BLOCK_SIZE;
        
        /* Try reallocate array of token_list items */
        _reallocated_items = (TokenNode**) realloc( token_list->items,
                                                    (token_list->size)
                                                        * sizeof(TokenNode *));
        
        /* Give a falta error if malloc has errors */
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
    for(i = 0; i < token_list->length; i++){
        /* Pick the token at index i */
        _token_i = listGetTokenByIndex(token_list, i + 1);
        
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
 * @param symbol_name Name of the given symbol.
 * @param symbol_value Value of the symbol, use NULL to store nil value.
 * @return A new symbol, or fatal erro cause it fails.
 */
SymbolNode* newSymbolNode(char *symbol_name, char *symbol_value){
    SymbolNode *_new_symbol_node;
    char *_new_symbol_name;
    char *_new_symbol_value;
    
    /* Try allocate this new symbol */
    _new_symbol_node = (SymbolNode*) malloc(sizeof(SymbolNode));
    
    /* Give a falta error if malloc has errors */
    if(_new_symbol_node == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW SYMBOL!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Try allocate the name of the new symbol */
    _new_symbol_name = (char*) malloc(sizeof(char) * (strlen(symbol_name) + 1));
    
    /* Give a falta error if malloc has errors */
    if(_new_symbol_name == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW SYMBOL!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Copy contents of symbol_value if it's not null */
    if(symbol_value != NULL){
        _new_symbol_value = (char *) malloc(sizeof(char) * (strlen(symbol_value) + 1));
        
        /* Give a falta error if malloc has errors */
        if(_new_symbol_value == NULL){
            fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW SYMBOL VALUE!\n");
            exit(EXIT_FAILURE);
        }
        
        /* Copy contents of symbol_value to the new symbol_value */
        strcpy(_new_symbol_value, symbol_value);
    }
    
    /* Copy content of the symbol name to this new structure */
    strcpy(_new_symbol_name, symbol_name);

    /* Set the new symbol node */
    _new_symbol_node->symbol_name = _new_symbol_name;
    _new_symbol_node->symbol_value = (symbol_value == NULL) ? NULL : _new_symbol_value;
    _new_symbol_node->isNull = (symbol_value == NULL);
    
    /* Return pointer to this new node */
    return _new_symbol_node;
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

/* ------------- Symbol Table Methods ------------- */

/** 
 * Create a new global Symbol Table.
 * 
 * @return A new global symbol table.
 */
SymbolTable* newGlobalSymbolTable(){
    SymbolTable *_new_global_symbol_table;
    SymbolNode **_items_table;
    
    /* Allocate a new symbol table */
    _new_global_symbol_table = (SymbolTable*) malloc(sizeof(SymbolTable));
    
    /* Give a fatal error if cannot allocate new symbol table */
    if(_new_global_symbol_table == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW GLOBAL SYMBOL TABLE!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Allocate item symbols */
    _items_table = (SymbolNode **) malloc(sizeof(SymbolNode *) * DEFAULT_BLOCK_SIZE);
    
     /* Give a fatal error if cannot allocate items of the table */
    if(_items_table == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE ARRAY OF SYMBOLS FOR GLOBAL SYMBOL TABLE!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Global Symbol Table Start Address */
    _new_global_symbol_table->start_address = GLOBAL_START_ADRESS;
    
    /* Set the new symbol table */
    _new_global_symbol_table->size = DEFAULT_BLOCK_SIZE;
    _new_global_symbol_table->length = 0;
    _new_global_symbol_table->shift_address = 0;
    _new_global_symbol_table->previous_scope = previous_scope;
    _new_global_symbol_table->items = _items_table;
    
    /* Return pointer of the new symbol table */
    return _new_global_symbol_table;
}

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
    _new_symbol_table = (SymbolTable*) malloc(sizeof(SymbolTable));
    
    /* Give a fatal error if cannot allocate new symbol table */
    if(_new_symbol_table == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW SYMBOL TABLE!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Allocate item symbols */
    _items_table = (SymbolNode **) malloc(sizeof(SymbolNode *) * DEFAULT_BLOCK_SIZE);
    
     /* Give a fatal error if cannot allocate items of the table */
    if(_items_table == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE ARRAY OF SYMBOLS!\n");
        exit(EXIT_FAILURE);
    }
    
    /* If there are previous scope */
    if(previous_scope != NULL){
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
 * Add a new symbol to symbol table.
 * 
 * @param symbol_table Table of symbols.
 * @param symbol_name Name of the symbol which will be appended to symbol table.
 * @param symbol_type Type of the symbol which will be appended to symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool symbolTableAddSymbol(SymbolTable *symbol_table, char *symbol_name, int symbol_type);
    SymbolNode *_new_symbol_node;
    SymbolNode **_reallocated_items;
    
    /* Check if symbol name is null or incorrect */
    if((symbol_name == NULL) || (strlen(symbol_name) == 0)){
        fprintf(stderr, "[ERROR] SYMBOL NAME EMPTY OR INVALID !\n");
        return false;
    }
    
    /* Check if the symbol is already on symbol table */
    if(symbol_table->length != 0 && symbolTableContains(symbol_table, symbol->symbol_name)){
        return false;
    }
    
    /* Check if there are space on symbol table */
    if(symbol_table->length == symbol_table->size){
        /* Increase structure size */
        symbol_table->size += DEFAULT_BLOCK_SIZE;
        
        /* Try reallocate array of symbol_table items */
        _reallocated_items = (SymbolNode**) realloc( symbol_table->items,
                                                    (symbol_table->size)
                                                        * sizeof(SymbolNode *));
        
        /* Give a falta error if malloc has errors */
        if(_reallocated_items == NULL){
            fprintf(stderr, "[ERROR] FATAL ERROR CANNOT REALLOCATE SYMBOL TABLE ITEMS!\n");
            exit(EXIT_FAILURE);
        }
        
        /* Store the pointer to the new items list array */
        symbol_table->items = _reallocated_items;
    }
    
    /* Try allocate the new symbol node */
    _new_symbol_node = newSymbolNode(symbol_name, _new_symbol_table->start_address + _new_symbol_table->shift_address + BYTE_VARIABLE_SIZE, symbol_type);
    
    /* Check if we can't allocate a new symbol node has returned no errors */
    if(_new_symbol_node == NULL){
        fprintf(stderr, "[ERROR] CANNOT CREATE NEW SYMBOL NODE!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Increase shift */
    _new_symbol_table->shift_address += BYTE_VARIABLE_SIZE;
    
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
    
    /* Check if the symbol table have elements and them search for */ 
    if(symbol_table->length != 0){
        int i;
        
        /* Check if symbol table already has a symbol with this name */
        for(i = 0; i < symbol_table->length; i++){
            if(symbolEqualsName(symbol_table->items[i], symbol_name)){
                return true;
            }
        }
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
    
    /* Check if the symbol table have elements and them search for */ 
    if(symbol_table->length != 0){
        int i;
        
        /* Check if symbol table already has a symbol with this name */
        for(i = 0; i < symbol_table->length; i++){
            if(symbolEqualsName(symbol_table->items[i], symbol_name)){
                return symbol_table->items[i];
            }
        }
    }
    
    /* The element is not on symbol table */
    return NULL;
}

/* ---------- Instruction Node Methods ----------- */

/**
 * Return a new Instruction Node.
 * 
 * @param instruction_string string of the instruction.
 * @param useTab Flag that define if this instruction starts or not with tab or '\t' char.
 * @param copyInstruction Flag that ask if the user want to copy new instruction or just store the pointer
 * @return a newly created instruction node.
 */
InstructionNode *newInstructionNode(char* instruction_string, bool useTab, bool copyInstruction){
    InstructionNode *_new_instruction_node;
    char *_new_instruction_string;
    
    /* Try allocate this new instruction */
    _new_instruction_node = (InstructionNode*) malloc(sizeof(InstructionNode));
    
    /* Give a falta error if malloc has errors */
    if(_new_instruction_node == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW SYMBOL!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Check if the user wants to copy the instruction or use old pointer */
    if(copyInstruction){
        /* Try allocate the name of the new instruction */
        _new_instruction_string = (char*) malloc(sizeof(char) * (strlen(instruction_string) + 1));
        
        /* Give a falta error if malloc has errors */
        if(_new_instruction_string == NULL){
            fprintf(stderr, "[ERROR] FATAL ERROR CANNOT ALLOCATE NEW SYMBOL!\n");
            exit(EXIT_FAILURE);
        }
        
        /* Copy content to the new instruction */
        strcpy(_new_instruction_string, instruction_string);
    }
    else{
        /* Store the new pointer into parameter */
        _new_instruction_string = instruction_string;
    }
    
    /* Set the new instrunction */
    _new_instruction_node->instruction = _new_instruction_string;
    _new_instruction_node->useTab = useTab;
    _new_instruction_node->length = (int) strlen(_new_instruction_string);

    /* Return pointer to the new instruction node */
    return _new_instruction_node;
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
    if(instruction_node->useTab){
        _fprintf_status = fprintf(_output_file, "%s" NEWLINE_CHAR, instruction_node->instruction);
    }
    else{
        _fprintf_status = fprintf(_output_file, "%s" NEWLINE_CHAR, instruction_node->instruction);
    }
    
    /* Check if fprintf has worked */
    if(_fprintf_status < 0){
        fprintf(stderr, "[ERROR] CANNOT WRITE TO OUTPUT FILE, ERROR %d!\n", _fprintf_status);
        return false;
    }
    
    /* Sucess */
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
    _new_instruction_queue = (InstructionQueue *) malloc(sizeof(InstructionQueue));
    
    /* Give a fatal error if malloc failled */
    if(_new_instruction_queue == NULL){
        fprintf(stderr, "[ERROR] CANNOT ALLOCATE NEW INSTRUCTION QUEUE\n");
        exit(EXIT_FAILURE);
    }
    
    /* Mallocate array of instruction nodes */
    _new_instruction_nodes = (InstructionNode **) malloc(sizeof(InstructionNode *) * DEFAULT_BLOCK_SIZE);
    
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
 * Add a new instruction to Instruction Queue.
 * 
 * @param instruction_queue Instruction Queue.
 * @param instruction_string Instruction String.
 * @param useTab Check if user want's add '\t' on start string.
 * @param copyInstruction check if user wants to copy instruction or just store the pointer.
 * @return true if there's no error on execution and false otherwise. 
 */
bool instructionQueueEnqueueInstruction(InstructionQueue *instruction_queue, char *instruction_string, bool useTab, bool copyInstruction){
    InstructionNode *_new_instruction_node;
    InstructionNode **_reallocated_instructions;
    
    /* Try to create a new instruction node */
    _new_instruction_node = newInstructionNode(instruction_string, useTab, copyInstruction);
    
    /* Check if for some reason new instruction node is null */
    if(_new_instruction_node == NULL){
        fprintf(stderr, "[ERROR] RECEIVED A NULL NEW INSTRUCTION NODE!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Check if there are left spaces on instruction queue */
    if(instruction_queue->length == instruction_queue->size){
        /* Increase block size */
        instruction_queue->size += DEFAULT_BLOCK_SIZE;
        
        /* Try to reallocate instructions array */
        _reallocated_instructions = (InstructionNode **) realloc(instruction_queue->instructions, sizeof(InstructionNode *) * instruction_queue->size);
        
        /* Give fatal error if malloc has failled */
        if(_reallocated_instructions == NULL){
            fprintf(stderr, "[ERROR] WHEN TRY TO REALLOCATE NEW INSTRUCTION NODE ARRAY!\n");
            exit(EXIT_FAILURE);
        }
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
    if(instruction_queue->length == instruction_queue->size){
        /* Increase block size */
        instruction_queue->size += DEFAULT_BLOCK_SIZE;
        
        /* Try to reallocate instructions array */
        _reallocated_instructions = (InstructionNode **) realloc(instruction_queue->instructions, sizeof(InstructionNode *) * instruction_queue->size);
        
        /* Give fatal error if malloc has failled */
        if(_reallocated_instructions == NULL){
            fprintf(stderr, "[ERROR] WHEN TRY TO REALLOCATE NEW INSTRUCTION NODE ARRAY!\n");
            exit(EXIT_FAILURE);
        }
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