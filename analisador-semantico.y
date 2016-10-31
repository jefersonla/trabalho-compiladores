%{
/** **** Analisador  Semantico **** **/
/** Desenvolvido por Jeferson Lima  **/
/**              e   Jefferson Renê **/
/** Versão      0.4                 **/
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

/* Utils */
#include "utils.h"

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

/* Master Root Token AST */
TokenNode *abstract_sintatic_tree;

/* Generated Code */
InstructionQueue *instruction_queue;

/* Global Symbol Table */
SymbolTable *global_symbol_table;

%}

/* Definition of token iterator */
%union{
    /* Pointer to a token node */
    TokenNode *token_node;
}

    /*** * Definitions Section * ***/

/* Boolean Comparators */

%token <token_node> T_AND
%token <token_node> T_OR
%token <token_node> T_NOT

/* Symbols */

/* Math Operators */
%token <token_node> T_PLUS
%token <token_node> T_MINUS
%token <token_node> T_TIMES
%token <token_node> T_DIV
%token <token_node> T_MOD
%token <token_node> T_EXP
%token <token_node> T_FLOOR
/* Boolean Operators */
%token <token_node> T_EQ
%token <token_node> T_NEQ
%token <token_node> T_LTEQ
%token <token_node> T_GTEQ
%token <token_node> T_LT
%token <token_node> T_GT
/* Separators and assign */
%token <token_node> T_COMMA
%token <token_node> T_SEMICOL
%token <token_node> T_ASSIGN
/* Bit-a-Bit Operators */
%token <token_node> T_BIT_AND
%token <token_node> T_BIT_OR
%token <token_node> T_BIT_N_XOR
%token <token_node> T_BIT_RSH
%token <token_node> T_BIT_LSH
/* Encapsulation Symbols */
%token <token_node> T_OPENPAR
%token <token_node> T_CLOSEPAR
%token <token_node> T_OPENBRACE
%token <token_node> T_CLOSEBRACE
%token <token_node> T_OPENBRACKET
%token <token_node> T_CLOSEBRACKET
/* Other Symbols */
%token <token_node> T_CONCAT
%token <token_node> T_VARARG
%token <token_node> T_SEP
%token <token_node> T_COLON
%token <token_node> T_LABEL

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
                                                            /* Initialize Global AST */  
                                                            allocateTokenAndChilds(&abstract_sintatic_tree, TI_PROGRAMA, 1, $1);
                                                            
                                                            /* Add NULL pointer to father */
                                                            nodeAddRootToken(abstract_sintatic_tree, NULL);
                                                            
                                                            /* Add Textual content */
                                                            //...
                                                            
                                                            #ifdef SEMANTIC_ANALYSER
                                                            /* Store output file result */
                                                            fprintf(output_file, "[programa %s]\n", $1->token_str);
                                                            
                                                            /* Print Finished Result */
                                                            fprintf(stderr,
                                                                    "\n::: SYNTATIC/SEMANTIC ANALYSER :::\n"
                                                                    "[programa %s]\n\n"
                                                                    "::: LEXICAL PARSER :::\n"
                                                                    "%s",
                                                                    $1->token_str, all_tokens);
                                                            #else 
                                                            /* Store output file result */
                                                            instructionQueueFilePrint(output_file, instruction_queue);
                                                            
                                                            /* Print Finished Result */
                                                            fprintf(stderr, "\n::: CODE GENERATOR :::\n");
                                                            
                                                            /* Print Instruction Queue */
                                                            instructionQueueFilePrint(stderr, instruction_queue);
                                                            
                                                            /* Print other structures */
                                                            fprintf(stderr, 
                                                                    "::: SYNTATIC/SEMANTIC ANALYSER :::\n"
                                                                    "[programa %s]\n\n"
                                                                    "::: LEXICAL PARSER :::\n"
                                                                    "%s",
                                                                    $1->token_str, all_tokens);
                                                            #endif
                                                            
                                                            /* Close Output File */
                                                            fclose(output_file);
                                                        }
                ;

/* -- Block are sections of code -- */
bloco           : comando_list comandoret               { 
                                                            allocateTokenAndChilds(&$$, TI_BLOCO, 2, $1, $2);
                                                            //allocate2Tokens($$, "[bloco%s %s]", $1, $2);
                                                        }
                | comando_list                          { 
                                                            allocateTokenAndChilds(&$$, TI_BLOCO, 1, $1);
                                                            //allocate1Token($$, "[bloco%s]", $1);
                                                        }
                ;

/* -- List of commands -- */
comando_list    : comando_list comando                  {
                                                            /* Initialize the list with no childs  */
                                                            allocateTokenAndChilds(&$$, TI_COMANDO_LIST, 0);
                                                            
                                                            /* If the last type wasn't empty copy childs of this node */
                                                            if($1->token_type != TI_EMPTY){
                                                                concatenateChildTokens($$, &$1);
                                                            }
                                                            
                                                            /* Add the last node */
                                                            listAddToken($$->child_list, $2);
                                                            //allocate2Tokens($$, "%s %s", $1, $2);
                                                        }
                | /* Empty */                           { 
                                                            allocateTokenAndChilds(&$$, TI_EMPTY, 0);
                                                            //allocateToken($$, "");
                                                        }
                ;

/* -- Commands belong to program, and represent all the actions that can occur -- */
comando         : T_SEMICOL                             {
                                                            $$ = $1;
                                                            //allocateToken($$, "[comando [T_SEMICOL ;]]");
                                                        }
                | label                                 {
                                                            $$ = $1;
                                                            //allocate1Token($$, "[comando %s]", $1);
                                                        }
                | T_BREAK                               { 
                                                            $$ = $1;
                                                            //allocateToken($$, "[comando [T_BREAK break]]");
                                                        }
                | chamadadefuncao                       { 
                                                            $$ = $1;
                                                            //allocate1Token($$, "[comando %s]", $1);
                                                        }
                | listadenomes T_ASSIGN listaexp        {
                                                            allocateTokenAndChilds(&$$, TI_ASSIGN, 3, $1, $2, $3);
                                                            //allocate2Tokens($$,
                                                            //                "[comando "
                                                            //                    "[listadenomes %s] "
                                                            //                    "[T_ASSIGN =] "
                                                            //                    "[listaexp %s]"
                                                            //                "]",
                                                            //                $1, $3);
                                                        }
                | T_DO bloco T_END                      {
                                                            allocateTokenAndChilds(&$$, TI_BLOCO_COMANDO, 3, $1, $2, $3);
                                                            //allocate1Token( $$,
                                                            //                "[comando "
                                                            //                    "[T_DO do] "
                                                            //                    "%s "
                                                            //                    "[T_END end]"
                                                            //                "]",
                                                            //                $2);
                                                        }
                | T_WHILE exp T_DO bloco T_END          {
                                                            allocateTokenAndChilds(&$$, TI_WHILE, 4, $1, $2, $3, $4);
                                                            //allocate2Tokens($$,
                                                            //                "[comando "
                                                            //                    "[T_WHILE while] "
                                                            //                    "%s "
                                                            //                    "[T_DO do] "
                                                            //                    "%s "
                                                            //                    "[T_END end]"
                                                            //                "]",
                                                            //                $2, $4);
                                                        }
                | T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_COMMA exp T_DO bloco T_END    {
                                                                                            allocateTokenAndChilds( &$$, TI_FOR_INC, 11,
                                                                                                                    $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11);
                                                                                            //allocate5Tokens($$,
                                                                                            //                "[comando "
                                                                                            //                    "[T_FOR for] "
                                                                                            //                    "[T_NAME %s] "
                                                                                            //                    "[T_ASSIGN =] "
                                                                                            //                    "%s "
                                                                                            //                    "[T_COMMA ,] "
                                                                                            //                    "%s "
                                                                                            //                    "[T_COMMA ,] "
                                                                                            //                    "%s "
                                                                                            //                    "[T_DO do] "
                                                                                            //                    "%s "
                                                                                            //                    "[T_END end]"
                                                                                            //                "]",
                                                                                            //                $2, $4, $6, $8, $1);
                                                                                        }
                | T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_DO bloco T_END                {
                                                                                            allocateTokenAndChilds( &$$, TI_FOR, 9,
                                                                                                                    $1, $2, $3, $4, $5, $6, $7, $8, $9);
                                                                                            //allocate4Tokens($$,
                                                                                            //                "[comando "
                                                                                            //                    "[T_FOR for] "
                                                                                            //                    "[T_NAME %s] "
                                                                                            //                    "[T_ASSIGN =] "
                                                                                            //                    "%s "
                                                                                            //                    "[T_COMMA ,] "
                                                                                            //                    "%s "
                                                                                            //                    "[T_DO do] "
                                                                                            //                    "%s "
                                                                                            //                    "[T_END end]"
                                                                                            //                "]",
                                                                                            //                $2, $4, $6, $8);
                                                                                        }
                | T_IF exp T_THEN bloco term_elseif T_ELSE bloco T_END                  {
                                                                                            allocateTokenAndChilds( &$$, TI_IF_ELSE, 8,
                                                                                                                    $1, $2, $3, $4, $5, $6, $7, $8);
                                                                                            //allocate4Tokens($$,
                                                                                            //                "[comando "
                                                                                            //                    "[T_IF if] "
                                                                                            //                    "%s "
                                                                                            //                    "[T_THEN then] "
                                                                                            //                    "%s"
                                                                                            //                    "%s "
                                                                                            //                    "[T_ELSE else] "
                                                                                            //                    "%s "
                                                                                            //                    "[T_END end]"
                                                                                            //                "]",
                                                                                            //                $2, $4, $5, $7);
                                                                                        }
                | T_IF exp T_THEN bloco term_elseif T_END                               {
                                                                                            allocateTokenAndChilds( &$$, TI_IF, 6,
                                                                                                                    $1, $2, $3, $4, $5, $6);
                                                                                            //allocate3Tokens($$,
                                                                                            //                "[comando "
                                                                                            //                    "[T_IF if] "
                                                                                            //                    "%s "
                                                                                            //                    "[T_THEN then] "
                                                                                            //                    "%s"
                                                                                            //                    "%s "
                                                                                            //                    "[T_END end]"
                                                                                            //                "]",
                                                                                            //                $2, $4, $5);
                                                                                        }
                | T_FUNCTION T_NAME T_OPENPAR listadenomes T_CLOSEPAR bloco T_END       {
                                                                                            allocateTokenAndChilds( &$$, TI_FUNCTION_PARAM, 7,
                                                                                                                    $1, $2, $3, $4, $5, $6, $7);
                                                                                            //allocate3Tokens($$,
                                                                                            //                "[comando "
                                                                                            //                    "[T_FUNCTION function] "
                                                                                            //                    "[T_NAME %s] "
                                                                                            //                    "[T_OPENPAR (] "
                                                                                            //                    "[listadenomes %s] "
                                                                                            //                    "[T_CLOSEPAR )] "
                                                                                            //                    "%s "
                                                                                            //                    "[T_END end]"
                                                                                            //                "]",
                                                                                            //                $2, $4, $6);
                                                                                        }
                | T_FUNCTION T_NAME T_OPENPAR T_CLOSEPAR bloco T_END                    {
                                                                                            allocateTokenAndChilds( &$$, TI_FUNCTION, 6,
                                                                                                                    $1, $2, $3, $4, $5, $6);
                                                                                            //allocate2Tokens($$,
                                                                                            //                "[comando "
                                                                                            //                    "[T_FUNCTION function] "
                                                                                            //                    "[T_NAME %s] "
                                                                                            //                    "[T_OPENPAR (] "
                                                                                            //                    "[T_CLOSEPAR )] "
                                                                                            //                    "%s "
                                                                                            //                    "[T_END end]"
                                                                                            //                "]",
                                                                                            //                $2, $5);
                                                                                        }
                | T_LOCAL listadenomes T_ASSIGN listaexp                                {
                                                                                            allocateTokenAndChilds( &$$, TI_LOCAL_ASSIGN, 4,
                                                                                                                    $1, $2, $3, $4);
                                                                                            //allocate2Tokens($$,
                                                                                            //                "[comando "
                                                                                            //                    "[T_LOCAL local] "
                                                                                            //                    "[listadenomes %s] "
                                                                                            //                    "[T_ASSIGN =] "
                                                                                            //                    "[listaexp %s]"
                                                                                            //                "]",
                                                                                            //                $2, $4);
                                                                                        }
                | T_LOCAL listadenomes                                                  {
                                                                                            allocateTokenAndChilds( &$$, TI_LOCAL_DEFINE, 2,
                                                                                                                    $1, $2);
                                                                                            //allocate1Token( $$,
                                                                                            //                "[comando "
                                                                                            //                    "[T_LOCAL local] "
                                                                                            //                    "[listadenomes %s]"
                                                                                            //                "]",
                                                                                            //                $2);
                                                                                        }
                ;


label           : T_LABEL T_NAME T_LABEL                { 
                                                            allocateTokenAndChilds( &$$, TI_LABEL, 3, $1, $2, $3);
                                                            //allocate1Token($$, "[T_LABEL ::] [T_NAME %s] [T_LABEL ::]", $2);
                                                        }

term_elseif     : term_elseif T_ELSEIF exp T_THEN bloco {
                                                            /* Initialize the list with no childs  */
                                                            allocateTokenAndChilds(&$$, TI_LIST_ELSEIF, 0);
                                                            
                                                            /* If the last type wasn't empty copy childs of this node */
                                                            if($1->token_type != TI_EMPTY){
                                                                concatenateChildTokens($$, &$1);
                                                            }
                                                            
                                                            /* Add the last node */
                                                            listAddToken($$->child_list, $2);
                                                            listAddToken($$->child_list, $3);
                                                            listAddToken($$->child_list, $4);
                                                            listAddToken($$->child_list, $5);
                                                            
                                                            //llocate3Tokens($$,"%s [T_ELSEIF elseif] %s [T_THEN then] %s", $1, $3, $5);
                                                        }
                | /* Empty */                           { 
                                                            allocateTokenAndChilds(&$$, TI_EMPTY, 0);
                                                            //allocateToken($$, "");
                                                        }
                ;

comandoret      : T_RETURN listaexp T_SEMICOL           {
                                                            allocateTokenAndChilds( &$$, TI_RETURN_EXPLIST, 2, $1, $2);
                                                            //allocate1Token( $$,
                                                            //                "[comandoret "
                                                            //                    "[T_RETURN return] "
                                                            //                    "[listaexp %s] "
                                                            //                    "[T_SEMICOL ;]"
                                                            //                "]",
                                                            //                $2);
                                                        }
                | T_RETURN listaexp                     {
                                                            allocateTokenAndChilds( &$$, TI_RETURN_EXPLIST, 2, $1, $2);
                                                            //allocate1Token($$, "[comandoret [T_RETURN return] [listaexp %s]]", $2);   
                                                        }
                | T_RETURN T_SEMICOL                    { 
                                                            allocateTokenAndChilds( &$$, TI_RETURN, 1, $1);
                                                            //allocateToken($$, "[comandoret [T_RETURN return] [T_SEMICOL ;]]");
                                                        }
                | T_RETURN                              {
                                                            allocateTokenAndChilds( &$$, TI_RETURN, 1, $1);
                                                            //allocateToken($$, "[comandoret [T_RETURN return]]");
                                                        }
                ;

exp             : T_NIL                                 {
                                                            $$ = $1;
                                                            //allocateToken($$, "[exp [T_NIL nil]]");
                                                        }
                | T_VARARG                              { 
                                                            $$ = $1;
                                                            //allocateToken($$, "[exp [T_VARARG ...]]");
                                                        }
                | T_NUMBER                              {
                                                            $$ = $1;
                                                            //allocate1Token($$, "[exp [T_NUMBER %s]]", $1);
                                                        }
                | T_LITERAL                             {
                                                            $$ = $1;
                                                            //allocate1Token($$, "[exp [T_LITERAL %s]]", $1);
                                                        }
                | T_NAME                                { 
                                                            $$ = $1;
                                                            //allocate1Token($$, "[exp [T_NAME %s]]", $1);
                                                        }
                | chamadadefuncao                       { 
                                                            $$ = $1;
                                                            //allocate1Token($$, "[exp %s]", $1);
                                                        }
                | T_MINUS exp                           {
                                                            allocateTokenAndChilds( &$$, TI_UMINUS, 2, $1, $2);
                                                            //allocate1Token($$, "[exp [opunaria [T_MINUS -]] %s]", $2);
                                                        }
                | T_NOT exp                             {
                                                            allocateTokenAndChilds( &$$, TI_NOT, 2, $1, $2);
                                                            //allocate1Token($$, "[exp [opunaria [T_NOT not]] %s]", $2);                
                                                        }
                | T_BIT_N_XOR exp                       {
                                                            allocateTokenAndChilds( &$$, TI_BIT_NOT, 2, $1, $2);
                                                            //allocate1Token($$, "[exp [opunaria [T_BIT_NOT ~]] %s]", $2);
                                                        }
                | exp T_PLUS exp                        {
                                                            allocateTokenAndChilds( &$$, TI_PLUS, 3, $1, $3);
                                                            //allocate2Tokens($$,  &I$$"[exp, 3, $1, $2, $3 %s [opbin [T_PLUS +]] %s]", $1, $3);
                                                        }
                | exp T_MINUS exp                       { 
                                                            allocateTokenAndChilds(&$$, TI_MINUS, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_MINUS -]] %s]", $1, $3);
                                                        }
                | exp T_TIMES exp                       {
                                                            allocateTokenAndChilds(&$$, TI_TIMES, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_TIMES *]] %s]", $1, $3);
                                                        }
                | exp T_DIV exp                         {
                                                            allocateTokenAndChilds(&$$, TI_DIV, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_DIV /]] %s]", $1, $3);
                                                        }
                | exp T_FLOOR exp                       { 
                                                            allocateTokenAndChilds(&$$, TI_FLOOR, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_FLOOR //]] %s]", $1, $3);
                                                        }
                | exp T_EXP exp                         {
                                                            allocateTokenAndChilds(&$$, TI_EXP, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_EXP ^]] %s]", $1, $3);
                                                        }
                | exp T_MOD exp                         { 
                                                            allocateTokenAndChilds(&$$, TI_MOD, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_MOD %%]] %s]", $1, $3);
                                                        }
                | exp T_BIT_AND exp                     { 
                                                            allocateTokenAndChilds(&$$, TI_BIT_AND, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_BIT_AND &]] %s]", $1, $3);
                                                        }
                | exp T_BIT_OR exp                      {
                                                            allocateTokenAndChilds(&$$, TI_BIT_OR, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_BIT_OR |]] %s]", $1, $3);
                                                        }
                | exp T_BIT_N_XOR exp                   {
                                                            allocateTokenAndChilds(&$$, TI_BIT_N_XOR, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_BIT_XOR ~]] %s]", $1, $3);
                                                        }
                | exp T_BIT_LSH exp                     {
                                                            allocateTokenAndChilds(&$$, TI_BIT_LSH, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_BIT_LSH <<]] %s]", $1, $3);
                                                        }
                | exp T_BIT_RSH exp                     {
                                                            allocateTokenAndChilds(&$$, TI_BIT_RSH, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_BIT_RSH >>]] %s]", $1, $3);
                                                        }
                | exp T_CONCAT exp                      { 
                                                            allocateTokenAndChilds(&$$, TI_CONCAT, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_CONCAT ..]] %s]", $1, $3);
                                                        }
                | exp T_LT exp                          {
                                                            allocateTokenAndChilds(&$$, TI_LT, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_LT <]] %s]", $1, $3);
                                                        }
                | exp T_LTEQ exp                        {
                                                            allocateTokenAndChilds(&$$, TI_LTEQ, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_LTEQ <=]] %s]", $1, $3);
                                                        }
                | exp T_GT exp                          {
                                                            allocateTokenAndChilds(&$$, TI_GT, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_GT >]] %s]", $1, $3);
                                                        }
                | exp T_GTEQ exp                        {
                                                            allocateTokenAndChilds(&$$, TI_GTEQ, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_GTEQ >=]] %s]", $1, $3);
                                                        }
                | exp T_EQ exp                          { 
                                                            allocateTokenAndChilds(&$$, TI_EQ, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_EQ ==]] %s]", $1, $3);
                                                        }
                | exp T_NEQ exp                         {
                                                            allocateTokenAndChilds(&$$, TI_NEQ, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_NEQ ~=]] %s]", $1, $3);
                                                        }
                | exp T_AND exp                         {
                                                            allocateTokenAndChilds(&$$, TI_AND, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_AND and]] %s]", $1, $3);
                                                        }
                | exp T_OR exp                          {
                                                            allocateTokenAndChilds(&$$, TI_OR, 3, $1, $2, $3);
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_OR or]] %s]", $1, $3);
                                                        }
                | T_OPENPAR exp T_CLOSEPAR              {
                                                            $$ = $1;
                                                            //allocate1Token($$, "[exp [T_OPENPAR (] %s [T_CLOSEPAR )]]", $2);
                                                        }
                /*
                | T_TRUE                                { 
                                                            allocateToken($$, "[exp [T_TRUE true]]");
                                                        }
                */
                /*
                | T_FALSE                               {
                                                            allocateToken($$, "[exp [T_FALSE false]]");
                                                        }
                */
                ;

chamadadefuncao : T_NAME T_OPENPAR listaexp T_CLOSEPAR  {
                                                            allocateTokenAndChilds(&$$, TI_CALL_FUNCTION_PAR, 4, $1, $2, $3, $4);
                                                            //allocate2Tokens($$,
                                                            //                "[chamadadefuncao "
                                                            //                    "[T_NAME %s] "
                                                            //                    "[T_OPENPAR (] "
                                                            //                    "[listaexp %s] "
                                                            //                    "[T_CLOSEPAR )] "
                                                            //                "]",
                                                            //                $1, $3);
                                                        }
                | T_NAME T_OPENPAR T_CLOSEPAR           {
                                                            allocateTokenAndChilds(&$$, TI_CALL_FUNCTION, 3, $1, $2, $3);
                                                            //allocate1Token( $$,
                                                            //                "[chamadadefuncao "
                                                            //                    "[T_NAME %s] "
                                                            //                    "[T_OPENPAR (] "
                                                            //                    "[T_CLOSEPAR )] "
                                                            //                "]",
                                                            //                $1);
                                                        }
                ;

listadenomes    : T_NAME                                {
                                                            $$ = $1;
                                                            //allocate1Token($$, "[T_NAME %s]", $1);
                                                        }
                | listadenomes T_COMMA T_NAME           {
                                                            /* Initialize the list with no childs  */
                                                            allocateTokenAndChilds(&$$, TI_LISTADENOMES, 0);
                                                            
                                                            /* Concatenate the first elements with the actual node */
                                                            concatenateChildTokens($$, &$1);
                                                            
                                                            /* Add the last nodes */
                                                            listAddToken($$->child_list, $2);
                                                            listAddToken($$->child_list, $3);
                                                            
                                                            //allocate2Tokens($$,"%s [T_COMMA ,] [T_NAME %s]", $1, $3);
                                                        }
                ;

listaexp        : exp                                   {
                                                            $$ = $1;
                                                            //allocate1Token($$, "%s", $1);
                                                        }
                | listaexp T_COMMA exp                  {
                                                            /* Initialize the list with no childs  */
                                                            allocateTokenAndChilds(&$$, TI_LISTAEXP, 0);
                                                            
                                                            /* Concatenate the first elements with the actual node */
                                                            concatenateChildTokens($$, &$1);
                                                            
                                                            /* Add the last nodes */
                                                            listAddToken($$->child_list, $2);
                                                            listAddToken($$->child_list, $3);
                                                            
                                                            //allocate2Tokens($$, "%s [T_COMMA ,] %s", $1, $3);
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
            "Version 0.4 - Pre Alpha - LUA SIMPLE UFBA Compilant\n"             \
            "Developed by Jeferson Lima and Jefferson Rene\n"                   \
            "\n");

    /* If you have one or more arguments and they start with -- */
    if( (argc == 1) && 
            (((strlen(argv[0]) == 1) && (argv[0][0] == '-')) ||
                ((strlen(argv[0]) > 2) && (argv[0][1] == '-')))){
        showHelpUsage();
        return EXIT_SUCCESS;
    }

    /* Verify number of parameters */
    switch(argc){
        /* STDIN Input file */
        case 0:
            /* Input stdin */
            yyin = stdin;

            /* Print specified format */
            printf(":: STDIN COMMAND MODE SELECTED ::\n\n");

            /* The output will be a.out*/
            strcpy(output_filename, "stdin.out");
            break;
        /* Default output filename */
        case 1:
            /* Input file for Flex */
            yyin = fopen(argv[0], "r");

            /* Print specified format */
            printf( ":: SPECIFIED INPUT FILE MODE SELECTED ::\n");

            /* Output filename will be input_name+.out */
            strcpy(output_filename, argv[0]);
            strcpy(&output_filename[strlen(output_filename)], ".out");

            break;
        /* Both parameters specified */
        case 2:
            /* Input file for Flex */
            yyin = fopen(argv[0], "r");

            /* Output filename */
            strcpy(output_filename, argv[1]);

            break;
        default:
            printf( "Incorrect number of parrameters used!\n"   \
                    "\n"                                        \
            );
            showHelpUsage();

            return EXIT_FAILURE;
    }
    
    /* Check if fopen has succeed */
    if(yyin == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT OPEN FILE!\n");
        return EXIT_FAILURE;
    }
    
    /* Check if we can write */
    if(output_filename == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNOT WRITE TO FILE!\n");
        return EXIT_FAILURE;
    }

    /* Message of starting compilation */
    printf( ":: OUTPUT FILE WILL BE %s ::\n\n"
            "::: STARTING SINTATIC AND SEMANTIC PROCESS :::\n", output_filename);

    /* Output File for Flex */
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

    /* Process entire file */
    return yyparse_return;
}
