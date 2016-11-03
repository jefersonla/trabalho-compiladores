#ifndef CODEGEN_FUNCTIONS_H
#define CODEGEN_FUNCTIONS_H

#include <stdbool.h>
#include "token_struct.h"
#include "utils.h"

/* ------------------------------------------------------------- */
/*                     Code Generator Constants                  */
/* ------------------------------------------------------------- */

/* Global Start Address */
#define GLOBAL_START_ADRESS                     -1

/* Function Prefix */
#define FUNCTION_PREFIX                         "function_"

/* Global reserved variables Prefix */
#define GLOBAL_SYSTEM_VARIABLE_PREFIX           "_"

/* Global variables Prefix */
#define GLOBAL_VARIABLE_PREFIX                  "__"

/* Value of a nil variable */
#define NIL_TYPE_MASK                           "0x80000000"
#define NIL_TYPE_VALUE                          "0x7FFFFFFF"

/* Standar way to add itens to main queue */
#define addInstructionMainQueue(VAR)            instructionQueueEnqueueInstruction(main_instruction_queue, VAR, false)

/* ------------------------------------------------------------- */
/*                         Global Structures                     */
/* ------------------------------------------------------------- */

/* Generated Code */
extern InstructionQueue *header_instruction_queue;

/* Main Generated Code */
extern InstructionQueue *main_instruction_queue;

/* Global Symbol Table */
extern SymbolTable *global_symbol_table;

/* ------------------------------------------------------------- */
/*                     Code Generator Functions                  */
/* ------------------------------------------------------------- */

/* Generate all code */
bool cgenAllCode(TokenNode *root_token);

/* Copy global variable definitions */
bool copyGlobalVariables();

/* Generate blocks of code */
bool cgenBlockCode(TokenNode *block_token, SymbolTable *previous_scope);

/* Generate code for command */
bool cgenCommand(TokenNode *command_token, SymbolTable *actual_symbol_table);

/* Generate code for command list */
bool cgenCommandList(TokenNode *command_list_token, SymbolTable *actual_symbol_table);

/* Generate code for command return */
bool cgenComandReturn(TokenNode *comand_return_token, SymbolTable *actual_symbol_table);

/* Generate code for function call */
bool cgenCallFunction(TokenNode *call_function_token, SymbolTable *actual_symbol_table);

/* Generate code for assign */
bool cgenAssign(TokenNode *assign_token, SymbolTable *actual_symbol_table);

/* Generate code for command block */
bool cgenCommandBlock(TokenNode *command_block_token, SymbolTable *actual_symbol_table);

/* Generate code for while */
bool cgenWhile(TokenNode *while_token, SymbolTable *actual_symbol_table);

/* Generate code for for */
bool cgenFor(TokenNode *for_token, SymbolTable *actual_symbol_table);

/* Generate code for if */
bool cgenIf(TokenNode *if_token, SymbolTable *actual_symbol_table);

/* Generate code for function definition call */
bool cgenFunction(TokenNode *function_def_token, SymbolTable *actual_symbol_table);

/* Generate code for local variable operation */
bool cgenLocalVariable(TokenNode *local_variable_token, SymbolTable *actual_symbol_table);

/* Generate code for expressions. */
bool cgenExpression(TokenNode *exp_token, SymbolTable *symbol_table);

/* Generate code for list expression */
bool cgenExpressionList(TokenNode *list_exp_token, SymbolTable *symbol_table);

/* Generate code for name list */
bool cgenNameList(TokenNode *list_exp_token, SymbolTable *symbol_table);

/* ------------------------------------------------------------- */
/*                      Code Generator Models                    */
/* ------------------------------------------------------------- */

/* Generated code Header */
extern const char mips_header[];

/* System Functions */        
extern const char mips_main[];

/* Close main */
extern const char mips_footer[];

/* Define a global variable */    
extern const char mips_global_define[];

/* Define a local variable */    
extern const char mips_local_define[];

/* Store a global variable */    
extern const char mips_global_store[];

/* Load a global variable into $a0 */
extern const char mips_global_load[];

/* Store a local variable */    
extern const char mips_local_store[];

/* Load a local variable into $a0 */
extern const char mips_local_load[];

/* Push temporary return of a expression */
extern const char mips_push_a0[];
    
/* Pop stack value */
extern const char mips_pop[];

/* Load top value to $t1 */
extern const char mips_top_t1[];

/* Load a static number into $a0 */
extern const char mips_static_number_load[];

/* Not a number in $a0 */
extern const char mips_not_a0[];
 
/* Convert $a0 to a negative number */
extern const char mips_neg_a0[];

/* Add value of $t1 with $a0 and store in $a0 */
extern const char mips_add_a0_t1_a0[];
    
/* Sub value of $t1 with $a0 and store in $a0 */
extern const char mips_sub_a0_t1_a0[];
    
/* Div value of $t1 with $a0 and store in $a0 */
extern const char mips_div_a0_t1_a0[];

/* Mul value of $t1 with $a0 and store in $a0 */
extern const char mips_mul_a0_t1_a0[];

/* And the value of $t1 with $a0 and store in $a0 */
extern const char mips_and_a0_t1_a0[];
    
/* Or the value of $t1 with $a0 and store in $a0 */
extern const char mips_or_a0_t1_a0[];

/* Check if is greater than, the value of $t1 with $a0 and store in $a0 */
extern const char mips_gt_a0_t1_a0[];

/* Check if is less than, the value of $t1 with $a0 and store in $a0 */
extern const char mips_lt_a0_t1_a0[];

/* Check if $a0 is equal $t1 */
extern const char mips_eq_a0_t1_a0[];

/* Check if $a0 is not equal $t1 */
extern const char mips_neq_a0_t1_a0[];

/* Check if $a0 is greater or equal $t1 */
extern const char mips_gte_a0_t1_a0[];
    
/* Check if $a0 is less or equal $t1 */
extern const char mips_lte_a0_t1_a0[];
    
/* Conditional type If-elseif-else */
extern const char mips_start_if[];

/* Check if condition */
extern const char mips_check_if[];

/* Check the next if condition */
extern const char mips_next_if[];

/* If end condition */
extern const char mips_end_if[];

/* Loop type while */
extern const char mips_start_while[];

/* While loop check while condition */
extern const char mips_check_while[];

/* While end loop */
extern const char mips_end_while[];

/* Loop type for */
extern const char mips_for[];

/* Function Call start model */
extern const char mips_start_function_call[];

/* Function Call end model */
extern const char mips_end_function_call[];

/* Start of function definition */
extern const char mips_start_function_def[];

/* End of function definition */
extern const char mips_end_function_def[];

/* Nil variable value */
extern const char mips_nil[];

/* ------------------------------------------------------------- */

#endif