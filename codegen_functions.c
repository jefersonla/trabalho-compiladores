/* Codegen header */
#include "codegen_functions.h"

/* Terminals define */
#ifdef DEBUG_MODE
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
    /* Check if command list token is null */
    if(call_function_token == NULL){
        fprintf(stderr, "[ERROR] COMMAND LIST IS INVALID!\n");
        return false;
    }
    
    /* Check if symbol table is null */
    if(actual_symbol_table == NULL){
        fprintf(stderr, "[ERROR] INVALID SYMBOL TABLE!\n");
        return false;
    }
    
    /* Add the header of the function call  */
    addInstructionMainQueue(mips_start_function_call);
    
    /* Check what is the type of the function call */
    if(call_function_token->token_type == TI_CALL_FUNCTION_PAR){
        cgenExpressionList(listGetTokenByIndex(call_function_token->child_list, 3), actual_symbol_table);
    }

    /* Add the end of the function call */
    addInstructionMainQueue(formatedInstruction(mips_end_function_call, call_function_token->lex_str));
    
    /* Return success */
    return true;
}

/** 
 * Generate code for assign.
 * 
 * @param _token
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenAssign(TokenNode *assign_token, SymbolTable *actual_symbol_table){
    // TODO! Implement codegenerator assign
    fprintf(stderr, "[TODO] 'cgenAssign' - NOT IMPLEMENTED YET!\n");
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
    // TODO! Implement codegenerator for blocks of commands
    fprintf(stderr, "[TODO] 'cgenCommandBlock' - NOT IMPLEMENTED YET!\n");
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
    // TODO! Implement codegenerator for 'while' loop structure
    fprintf(stderr, "[TODO] 'cgenWhile' - NOT IMPLEMENTED YET!\n");
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
    // TODO! Implement codegenerator for 'for' loop structure
    fprintf(stderr, "[TODO] 'cgenFor' - NOT IMPLEMENTED YET!\n");
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
    // TODO! Implement codegenerator for 'if' structure
    fprintf(stderr, "[TODO] 'cgenIf' - NOT IMPLEMENTED YET!\n");
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
    // TODO! Implement codegenerator for 'function' definition
    fprintf(stderr, "[TODO] 'cgenFunction' - NOT IMPLEMENTED YET!\n");
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
    // TODO! Implement codegenerator for local variables assign and declaration
    fprintf(stderr, "[TODO] 'cgenLocalVariable' - NOT IMPLEMENTED YET!\n");
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
    // TODO! Implement codegenerator for return of commands 
    fprintf(stderr, "[TODO] 'cgenCommandReturn' - NOT IMPLEMENTED YET!\n");
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
 * Generate code for list of expressions.
 * 
 * @param list_exp_token Receive a list of expressions token.
 * @param actual_symbol_table Actual table symbol escope.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenExpressionList(TokenNode *list_exp_token, SymbolTable *symbol_table) {
    int i;
    TokenNode *token_node;
    
    /* Execute from right to left child list that is non T_COMMA */
    for(i = list_exp_token->child_list->length; i > 0; i--) {
        /* Get the token i */
        token_node = listGetTokenByIndex(list_exp_token->child_list, i);
        
        /* Check if this token is null */
        if(token_node == NULL){
            fprintf(stderr, "[ERROR] INVALID TOKEN NODE!\n");
            continue;
        }
        
        /* Ignore comma  */
        if(token_node->token_type == T_COMMA){
            continue;
        }
        
        /* CGEN(exp) */
        cgenExpression(token_node, symbol_table);
        
        /* push a0 */
        addInstructionMainQueue(mips_else);
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
bool cgenExpression(TokenNode *exp_token, SymbolTable *symbol_table) {
    TokenNode *token_left;
    TokenNode *token_right;
    TokenNode *token_operand;
    TokenNode *token_terminal;
    TokenNode *token_name;
    SymbolNode *symbol_node;
    
    /* Check if the operand is unary */
    if(IS_UNARY_OPERAND(exp_token->token_type)){
        
        /* Get the token operand */
        token_operand = listGetTokenByIndex(exp_token->child_list, 2);
        
        /* CGEN(exp) */
        cgenExpression(token_operand, symbol_table);
        
        /* CGEN(operand) */
        switch(exp_token->token_type){
            case TI_UMINUS:
                /* Add a minus transform operation to the instruction queue */
                addInstructionMainQueue(mips_neg_a0);
                break;
            case TI_NOT:
                /* Add a negation transform operation to the instruction queue */
                addInstructionMainQueue(mips_not_a0);
                break;
            default:
                /* Binary expression not found or not implemented yet */
                fprintf(stderr, "[WARNING] OPERAND NOT RECOGNIZED OR NOT IMPLEMENTED YET!\n");
                break;
        }
    }
    /* Check if the operand is binary */
    else if(IS_BINARY_OPERAND(exp_token->token_type)) {
        
        /* Get left and right childs of the expression */
        token_right = listGetTokenByIndex(exp_token->child_list, 1);
        token_left = listGetTokenByIndex(exp_token->child_list, 3);
    
        /* CGEN(exp1) */
        cgenExpression(token_left, symbol_table);
        
        /* push_a0 */
        addInstructionMainQueue(mips_push_a0);
        
        /* CGEN(exp2) */
        cgenExpression(token_right, symbol_table);
        
        /* top_t1 */
        addInstructionMainQueue(mips_top_t1);
    
        /* CGEN(operand) */
        switch(exp_token->token_type) {
            case TI_PLUS:
                /* Add an add operation to the instruction queue */
                addInstructionMainQueue(mips_add_a0_t1_a0);
                break;
            case TI_MINUS:
                /* Add a subtraction operation to the instruction queue */
                addInstructionMainQueue(mips_sub_a0_t1_a0);
                break;
            case TI_TIMES:
                /* Add a multiplication operation to the instruction queue */
                addInstructionMainQueue(mips_mul_a0_t1_a0);
                break;
            case TI_DIV:
                /* Add a division operation instruction to the instruction queue  */
                addInstructionMainQueue(mips_div_a0_t1_a0);
                break;
            case TI_GT:
                /* Add a '>' operation to the instruction queue */
                addInstructionMainQueue(mips_gt_a0_t1_a0);
                break;
            case TI_GTEQ:
                /* Add a '>= operation to the instruction queue */
                addInstructionMainQueue(mips_gte_a0_t1_a0);
                break;
            case TI_LT:
                /* Add a '<' operation to the instruction queue */
                addInstructionMainQueue(mips_lt_a0_t1_a0);
                break;
            case TI_LTEQ:
                /* Add a '<=' operation to the instruction queue */
                addInstructionMainQueue(mips_lte_a0_t1_a0);
                break;
            case TI_EQ:
                /* Add a '==' operation to the instruction queue */
                addInstructionMainQueue(mips_eq_a0_t1_a0);
                break;
            case TI_NEQ:
                /* Add a '~=' operation to the instruction queue */
                addInstructionMainQueue(mips_neq_a0_t1_a0);
                break;
            default:
                /* Binary expression not found or not implemented yet */
                fprintf(stderr, "[WARNING] OPERAND NOT RECOGNIZED OR NOT IMPLEMENTED YET!\n");
                break;
        }
        
        /* pop */
        addInstructionMainQueue(mips_pop);
    }
    /* Other possible expressions */
    else{
        /* Get the terminal */
        token_terminal = exp_token;
        
        switch (token_terminal->token_type) {
            case T_NUMBER:
                /* Add constant number load instruction */
                addInstructionMainQueue(formatedInstruction(mips_static_number_load, token_terminal->lex_str));
                break;
            case T_NIL:
                /* Nil value load */
                addInstructionMainQueue(mips_nil);
                break;
            case T_NAME:
                /* Look for this token inside the symbol table */
                symbol_node = symbolTableGetSymbolNodeByName(symbol_table, token_terminal->lex_str);
                
                /* If token name is inside the symbol table so this t_name is local otherwise is global */
                if(symbol_node == NULL){
                    symbol_node = symbolTableGetSymbolNodeByName(global_symbol_table, token_terminal->lex_str);
                }
                
                /* If this symbol is not on global symbol table, so it's nil */
                if(symbol_node == NULL){
                    addInstructionMainQueue(mips_nil);
                }
                else{
                    /* Store the correct load instruction */
                    instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetLoadInstruction(symbol_node));
                }
                break;
            case TI_CALL_FUNCTION:
            case TI_CALL_FUNCTION_PAR:
                /* Generating code for call function */
                cgenCallFunction(token_terminal, symbol_table);
                break;
            default:
                /* For types not implemented yet */
                fprintf(stderr, "[WARNING] TYPE NOT RECOGNIZED OR NOT IMPLEMENTED YET!\n");
                break;
        }
    }
    
    /* Return success */
    return true;
}