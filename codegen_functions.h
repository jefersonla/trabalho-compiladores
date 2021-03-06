#ifndef CODEGEN_FUNCTIONS_H
#define CODEGEN_FUNCTIONS_H

#include <stdbool.h>
#include "token_struct.h"
#include "utils.h"

/* ------------------------------------------------------------- */
/*                     Code Generator Constants                  */
/* ------------------------------------------------------------- */

/* Register Types */

/* Register Type GP (Global Pointer) */
#define REGISTER_TYPE_GP                            1

/* Register Type SP (Stack Pointer) */
#define REGISTER_TYPE_SP                            2

/* Register Type FP (Frame Pointer) */              
#define REGISTER_TYPE_FP                            3

/* Function Prefix */
#define FUNCTION_PREFIX                             "function_"

/* Global reserved variables Prefix */
#define GLOBAL_SYSTEM_VARIABLE_PREFIX               "_"

/* Global variables Prefix */
#define GLOBAL_VARIABLE_PREFIX                      "__"

/* Value of a nil variable */
#define NIL_TYPE_MASK                               "0x7FFFFFFD"
#define NIL_TYPE_VALUE                              "0x80000002"

/* Logical type */
#define VOID_TYPE                                   0

/* Integer type */
#define NUMBER_TYPE                                 1

/* Standard way to add itens to main queue */
#define addInstructionMainQueue(VAR)                instructionQueueEnqueueInstruction( main_instruction_queue, \
                                                                                        formatedInstruction(VAR), false)

/* New array of instructions with variadic parameters */
#define addInstructionMainQueueFormated(VAR, ...)   instructionQueueEnqueueInstruction( main_instruction_queue, \
                                                                                        formatedInstruction(VAR, ##__VA_ARGS__), \
                                                                                        false)

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
int cgenExpressionList(TokenNode *list_exp_token, SymbolTable *symbol_table);

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

/* Pop stack params */
extern const char mips_pop_params[];

/* Pop local variables on stack */
extern const char mips_pop_local[];

/* Load top value to $t1 */
extern const char mips_top_t1[];

/* Load top value to $a0 */
extern const char mips_top_a0[];

/* Load a static number into $a0 */
extern const char mips_static_number_load[];

/* Move $a0 to $t1 */
extern const char mips_move_a0_t1[];

/* Move from $a0 to $t4 */
extern const char mips_move_a0_t4[];

/* Move from $a0 to $a2 */
extern const char mips_move_a0_a2[];

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

/* Header for else if */
extern const char mips_elseif_start[];

/* Header for else */
extern const char mips_else_start[];

/* Loop type while */
extern const char mips_start_while[];

/* While loop check while condition */
extern const char mips_check_while[];

/* While end loop */
extern const char mips_end_while[];

/* Loop type for */
extern const char mips_for_ini[];

/* Store for interval type */
extern const char mips_for_interval[];

/* Begin of for */
extern const char mips_start_for[];

/* For condition check */
extern const char mips_for_check[];

/* Store */
extern const char mips_for_load_inc[];

/* Mips default for inc */
extern const char mips_for_inc[];

/* End of for defition */
extern const char mips_end_for[];

/* Function Call start model */
extern const char mips_start_function_call[];

/* Function Call end model */
extern const char mips_end_function_call[];

/* Start of function definition */
extern const char mips_start_function_def[];

/* Continuation of function definition */
extern const char mips_start_function_def2[];

/* Empty return */
extern const char mips_end_of_function[];

/* End of function definition with nil return */
extern const char mips_end_function_def[];

/* End of function definition without nil return */
extern const char mips_end_function_defX[];

/* End of function definition */
extern const char mips_end_function_def2[];

/* Nil variable value */
extern const char mips_nil[];

/* Check if a given variable is nil */
extern const char mips_check_a0_nil[];

/* Begin of the and check */
extern const char mips_and_sc_header[];

/* Skip the next evaluation if the first has failed */
extern const char mips_and_sc_skip[];

/* End of mips sc */
extern const char mips_and_sc_footer[];

/* Begin of the and check */
extern const char mips_or_sc_header[];

/* Skip the next evaluation if the first has failed */
extern const char mips_or_sc_skip[];

/* End of mips sc */
extern const char mips_or_sc_footer[];

/* Begin of a assign */
extern const char mips_start_assign[];

/* Expression Execution marker */
extern const char mips_marker_exp[];

/* Assign marker */
extern const char mips_marker_assign[];

/* End of a assign */
extern const char mips_end_assign[];

/* Begin of a local assign */
extern const char mips_start_local_assign[];

/* Assign local assign marker */
extern const char mips_marker_local_assign[];

/* End of a local assign */
extern const char mips_end_local_assign[];

/* Begin of the return values model */
extern const char mips_start_return[];

/* Multiple returns stack rearange */
extern const char mips_return_multiple[];

/* End of a return */
extern const char mips_end_return[];

/* ------------------------------------------------------------- */

#endif