/* Codegen header */
#include "codegen_functions.h"

/* Token Structure */
#include "token_struct.h"

/* Utils */
#include "utils.h"

/* Generated Code */
extern InstructionQueue *instruction_queue;

/* Global Symbol Table */
extern SymbolTable *global_symbol_table;

/**
 * Generate code based on AST.
 * 
 * @param root_token the root token node of the AST.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenAllCode(TokenNode *root_token);
    /* Initialize Instruction Queue */
    instruction_queue = newInstructionQueue();
    
    /* Initialize Global Symbol Table */
    global_symbol_table = newSymbolTable(GLOBAL_START_ADRESS);
    
    /* Check if the global symbol was correct initialized */
    if(global_symbol_table == NULL){
        fprintf(stderr, "")
    }
    
    /* Block token node */
    TokenNode block_token = listGetTokenByIndex(root_token->child_list, 1);
    
    /* Generate code for main application */
    cgenBlockCode(block_token);
    
    /* Return success */
    return true;
}


void solveExpression(TokenNode token_node) {
    
    switch(token_node->token_type) {
        case TI_AND:
            instruction_queue = mips_add_a0_t1_a0;
            break;
    }
    
    
    instruction_queue
}