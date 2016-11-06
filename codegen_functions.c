/* Codegen header */
#include "codegen_functions.h"

/* Includes */
#include <stdio.h>
#include <stdlib.h>

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

/* *** Counter for operations *** */

/* Short Circuit 'and' counter */
int sc_and_counter;

/* Short Circuit 'or' counter */
int sc_or_counter;

/* Loop 'while' counter */
int loop_while_counter;

/* Loop 'for' counter */
int loop_for_counter;

/* Conditional 'if' counter */
int cond_if_counter;

/** 
 * Copy global variables into header instruction queue.
 * 
 * @return true if there's no error on execution and false otherwise.
 */
bool copyGlobalVariables(){
    int i;
    
    /* Check if header instruction queue is null */
    if(header_instruction_queue == NULL){
        printError("HEADER INSTRUCTION QUEUE, NOT INITIALIZED!");
        return false;
    }
    
    /* Check if global symbol table is null */
    if(global_symbol_table == NULL){
        printError("GLOBAL SYMBOL TABLE, NOT INITIALIZED!");
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
    /* Initialize global operation counters */
    sc_or_counter = 0;
    sc_and_counter = 0;
    cond_if_counter = 0;
    loop_for_counter = 0;
    loop_while_counter = 0;
    
    /* Initialize Header Instruction Queue */
    header_instruction_queue = newInstructionQueue();
    
    /* Check if the header instruction queue was corrected initialized */
    if(header_instruction_queue == NULL){
        printError("CANNOT INITIALIZE HEADER INSTRUCTION QUEUE.");
        return false;
    }
    
    /* Initialize Main Instruction Queue */
    main_instruction_queue = newInstructionQueue();
    
    /* Check if the header instruction queue was corrected initialized */
    if(main_instruction_queue == NULL){
        printError("CANNOT INITIALIZE MAIN INSTRUCTION QUEUE.");
        return false;
    }
    
    /* Initialize Global Symbol Table */
    global_symbol_table = newSymbolTable(NULL);
    
    /* Check if the global symbol was correct initialized */
    if(global_symbol_table == NULL){
        printError("CANNOT CREATE GLOBAL SYMBOL TABLE.");
        return false;
    }
    
    /* Configure symbol table as global */
    global_symbol_table->start_address = GLOBAL_START_ADRESS;
    
    /* Block token node */
    TokenNode *block_token = listGetTokenByIndex(root_token->child_list, 1);
    
    /* Check if the block token really exists */
    if(block_token == NULL){
        printError("CANNOT GET BLOCK TOKEN NODE.");
        return false;
    }
    
    /* Add header on header instruction queue */
    instructionQueueEnqueueInstruction(header_instruction_queue, formatedInstruction(mips_header), false);
    
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
        printError("COMMAND LIST IS INVALID!");
        return false;
    }
    
    /* Check if symbol table is null */
    if(actual_symbol_table == NULL){
        printError("INVALID SYMBOL TABLE!");
        return false;
    }
    
    /* Add the header of the function call  */
    addInstructionMainQueue(mips_start_function_call);
    
    /* Check what is the type of the function call */
    if(call_function_token->token_type == TI_CALL_FUNCTION_PAR){
        cgenExpressionList(listGetTokenByIndex(call_function_token->child_list, 3), actual_symbol_table);
    }

    /* Add the end of the function call */
    addInstructionMainQueueFormated(mips_end_function_call, listGetTokenByIndex(call_function_token->child_list, 1)->lex_str);
    
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
    printTodo("'cgenAssign' - NOT IMPLEMENTED YET!");
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
    /* Check if command list token is null */
    if(command_block_token == NULL){
        printError("COMMAND BLOCK IS INVALID!");
        return false;
    }
    
    /* Check if symbol table is null */
    if(actual_symbol_table == NULL){
        printError("INVALID SYMBOL TABLE!");
        return false;
    }
    
    /* Generate Code for the T_DO [bloco] T_END */
    cgenBlockCode(listGetTokenByIndex(command_block_token->child_list, 2), actual_symbol_table);
    
    return true;
}

/** 
 * Generate code for while.
 * 
 * @param while_token token with while structure.
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenWhile(TokenNode *while_token, SymbolTable *previous_symbol_table){
    SymbolTable *new_symbol_table;
    TokenNode *block_token;
    TokenNode *exp_token;
    
    /* Check if while token is null */
    if(while_token == NULL){
        printError("WHILE COMMAND IS INVALID!");
        return false;
    }
    
    /* Check if symbol table is null */
    if(previous_symbol_table == NULL){
        printError("INVALID SYMBOL TABLE!");
        return false;
    }
    
    /* Create a new symbol table */
    new_symbol_table = newSymbolTable(previous_symbol_table);
    
    /* Check the new symbol table */
    if(new_symbol_table == NULL){
        printError("CANNOT CREATE A NEW SYMBOL TABLE!");
        return false;
    }
    
    /* Get expression */
    exp_token = listGetTokenByIndex(while_token->child_list, 2);
    
    /* Check if this expression is null */
    if(exp_token == NULL){
        printError("EXPRESSION TOKEN IS NULL!");
        return false;
    }
    
    /* Get bloco */
    block_token = listGetTokenByIndex(while_token->child_list, 4);
    
    /* Check if block token is null */
    if(block_token == NULL){
        printError("BLOCK TOKEN IS NULL!");
    }
    
    /*  */
    
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
    printTodo("'cgenFor' - NOT IMPLEMENTED YET!");
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
    printTodo("'cgenIf' - NOT IMPLEMENTED YET!");
    return false;    
}

/** 
 * Generate code for function definition.
 * 
 * @param function_def_token function definition token.
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenFunction(TokenNode *function_def_token, SymbolTable *actual_symbol_table) {
    int i;
    SymbolTable *new_table;
    TokenNode *t_name;
    TokenNode *parameters_list_token;
    TokenNode *parameter_token;
    TokenNode *block_token;
     
    /* Check if function def is null */
    if(function_def_token == NULL){
        printError("FUNCTION DEF TOKEN IS NULL!");
        return false;
    }
    
    /* New scope symbol_table */
    new_table = newSymbolTable(NULL);
    
    /* Check if the new table is null */
    if(new_table == NULL){
        fprintf(stderr,"[ERROR] NEW SYMBOL TABLE IS NULL!");
        return false;
    }
    
    /* Get Token name */
    t_name = listGetTokenByIndex(function_def_token->child_list, 2); 
    
    /* Check if t name is null */
    if(t_name == NULL){
        printError("TOKEN NAME IS NULL!");
        return false;
    }
    
    if(function_def_token->token_type == TI_FUNCTION_PARAM){
        /* Get parameters list */
        parameters_list_token = listGetTokenByIndex(function_def_token->child_list, 4);
        
        /* Check if parameters list it's not null */
        if(parameters_list_token == NULL){
            printError("PARAMETERS LIST TOKEN IS NULL!");
            return false;
        }
        
        /* Parameters list could be only one name on this case we skip this part */
        if(parameters_list_token->token_type == TI_LISTADENOMES){
            
            /* If we have a list of names we need to start from the last one to the first */
            for(i = parameters_list_token->child_list->length; i > 0; i++){
                
                /* Get the token */
                parameter_token = listGetTokenByIndex(parameters_list_token->child_list, i);
                
                /* Add this token as a symbol in the symbol table */
                symbolTableAddSymbol(new_table, parameter_token->lex_str, NUMBER_TYPE);
            }
        }
        else{
            /* If we have only one name we just add this name to the new table */
            parameter_token = parameters_list_token;
            symbolTableAddSymbol(new_table, parameter_token->lex_str, NUMBER_TYPE);
        }
        
        /* Get the block token */
        block_token = listGetTokenByIndex(function_def_token->child_list, 6);
    }
    else if(function_def_token->token_type == TI_FUNCTION){
        /* Store the block token on functions that don't have a parameters list */
        block_token = listGetTokenByIndex(function_def_token->child_list, 5);
    }
    else{
        printError("THIS TOKEN IS NOT A FUNCTION DEFINITION! -- 0x%X : %d\n", function_def_token->token_type, function_def_token->token_type);
        return false;
    }
    
    /* Check if block token is not null */
    if(block_token == NULL){
        printError("BLOCK TOKEN IS NULL!");
        return false;
    }
    
    /* Start code for function definition */
    addInstructionMainQueueFormated(mips_start_function_def, (t_name->lex_str));
    addInstructionMainQueueFormated(mips_start_function_def2, (t_name->lex_str));
    
    /* Generate code for block */
    cgenBlockCode(block_token, new_table);
    
    /* Finish function definition poping Record Activation */
    addInstructionMainQueueFormated(mips_end_function_def, (t_name->lex_str), (new_table->shift_address + 4));
    addInstructionMainQueueFormated(mips_end_function_def2, (t_name->lex_str));
    
    /* Delete local symbol table */
    deleteSymbolTable(&new_table);
    
    /* Return success */
    return true;
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
    printTodo("'cgenLocalVariable' - NOT IMPLEMENTED YET!");
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
        printError("COMMAND LIST IS INVALID!");
        return false;
    }
    
    /* Check if symbol table is null */
    if(actual_symbol_table == NULL){
        printError("INVALID SYMBOL TABLE!");
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
            printWarning("NOT IMPLEMENTED YET, TODO!");
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
    printTodo("'cgenCommandReturn' - NOT IMPLEMENTED YET!");
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
        printError("BLOCK TOKEN IS NULL!");
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
    
    /* Delete local symbol table */
    deleteSymbolTable(&actual_symbol_table);
    
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
        printError("COMMAND LIST IS INVALID!");
        return false;
    }
    
    /* Check if symbol table is null */
    if(actual_symbol_table == NULL){
        printError("INVALID SYMBOL TABLE!");
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
    
    if(list_exp_token->token_type == TI_LISTAEXP){
        /* Execute from right to left child list that is non T_COMMA */
        for(i = list_exp_token->child_list->length; i > 0; i--) {
            /* Get the token i */
            token_node = listGetTokenByIndex(list_exp_token->child_list, i);
            
            /* Check if this token is null */
            if(token_node == NULL){
                printError("INVALID TOKEN NODE!");
                continue;
            }
            
            /* Ignore comma  */
            if(token_node->token_type == T_COMMA){
                continue;
            }
            
            /* CGEN(exp) */
            cgenExpression(token_node, symbol_table);
            
            /* push a0 */
            addInstructionMainQueue(mips_push_a0);
        }
    }
    else{
        /* If this is not an expression list, so it could be only a expression */
        token_node = list_exp_token;
        
        /* Check if this token is null */
        if(token_node == NULL){
            printError("INVALID TOKEN NODE!");
            return false;
        }

        /* CGEN(exp) */
        cgenExpression(token_node, symbol_table);
        
        /* push a0 */
        addInstructionMainQueue(mips_push_a0);
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
    SymbolNode *symbol_node;
    
    /* Check if the operand is unary */
    if(IS_UNARY_OPERAND(exp_token->token_type)){
        
        /* Get the token operand */
        token_operand = listGetTokenByIndex(exp_token->child_list, 2);
        
        /* CGEN(exp) */
        cgenExpression(token_operand, symbol_table);
        
        /* mips_check_a0_nil() */
        addInstructionMainQueue(mips_check_a0_nil);
        
        /* CGEN(operand) */
        switch(exp_token->token_type){
            case TI_UMINUS:
                /* Add a minus transform operation to the instruction queue */
                addInstructionMainQueue(mips_neg_a0);
                break;
            case TI_NOT:
                /* Add a boolean negation transform operation to the instruction queue */
                addInstructionMainQueue(mips_not_a0);
                break;
            default:
                /* Binary expression not found or not implemented yet */
                printWarning("OPERAND NOT RECOGNIZED OR NOT IMPLEMENTED YET! -- 0x%X : %d\n", exp_token->token_type, exp_token->token_type);
                break;
        }
    }
    /* Check if the operand is binary */
    else if(IS_BINARY_OPERAND(exp_token->token_type)) {
        
        /* Get left and right childs of the expression */
        token_left = listGetTokenByIndex(exp_token->child_list, 1);
        token_right = listGetTokenByIndex(exp_token->child_list, 3);
    
        /* CGEN(exp1) */
        cgenExpression(token_left, symbol_table);
        
        /* if the operand is different of equal and different symbols, we can assign nil as 0 */
        if(exp_token->token_type){
            /* mips_check_a0_nil() */
            addInstructionMainQueue(mips_check_a0_nil);
        }
        
        /* push_a0 */
        addInstructionMainQueue(mips_push_a0);
        
        /* CGEN(exp2) */
        cgenExpression(token_right, symbol_table);
        
        /* if the operand is different of equal and different symbols, we can assign nil as 0 */
        if(exp_token->token_type != TI_EQ && exp_token->token_type != TI_NEQ){
            /* mips_check_a0_nil() */
            addInstructionMainQueue(mips_check_a0_nil);
        }
        
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
                printWarning("OPERAND NOT RECOGNIZED OR NOT IMPLEMENTED YET! -- 0x%X : %d\n", exp_token->token_type, exp_token->token_type);
                break;
        }
        
        /* pop */
        addInstructionMainQueue(mips_pop);
    }
    /* And & Or should use short-circuit evaluation */
    else if(IS_SHORT_CIRCUIT_OP(exp_token->token_type)){
        
        /* In short circuit evaluation we check if one of the sides or both of the sentence are true or not */
        
        /* Get left and right childs of the expression */
        token_left = listGetTokenByIndex(exp_token->child_list, 1);
        token_right = listGetTokenByIndex(exp_token->child_list, 3);
        
        /* In short circuit 'and' we check if both sides of this binary operation are valid */
        if(exp_token->token_type == TI_AND){
            /* Add begin of the and sc verification */
            addInstructionMainQueue(mips_and_sc_header);
            
            /* CGEN(exp1) */
            cgenExpression(token_left, symbol_table);
            
            /* mips_check_a0_nil */
            addInstructionMainQueue(mips_check_a0_nil);
            
            /* Add check or skip with counter */
            addInstructionMainQueueFormated(mips_and_sc_skip, sc_and_counter);
            
            /* CGEN(exp2) */
            cgenExpression(token_right, symbol_table);
            
            /* mips_check_a0_nil */
            addInstructionMainQueue(mips_check_a0_nil);
            
            /* Add footer of this instruction */
            addInstructionMainQueueFormated(mips_and_sc_footer, sc_and_counter);
            
            /* Increment counter */
            sc_and_counter += 1;
        }
        /* In short circuit 'or' we check if at least one of the sentences are true */
        else if(exp_token->token_type == TI_OR){
            /* Add begin of the and sc verification */
            addInstructionMainQueue(mips_or_sc_header);
            
            /* CGEN(exp1) */
            cgenExpression(token_left, symbol_table);
            
            /* mips_check_a0_nil */
            addInstructionMainQueue(mips_check_a0_nil);
            
            /* Add check or skip with counter */
            addInstructionMainQueueFormated(mips_or_sc_skip, sc_or_counter);
            
            /* CGEN(exp2) */
            cgenExpression(token_right, symbol_table);
            
            /* mips_check_a0_nil */
            addInstructionMainQueue(mips_check_a0_nil);
            
            /* Add footer of this instruction */
            addInstructionMainQueueFormated(mips_or_sc_footer, sc_or_counter);
            
            /* Increment counter */
            sc_or_counter += 1;
        }
        /* This should never occur */
        else{
            printFatalError("SOMETHING UNBELIVABLE HAPPENED, EXITING NOW!");
            exit(EXIT_FAILURE);
        }
    }
    /* Other possible expressions */
    else{
        /* Get the terminal */
        token_terminal = exp_token;
        
        switch (token_terminal->token_type) {
            case T_NUMBER:
                /* Add constant number load instruction */
                addInstructionMainQueueFormated(mips_static_number_load, token_terminal->lex_str);
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
                printWarning("TYPE [%s] NOT RECOGNIZED OR NOT IMPLEMENTED YET!\n", token_terminal->token_str);
                break;
        }
    }
    
    /* Return success */
    return true;
}