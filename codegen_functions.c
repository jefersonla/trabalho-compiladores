/* Codegen header */
#include "codegen_functions.h"

/* Terminals define */
#ifdef DEBUG
#include "debug.y.tab.h"
#else
#include "y.tab.h"
#endif

/* Non Terminals defines */
#include "parser.defs.h"

/* Token Structure */
#include "token_struct.h"

/* Utils */
#include "utils.h"

/* Generated Code */
InstructionQueue *header_instruction_queue;

/* Main Generated Code */
InstructionQueue *main_instruction_queue;

/* Global Symbol Table */
SymbolTable *global_symbol_table;

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
        
        /* Check if instruction queue has failed */
        if(!instructionQueueEnqueueInstructionNode( header_instruction_queue,
                                                    symbolNodeGetLoadInstruction(global_symbol_table->items[i]))){
            return false;
        }
    }
    
    /* Return success */
    return true;
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
    global_symbol_table = newSymbolTable(NULL);
    
    /* Check if the global symbol was correct initialized */
    if(global_symbol_table == NULL){
        fprintf(stderr, "[ERROR] CANNOT CREATE GLOBAL SYMBOL TABLE.\n");
        return false;
    }
    
    /* Configure symbol table as global */
    global_symbol_table->start_address = GLOBAL_START_ADRESS;
    
    /* Block token node */
    TokenNode *block_token = listGetTokenByIndex(root_token->child_list, 1);
    
    /* Check if the block token really exists */
    if(block_token == NULL){
        fprintf(stderr, "[ERROR] CANNOT GET BLOCK TOKEN NODE.\n");
        return false;
    }
    
    /* Add header on header instruction queue */
    instructionQueueEnqueueInstruction(header_instruction_queue, (char *) mips_header, false);
    
    /* Add header on main instruction queue */
    addInstructionMainQueue(mips_main);
    
    /* Generate code for main application */
    cgenBlockCode(block_token, NULL);
    
    /* Copy variable definitions to main-instruction_queue */
    copyGlobalVariables();
    
    /* Add header on main instruction queue */
    addInstructionMainQueue(mips_footer);
    
    /* Return success */
    return true;
}


/** 
 * Generate code for function call.
 * 
 * @param _token
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenCallFunction(TokenNode *call_function_token, SymbolTable *actual_symbol_table){
    // TODO!
    fprintf(stderr, "[TODO] NOT IMPEMENTED YET!\n");
    return false;
}

/** 
 * Generate code for assign.
 * 
 * @param _token
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenAssign(TokenNode *assign_token, SymbolTable *actual_symbol_table){
    // TODO!
    fprintf(stderr, "[TODO] NOT IMPEMENTED YET!\n");
    return false;
}

/** 
 * Generate code for command block.
 * 
 * @param _token
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenCommandBlock(TokenNode *command_block_token, SymbolTable *actual_symbol_table){
    // TODO!
    fprintf(stderr, "[TODO] NOT IMPEMENTED YET!\n");
    return false;    
}

/** 
 * Generate code for while.
 * 
 * @param _token
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenWhile(TokenNode *while_token, SymbolTable *actual_symbol_table){
    // TODO!
    fprintf(stderr, "[TODO] NOT IMPEMENTED YET!\n");
    return false;    
}

/** 
 * Generate code for for.
 * 
 * @param _token
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenFor(TokenNode *for_token, SymbolTable *actual_symbol_table){
    // TODO!
    fprintf(stderr, "[TODO] NOT IMPEMENTED YET!\n");
    return false;    
}

/** 
 * Generate code for if.
 * 
 * @param _token
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenIf(TokenNode *if_token, SymbolTable *actual_symbol_table){
    // TODO!
    fprintf(stderr, "[TODO] NOT IMPEMENTED YET!\n");
    return false;    
}

/** 
 * Generate code for function definition call.
 * 
 * @param _token
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenFunction(TokenNode *function_def_token, SymbolTable *actual_symbol_table){
    // TODO!
    fprintf(stderr, "[TODO] NOT IMPEMENTED YET!\n");
    return false;    
}

/** 
 * Generate code for local variable operation.
 * 
 * @param _token
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenLocalVariable(TokenNode *local_variable_token, SymbolTable *actual_symbol_table){
    // TODO!
    fprintf(stderr, "[TODO] NOT IMPEMENTED YET!\n");
    return false;    
}

/** 
 * Generate code for command.
 * 
 * @param block_token Receive a command token.
 * @param actual_symbol_table Actual table symbol escope
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenCommand(TokenNode *command_token, SymbolTable *actual_symbol_table){
    /* Check if command list token is null */
    if(command_token == NULL){
        fprintf(stderr, "[ERROR] COMMAND LIST IS INVALID!\n");
        return false;
    }
    
    /* Check if symbol table is null */
    if(actual_symbol_table == NULL){
        fprintf(stderr, "[ERROR] INVALID SYMBOL TABLE!\n");
        return false;
    }
    
    /* Check the command and call the aproprietaded cgen function */
    switch(command_token->token_type){
        case T_SEMICOL:
            /* Just ignore */
            break;
        case TI_CALL_FUNCTION:
        case TI_CALL_FUNCTION_PAR:
            /* Generate code for 'function call' */
            cgenCallFunction(command_token, actual_symbol_table);
            break;
        case TI_ASSIGN:
            /* Generate code for 'assign' */
            cgenAssign(command_token, actual_symbol_table);
            break;
        case TI_BLOCO_COMANDO:
            /* Generate code for command 'block' */
            cgenCommandBlock(command_token, actual_symbol_table);
            break;
        case TI_WHILE:
            /* Generate command for 'while' */
            cgenWhile(command_token, actual_symbol_table);
            break;
        case TI_FOR:
        case TI_FOR_INC:
            /* Generate code for 'for' */
            cgenFor(command_token, actual_symbol_table);
            break;
        case TI_IF:
        case TI_IF_ELSE:
            /* Generate code for 'if' */
            cgenIf(command_token, actual_symbol_table);
            break;
        case TI_FUNCTION:
        case TI_FUNCTION_PARAM:
            /* Generate code for 'function definition' */
            cgenFunction(command_token, actual_symbol_table);
            break;
        case TI_LOCAL_ASSIGN:
        case TI_LOCAL_DEFINE:
            /* Generate code for local variable definition or assing */
            cgenLocalVariable(command_token, actual_symbol_table);
            break;
        default:
            fprintf(stderr, "[WARNING] NOT IMPLEMENTED YET, TODO!\n");
            return false;
    }
    
    return true;
}

/**
 * Generate code for command return.
 * 
 * @param block_token Receive a command return token.
 * @param actual_symbol_table Actual table symbol escope
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenCommandReturn(TokenNode *command_return_token, SymbolTable *actual_symbol_table){
    // TODO!
    fprintf(stderr, "[TODO] NOT IMPEMENTED YET!\n");
    return false;
}

/**
 * Generate block sections of code.
 * 
 * @param block_token Receive a block token.
 * @param previous_scope Previous table symbol escope.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenBlockCode(TokenNode *block_token, SymbolTable *previous_scope){
    TokenNode *command_list_token;
    TokenNode *command_return_token;
    SymbolTable *actual_symbol_table;
    
    /* Check if the actual block_token is not NULL */
    if(block_token == NULL){
        fprintf(stderr, "[ERROR] BLOCK TOKEN IS NULL!\n");
        return false;
    }
    
    /* Actual Symbol Table */
    actual_symbol_table = newSymbolTable(previous_scope);
    
    /* Command list token */
    command_list_token = listGetTokenByIndex(block_token->child_list, 1);
    
    /* Generate command list code */
    cgenCommandList(command_list_token, actual_symbol_table);
    
    /* Check if there are a return command */
    if(block_token->child_list->length > 1){
        
        /* Command return assign */
        command_return_token = listGetTokenByIndex(block_token->child_list, 2);
        
        /* Generate command return code */
        cgenCommandReturn(command_return_token, actual_symbol_table);
    }
    
    /* Return success */
    return true;
}

/**
 * Generate code for command list.
 * 
 * @param block_token Receive a command list token.
 * @param actual_symbol_table Actual table symbol escope.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenCommandList(TokenNode *command_list_token, SymbolTable *actual_symbol_table){
    int i;
    
    /* Check if command list token is null */
    if(command_list_token == NULL){
        fprintf(stderr, "[ERROR] COMMAND LIST IS INVALID!\n");
        return false;
    }
    
    /* Check if symbol table is null */
    if(actual_symbol_table == NULL){
        fprintf(stderr, "[ERROR] INVALID SYMBOL TABLE!\n");
        return false;
    }
    
    /* Stop process command list if command list is empty */
    if(command_list_token->token_type == TI_EMPTY){
        return true;
    }
    
    /* If command list is not empty, exec every command */
    for(i = 1; i <= command_list_token->child_list->length; i++){
        cgenCommand(listGetTokenByIndex(command_list_token->child_list, i), actual_symbol_table);
    }
    
    /* Return success */
    return true;
}

/**
 * Generate code for expressions.
 * 
 * @param exp_token receive an expression token.
 * @param previous_scope Previous table symbol escope.
 * @return true if there's no error on execution and false otherwise.
 */
void cgenExpression(TokenNode *exp_token, SymbolTable *symbol_table) {
    TokenNode *token_left = listGetTokenByIndex(exp_token->child_list, 1);
    TokenNode *token_right = listGetTokenByIndex(exp_token->child_list, 3);
    
    if(IS_BINARY_OPERAND(exp_token->token_type)) {
        //CGEN(exp1)
        cgenExpression(token_left, symbol_table);
        
        // push_a0
        addInstructionMainQueue(mips_push_a0);
        
        //CGEN(exp2)
        cgenExpression(token_right, symbol_table);
        
        //top_t1
        addInstructionMainQueue(mips_top_t1);
    
        //CGEN(operand)
        switch(exp_token->token_type) {
            case TI_PLUS:
                addInstructionMainQueue(mips_add_a0_t1_a0);
                break;
                
            case TI_MINUS:
                addInstructionMainQueue(mips_sub_a0_t1_a0);
                break;
                
            case TI_TIMES:
                addInstructionMainQueue(mips_mul_a0_t1_a0);
                break;
                
            case TI_DIV:
                addInstructionMainQueue(mips_div_a0_t1_a0);
                break;
                
            case TI_GT:
                addInstructionMainQueue(mips_gt_a0_t1_a0);
                break;
                
            case TI_GTEQ:
                addInstructionMainQueue(mips_gte_a0_t1_a0);
                break;
                
            case TI_LT:
                addInstructionMainQueue(mips_lt_a0_t1_a0);
                break;
                
            case TI_LTEQ:
                addInstructionMainQueue(mips_lte_a0_t1_a0);
                break;
                
            case TI_EQ:
                addInstructionMainQueue(mips_eq_a0_t1_a0);
                break;
                
            case TI_NEQ:
                addInstructionMainQueue(mips_neq_a0_t1_a0);
                break;
                
            default:
                break;
        }
        
        //pop
        addInstructionMainQueue(mips_pop);
    }
    
    int i;
    TokenNode * first_root_child = listGetTokenByIndex(exp_token->root_token->child_list, 1);
    
    switch (exp_token->token_type) {
        case TI_LISTAEXP:
            if (exp_token->root_token->token_type == TI_CALL_FUNCTION) {
                addInstructionMainQueue(mips_start_function_call);
                
                for(i = exp_token->child_list->length - 1; i >= 0; i--) {
                    cgenExpression(listGetTokenByIndex(exp_token->child_list, i), symbol_table);
                    addInstructionMainQueue(mips_push_a0);
                }
                
                //jal f_entry
                addInstructionMainQueue(formatedInstruction(mips_end_function_call, first_root_child->lex_str));
            }
        
            break;
            
        default:
            break;
    }
}