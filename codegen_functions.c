/* Codegen header */
#include "codegen_functions.h"

/* Token Structure */
#include "token_struct.h"

/* Utils */
#include "utils.h"

/* Generated Code */
extern InstructionQueue *header_instruction_queue;

/* Main Generated Code */
extern InstructionQueue *main_instruction_queue;

/* Global Symbol Table */
extern SymbolTable *global_symbol_table;

/** 
 * Copy global variables into header instruction queue.
 * 
 * @return true if there's no error on execution and false otherwise.
 */
bool copyGlobalVariables(){
    int i;
    
    /* Check if header instruction queue is null */
    if(header_instruction_queue == NULL){
        fprintf(stderr, "[ERROR] HEADER INSTRUCTION QUEUE, NOT INITIALIZED!\n");
        return false;
    }
    
    /* Check if global symbol table is null */
    if(global_symbol_table == NULL){
        fprintf(stderr, "[ERROR] GLOBAL SYMBOL TABLE, NOT INITIALIZED!\n");
        return false;
    }
    
    /* Enqueue global definition of every node in global symbol table */
    for(i = 0; i < global_symbol_table->length; i++){
        (global_symbol_table->items[i]);
    }
}

/**
 * Generate code based on AST.
 * 
 * @param root_token the root token node of the AST.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenAllCode(TokenNode *root_token){
    /* Initialize Header Instruction Queue */
    header_instruction_queue = newInstructionQueue();
    
    /* Check if the header instruction queue was corrected initialized */
    if(header_instruction_queue == NULL){
        fprintf(stderr, "[ERROR] CANNOT INITIALIZE HEADER INSTRUCTION QUEUE.\n");
        return false;
    }
    
    /* Initialize Main Instruction Queue */
    main_instruction_queue = newInstructionQueue();
    
    /* Check if the header instruction queue was corrected initialized */
    if(main_instruction_queue == NULL){
        fprintf(stderr, "[ERROR] CANNOT INITIALIZE MAIN INSTRUCTION QUEUE.\n");
        return false;
    }
    
    /* Initialize Global Symbol Table */
    global_symbol_table = newSymbolTable(GLOBAL_START_ADRESS);
    
    /* Check if the global symbol was correct initialized */
    if(global_symbol_table == NULL){
        fprintf(stderr, "[ERROR] CANNOT CREATE GLOBAL SYMBOL TABLE.\n");
        return false;
    }
    
    /* Block token node */
    TokenNode block_token = listGetTokenByIndex(root_token->child_list, 1);
    
    /* Check if the block token really exists */
    if(block_token == NULL){
        fprintf(stderr, "[ERROR] CANNOT GET BLOCK TOKEN NODE.\n");
        return false
    }
    
    /* Add header on header instruction queue */
    instructionQueueEnqueueInstruction(header_instruction_queue, mips_header, false);
    
    /* Add header on main instruction queue */
    instructionQueueEnqueueInstruction(main_instruction_queue, mips_main, false);
    
    /* Generate code for main application */
    cgenBlockCode(block_token, NULL);
    
    /* Copy variable definitions to main-instruction_queue */
    copyGlobalVariables();
    
    /* Add header on main instruction queue */
    instructionQueueEnqueueInstruction(main_instruction_queue, mips_footer, false);
    
    /* Return success */
    return true;
}

/**
 * Generate block sections of code.
 * 
 * @param block_token Receive a block token.
 * @param previous_scope Previous table symbol escope.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenBlockCode(TokenNode *block_token, SymbolTable *previous_scope){
    
}

void solveExpression(TokenNode token_node) {
    TokenNode * token_left = listGetTokenByIndex(token_node->child_list, 1);
    TokenNode * token_right = listGetTokenByIndex(token_node->child_list, 3);
    
    if(IS_BINARY_OPERAND(token_node->token_type)) {
        //CGEN(exp1)
        solveExpression(token_left);
        
        // push_a0
        instructionQueueEnqueueInstruction(main_instruction_queue, mips_push_a0, false);
        
        //CGEN(exp2)
        solveExpression(token_right);
        
        //top_t1
        instructionQueueEnqueueInstruction(main_instruction_queue, mips_top_t1, false);
    
        //CGEN(operand)
        switch(token_node->token_type) {
            case TI_PLUS:
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_add_a0_t1_a0, false);
                break;
                
            case TI_MINUS:
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_sub_a0_t1_a0, false);
                break;
                
            case TI_TIMES:
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_mul_a0_t1_a0, false);
                break;
                
            case TI_DIV:
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_div_a0_t1_a0, false);
                break;
                
            case TI_GT:
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_gt_a0_t1_a0, false);
                break;
                
            case TI_GTEQ:
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_gte_a0_t1_a0, false);
                break;
                
            case TI_LT:
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_lt_a0_t1_a0, false);
                break;
                
            case TI_LTEQ:
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_lte_a0_t1_a0, false);
                break;
                
            case TI_EQ:
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_eq_a0_t1_a0, false);
                break;
                
            case TI_NEQ:
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_neq_a0_t1_a0, false);mips_neq_a0_t1_a0
                break;
                
            default:
                break;
        }
        
        //pop
        instructionQueueEnqueueInstruction(main_instruction_queue, mips_pop, false);
    }
    
    int i;
    TokenNode * first_root_child = listGetTokenByIndex(token_node->root_token->child_list, 1);
    
    switch (token_node->type) {
        case TI_LISTAEXP:
            if (token_node->root_token->type == TI_CALL_FUNCTION) {
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_start_function_call, false);
                
                for(i = token_node->child_list->length - 1; i >= 0; i--) {
                    solveExpression(listGetTokenByIndex(token_node->child_list, i));
                    instructionQueueEnqueueInstruction(main_instruction_queue, mips_push_a0, false);
                }
                
                //jal f_entry
                instructionQueueEnqueueInstruction(main_instruction_queue, formatedInstruction(mips_end_function_call, first_root_child->lex_str)), false);
            }
        
            break;
    }
}