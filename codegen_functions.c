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

/* Counter for multiple values 'return' */
int multiple_return_counter;

/**
 * Pop local varibles, this include, the instruction pop, and the logical (symbol table) pop too.
 * 
 * @param symbol_table The symbol table which is being popped.
 * @return true if there's no error on execution and false otherwise.
 */
bool popSymbolTable(SymbolTable *symbol_table){
    int i;
    int no_variables;
 
    /* Check if we have a valid symbol table */
    if(symbol_table == NULL){
        printError("INVALID SYMBOL TABLE!");
        return false;
    }
 
    /* Include the instruction to pop local variables */
    addInstructionMainQueueFormated(mips_pop_local, (symbol_table->shift_address));
    
    /* Get how many variables we have */
    no_variables = symbol_table->length;
    
    /* Remove each variable shift from all scopes */
    for(i = 0; i < no_variables; i++){
        symbolTablePopVar(symbol_table);
    }
    
    /* Return success */
    return true;
}

/**
 * Assign a list of names all values on stack.
 * 
 * @param list_names a valid list of *ONLY* token names.
 * @return true if there's no error on execution and false otherwise.
 */
bool assignStackHelper(TokenList *list_names, SymbolTable *actual_symbol_table){
    int i;
    TokenNode *token_name;
    SymbolNode *symbol_node;
    
    /* Execute list of names and assign values for each symbol name */
    for(i = 1; i <= list_names->length; i++){
        
        /* Get token name */
        token_name = listGetTokenByIndex(list_names, i);
        
        /* Check if token name is not null */
        if(token_name == NULL){
            printError("INVALID TOKEN NAME!");
            return false;
        }
        
        /* Add header of the instruction */
        addInstructionMainQueueFormated(mips_marker_assign, token_name->lex_str, i);
        
        /* $a0 = top */
        addInstructionMainQueue(mips_top_a0);
        
        /* Get the symbol node */
        symbol_node = symbolTableGetSymbolNodeByName(actual_symbol_table, token_name->lex_str);
        
        /* If token name isn't inside the actual symbol table so this t_name is global */
        if(symbol_node == NULL){
            symbol_node = symbolTableGetSymbolNodeByName(global_symbol_table, token_name->lex_str);
        }
        /* If this symbol doesn't have a valid root symbol table */
        else if(symbol_node->root_symbol_table == NULL){
            printError("INVALID SYMBOL NODE ROOT TOKEN!");
            return false;
        }
        /* If this symbol has a valid root symbol table, and the type of this symbol table is SP - Stack Pointer */
        else if(symbol_node->root_symbol_table->register_type == REGISTER_TYPE_SP){
            
            /* Update address temporarily, because stack contains a lot of new values */
            symbol_node->symbol_address += ((list_names->length - i + 1) * 4);
        }
        
        /* If this symbol is not on global symbol table, so need to add him first */
        if(symbol_node == NULL){
            
            /* Add the symbol to the global symbol table */
            symbolTableAddSymbol(global_symbol_table, token_name->lex_str, NUMBER_TYPE);
            
            /* Get the inserted symbol node */
            symbol_node = symbolTableGetSymbolNodeByName(global_symbol_table, token_name->lex_str);
            
            /* Check if symbol node is valid */
            if(symbol_node == NULL){
                printError("INVALID SYMBOL NODE!");
                return false;
            }
        }
        
        /* Store the correct store instruction */
        instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetStoreInstruction(symbol_node));
        
        /* pop */
        addInstructionMainQueue(mips_pop);
        
        /* Pop address on symbol table too. This assume that the symbol table has a lot of blank spaces */
        symbolTablePopVar(actual_symbol_table);
    }
    
    /* Return success */
    return true;
}

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
                                                    symbolNodeGetDefineInstruction(global_symbol_table->items[i]))){
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
    SymbolTable *main_symbol_table;
    
    /* Initialize global operation counters */
    sc_or_counter = 0;
    sc_and_counter = 0;
    cond_if_counter = 0;
    loop_for_counter = 0;
    loop_while_counter = 0;
    multiple_return_counter = 0;
    
    /* Initialize Header Instruction Queue */
    header_instruction_queue = newInstructionQueue();
    
    /* Check if the header instruction queue was corrected initialized */
    if(header_instruction_queue == NULL){
        printFatalError("CANNOT INITIALIZE HEADER INSTRUCTION QUEUE.");
        exit(EXIT_FAILURE);
    }
    
    /* Initialize Main Instruction Queue */
    main_instruction_queue = newInstructionQueue();
    
    /* Check if the header instruction queue was corrected initialized */
    if(main_instruction_queue == NULL){
        printFatalError("CANNOT INITIALIZE MAIN INSTRUCTION QUEUE.");
        exit(EXIT_FAILURE);
    }
    
    /* Initialize Global Symbol Table */
    global_symbol_table = newGlobalSymbolTable();
    
    /* Check if the global symbol was correct initialized */
    if(global_symbol_table == NULL){
        printFatalError("CANNOT CREATE GLOBAL SYMBOL TABLE.");
        exit(EXIT_FAILURE);
    }

    /* Block token node */
    TokenNode *block_token = listGetTokenByIndex(root_token->child_list, 1);
    
    /* Check if the block token really exists */
    if(block_token == NULL){
        printFatalError("CANNOT GET BLOCK TOKEN NODE.");
        exit(EXIT_FAILURE);
    }
    
    /* Create the main symbol table */
    main_symbol_table = newSymbolTable(NULL, REGISTER_TYPE_SP);
    
    /* Check if the main symbol table is working */
    if(main_symbol_table == NULL){
        printFatalError("INVALID MAIN SYMBOL TABLE! STOPPING NOW!");
        exit(EXIT_FAILURE);
    }
    
    /* Add header on header instruction queue */
    instructionQueueEnqueueInstruction(header_instruction_queue, formatedInstruction(mips_header), false);
    
    /* Add header on main instruction queue */
    addInstructionMainQueue(mips_main);
    
    //symbolTableAddSymbol(main_symbol_table, "$RA",NUMBER_TYPE); OTHER DUMMY DEBUG! -_-
    
    /* Generate code for main application */
    cgenBlockCode(block_token, main_symbol_table);

    /* Pop Main Symbol table */
    popSymbolTable(main_symbol_table);

    /* Delete main symbol table */
    deleteSymbolTable(&main_symbol_table);

    /* Copy variable definitions to main-instruction_queue */
    copyGlobalVariables();
    
    /* Add footer of the main instruction queue */
    addInstructionMainQueue(mips_footer);
    
    /* Return success */
    return true;
}

/** 
 * Generate code for function call.
 * 
 * @param call_function_token Call function token.
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenCallFunction(TokenNode *call_function_token, SymbolTable *actual_symbol_table){
    int i;
    int no_exp_executed;
    TokenNode *exp_list;
    TokenNode *token_name;
    
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
    
    /* Get function name */
    token_name = listGetTokenByIndex(call_function_token->child_list, 1);
    
    /* Check if token_name is valid */
    if(token_name == NULL){
        printError("INVALID TOKEN NAME!");
        return false;
    }
    
    /* Add the header of the function call  */
    addInstructionMainQueue(mips_start_function_call);
    
    /* Push var */
    symbolTablePushVar(actual_symbol_table);
    
    /* Iniitalize list of expressions executed */
    no_exp_executed = 0;
    
    /* Check what is the type of the function call */
    if(call_function_token->token_type == TI_CALL_FUNCTION_PAR){
        
        /* Get the list of expressions */
        exp_list = listGetTokenByIndex(call_function_token->child_list, 3);
        
        /* Check if expression list is valid */
        if(exp_list == NULL){
            printError("CANNOT GET LIST OF EXPRESSIONS!");
            return false;
        }
        
        /* Execute expression list and assign number of params processed */
        no_exp_executed = cgenExpressionList(exp_list, actual_symbol_table);
    }

    /* Add the end of the function call */
    addInstructionMainQueueFormated(mips_end_function_call, token_name->lex_str);
    
    /* Pop All parameters */
    for(i = 0; i < no_exp_executed; i++){
        symbolTablePopVar(actual_symbol_table);
    }
    
    /* Pop var */
    symbolTablePopVar(actual_symbol_table);
    
    /* Return success */
    return true;
}

/** 
 * Generate code for assign.
 * 
 * @param assign_token Token with the assign operations
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenAssign(TokenNode *assign_token, SymbolTable *actual_symbol_table){
    TokenList *list_names;
    TokenNode *token_exp;
    TokenNode *token_name;
    TokenNode *list_exp_token;
    TokenNode *list_names_token;
    SymbolNode *symbol_node;
    
    /* Check if command list token is null */
    if(assign_token == NULL){
        printError("ASSIGN TOKEN IS INVALID!");
        return false;
    }
    
    /* Check if symbol table is null */
    if(actual_symbol_table == NULL){
        printError("INVALID SYMBOL TABLE!");
        return false;
    }
    
    /* Get token list of names */
    list_names_token = listGetTokenByIndex(assign_token->child_list, 1);
    
    /* Check list of names token */
    if(list_names_token == NULL){
        printError("INVALID LIST OF NAMES!");
        return false;
    }
    
    /* Get token list of expressions */
    list_exp_token = listGetTokenByIndex(assign_token->child_list, 3);
    
    /* Check if list of expressions token is not null */
    if(list_exp_token == NULL){
        printError("INVALID LIST OF EXPRESSIONS!");
        return false;
    }
    
    /* Add header of an assign to the main instruction queue */
    addInstructionMainQueue(mips_start_assign);
    
    /* If list_names is a token name, we only have one operand */
    if(list_names_token->token_type == T_NAME){
        
        /* Get the actual token expression node */
        token_exp = list_exp_token;
        
        /* Add header of this instruction to main queue */
        addInstructionMainQueueFormated(mips_marker_exp, 0);
        
        /* CGEN(exp) */
        cgenExpression(token_exp, actual_symbol_table);
        
        /* Get the actual token name node */
        token_name = list_names_token;
        
        /* Add header of the instruction */
        addInstructionMainQueueFormated(mips_marker_assign, token_name->lex_str, 0);
        
        /* Get the symbol node */
        symbol_node = symbolTableGetSymbolNodeByName(actual_symbol_table, token_name->lex_str);
        
        printf("%s -- symbol name\n", token_name->lex_str);
        
        /* If token name isn't inside the actual symbol table so this t_name is global */
        if(symbol_node == NULL){
            symbol_node = symbolTableGetSymbolNodeByName(global_symbol_table, token_name->lex_str);
            printf("NOT FOUND LOCAL!\n");
        }
        
        /* If this symbol is not on global symbol table, so need to add him first, on global symbol table */
        if(symbol_node == NULL){
            
            printf("NOT FOUND GLOBAL!\n");
            
            /* Add the symbol to the global symbol table */
            symbolTableAddSymbol(global_symbol_table, token_name->lex_str, NUMBER_TYPE);
            
            /* Get the inserted symbol node */
            symbol_node = symbolTableGetSymbolNodeByName(global_symbol_table, token_name->lex_str);
            
            /* Check if symbol node is valid */
            if(symbol_node == NULL){
                printError("INVALID SYMBOL NODE!");
                return false;
            }
        }
        
        /* Store the correct store instruction */
        instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetStoreInstruction(symbol_node));
    }
    /* Otherwise we have a list of operands */
    else{
        /* Get list of names */
        list_names = listGetTokensByType(list_names_token->child_list, T_NAME);
        
        /* Check if the list of names is correct */
        if(list_names == NULL){
            printError("INVALID LIST OF NAMES!");
            return false;
        }

        /* Execute list of expressions, and put reverse results on stack */
        cgenExpressionList(list_exp_token, actual_symbol_table);
        
        /* Assign our list of names with our actual symbol table, the values stacked */
        assignStackHelper(list_names, actual_symbol_table);
    }
    
    /* Add footer of a assign instruction */
    addInstructionMainQueue(mips_end_assign);
    
    /* Return success */
    return true;
}

/** 
 * Generate code for command block.
 * 
 * @param command_block_token Command block token.
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenCommandBlock(TokenNode *command_block_token, SymbolTable *actual_symbol_table){
    TokenNode *block_token;
    SymbolTable *new_symbol_table;
    
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
    
    /* Get block token */
    block_token = listGetTokenByIndex(command_block_token->child_list, 2);
    
    /* Check if block token is not null */
    if(block_token == NULL){
        printError("ERROR INVALID BLOCK TOKEN!");
        return false;
    }
    
    /* Create a new scope */
    new_symbol_table = newSymbolTable(actual_symbol_table, REGISTER_TYPE_SP);
    
    /* Check if the new symbol table is valid */
    if(new_symbol_table == NULL){
        printError("INVALID NEW SYMBOL TABLE!");
        return false;
    }
    
    /* Generate Code for the T_DO [bloco] T_END */
    cgenBlockCode(block_token, new_symbol_table);
    
    /* Pop variables */
    popSymbolTable(new_symbol_table);
    
    /* Destroy new symbol table */
    deleteSymbolTable(&new_symbol_table);
    
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
    new_symbol_table = newSymbolTable(previous_symbol_table, REGISTER_TYPE_SP);
    
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
        return false;
    }
    
    /* Add header message */
    addInstructionMainQueueFormated(mips_start_while, loop_while_counter);
    
    /* CGEN(exp) */
    cgenExpression(exp_token, previous_symbol_table);
    
    /* Check condition */
    addInstructionMainQueueFormated(mips_check_while, loop_while_counter);
    
    /* CGEN(bloco) */
    cgenBlockCode(block_token, new_symbol_table);

    /* Pop local symbol table */
    popSymbolTable(new_symbol_table);
    
    /* Add while end */
    addInstructionMainQueueFormated(mips_end_while, loop_while_counter, loop_while_counter);
    
    /* Increment while loop counter */
    loop_while_counter += 1;
    
    /* Delete Scope */
    deleteSymbolTable(&new_symbol_table);
    
    /* Return succes */
    return true;    
}

/** 
 * Generate code for for.
 * 
 * @param for_token token for.
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenFor(TokenNode *for_token, SymbolTable *actual_symbol_table){
    TokenNode *token_exp;
    TokenNode *token_name;
    TokenNode *token_block;
    TokenNode *token_assign;
    TokenNode *token_increment;
    SymbolNode *symbol_node;
    SymbolTable *new_symbol_table;
    SymbolTable *new_symbol_table_iterator;
    
    /* Check if for token is null */
    if(for_token == NULL){
        printError("IF COMMAND IS INVALID!");
        return false;
    }
    
    /* Check if symbol table is null */
    if(actual_symbol_table == NULL){
        printError("INVALID SYMBOL TABLE!");
        return false;
    }
    
    /* Get token_name */
    token_name = listGetTokenByIndex(for_token->child_list, 2);
    
    /* Check if token name is valid */
    if(token_name == NULL){
        printError("INVALID TOKEN NAME!");
        return false;
    }
    
    /* Get assign expression token */
    token_assign = listGetTokenByIndex(for_token->child_list, 4);
    
    /* Check if this expression is valid */
    if(token_assign == NULL){
        printError("INVALID EXPRESSION TOKEN!");
        return false;
    }
    
    /* Get assign expression token */
    token_exp = listGetTokenByIndex(for_token->child_list, 6);
    
    /* Check if this expression is valid */
    if(token_exp == NULL){
        printError("INVALID EXPRESSION TOKEN!");
        return false;
    }
    
    /* Header of the for operation */
    addInstructionMainQueue(mips_for_ini);
    
    /* Initialize a new symbol table just for iterator */
    new_symbol_table_iterator = newSymbolTable(actual_symbol_table, REGISTER_TYPE_SP);
    
    /* Check if new symbol table hasn't failed */
    if(new_symbol_table_iterator == NULL){
        printError("CANNOT CREATE A NEW SYMBOL TABLE!");
        return false;
    }
    
    /* Initialize a new symbol table */
    new_symbol_table = newSymbolTable(new_symbol_table_iterator, REGISTER_TYPE_SP);
    
    /* Check if new symbol table hasn't failed */
    if(new_symbol_table == NULL){
        printError("CANNOT CREATE A NEW SYMBOL TABLE!");
        return false;
    }
    
    /* Add our token name to the new symbol table */
    symbolTableAddSymbol(new_symbol_table_iterator, token_name->lex_str, NUMBER_TYPE);

    ///// JUST A TEST
    ///// symbolTablePopVar(new_symbol_table_iterator);
    ///// INITIALIZING LOCAL VARIABLES WITH WRONG ADDRESS -_-
    
    /* Get the symbol node */
    symbol_node = symbolTableGetSymbolNodeByName(new_symbol_table_iterator, token_name->lex_str);
    
    /* Check if symbol node is valid */
    if(symbol_node == NULL){
        printError("CANNOT RETRIEVE SYMBOL NODE!");
        return false;
    }
    
    /* Add definition of this token */
    instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetDefineInstruction(symbol_node));
    
    /* Execute token assign */
    cgenExpression(token_assign, new_symbol_table_iterator);
    
    /* Assign the local iterator variable */
    instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetStoreInstruction(symbol_node));
    
    /* Add label of for begin */
    addInstructionMainQueueFormated(mips_start_for, loop_for_counter);
    
    /* Execute for expression condition */
    cgenExpression(token_exp, new_symbol_table);
    
    /* Move iterator from $a0 to $t1 */
    addInstructionMainQueue(mips_move_a0_t1);
    
    /* Load value of the iterator */
    instructionQueueEnqueueInstructionNode( main_instruction_queue,
                                            symbolNodeGetLoadInstruction(symbol_node));
    
    /* Add check expression */
    addInstructionMainQueueFormated(mips_for_check, loop_for_counter);
    
    /* Block token and increment may vary so we first check wat's the type of the 'for' */
    if(for_token->token_type == TI_FOR_INC){
        /* Get block token */
        token_block = listGetTokenByIndex(for_token->child_list, 10);
    }
    else{
        /* Get block token */
        token_block = listGetTokenByIndex(for_token->child_list, 8);
    }
    
    /* Check if block is valid */
    if(token_block == NULL){
        printError("BLOCK TOKEN IS INVALID!");
        return false;
    }
    
    /* Execute block code */
    cgenBlockCode(token_block, new_symbol_table);
    
    /* Check if the for have an increment expression */
    if(for_token->token_type == TI_FOR_INC){
        
        /* Get expression increment */
        token_increment = listGetTokenByIndex(for_token->child_list, 8);
        
        /* Check if this token is valid */
        if(token_increment == NULL){
            printError("INVALID TOKEN EXPRESSION FOR INCREMENT!");
            return false;
        }
        
        /* Execute increment expression */
        cgenExpression(token_increment, new_symbol_table);
    }
    else{
        /* Add 1 into a0, this is the same as increment of 1 */
        addInstructionMainQueueFormated(mips_static_number_load, " 1 ");
    }
    
    /* Store in $t1 returned by expression */
    addInstructionMainQueue(mips_for_load_inc);
    
    /* Load old value of the iterator */
    instructionQueueEnqueueInstructionNode( main_instruction_queue,
                                            symbolNodeGetLoadInstruction(symbol_node));
    
    /* Increment the iterator */
    addInstructionMainQueue(mips_for_inc);
    
    /* Store the new value of the iterator */
    instructionQueueEnqueueInstructionNode( main_instruction_queue,
                                            symbolNodeGetStoreInstruction(symbol_node));
    
    /* Pop local scope */
    popSymbolTable(new_symbol_table);
    
    
    /* Add the footer of the for instruction */
    addInstructionMainQueueFormated(mips_end_for, loop_for_counter, loop_for_counter);
    
    /* Pop local iterator */
    popSymbolTable(new_symbol_table_iterator);

    /* Delte actual scope */
    deleteSymbolTable(&new_symbol_table);
    deleteSymbolTable(&new_symbol_table_iterator);
    
    /* Increment for counter */
    loop_for_counter += 1;
    
    /* Return success */
    return true;
}

/** 
 * Generate code for if.
 * 
 * @param if_token If comand token structure.
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenIf(TokenNode *if_token, SymbolTable *actual_symbol_table){
    int i;
    TokenNode *exp_token;
    TokenNode *block_token;
    TokenNode *list_elseif;
    TokenNode *token_node;
    TokenList *exp_list;
    TokenList *block_list;
    SymbolTable *new_symbol_table;
    
    /* Check if for token is null */
    if(if_token == NULL){
        printError("IF COMMAND IS INVALID!");
        return false;
    }
    
    /* Check if symbol table is null */
    if(actual_symbol_table == NULL){
        printError("INVALID SYMBOL TABLE!");
        return false;
    }
    
    /* Get expression */
    exp_token = listGetTokenByIndex(if_token->child_list, 2);
    
    /* Check if this expression is null */
    if(exp_token == NULL){
        printError("EXPRESSION TOKEN IS NULL!");
        return false;
    }
    
    /* Get bloco */
    block_token = listGetTokenByIndex(if_token->child_list, 4);
    
    /* Check if block token is null */
    if(block_token == NULL){
        printError("BLOCK TOKEN IS NULL!");
        return false;
    }
    
    /* Get list of elseif conditions */
    list_elseif = listGetTokenByIndex(if_token->child_list, 5);
    
    /* Check if block token is null */
    if(list_elseif == NULL){
        printError("INVALID TOKEN LIST ESLE IF!");
        return false;
    }
    
    /* Add header message */
    addInstructionMainQueue(mips_start_if);
    
    /* CGEN(exp) */
    cgenExpression(exp_token, actual_symbol_table);
    
    /* Check condition */
    addInstructionMainQueueFormated(mips_check_if, cond_if_counter, 0);
    
    /* Create a new escope */
    new_symbol_table = newSymbolTable(actual_symbol_table, REGISTER_TYPE_SP);
    
    /* Check if the new symbol table is correct */
    if(new_symbol_table == NULL){
        printError("FAILED TO CREATE LOCAL SYMBOL TABLE!");
        return false;
    }
    
    /* CGEN(bloco) */
    cgenBlockCode(block_token, new_symbol_table);
     
    /* Pop local scope */
    popSymbolTable(new_symbol_table);
    
    /* Add check for next if condition {else if / else} */
    addInstructionMainQueueFormated(mips_next_if, cond_if_counter, cond_if_counter, 0);
    
    /* Check if there are a list of else if */
    if(list_elseif->token_type == TI_LIST_ELSEIF){
        
        /* Get all expressions */
        exp_list = newTokenList();
        
        /* Check if the list of expression list is valid */
        if(exp_list == NULL){
            printError("EXPRESSION LIST IS INVALID!");
            return false;
        }
        
        /* Pick all expressions in elseif */
        for(i = 1; i <= list_elseif->child_list->length; i++){
            
            /* Get the token node */
            token_node = listGetTokenByIndex(list_elseif->child_list, i);
            
            /* Check if this token node is valid or not */
            if(token_node == NULL){
                printError("INVALID TOKEN NODE!");
                return false;
            }
            
            /* If this token is an expression add him to the token list */
            if(IS_EXPRESSION(token_node->token_type)){
                listAddToken(exp_list, token_node);
            }
        }
        
        /* Get all blocks */
        block_list = newTokenList();
        
        /* Check if the list of blocks is valid */
        if(block_list == NULL){
            printError("BLOCK LIST IS INVALID!");
            return false;
        }
        
        /* Now as we have two types of block we should do it manually */
        /* Pick all blocks of elseif */
        for(i = 1; i <= list_elseif->child_list->length; i++){
            
            /* Get the token node */
            token_node = listGetTokenByIndex(list_elseif->child_list, i);
            
            /* Check if this token node is valid or not */
            if(token_node == NULL){
                printError("INVALID TOKEN NODE!");
                return false;
            }
            
            /* If this token is an expression add him to the token list */
            if(IS_BLOCK(token_node->token_type)){
                listAddToken(block_list, token_node);
            }
        }
        
        /* Add the other 'else if' conditionals */
        for(i = 1; i <= exp_list->length; i++){
            
            /* Get expression[i] */
            exp_token = listGetTokenByIndex(exp_list, i);
            
            /* Check if this expression is null */
            if(exp_token == NULL){
                printError("EXPRESSION TOKEN IS NULL!");
                return false;
            }
            
            /* Get block[i] */
            block_token = listGetTokenByIndex(block_list, i);
            
            /* Check if this expression is null */
            if(block_token == NULL){
                printError("BLOCK TOKEN IS NULL!");
                return false;
            }
            
            /* Header for elseif */
            addInstructionMainQueueFormated(mips_elseif_start, cond_if_counter, i);
            
            /* CGEN(exp) */
            cgenExpression(exp_token, actual_symbol_table);
            
            /* Check condition */
            addInstructionMainQueueFormated(mips_check_if, cond_if_counter, i);
            
            /* Create a new escope */
            new_symbol_table = newSymbolTable(actual_symbol_table, REGISTER_TYPE_SP);
            
            /* Check if the new symbol table is correct */
            if(new_symbol_table == NULL){
                printError("FAILED TO CREATE LOCAL SYMBOL TABLE!");
                return false;
            }
            
            /* CGEN(bloco) */
            cgenBlockCode(block_token, new_symbol_table);
            
            /* Pop local scope */
            popSymbolTable(new_symbol_table);
            
            /* Delete the temporary escope */
            deleteSymbolTable(&new_symbol_table);
            
            /* Add check for next if condition {else if / else} */
            addInstructionMainQueueFormated(mips_next_if, cond_if_counter, cond_if_counter, i);
        }
    }
    
    /* Check if 'if token' has a else condition */
    if(if_token->token_type == TI_IF_ELSE){
        
        /* Get block else */
        block_token = listGetTokenByIndex(if_token->child_list, 7);
        
        /* Check if this expression is null */
        if(block_token == NULL){
            printError("BLOCK TOKEN IS NULL!");
            return false;
        }
        
        /* Create a new escope */
        new_symbol_table = newSymbolTable(actual_symbol_table, REGISTER_TYPE_SP);
        
        /* Check if the new symbol table is correct */
        if(new_symbol_table == NULL){
            printError("FAILED TO CREATE LOCAL SYMBOL TABLE!");
            return false;
        }
        
        /* Header for else */
        addInstructionMainQueueFormated(mips_else_start, cond_if_counter);
        
        /* CGEN(bloco) */
        cgenBlockCode(block_token, new_symbol_table);
        
        /* Pop local scope */
        popSymbolTable(new_symbol_table);
        
        /* Delete the temporary escope */
        deleteSymbolTable(&new_symbol_table);
    }
    
    /* Add if footer */
    addInstructionMainQueueFormated(mips_end_if, cond_if_counter);
    
    /* Increment if counter */
    cond_if_counter += 1;
    
    /* Return success */
    return true;
}

/** 
 * Generate code for function definition.
 * 
 * @param function_def_token function definition token.
 * @param actual_symbol_table The actual or previous symbol table.
 * @return true if there's no error on execution and false otherwise.
 */
bool cgenFunction(TokenNode *function_def_token, SymbolTable *actual_symbol_table){
    int i;
    SymbolTable *new_table;
    SymbolTable *new_params_table;
    TokenNode *t_name;
    TokenNode *block_token;
    TokenNode *parameter_token;
    TokenNode *parameters_list_token;
     
    /* Check if function def is null */
    if(function_def_token == NULL){
        printError("FUNCTION DEF TOKEN IS NULL!");
        return false;
    }
    
    /* New scope symbol_table */
    new_table = newSymbolTable(NULL, REGISTER_TYPE_SP);
    
    /* Check if the new table is null */
    if(new_table == NULL){
        printError("NEW SYMBOL TABLE IS NULL!");
        return false;
    }
    
    /* New parameters list symbol table */
    new_params_table = newSymbolTable(NULL, REGISTER_TYPE_FP);
    
    /* Check if the new table is not null */
    if(new_params_table == NULL){
        printError("PARAMETERS SYMBOL TABLE IS NULL!");
        return false;
    }
    
    /* Link new_table with the parameters list table */
    symbolTableAddBrother(new_table, new_params_table);
    
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
            for(i = parameters_list_token->child_list->length; i > 0; i--){
                
                /* Get the token */
                parameter_token = listGetTokenByIndex(parameters_list_token->child_list, i);
                
                /* Check if this parameter is a t_name */
                if(parameter_token->token_type == T_NAME){
                    /* Add this token as a symbol in the symbol table */
                    symbolTableAddSymbol(new_params_table, parameter_token->lex_str, NUMBER_TYPE);
                }
            }
        }
        else{
            /* If we have only one name we just add this name to the new table */
            parameter_token = parameters_list_token;
            symbolTableAddSymbol(new_params_table, parameter_token->lex_str, NUMBER_TYPE);
        }
        
        /* Get the block token */
        block_token = listGetTokenByIndex(function_def_token->child_list, 6);
    }
    else if(function_def_token->token_type == TI_FUNCTION){
        /* Store the block token on functions that don't have a parameters list */
        block_token = listGetTokenByIndex(function_def_token->child_list, 5);
    }
    else{
        printError("THIS TOKEN IS NOT A FUNCTION DEFINITION! -- 0x%X : %d", function_def_token->token_type, function_def_token->token_type);
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
    
    /* 
        // DEPRECATED //
        Check if we have a block with return or not. Functions which return values, should push the return and jr
        acordingle, and the other who don't have values to return should pop local variables and jump to end_function_%s.
        
        // NEW STANDARD // DEPRECATED //
        All types of function should go to end when found  return and arranged the return values.
        Now this check is only to know if the first return is nil or the first expression!
        
        // DEFINITIVE STANDARD //
        There's no need to jump to end of function, in functions with return.
    */
    if(block_token->token_type == TI_BLOCO){
        /* Pop local variables on stack */
        addInstructionMainQueueFormated(mips_pop_params, (new_table->shift_address));
        
        /* Finish function definition poping Record Activation, and puting nil on $a0*/
        addInstructionMainQueueFormated(mips_end_function_def, (t_name->lex_str));

        /* Pop parameters on stack */
        addInstructionMainQueueFormated(mips_pop_params, (new_params_table->shift_address));
        
        /* Add final part */
        addInstructionMainQueueFormated(mips_end_function_def2, (t_name->lex_str));
    }

    /* Delete local symbol table and parameters symbol table */
    deleteSymbolTable(&new_table);
    deleteSymbolTable(&new_params_table);
    
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
    int i;
    TokenList *list_names;
    TokenNode *token_exp;
    TokenNode *token_name;
    TokenNode *list_exp_token;
    TokenNode *list_names_token;
    SymbolNode *symbol_node;
    
    /* Check if local variable token is null */
    if(local_variable_token == NULL){
        printError("LOCAL VARIABLE TOKEN IS INVALID!");
        return false;
    }
    
    /* Check if symbol table is null */
    if(actual_symbol_table == NULL){
        printError("INVALID SYMBOL TABLE!");
        return false;
    }
    
    /* Get token list of names */
    /* I NEED 3 DAYS TO FOUND THAT INDEX OF THE LIST OF NAMES IS 2 NOT 1... PLEASE ALWAYS REMEMBER THAT! */
    list_names_token = listGetTokenByIndex(local_variable_token->child_list, 2); //1);
    
    /* Check list of names token */
    if(list_names_token == NULL){
        printError("INVALID LIST OF NAMES!");
        return false;
    }
    
    /* Add header for a local assign */
    addInstructionMainQueue(mips_start_local_assign);
    
    /* Local_variable_token is of type 'LOCAL DEFINE' so we are only creating variables with nil value */
    if(local_variable_token->token_type == TI_LOCAL_DEFINE){
        
        /* If we have only one variable so our list of names will be only a T_NAME */
        if(list_names_token->token_type == T_NAME){
            
            /* In that case we only have a T_NAME token */
            token_name = list_names_token;
            
            /* Add the new token name to our symbol table */
            symbolTableAddSymbol(actual_symbol_table, token_name->lex_str, NUMBER_TYPE);
            
            /* Retrieve the added symbol node */
            symbol_node = symbolTableGetSymbolNodeByName(actual_symbol_table, token_name->lex_str);
            
            /* Check if the retrieved symbol node is correct */
            if(symbol_node == NULL){
                printError("INVALID SYMBOL NODE!");
                return false;
            }
            
            /* Add the definition node to stack */
            instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetDefineInstruction(symbol_node));
            
            /* As we are only definig variable we'll assign nil value to this variable */
            addInstructionMainQueue(mips_nil);
            
            /* Add the a0 value to the variable we had created */
            instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetStoreInstruction(symbol_node));
        }
        /* Otherwise we have a token with all childs being or T_NAME or T_COMMA (͡° ͜ʖ ͡°) */
        else{
            
            /* Get list of names */
            list_names = listGetTokensByType(list_names_token->child_list, T_NAME);
            
            /* Check if the list of names is correct */
            if(list_names == NULL){
                printError("INVALID LIST OF NAMES!");
                return false;
            }
            
            /* The only difference now is that we have a list of names, not just a name */
            for(i = list_names->length; i > 0; i--){
                
                /* Get the i token name */
                token_name = listGetTokenByIndex(list_names, i);
                
                /* Check if the token retrieved is valid */
                if(token_name == NULL){
                    printError("INVALID TOKEN NAME!");
                    return false;
                }
                
                /* Add the new token name to our symbol table */
                symbolTableAddSymbol(actual_symbol_table, token_name->lex_str, NUMBER_TYPE);
                
                /* Retrieve the added symbol node */
                symbol_node = symbolTableGetSymbolNodeByName(actual_symbol_table, token_name->lex_str);
                
                /* Check if the retrieved symbol node is correct */
                if(symbol_node == NULL){
                    printError("INVALID SYMBOL NODE!");
                    return false;
                }
                
                /* Add the definition node to stack */
                instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetDefineInstruction(symbol_node));
                
                /* As we are only definig variable we'll assign nil value to this variable */
                addInstructionMainQueue(mips_nil);
                
                /* Add the a0 value to the variable we had created */
                instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetStoreInstruction(symbol_node));
            }
        }
    }
    /* Local assign with expression values */
    else{
        
        /* Get token list of expressions */
        list_exp_token = listGetTokenByIndex(local_variable_token->child_list, 4);
        
        /* Check if list of expressions token is not null */
        if(list_exp_token == NULL){
            printError("INVALID LIST OF EXPRESSIONS!");
            return false;
        }
        
        /* 
            First of all, our list of expression could be only one name, and since we
            know that all functions store the first return in $a0, there's no way we
            have a function with two returns assign to a single variable, and since
            semantic proccess has been done, otherwise we won't get there, there's 
            no need to pop more than one return of the expression list on that case.
        */
        if(list_names_token->token_type == T_NAME){

            /* Get the actual token expression node */
            token_exp = list_exp_token;

            /* In that case we only have a T_NAME token */
            token_name = list_names_token;
            
            /* Add header of the instruction */
            addInstructionMainQueueFormated(mips_marker_local_assign, token_name->lex_str, 0);
            
            /* Add the new token name to our symbol table */
            symbolTableAddSymbol(actual_symbol_table, token_name->lex_str, NUMBER_TYPE);
            
            /* Retrieve the added symbol node */
            symbol_node = symbolTableGetSymbolNodeByName(actual_symbol_table, token_name->lex_str);
            
            /* Check if the retrieved symbol node is correct */
            if(symbol_node == NULL){
                printError("INVALID SYMBOL NODE!");
                return false;
            }

            /* Add the definition node to stack */
            instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetDefineInstruction(symbol_node));

            /* Add header of this instruction to main queue */
            addInstructionMainQueueFormated(mips_marker_exp, 0);

            /* CGEN(exp) */
            cgenExpression(token_exp, actual_symbol_table);
            
            /* Add the correct store $a0 instruction */
            instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetStoreInstruction(symbol_node));
        }
        /* Otherwise we have a list of names and expressions */
        else{
            
            /* Get list of names */
            list_names = listGetTokensByType(list_names_token->child_list, T_NAME);
            
            /* Check if the list of names is correct */
            if(list_names == NULL){
                printError("INVALID LIST OF NAMES!");
                return false;
            }
            
            /* Add all variables on list of names to stack and symbol table */
            for(i = list_names->length; i > 0; i--){
                
                /* Get the i token name */
                token_name = listGetTokenByIndex(list_names, i);
                
                /* Check if the token retrieved is valid */
                if(token_name == NULL){
                    printError("INVALID TOKEN NAME!");
                    return false;
                }
                
                /* Add the new token name to our symbol table */
                symbolTableAddSymbol(actual_symbol_table, token_name->lex_str, NUMBER_TYPE);
                
                /* Retrieve the added symbol node */
                symbol_node = symbolTableGetSymbolNodeByName(actual_symbol_table, token_name->lex_str);
                
                /* Check if the retrieved symbol node is correct */
                if(symbol_node == NULL){
                    printError("INVALID SYMBOL NODE!");
                    return false;
                }
                
                /* Add the definition node to stack */
                instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetDefineInstruction(symbol_node));
            }
            
            /* Execute list of expressions, and put reverse results on stack */
            cgenExpressionList(list_exp_token, actual_symbol_table);
            
            /* Assign our list of names with our actual symbol table, the values stacked */
            assignStackHelper(list_names, actual_symbol_table);
        }
    }
    
    /* Add footer of a local assign */
    addInstructionMainQueue(mips_end_local_assign);
    
    /*
        ONLY NECESSARY FOR DEBUGS... IGNORE THIS CODE
        SymbolTable *previous_table;
        previous_table = actual_symbol_table;
        while(previous_table!=NULL){
            for(i = 0; i < previous_table->length; i++){
                printf("%s ... 0x%X\n", previous_table->items[i]->symbol_name, previous_table->items[i]->symbol_address);
            }
            previous_table = previous_table->previous_scope;
        }
    */
    
    /* Return success */
    return true;
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
            printWarning("NOT IMPLEMENTED YET!");
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
    int pop_size;
    int ra_address;
    int fp_address;
    int exp_executed;
    int old_fp_address;
    int shift_stack_size;
    char *function_name;
    TokenNode *token_name;
    TokenNode *root_token;
    TokenNode *list_exp_token;
    SymbolTable *symbol_table;
    
    /* Check if token return command is valid */
    if(command_return_token == NULL){
        printError("INVALID COMMAND RETURN TOKEN!");
        return false;
    }
    
    /* Check if actual symbol table is valid */
    if(actual_symbol_table == NULL){
        printError("INVALID SYMBOL TABLE!");
        return false;
    }
    
    /* 
        The result of a given command is always, a jump to the $ra, but before we do that we
        need to stack the return, and this is really hard to do, at least for me.
        The idea that is implemented here is the following, first we perform a shift of all stack
        from the last $fp to the first variable, and to perform this, first I found what is the size
        of the shift necessary for us to return the values, after found this size and perform the
        shift we execute the list of parameters, assuming that the old $fp is on the top of the stack
        and the first return is always on $a0, and if there are others, they are on stack, we can
        continue the code.
        Example:
        
            Initial stack:
            
                4+  $$local_variables$$
                12  $ra
                16  $fp
                20+ $$function_parameters$$
                28  $old_fp
                
            Executing return exp_list:
            
                4+  $$local_variables$$
                12  $ra
                16  $fp
                20+ $$function_parameters$$
                28  $old_fp
                32+ $$return$$
                
            Final stack:
                
                4   $old_fp
                8+  $$return$$
                
        PS.:    For the examples above I had assumed that $$return$$, $$local_variables$$ and $$function_parameters$$
                has at least 2 blocks of code, on in other words, I assume that these variables, represent two 
                other variables.
        
        But if we have a procedure (a void function) or a function which don't return a value (the same as the return is null)
        we don't need to change too much things, we just need to free all of the local variables and then jump to the end
        of this procedure, 'cause the function already knows how to finish this function.
        
    */
        
    /* Since we can be inside a lot of blocks of code, we need to pop all variables until the function definition */
    /* First symbol table to check */
    symbol_table = actual_symbol_table;
    
    /* Initialize pop size */
    pop_size = 0;
    
    /* Recurse and count the size of the pop */
    while(symbol_table != NULL){
        pop_size += symbol_table->shift_address;
        symbol_table = symbol_table->previous_scope;
    }
    
    /* First root token */
    root_token = command_return_token->root_token;
    
    /* Recurse and get the name of the function which is being returned */
    while(root_token != NULL){
        
        //printf("0x%X --\n", root_token->token_type);
        
        /* If the actual token is of the type we wanted stop */
        if((root_token->token_type == TI_FUNCTION) || (root_token->token_type == TI_FUNCTION_PARAM)){
            break;
        }
        
        /* Get the next 'previous token' */
        root_token = root_token->root_token;
    }

    /* If this a void return, we don't need to evaluation expressions return */
    if(command_return_token->token_type == TI_RETURN){
        
        /* Pop local variables on stack */
        addInstructionMainQueueFormated(mips_pop_params, pop_size);
        
        /* If we can't find a function definition we are on main, and need to use a specific jump to end */
        if(root_token == NULL){
            instructionQueueEnqueueInstruction(main_instruction_queue, "\tj end_main_function # End of main\n", true);
            
            /* Return success */
            return true;
        }
        
        /* Get token name of a function definition */
        token_name = listGetTokenByIndex(root_token->child_list, 2);
        
        /* Check if this token name is valid */
        if(token_name == NULL){
            printError("INVALID FUNCTION NAME!");
            return false;
        }
        
        /* Pick the pointer of the string of the function name */
        function_name = token_name->lex_str;
        
        /* Add jump to function return */
        addInstructionMainQueueFormated(mips_end_of_function, function_name);
        
        /* Return success */
        return true;
    }
    
    /*
        If we get there, we have a return with multiple values, the script to return multiple values is one
        of the harder things in this code. First of all we assume that you can't return  execution of another function
        and we make sure of this with the semantic analyser. Second if we count the number of expressions...
        Hmmmm...
        No call_function is a valid expression... I need to found a way of return multiple values of a call function expression.
        I know that a function call, which have more than one return value, will put this values on stack, and I just need
        to evaluate the list of expressions, the problem is that I need to shift all variables before I do this.
        Hmmm... I know how to move the last $fp from stack to the top of my new stack, and it's easy, because I know
        how much parameters, I have on stack, and it's easy remove all of them, since computer need this parameters to
        evaluate the return, I can't remove them before finish expression evaluation... Hmmmm... GOT IT!
        If we now how many expression we had evaluated, we just need to shift all of them to begin of the stack, but, to do this
        I need to write a code that push the entire stack with the same model of the procedure return, and if I maintain the
        same model as a normal expression, then I don't need to use $ra, because all of the function registers, will remain stacked
        where they should stay, so in this moment I can jump to end and remove the local variables, the $ra, the $fp, the parameters
        and left the old $fp untouched.
        
        Example:
        
        This is how stack will be after the expression list execution:
        
            +   $$>>return<<$$
            +   $$local$$
            4   $ra
            8   $fp
            +   $$parameters$$
            4   $old-fp
            
        Then all I need to do is just shift return that is on stack, to the bottom of this stack, like this:
        
            Removed:
                +   $$local$$
                4   $ra
                8   $fp
                +   $$parameters$$
            
            New Stack:
                4   $old-fp
                +   $$>>return<<$$
            
    */
    
    /* Get list of expressions */
    list_exp_token = listGetTokenByIndex(command_return_token->child_list, 2);
    
    /* Check the integrity of this pointer */
    if(list_exp_token == NULL){
        printError("FAILED TO RETRIEVE LIST EXP TOKEN!");
        return false;
    }
    
    /* Execute list of expressions */
    cgenExpressionList(list_exp_token, actual_symbol_table);
    
    /* 
        Now all expressions that is being returned is on top of stack, I Need to found a way to put them on bottom of stack.
        I know that the list of expressions that is inside on my stack, had changed the pointers of the symbol table,
        so indirectly I know the size of the stack, and the size of stack is the address of the last item on my symbol table
        
                            (symbol_table->items[symbol_table->length - 1]->symbol_address - 4)
        
        I know that the size of my parameters is the size of the brother table, and I can go to brother table easily, because
        there are only one brother table in a normal symbol table, of course there are a lot of previous scope so i need to go
        to the first one to found the brother table
        
            previous_scope = symbol_table; while(previous_scope != NULL){ if(previous_scope->brother_table != NULL) break; }
               if(previous_scope == NULL) { no_params_table; } else { brother_table = previous_scope->brother_table; }
        
        The size of $old_fp, $fp, $ra, are always 12, so the last parameter is the size of local variables that is
            
                                           (symbol_table->shift_address - 4)
        
        This is the complete size of our stack after execute the return expression list, and all we need to do is rearrange 
        this stack and jump to the end of this function.
    */
    
    /* 
        With the correct algorithm to shift the expression stack, all i need to do now, is just pick the correct variables,
        despite the fact, that this seems easy, found the correct variables could be not too easy, than your thoughts.
        The variables are X, Y, Z and Q, and a more formal description of this variables are declared below.
            
            -- Address of $ra
            X + 4 =>
            -- Address of $fp
            X + 8 =>
            -- Address of old $fp
            Y =>
            -- Shift to clear stack
            Z =>
            -- Size of expression stack
            Q =>
    */
    
    // TODO!
    printTodo("GET VARIABLES TO EXECUTE THE CORRECT RETURN PROCEDURE!");
    
    /* Return address address */
    ra_address = 0;
    fp_address = 0;
    exp_executed = 0;
    old_fp_address = 0;
    shift_stack_size = 0;

    /* Add start of the return */
    addInstructionMainQueueFormated(mips_start_return, ra_address, fp_address, old_fp_address, shift_stack_size);
    
    /* Add loop for rearrange values of the new stack */
    addInstructionMainQueueFormated(mips_return_multiple, exp_executed, multiple_return_counter, multiple_return_counter);
    
    /* Increase the multiple return counter */
    multiple_return_counter += 1;
    
    /* Add end of a return expression */
    addInstructionMainQueue(mips_end_return);
    
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
    TokenNode *command_list_token;
    TokenNode *command_return_token;
    
    /* Check if the actual block_token is not NULL */
    if(block_token == NULL){
        printError("BLOCK TOKEN IS NULL!");
        return false;
    }
    
    /* Command list token */
    command_list_token = listGetTokenByIndex(block_token->child_list, 1);
    
    /* Generate command list code */
    cgenCommandList(command_list_token, previous_scope);
    
    /* Check if this block has a return command */
    if(block_token->token_type == TI_BLOCO_RETURN){
        
        /* Command return assign */
        command_return_token = listGetTokenByIndex(block_token->child_list, 2);
        
        /* Generate command return code */
        cgenCommandReturn(command_return_token, previous_scope);
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
 * @return Number of expressions processed.
 */
int cgenExpressionList(TokenNode *list_exp_token, SymbolTable *symbol_table) {
    int i;
    int processed_exp;
    TokenNode *token_node;
    
    /* Check if we have a list of expressions or just an expression */
    if(list_exp_token->token_type == TI_LISTAEXP){

        /* Execute from right to left child list that is non T_COMMA */
        for(i = list_exp_token->child_list->length, processed_exp = 0; i > 0; i--) {
            
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
            
            /* Increase symbol table */
            symbolTablePushVar(symbol_table);
            
            /* If we get there we have an expression, so increment counter of expressions */
            processed_exp += 1;
        }
    }
    else{
        /* If this is not an expression list, so it could be only a expression */
        token_node = list_exp_token;
        
        /* Check if this token is null */
        if(token_node == NULL){
            printError("INVALID TOKEN NODE!");
            return -1;
        }

        /* CGEN(exp) */
        cgenExpression(token_node, symbol_table);
        
        /* push a0 */
        addInstructionMainQueue(mips_push_a0);
        
        /* Increase symbol table */
        symbolTablePushVar(symbol_table);
        
        /* Processed only one exp */
        processed_exp = 1;
    }
    
    /* Return success */
    return processed_exp;
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
                printWarning("OPERAND NOT RECOGNIZED OR NOT IMPLEMENTED YET! -- 0x%X : %d", exp_token->token_type, exp_token->token_type);
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
        if(exp_token->token_type != TI_EQ && exp_token->token_type != TI_NEQ){
            /* mips_check_a0_nil() */
            addInstructionMainQueue(mips_check_a0_nil);
        }
        
        /* push_a0 */
        addInstructionMainQueue(mips_push_a0);
        
        /* Increase symbol table to avoid errors */
        symbolTablePushVar(symbol_table);
        
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
                printWarning("OPERAND NOT RECOGNIZED OR NOT IMPLEMENTED YET! -- 0x%X : %d", exp_token->token_type, exp_token->token_type);
                break;
        }
        
        /* pop */
        addInstructionMainQueue(mips_pop);
        
        /* Pop symbol table */
        symbolTablePopVar(symbol_table);
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
                
                /* If this symbol is not on global symbol table, so it's nil, and will be declared on global table */
                if(symbol_node == NULL){
                    /* Add Symbol to table */
                    symbolTableAddSymbol(global_symbol_table, token_terminal->lex_str, NUMBER_TYPE);
                    
                    /* Get the recent added symbol and assign nil to this variable */
                    symbol_node = symbolTableGetSymbolNodeByName(global_symbol_table, token_terminal->lex_str);
                    
                    /* Check if new symbol node is valid */
                    if(symbol_node == NULL){
                        printError("CANNOT CREATE NEW SYMBOL NODE ON GLOBAL SYMBOL TABLE!");
                        return false;
                    }
                }
                
                /* Store the correct load instruction */
                instructionQueueEnqueueInstructionNode(main_instruction_queue, symbolNodeGetLoadInstruction(symbol_node));
                break;
            case TI_CALL_FUNCTION:
            case TI_CALL_FUNCTION_PAR:
                /* Generating code for call function */
                cgenCallFunction(token_terminal, symbol_table);
                break;
            default:
                /* For types not implemented yet */
                printWarning("TYPE [%s] NOT RECOGNIZED OR NOT IMPLEMENTED YET!", token_terminal->token_str);
                break;
        }
    }
    
    /* Return success */
    return true;
}