#ifndef CODEGEN_FUNCTIONS_H
#define CODEGEN_FUNCTIONS_H

/* ------------------------------------------------------------- */
/*                     Code Generator Functions                  */
/* ------------------------------------------------------------- */

/* Generate all code */
bool cgenAllCode();



/* ------------------------------------------------------------- */
/*                         System Templates                      */
/* ------------------------------------------------------------- */

/* Generated code Header */
const char mips_header[] = 
    "# ### START OF GENERATED CODE ### #\n"
    "\n"
    "# Code Generated By Jeferson Lima and Jeferson Rene Compiler\n"
    "# LUA MIPS Compiler Version 0.3 \n"
    "\n"
    "# Variable Declarations\n"
        "\t.data\n"
    "\n"
    "# System default variables \n"
    "__newline: .asciiz \"\\n\"\n"
    "__nil:\t.asciiz \"nil\"\n"
    "\n"
    "# User Global Variables";
    
/* -- GLOBAL VARS -- "%s" */

/* System Functions */        
const char mips_system_functions[] =
    "\n"
    "# Start of mips code\n"
        "\t.text\n"
        "\t.globl main\n"
        "\n"
    "# System Defined Functions\n"
    "\n"
    "# -- Print Function -- #\n"
    "function_print:\n"
        "\t# Load Function Frame Pointer and Return Adress\n"
        "\tmove $fp, $sp\n"
        "\tsw $ra, 0($sp)\n"
        "\taddiu $sp, $sp, -4\n"
        "\n"
        "\t# Load First Parameter\n"
        "\tlw $a0, 4($fp)\n"
        "\tla $t0, _nil\n"
        "\n"
        "\t# Check if it's a nil number\n"
        "\tbe $a0, $t0, print_nil_value\n"
        "\t# Print number if it's not nil\n"
        "\tli $v0, 1\n"
        "\tsyscall\n"
        "\tj end_print\n"
        "\n"
    "print_nil_value:\n"
        "\t# Print Value nil\n"
        "\tli $v0, 4\n"
        "\tla $a0, __nil\n"
        "\tsyscall\n"
        "\n"
    "end_print:\n"
        "\t# Print linefeed\n"
        "\tli $v0, 4\n"
        "\tla $a0, __newline\n"
        "\tsyscall\n"
        "\n"
        "\t# Close Print Function \n"
        "\tlw $ra, 0($fp)\n"
        "\taddiu $sp, $sp, 12\n"
        "\tlw $fp, 0($sp)\n"
        "\tjr $ra\n"
    "# -- End Print Function -- #\n"
    "\n"
    "# Main Definition\n"
    "main:\n";
    
/* -- ALL INSTRUCTIONS -- "%s" */

/* Close main */
const char mips_footer[] =
        "\n"
    "# Close Main Declaration\n"
    "close_main:\n"
        "\tli $v0, 10\n"
        "\tsyscall\n"
        "\n"
    "# ### END OF GENERATED CODE ### #\n";

/* ------------------------------------------------------------- */
/*                   Basic Instructions Template                 */
/* ------------------------------------------------------------- */

/* Store a global variable */    
const char mips_global_atributtion[] =
    "\t# --------- Store $a0 in global variable -------- #\n"
    "\tsw $a0, _%s\n"
    "\t# ----------------------------------------------- #\n";

/* Load a global variable into $a0 */
const char mips_global_load[] =
    "\t# --------- Load global variable in $a0 --------- #\n"
    "\tlw $a0, _%s\n"
    "\t# ----------------------------------------------- #\n";

/* Push temporary return of a expression */
const char mips_push_a0[] =
    "\t# -------------- Push $a0 to stack -------------- #\n"
    "\tsw $a0, 0($sp)\n"
    "\taddiu $sp, $sp, -4\n"
    "\t# ----------------------------------------------- #\n";
    
/* Pop stack value */
const char mips_pop_a0[] =
    "\t# -------------- Pop $a0 from stack ------------- #\n"
    "\taddiu $sp, $sp, 4\n"
    "\t# ----------------------------------------------- #\n";

/* Load top value to $t1 */
const char mips_top_t1[] =
    "\t# ------------- Top of stack to $t1 ------------- #\n"
    "\tlw $t1, 4($sp)\n"
    "\t# ----------------------------------------------- #\n";

/* Load a static number into $a0 */
const char mips_static_number_load[] =
    "\t# --------- Load static number into $a0 --------- #\n"
    "\tli $a0, %s\n"
    "\t# ----------------------------------------------- #\n";

/* ------------------------------------------------------------- */
/*                  Unary Operations Template                    */
/* ------------------------------------------------------------- */

/**
 * Model for unary operations
 * 
 *  Default Model:
 *      CGEN(operand exp) ->
 *          CGEN(exp)
 *          CGEN(operand)
 */

/* Not a number in $a0 */
const char mips_not_a0_a0[] =
    "\t# ---------------- Not $a0 number --------------- #\n"
    "\tnot $a0, $a0\n"
    "\t# ----------------------------------------------- #\n";
 
/* Convert $a0 to a negative number */
const char mips_static_number_load[] =
    "\t# -------------- Negative $a0 number ------------ #\n"
    "\tnegu $a0, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* ------------------------------------------------------------- */
/*                  Binary Operations Template                   */
/* ------------------------------------------------------------- */

/**
 * Model for binary operations
 * 
 *  Default Model:
 *      CGEN(exp1 operand exp2) ->
 *          CGEN(exp1)
 *          push_a0
 *          CGEN(exp2)
 *          top_t1
 *          CGEN(operand)
 *          pop_a0
 */

/* Add value of $t1 with $a0 and store in $a0 */
const char mips_add_a0_t1_a0[] =
    "\t# ------------- Add $a0 = $t1 + $a0 ------------- #\n"
    "\tadd $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";
    
/* Sub value of $t1 with $a0 and store in $a0 */
const char mips_sub_a0_t1_a0[] =
    "\t# ------------- Sub $a0 = $t1 - $a0 ------------- #\n"
    "\tsub $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";
    
/* Div value of $t1 with $a0 and store in $a0 */
const char mips_div_a0_t1_a0[] =
    "\t# ------------- Div $a0 = $t1 / $a0 ------------- #\n"
    "\tdiv $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* Mul value of $t1 with $a0 and store in $a0 */
const char mips_mul_a0_t1_a0[] =
    "\t# ------------- Mul $a0 = $t1 * $a0 ------------- #\n"
    "\tmul $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* And the value of $t1 with $a0 and store in $a0 */
const char mips_and_a0_t1_a0[] =
    "\t# ------------- And $a0 = $t1 && $a0 ------------ #\n"
    "\tand $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";
    
/* Or the value of $t1 with $a0 and store in $a0 */
const char mips_or_a0_t1_a0[] =
    "\t# ------------- Or $a0 = $t1 || $a0 ------------- #\n"
    "\tor $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* Check if is greater than, the value of $t1 with $a0 and store in $a0 */
const char mips_gt_a0_t1_a0[] =
    "\t# ------------- Gt $a0 = $t1 > $a0 -------------- #\n"
    "\tslt $a0, $a0, $t1\n"
    "\t# ----------------------------------------------- #\n";

/* Check if is less than, the value of $t1 with $a0 and store in $a0 */
const char mips_lt_a0_t1_a0[] =
    "\t# ------------- Lt $a0 = $t1 < $a0 -------------- #\n"
    "\tslt $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* Check if $a0 is equal $t1 */
const char mips_eq_a0_t1_a0[] =
    "\t# ------------- Eq $a0 = $t1 == $a0 ------------- #\n"
    "\tsubu $a0, $t1, $a0\n"
    "\tsltu $a0, $0, $a0\n"
    "\txori $a0, $a0, 1\n"
    "\t# ----------------------------------------------- #\n";

/* Check if $a0 is not equal $t1 */
const char mips_neq_a0_t1_a0[] =
    "\t# ------------- Neq $a0 = $t1 ~= $a0 ------------- #\n"
    "\tsubu $a0, $t1, $a0\n"
    "\tsltu $a0, $0, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* Check if $a0 is greater or equal $t1 */
const char mips_gte_a0_t1_a0[] =
    "\t# ------------- Gte $a0 = $t1 >= $a0 ------------- #\n"
    "\tslt $a0, $t1, $a0\n"
    "\txori $a0, $a0, 1\n"
    "\t# ----------------------------------------------- #\n";
    
/* Check if $a0 is less or equal $t1 */
const char mips_lte_a0_t1_a0[] =
    "\t# ------------- Lte $a0 = $t1 <= $a0 ------------- #\n"
    "\tslt $a0, $a0, $t1\n"
    "\txori $a0, $a0, 1\n"
    "\t# ----------------------------------------------- #\n";

/* ------------------------------------------------------------- */
/*                  ..........................                   */
/* ------------------------------------------------------------- */

/**
 * Model ...
 * 
 *  Default Model:
 *      ...
 */

/* ------------------------------------------------------------- */

#endif