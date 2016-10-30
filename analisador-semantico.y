%{
/** **** Analisador  Semantico **** **/
/** Desenvolvido por Jeferson Lima  **/
/**              e   Jefferson Renê **/
/** Versão      0.3                 **/
/** Linguagem   LUA                 **/
/** Licença     MIT                 **/
/** Descrição:  Analisador          **/
/**             Sintatico/Semantico **/
/**             para a linguagem    **/
/**             LUA.                **/
/**                                 **/
/**   2º TRABALHO DE COMPILADORES   **/
/** ******************************* **/

/** ******************************* **/
/**  Compilar este  projeto usando  **/
/**  make parse    ou         make  **/
/** ******************************* **/

/** ******************************* **/
/**       LEIA O README PARA        **/
/**        MAIS INFORMAÇÔES         **/
/** ******************************* **/

/* String Manipulation */
#include <string.h>

/* Definições do Flex */
#include "lex.yy.h"

/* Definições dos tipos */
#include "parser.defs.h"

/* Token Structure */
#include "token_struct.h"

/* Needed by bison */
void yyerror(const char *);

/* Variables for File Manipulation */
char output_filename[MAX_OUTPUT_FILENAME];
FILE *output_file;

/* Contain actual token */
char *string_addr;

/* Token Data */
char *all_tokens;
int last_char;

/* Generated Code */
char *code_gen;

%}

/* Definition of token iterator */
%union{
    /* Pointer to a token node */
    TokenNode token_node;
}

    /*** * Definitions Section * ***/

/* Boolean Comparators */

%token <token_node> T_AND
%token <token_node> T_OR
%token <token_node> T_NOT

/* Symbols */

/* Math Operators */
%token T_PLUS
%token T_MINUS
%token T_TIMES
%token T_DIV
%token T_MOD
%token T_EXP
%token T_FLOOR
/* Boolean Operators */
%token T_EQ
%token T_NEQ
%token T_LTEQ
%token T_GTEQ
%token T_LT
%token T_GT
/* Separators and assign */
%token T_COMMA
%token T_SEMICOL
%token T_ASSIGN
/* Bit-a-Bit Operators */
%token T_BIT_AND
%token T_BIT_OR
%token T_BIT_N_XOR
%token T_BIT_RSH
%token T_BIT_LSH
/* Encapsulation Symbols */
%token T_OPENPAR
%token T_CLOSEPAR
%token T_OPENBRACE
%token T_CLOSEBRACE
%token T_OPENBRACKET
%token T_CLOSEBRACKET
/* Other Symbols */
%token T_CONCAT
%token T_VARARG
%token T_SEP
%token T_COLON
%token T_LABEL

/* -- Precendence -- */
%left T_EXP T_FLOOR
%left T_CONCAT T_VARARG T_SEP T_COLON T_LABEL
%left T_BIT_AND T_BIT_OR
%left T_BIT_N_XOR T_BIT_RSH T_BIT_LSH
%left T_OR
%left T_AND
%left T_LT T_GT T_LTEQ T_GTEQ T_NEQ T_EQ
%left T_PLUS T_MINUS
%left T_MOD T_TIMES T_DIV
%left T_NOT

/* Reserved Words */

/* Conditional Expression */
%token <token_node> T_IF
%token <token_node> T_ELSEIF
%token <token_node> T_ELSE
%token <token_node> T_THEN
/* Loop Expression */
%token <token_node> T_FOR
%token <token_node> T_WHILE
%token <token_node> T_DO
/* Close Expression */
%token <token_node> T_END
%token <token_node> T_RETURN
/* Variable and Functions Manipultion */
%token <token_node> T_FUNCTION
%token <token_node> T_LOCAL
%token <token_node> T_NIL

/* Extra Reserved Words */

/* Boolean Names */
/*%token <token_node> T_TRUE*/
/*%token <token_node> T_FALSE*/
/* Other Loop Expression */
%token <token_node> T_REPEAT
%token <token_node> T_UNTIL
%token <token_node> T_BREAK
/* Other Reserved Words */
%token <token_node> T_GLOBAL
%token <token_node> T_IN

/* Variable Types */
%token <token_node> T_NUMBER
%token <token_node> T_LITERAL
%token <token_node> T_NAME

/* Expression Types */
%type <token_node> programa
%type <token_node> bloco
%type <token_node> comando
%type <token_node> comandoret
%type <token_node> exp
%type <token_node> chamadadefuncao
%type <token_node> listadenomes
%type <token_node> listaexp
%type <token_node> comando_list
%type <token_node> term_elseif
%type <token_node> label

/* Start Type */
%start programa

/* Enable Verbose Errors */
%error-verbose

/* Number of Shift Reducions */
%expect 0

%%

    /*** * Language Definitions * ***/

/* -- Program Section  -- */
/*  > Store Application if success parsing */
programa        : bloco                                 {
                                                            #ifdef SEMANTIC_ANALYSER
                                                            /* Store output file result */
                                                            fprintf(output_file, "[programa %s]\n", $1.token_str);
                                                            
                                                            /* Print Finished Result */
                                                            fprintf(stderr,
                                                                    "\n::: SYNTATIC/SEMANTIC ANALYSER :::\n"
                                                                    "[programa %s]\n\n"
                                                                    "::: LEXICAL PARSER :::\n"
                                                                    "%s",
                                                                    $1.token_str, all_tokens);
                                                            #else 
                                                            /* Store output file result */
                                                            fprintf(output_file, "%s", code_gen);
                                                            
                                                            /* Print Finished Result */
                                                            fprintf(stderr,
                                                                    "\n::: CODE GENERATOR :::\n"
                                                                    "%s\n"
                                                                    "::: SYNTATIC/SEMANTIC ANALYSER :::\n"
                                                                    "[programa %s]\n\n"
                                                                    "::: LEXICAL PARSER :::\n"
                                                                    "%s",
                                                                    code_gen, $1.token_str, all_tokens);
                                                            #endif
                                                            
                                                            /* Close Output File */
                                                            fclose(output_file);
                                                        }
                ;

/* -- Block are sections of code -- */
bloco           : comando_list comandoret               { 
                                                            allocate2Tokens($$, "[bloco%s %s]", $1, $2);
                                                        }
                | comando_list                          { 
                                                            allocate1Token($$, "[bloco%s]", $1);
                                                        }
                ;

comando_list    : comando_list comando                  {
                                                            allocate2Tokens($$, "%s %s", $1, $2);
                                                        }
                | /* Empty */                           { 
                                                            allocateToken($$, "");
                                                        }
                ;

/* -- Commands belong to program, and represent all the actions that can occur -- */
comando         : T_SEMICOL                             { 
                                                            allocateToken($$, "[comando [T_SEMICOL ;]]");
                                                        }
                | label                                 {
                                                            allocate1Token($$, "[comando %s]", $1);
                                                        }
                | T_BREAK                               { 
                                                            allocateToken($$, "[comando [T_BREAK break]]");
                                                        }
                | chamadadefuncao                       { 
                                                            allocate1Token($$, "[comando %s]", $1);
                                                        }
                | listadenomes T_ASSIGN listaexp        {
                                                            allocate2Tokens($$,
                                                                            "[comando "
                                                                                "[listadenomes %s] "
                                                                                "[T_ASSIGN =] "
                                                                                "[listaexp %s]"
                                                                            "]",
                                                                            $1, $3);
                                                        }
                | T_DO bloco T_END                      {
                                                            allocate1Token( $$,
                                                                            "[comando "
                                                                                "[T_DO do] "
                                                                                "%s "
                                                                                "[T_END end]"
                                                                            "]",
                                                                            $2);
                                                        }
                | T_WHILE exp T_DO bloco T_END          {
                                                            allocate2Tokens($$,
                                                                            "[comando "
                                                                                "[T_WHILE while] "
                                                                                "%s "
                                                                                "[T_DO do] "
                                                                                "%s "
                                                                                "[T_END end]"
                                                                            "]",
                                                                            $2, $4);
                                                                                        }
                | T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_COMMA exp T_DO bloco T_END    {
                                                                                            allocate5Tokens($$,
                                                                                                            "[comando "
                                                                                                                "[T_FOR for] "
                                                                                                                "[T_NAME %s] "
                                                                                                                "[T_ASSIGN =] "
                                                                                                                "%s "
                                                                                                                "[T_COMMA ,] "
                                                                                                                "%s "
                                                                                                                "[T_COMMA ,] "
                                                                                                                "%s "
                                                                                                                "[T_DO do] "
                                                                                                                "%s "
                                                                                                                "[T_END end]"
                                                                                                            "]",
                                                                                                            $2, $4, $6, $8, $1);
                                                                                        }
                | T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_DO bloco T_END                {
                                                                                            allocate4Tokens($$,
                                                                                                            "[comando "
                                                                                                                "[T_FOR for] "
                                                                                                                "[T_NAME %s] "
                                                                                                                "[T_ASSIGN =] "
                                                                                                                "%s "
                                                                                                                "[T_COMMA ,] "
                                                                                                                "%s "
                                                                                                                "[T_DO do] "
                                                                                                                "%s "
                                                                                                                "[T_END end]"
                                                                                                            "]",
                                                                                                            $2, $4, $6, $8);
                                                                                        }
                | T_IF exp T_THEN bloco term_elseif T_ELSE bloco T_END                  {
                                                                                            allocate4Tokens($$,
                                                                                                            "[comando "
                                                                                                                "[T_IF if] "
                                                                                                                "%s "
                                                                                                                "[T_THEN then] "
                                                                                                                "%s"
                                                                                                                "%s "
                                                                                                                "[T_ELSE else] "
                                                                                                                "%s "
                                                                                                                "[T_END end]"
                                                                                                            "]",
                                                                                                            $2, $4, $5, $7);
                                                                                        }
                | T_IF exp T_THEN bloco term_elseif T_END                               {
                                                                                            allocate3Tokens($$,
                                                                                                            "[comando "
                                                                                                                "[T_IF if] "
                                                                                                                "%s "
                                                                                                                "[T_THEN then] "
                                                                                                                "%s"
                                                                                                                "%s "
                                                                                                                "[T_END end]"
                                                                                                            "]",
                                                                                                            $2, $4, $5);
                                                                                        }
                | T_FUNCTION T_NAME T_OPENPAR listadenomes T_CLOSEPAR bloco T_END       {
                                                                                            allocate3Tokens($$,
                                                                                                            "[comando "
                                                                                                                "[T_FUNCTION function] "
                                                                                                                "[T_NAME %s] "
                                                                                                                "[T_OPENPAR (] "
                                                                                                                "[listadenomes %s] "
                                                                                                                "[T_CLOSEPAR )] "
                                                                                                                "%s "
                                                                                                                "[T_END end]"
                                                                                                            "]",
                                                                                                            $2, $4, $6);
                                                                                        }
                | T_FUNCTION T_NAME T_OPENPAR T_CLOSEPAR bloco T_END                    {
                                                                                            allocate2Tokens($$,
                                                                                                            "[comando "
                                                                                                                "[T_FUNCTION function] "
                                                                                                                "[T_NAME %s] "
                                                                                                                "[T_OPENPAR (] "
                                                                                                                "[T_CLOSEPAR )] "
                                                                                                                "%s "
                                                                                                                "[T_END end]"
                                                                                                            "]",
                                                                                                            $2, $5);
                                                                                        }
                | T_LOCAL listadenomes T_ASSIGN listaexp                                {
                                                                                            allocate2Tokens($$,
                                                                                                            "[comando "
                                                                                                                "[T_LOCAL local] "
                                                                                                                "[listadenomes %s] "
                                                                                                                "[T_ASSIGN =] "
                                                                                                                "[listaexp %s]"
                                                                                                            "]",
                                                                                                            $2, $4);
                                                                                        }
                | T_LOCAL listadenomes                                                  {
                                                                                            allocate1Token( $$,
                                                                                                            "[comando "
                                                                                                                "[T_LOCAL local] "
                                                                                                                "[listadenomes %s]"
                                                                                                            "]",
                                                                                                            $2);
                                                                                        }
                ;


label           : T_LABEL T_NAME T_LABEL                { 
                                                            allocate1Token($$, "[T_LABEL ::] [T_NAME %s] [T_LABEL ::]", $2);
                                                        }

term_elseif     : term_elseif T_ELSEIF exp T_THEN bloco {
                                                            allocate3Tokens($$,"%s [T_ELSEIF elseif] %s [T_THEN then] %s", $1, $3, $5);
                                                        }
                | /* Empty */                           { 
                                                            allocateToken($$, "");
                                                        }
                ;

comandoret      : T_RETURN listaexp T_SEMICOL           {
                                                            allocate1Token( $$,
                                                                            "[comandoret "
                                                                                "[T_RETURN return] "
                                                                                "[listaexp %s] "
                                                                                "[T_SEMICOL ;]"
                                                                            "]",
                                                                            $2);
                                                        }
                | T_RETURN listaexp                     {
                                                            allocate1Token($$, "[comandoret [T_RETURN return] [listaexp %s]]", $2);   
                                                        }
                | T_RETURN T_SEMICOL                    { 
                                                            allocateToken($$, "[comandoret [T_RETURN return] [T_SEMICOL ;]]");
                                                        }
                | T_RETURN                              {
                                                            allocateToken($$, "[comandoret [T_RETURN return]]");
                                                        }
                ;

exp             : T_NIL                                 {
                                                            allocateToken($$, "[exp [T_NIL nil]]");
                                                        }
                | T_VARARG                              { 
                                                            allocateToken($$, "[exp [T_VARARG ...]]");
                                                        }
                /*| T_TRUE                              { 
                                                            allocateToken($$, "[exp [T_TRUE true]]");
                                                        }*/
                /*| T_FALSE                             {
                                                            allocateToken($$, "[exp [T_FALSE false]]");
                                                        }*/
                | T_NUMBER                              {
                                                            allocate1Token($$, "[exp [T_NUMBER %s]]", $1);
                                                        }
                | T_LITERAL                             {
                                                            allocate1Token($$, "[exp [T_LITERAL %s]]", $1);
                                                        }
                | T_NAME                                { 
                                                            allocate1Token($$, "[exp [T_NAME %s]]", $1);
                                                        }
                | chamadadefuncao                       { 
                                                            allocate1Token($$, "[exp %s]", $1);
                                                        }
                | T_MINUS exp                           {
                                                            allocate1Token($$, "[exp [opunaria [T_MINUS -]] %s]", $2);
                                                        }
                | T_NOT exp                             {
                                                            allocate1Token($$, "[exp [opunaria [T_NOT not]] %s]", $2);                
                                                        }
                | T_BIT_N_XOR exp                       {
                                                            allocate1Token($$, "[exp [opunaria [T_BIT_NOT ~]] %s]", $2);
                                                        }
                | exp T_PLUS exp                        {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_PLUS +]] %s]", $1, $3);
                                                        }
                | exp T_MINUS exp                       { 
                                                            allocate2Tokens($$, "[exp %s [opbin [T_MINUS -]] %s]", $1, $3);
                                                        }
                | exp T_TIMES exp                       {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_TIMES *]] %s]", $1, $3);
                                                        }
                | exp T_DIV exp                         {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_DIV /]] %s]", $1, $3);
                                                        }
                | exp T_FLOOR exp                       { 
                                                            allocate2Tokens($$, "[exp %s [opbin [T_FLOOR //]] %s]", $1, $3);
                                                        }
                | exp T_EXP exp                         {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_EXP ^]] %s]", $1, $3);
                                                        }
                | exp T_MOD exp                         { 
                                                            allocate2Tokens($$, "[exp %s [opbin [T_MOD %%]] %s]", $1, $3);
                                                        }
                | exp T_BIT_AND exp                     { 
                                                            allocate2Tokens($$, "[exp %s [opbin [T_BIT_AND &]] %s]", $1, $3);
                                                        }
                | exp T_BIT_OR exp                      {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_BIT_OR |]] %s]", $1, $3);
                                                        }
                | exp T_BIT_N_XOR exp                   {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_BIT_XOR ~]] %s]", $1, $3);
                                                        }
                | exp T_BIT_LSH exp                     {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_BIT_LSH <<]] %s]", $1, $3);
                                                        }
                | exp T_BIT_RSH exp                     {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_BIT_RSH >>]] %s]", $1, $3);
                                                        }
                | exp T_CONCAT exp                      { 
                                                            allocate2Tokens($$, "[exp %s [opbin [T_CONCAT ..]] %s]", $1, $3);
                                                        }
                | exp T_LT exp                          {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_LT <]] %s]", $1, $3);
                                                        }
                | exp T_LTEQ exp                        {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_LTEQ <=]] %s]", $1, $3);
                                                        }
                | exp T_GT exp                          {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_GT >]] %s]", $1, $3);
                                                        }
                | exp T_GTEQ exp                        {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_GTEQ >=]] %s]", $1, $3);
                                                        }
                | exp T_EQ exp                          { 
                                                            allocate2Tokens($$, "[exp %s [opbin [T_EQ ==]] %s]", $1, $3);
                                                        }
                | exp T_NEQ exp                         {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_NEQ ~=]] %s]", $1, $3);
                                                        }
                | exp T_AND exp                         {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_AND and]] %s]", $1, $3);
                                                        }
                | exp T_OR exp                          {
                                                            allocate2Tokens($$, "[exp %s [opbin [T_OR or]] %s]", $1, $3);
                                                        }
                | T_OPENPAR exp T_CLOSEPAR              {
                                                            allocate1Token($$, "[exp [T_OPENPAR (] %s [T_CLOSEPAR )]]", $2);
                                                        }
                ;

chamadadefuncao : T_NAME T_OPENPAR listaexp T_CLOSEPAR  {
                                                            allocate2Tokens($$,
                                                                            "[chamadadefuncao "
                                                                                "[T_NAME %s] "
                                                                                "[T_OPENPAR (] "
                                                                                "[listaexp %s] "
                                                                                "[T_CLOSEPAR )] "
                                                                            "]",
                                                                            $1, $3);
                                                        }
                | T_NAME T_OPENPAR T_CLOSEPAR           {
                                                            allocate1Token( $$,
                                                                            "[chamadadefuncao "
                                                                                "[T_NAME %s] "
                                                                                "[T_OPENPAR (] "
                                                                                "[T_CLOSEPAR )] "
                                                                            "]",
                                                                            $1);
                                                        }
                ;

listadenomes    : T_NAME                                {
                                                            allocate1Token($$, "[T_NAME %s]", $1);
                                                        }
                | listadenomes T_COMMA T_NAME           {
                                                            allocate2Tokens($$,"%s [T_COMMA ,] [T_NAME %s]", $1, $3);
                                                        }
                ;

listaexp        : exp                                   {
                                                            allocate1Token($$, "%s", $1);
                                                        }
                | listaexp T_COMMA exp                  {
                                                            allocate2Tokens($$, "%s [T_COMMA ,] %s", $1, $3);
                                                        }
                ;

%%

    /*** **** C Code Section **** ***/

/**
 * Print an error generated by bison.
 *
 * @param s String with error informated.
 */
void yyerror(const char *s) {
    fprintf(stderr, "\nFailled to compile this file. An error has occurred.\n");
    fprintf(stderr, "%s\n", s);
    fclose(output_file);
    free(all_tokens);
    exit(EXIT_FAILURE);
}

/**
 * Print Help Usage
 */
void showHelpUsage(){
    printf( "Help usage!\n"                                                              \
            "\n"                                                                         \
            "usage:\tcompilador [<input_file>] [<output_file>]\n"                        \
            "\t<input_file>\tFile to be compiled\n"                                      \
            "\t<output_file>\tCompiled file result\n"                                    \
            "\n"                                                                         \
            "Both <input_file> and <output_file> parameters are optional if no\n"        \
            "parameter is specified input file will be stdin and the result content\n"   \
            "will be available both in stdout and file 'stdout.out'. The default\n"      \
            "behavior when there's no <output_file> is create a file in the following\n" \
            "form: '<input_file>.out'. Code generated is printed in stdout by default.\n"\
            "\n"                                                                         \
    );
}

/* Main Execution Code */
int main(int argc, char *argv[]){
    ++argv, --argc; /* skip over program name */

#ifdef DEBUG
    /* Debug Configuration */
    extern int yydebug;
    yydebug = 1;
#endif

    /* Code Begin Message */
    printf( " ::: LUA MIPS Compiler - Sintatical and Semantic Analyser ::: \n"  \
            "Version 0.3 - Pre Alpha - LUA 5.3 Compilant\n"     \
            "Developed by Jeferson Lima and Jefferson Rene\n"   \
            "\n");

    // If you have one or more arguments and they start with --
    if((argc == 1) && (argv[0][0] == '-' && argv[0][1] == '-')){
        showHelpUsage();
        return EXIT_SUCCESS;
    }

    // Verify number of parameters
    switch(argc){
        // STDIN Input file
        case 0:
            // Input stdin
            yyin = stdin;

            // Print specified format
            printf(":: STDIN COMMAND MODE SELECTED ::\n\n");

            // The output will be a.out
            strcpy(output_filename, "stdin.out");
            break;
        // Default output filename
        case 1:
            // Input file for Flex
            yyin = fopen(argv[0], "r");

            // Print specified format
            printf( ":: SPECIFIED INPUT FILE MODE SELECTED ::\n");

            // Output filename will be input_name+.out
            strcpy(output_filename, argv[0]);
            strcpy(&output_filename[strlen(output_filename)], ".out");

            break;
        // Both parameters specified
        case 2:
            // Input file for Flex
            yyin = fopen(argv[0], "r");

            // Output filename
            strcpy(output_filename, argv[1]);

            break;
        default:
            printf( "Incorrect number of parrameters used!\n"   \
                    "\n"                                        \
            );
            showHelpUsage();

            return EXIT_FAILURE;
    }

    /* Allocate generated code header */
    char mips_header[] =    "# ### START OF GENERATED CODE ### #\n"
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
                            "# User Global Variables"
                            "%s"
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
                            "main:\n"
                                "%s"
                                "\n"
                            "# Close Main Declaration\n"
                            "close_main:\n"
                                "\tli $v0, 10\n"
                                "\tsyscall\n"
                                "\n"
                            "# ### END OF GENERATED CODE ### #\n";
    code_gen = (char *) calloc(strlen(mips_header), sizeof(char));
    strcpy(code_gen, mips_header);

    // Message of starting compilation
    printf( ":: OUTPUT FILE WILL BE %s ::\n\n"
            "::: STARTING SINTATIC AND SEMANTIC PROCESS :::\n", output_filename);

    // Output File for Flex
    output_file = fopen(output_filename, "w");

    /* Token Data Initialization */
    last_char = 0;
    all_tokens = calloc(1, sizeof(char));
    all_tokens[0] = '\0';

    /* Parse entire file */
    int yyparse_return = yyparse();
    printf("\n\n::: COMPILATION END :::\n");

    /* Free List of tokens since, it's not necessary anymore */
    free(all_tokens);

    // Process entire file
    return yyparse_return;
}
