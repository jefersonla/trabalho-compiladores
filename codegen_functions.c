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
 * Generate code based on AST.
 * 
 * @param root_token the root token node of the AST.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenAllCode(TokenNode *root_token);
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
    instructionQueueEnqueueInstruction(header_instruction_queue, mips_header, false, false);
    
    /* Add header on main instruction queue */
    instructionQueueEnqueueInstruction(main_instruction_queue, mips_system_functions, false, false);
    
    /* Generate code for main application */
    cgenBlockCode(block_token, NULL);
    
    /* Add header on main instruction queue */
    instructionQueueEnqueueInstruction(main_instruction_queue, mips_footer, false, false);
    
    /* Return success */
    return true;
}

bool cgenBlockCode(block_token, )

void solveExpression(TokenNode token_node) {
    TokenNode * token_left = listGetTokenByIndex(token_node->child_list, 1);
    TokenNode * token_right = listGetTokenByIndex(token_node->child_list, 3);
    
    if(IS_BINARY_OPERAND(token_node->token_type)) {
        //CGEN(exp1)
        solveExpression(token_left);
        
        // push_a0
        instructionQueueEnqueueInstruction(main_instruction_queue, mips_push_a0);
        
        //CGEN(exp2)
        solveExpression(token_right);
        
        //top_t1
        instructionQueueEnqueueInstruction(main_instruction_queue, mips_top_t1);
    
        //CGEN(operand)
        switch(token_node->token_type) {
            case TI_PLUS:
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_add_a0_t1_a0);
                break;
                
            case TI_MINUS:
                instructionQueueEnqueueInstruction(main_instruction_queue, mips_sub_a0_t1_a0);
                break;
                
            default:
                break;
        }
        
        //pop
        instructionQueueEnqueueInstruction(main_instruction_queue, mips_pop);
    }
}