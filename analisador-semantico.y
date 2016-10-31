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
                                                            allocateTokenText(abstract_sintatic_tree, 3, "[programa ", $1->token_str, "]\n");
                                                            
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
                                                            /* Allocate Token and Childs */
                                                            allocateTokenAndChilds(&$$, TI_BLOCO, 2, $1, $2);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[bloco", $1->token_str, " ", $2->token_str, "]");
                                                        }
                | comando_list                          { 
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_BLOCO, 1, $1);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[bloco", $1->token_str, "]");
                                                        }
                ;

/* -- List of commands -- */
comando_list    : comando_list comando                  {
                                                            /* Initialize the list with no childs  */
                                                            allocateTokenAndChilds(&$$, TI_COMANDO_LIST, 0);
                                                            
                                                            /* Allocate Text */
                                                            allocateTokenText($$, 3, $1->token_str, " ", $2->token_str);
                                                            
                                                            /* If the last type wasn't empty copy childs of this node */
                                                            if($1->token_type != TI_EMPTY){
                                                                concatenateChildTokens($$, &$1);
                                                            }
                                                            
                                                            /* Check if there are errors with the actual node */
                                                            if($$ == NULL){
                                                                fprintf(stderr, "[ERROR] FATAL ERROR ON ACTUAL NODE!\n");
                                                                return EXIT_FAILURE;
                                                            }
                                                            
                                                             /* Check if there are errors with the actual node */
                                                            if($$->child_list == NULL){
                                                                fprintf(stderr, "[ERROR] FATAL ERROR ON CHILD LIST!\n");
                                                                return EXIT_FAILURE;
                                                            }
                                                            
                                                            /* Add the last node */
                                                            listAddToken($$->child_list, $2);
                                                        }
                | /* Empty */                           {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_EMPTY, 0);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 1, "");
                                                        }
                ;

/* -- Commands belong to program, and represent all the actions that can occur -- */
comando         : T_SEMICOL                             {
                                                            /* Copy pointer of the terminal token */
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 1, "[comando [T_SEMICOL ;]]");
                                                        }
                | label                                 {
                                                            /* Copy pointer of the terminal token */
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[comando ", $1->token_str, "]");
                                                        }
                | T_BREAK                               { 
                                                            /* Copy pointer of the terminal token */
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 1, "[comando [T_BREAK break]]");
                                                        }
                | chamadadefuncao                       { 
                                                            /* Copy pointer of the terminal token */
                                                            $$ = $1;
                                                            
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[comando ", $1->token_str, "]");
                                                        }
                | listadenomes T_ASSIGN listaexp        {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_ASSIGN, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText(  $$, 5,
                                                                                "[comando "
                                                                                    "[listadenomes ", $1->token_str,"] "
                                                                                    "[T_ASSIGN =] "
                                                                                    "[listaexp ", $3->token_str,"]"
                                                                                "]");
                                                        }
                | T_DO bloco T_END                      {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_BLOCO_COMANDO, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText(  $$, 3,
                                                                                "[comando "
                                                                                    "[T_DO do] "
                                                                                    , $2->token_str, " "
                                                                                    "[T_END end]"
                                                                                "]");
                                                        }
                | T_WHILE exp T_DO bloco T_END          {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_WHILE, 4, $1, $2, $3, $4);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText(  $$, 5,
                                                                                "[comando "
                                                                                    "[T_WHILE while] "
                                                                                    , $2->token_str, " "
                                                                                    "[T_DO do] "
                                                                                    , $4->token_str, " "
                                                                                    "[T_END end]"
                                                                                "]");
                                                        }
                | T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_COMMA exp T_DO bloco T_END    {
                                                                                            /* Allocate Token and append childs */
                                                                                            allocateTokenAndChilds( &$$, TI_FOR_INC, 11,
                                                                                                                    $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11);
                                                                                            
                                                                                            /* Allocate a concatenation of token text strings */
                                                                                            allocateTokenText(  $$, 11,
                                                                                                                "[comando "
                                                                                                                    "[T_FOR for] "
                                                                                                                    "[T_NAME ", $2->lex_str, "] "
                                                                                                                    "[T_ASSIGN =] "
                                                                                                                    , $4->token_str, " "
                                                                                                                    "[T_COMMA ,] "
                                                                                                                    , $6->token_str, " "
                                                                                                                    "[T_COMMA ,] "
                                                                                                                    , $8->token_str, " "
                                                                                                                    "[T_DO do] "
                                                                                                                    , $10->token_str, " "
                                                                                                                    "[T_END end]"
                                                                                                                "]");
                                                                                        }
                | T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_DO bloco T_END                {
                                                                                            /* Allocate Token and append childs */
                                                                                            allocateTokenAndChilds( &$$, TI_FOR, 9,
                                                                                                                    $1, $2, $3, $4, $5, $6, $7, $8, $9);
                                                                                            
                                                                                            /* Allocate a concatenation of token text strings */
                                                                                            allocateTokenText(  $$, 9,
                                                                                                                "[comando "
                                                                                                                    "[T_FOR for] "
                                                                                                                    "[T_NAME ", $2->lex_str, "] "
                                                                                                                    "[T_ASSIGN =] "
                                                                                                                    , $4->token_str, " "
                                                                                                                    "[T_COMMA ,] "
                                                                                                                    , $6->token_str, " "
                                                                                                                    "[T_DO do] "
                                                                                                                    , $8->token_str, " "
                                                                                                                    "[T_END end]"
                                                                                                                "]");
                                                                                        }
                | T_IF exp T_THEN bloco term_elseif T_ELSE bloco T_END                  {
                                                                                            /* Allocate Token and append childs */
                                                                                            allocateTokenAndChilds( &$$, TI_IF_ELSE, 8,
                                                                                                                    $1, $2, $3, $4, $5, $6, $7, $8);
                                                                                                                    
                                                                                            /* Allocate a concatenation of token text strings */
                                                                                            allocateTokenText(  $$, 8,
                                                                                                                "[comando "
                                                                                                                    "[T_IF if] "
                                                                                                                    , $2->token_str, " "
                                                                                                                    "[T_THEN then] "
                                                                                                                    , $4->token_str
                                                                                                                    , $5->token_str, " "
                                                                                                                    "[T_ELSE else] "
                                                                                                                    , $7->token_str, " "
                                                                                                                    "[T_END end]"
                                                                                                                "]");
                                                                                        }
                | T_IF exp T_THEN bloco term_elseif T_END                               {
                                                                                            /* Allocate Token and append childs */
                                                                                            allocateTokenAndChilds( &$$, TI_IF, 6,
                                                                                                                    $1, $2, $3, $4, $5, $6);
                                                                                            
                                                                                            /* Allocate a concatenation of token text strings */
                                                                                            allocateTokenText(  $$, 6,
                                                                                                                "[comando "
                                                                                                                    "[T_IF if] "
                                                                                                                    , $2->token_str, " "
                                                                                                                    "[T_THEN then] "
                                                                                                                    , $4->token_str
                                                                                                                    , $5->token_str, " "
                                                                                                                    "[T_END end]"
                                                                                                                "]");
                                                                                        }
                | T_FUNCTION T_NAME T_OPENPAR listadenomes T_CLOSEPAR bloco T_END       {
                                                                                            /* Allocate Token and append childs */
                                                                                            allocateTokenAndChilds( &$$, TI_FUNCTION_PARAM, 7,
                                                                                                                    $1, $2, $3, $4, $5, $6, $7);
                                                                                            
                                                                                            /* Allocate a concatenation of token text strings */
                                                                                            allocateTokenText(  $$, 7,
                                                                                                                "[comando "
                                                                                                                    "[T_FUNCTION function] "
                                                                                                                    "[T_NAME ", $2->lex_str, "] "
                                                                                                                    "[T_OPENPAR (] "
                                                                                                                    "[listadenomes ", $4->token_str, "] "
                                                                                                                    "[T_CLOSEPAR )] "
                                                                                                                    , $6->token_str, " "
                                                                                                                    "[T_END end]"
                                                                                                                "]");
                                                                                        }
                | T_FUNCTION T_NAME T_OPENPAR T_CLOSEPAR bloco T_END                    {
                                                                                            /* Allocate Token and append childs */
                                                                                            allocateTokenAndChilds( &$$, TI_FUNCTION, 6,
                                                                                                                    $1, $2, $3, $4, $5, $6);
                                                                                                                    
                                                                                            /* Allocate a concatenation of token text strings */
                                                                                            allocateTokenText(  $$, 5,
                                                                                                                "[comando "
                                                                                                                    "[T_FUNCTION function] "
                                                                                                                    "[T_NAME ", $2->lex_str, "] "
                                                                                                                    "[T_OPENPAR (] "
                                                                                                                    "[T_CLOSEPAR )] "
                                                                                                                    , $5->token_str, " "
                                                                                                                    "[T_END end]"
                                                                                                                "]");
                                                                                        }
                | T_LOCAL listadenomes T_ASSIGN listaexp                                {
                                                                                            /* Allocate Token and append childs */
                                                                                            allocateTokenAndChilds( &$$, TI_LOCAL_ASSIGN, 4,
                                                                                                                    $1, $2, $3, $4);
                                                                                                                    
                                                                                            /* Allocate a concatenation of token text strings */
                                                                                            allocateTokenText(  $$, 5,
                                                                                                                "[comando "
                                                                                                                    "[T_LOCAL local] "
                                                                                                                    "[listadenomes ", $2->token_str, "] "
                                                                                                                    "[T_ASSIGN =] "
                                                                                                                    "[listaexp ", $4->token_str, "]"
                                                                                                                "]");
                                                                                        }
                | T_LOCAL listadenomes                                                  {
                                                                                            /* Allocate Token and append childs */
                                                                                            allocateTokenAndChilds( &$$, TI_LOCAL_DEFINE, 2,
                                                                                                                    $1, $2);
                                                                                            
                                                                                            /* Allocate a concatenation of token text strings */
                                                                                            allocateTokenText( $$, 3,
                                                                                                                "[comando "
                                                                                                                    "[T_LOCAL local] "
                                                                                                                    "[listadenomes ", $2->token_str, "]"
                                                                                                                "]");
                                                                                        }
                ;


label           : T_LABEL T_NAME T_LABEL                { 
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds( &$$, TI_LABEL, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[T_LABEL ::] [T_NAME ", $2->lex_str, "] [T_LABEL ::]");
                                                        }
                ;

term_elseif     : term_elseif T_ELSEIF exp T_THEN bloco {
                                                            /* Initialize the list with no childs  */
                                                            allocateTokenAndChilds(&$$, TI_LIST_ELSEIF, 0);
                                                            
                                                            /* If the last type wasn't empty copy childs of this node */
                                                            if($1->token_type != TI_EMPTY){
                                                                concatenateChildTokens($$, &$1);
                                                            }
                                                            
                                                            /* Check if there are errors with the actual node */
                                                            if($$ == NULL){
                                                                fprintf(stderr, "[ERROR] FATAL ERROR BUG ON CHILD LIST!\n");
                                                                return EXIT_FAILURE;
                                                            }
                                                            
                                                            /* Add the last node */
                                                            listAddToken($$->child_list, $2);
                                                            listAddToken($$->child_list, $3);
                                                            listAddToken($$->child_list, $4);
                                                            listAddToken($$->child_list, $5);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, $1->token_str, " [T_ELSEIF elseif] ", $3->token_str, " [T_THEN then] ", $5->token_str);
                                                        }
                | /* Empty */                           { 
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_EMPTY, 0);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 1, "");
                                                        }
                ;

comandoret      : T_RETURN listaexp T_SEMICOL           {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds( &$$, TI_RETURN_EXPLIST, 2, $1, $2);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText(  $$, 3,
                                                                                "[comandoret "
                                                                                    "[T_RETURN return] "
                                                                                    "[listaexp ", $2->token_str, "] "
                                                                                    "[T_SEMICOL ;]"
                                                                                "]");
                                                        }
                | T_RETURN listaexp                     {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds( &$$, TI_RETURN_EXPLIST, 2, $1, $2);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[comandoret [T_RETURN return] [listaexp ", $2->token_str, "]]");
                                                        }
                | T_RETURN T_SEMICOL                    { 
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds( &$$, TI_RETURN, 1, $1);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 1, "[comandoret [T_RETURN return] [T_SEMICOL ;]]");
                                                        }
                | T_RETURN                              {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds( &$$, TI_RETURN, 1, $1);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 1, "[comandoret [T_RETURN return]]");
                                                        }
                ;

exp             : T_NIL                                 {
                                                            /* Copy pointer of the terminal */
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 1, "[exp [T_NIL nil]]");
                                                        }
                | T_VARARG                              { 
                                                            /* Copy pointer of the terminal */
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 1, "[exp [T_VARARG ...]]");
                                                        }
                | T_NUMBER                              {
                                                            /* Copy pointer of the terminal */
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[exp [T_NUMBER ", $1->lex_str, "]]");
                                                        }
                | T_LITERAL                             {
                                                            /* Copy pointer of the terminal */
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[exp [T_LITERAL \"", $1->lex_str, "\"]]");
                                                        }
                | T_NAME                                { 
                                                            /* Copy pointer of the terminal */
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[exp [T_NAME ", $1->lex_str, "]]");
                                                        }
                | chamadadefuncao                       { 
                                                            /* Copy pointer of the terminal or last token since this token is intermediary */
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[exp ", $1->token_str, "]");
                                                        }
                | T_MINUS exp                           {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds( &$$, TI_UMINUS, 2, $1, $2);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[exp [opunaria [T_MINUS -]] ", $2->token_str, "]");
                                                        }
                | T_NOT exp                             {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds( &$$, TI_NOT, 2, $1, $2);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[exp [opunaria [T_NOT not]] ", $2->token_str, "]");                
                                                        }
                | T_BIT_N_XOR exp                       {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds( &$$, TI_BIT_NOT, 2, $1, $2);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[exp [opunaria [T_BIT_NOT ~]] ", $2->token_str, "]");
                                                        }
                | exp T_PLUS exp                        {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds( &$$, TI_PLUS, 3, $1, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_PLUS +]] ", $3->token_str, "]");
                                                        }
                | exp T_MINUS exp                       { 
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_MINUS, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_MINUS -]] ", $3->token_str, "]");
                                                        }
                | exp T_TIMES exp                       {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_TIMES, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_TIMES *]] ", $3->token_str, "]");
                                                        }
                | exp T_DIV exp                         {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_DIV, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_DIV /]] ", $3->token_str, "]");
                                                        }
                | exp T_FLOOR exp                       { 
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_FLOOR, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_FLOOR //]] ", $3->token_str, "]");
                                                        }
                | exp T_EXP exp                         {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_EXP, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_EXP ^]] ", $3->token_str, "]");
                                                        }
                | exp T_MOD exp                         { 
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_MOD, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_MOD %%]] ", $3->token_str, "]");
                                                        }
                | exp T_BIT_AND exp                     { 
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_BIT_AND, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_BIT_AND &]] ", $3->token_str, "]");
                                                        }
                | exp T_BIT_OR exp                      {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_BIT_OR, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_BIT_OR |]] ", $3->token_str, "]");
                                                        }
                | exp T_BIT_N_XOR exp                   {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_BIT_N_XOR, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_BIT_XOR ~]] ", $3->token_str, "]");
                                                        }
                | exp T_BIT_LSH exp                     {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_BIT_LSH, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_BIT_LSH <<]] ", $3->token_str, "]");
                                                        }
                | exp T_BIT_RSH exp                     {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_BIT_RSH, 3, $1, $2, $3);
                                                            
                                                            
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_BIT_RSH >>]] ", $3->token_str, "]");
                                                        }
                | exp T_CONCAT exp                      { 
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_CONCAT, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_CONCAT ..]] ", $3->token_str, "]");
                                                        }
                | exp T_LT exp                          {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_LT, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_LT <]] ", $3->token_str, "]");
                                                        }
                | exp T_LTEQ exp                        {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_LTEQ, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_LTEQ <=]] ", $3->token_str, "]");
                                                        }
                | exp T_GT exp                          {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_GT, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            // allocate2Tokens($$, "[exp %s [opbin [T_GT >]] ", $3->token_str, "]");
                                                        }
                | exp T_GTEQ exp                        {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_GTEQ, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_GTEQ >=]] ", $3->token_str, "]");
                                                        }
                | exp T_EQ exp                          { 
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_EQ, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_EQ ==]] ", $3->token_str, "]");
                                                        }
                | exp T_NEQ exp                         {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_NEQ, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_NEQ ~=]] ", $3->token_str, "]");
                                                        }
                | exp T_AND exp                         {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_AND, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_AND and]] ", $3->token_str, "]");
                                                        }
                | exp T_OR exp                          {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_OR, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 5, "[exp ", $1->token_str, " [opbin [T_OR or]] ", $3->token_str, "]");
                                                        }
                | T_OPENPAR exp T_CLOSEPAR              {
                                                            /* Copy pointer of the terminal or last token since this token is intermediary */
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[exp [T_OPENPAR (] ", $2->token_str, " [T_CLOSEPAR )]]");
                                                        }
                /*
                | T_TRUE                                { 
                                                            /* Copy pointer of the terminal or last token since this token is intermediary * /
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings * /
                                                            allocateTokenText($$, 1, "[exp [T_TRUE true]]");
                                                        }
                | T_FALSE                               {
                                                            /* Copy pointer of the terminal or last token since this token is intermediary * /
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings * /
                                                            allocateTokenText($$, 1, "[exp [T_FALSE false]]");
                                                        }
                */
                ;

chamadadefuncao : T_NAME T_OPENPAR listaexp T_CLOSEPAR  {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_CALL_FUNCTION_PAR, 4, $1, $2, $3, $4);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText(  $$, 5,
                                                                                "[chamadadefuncao "
                                                                                    "[T_NAME ", $1->lex_str, "] "
                                                                                    "[T_OPENPAR (] "
                                                                                    "[listaexp ", $3->token_str, "] "
                                                                                    "[T_CLOSEPAR )] "
                                                                                "]");
                                                        }
                | T_NAME T_OPENPAR T_CLOSEPAR           {
                                                            /* Allocate Token and append childs */
                                                            allocateTokenAndChilds(&$$, TI_CALL_FUNCTION, 3, $1, $2, $3);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText(  $$, 3,
                                                                                "[chamadadefuncao "
                                                                                    "[T_NAME ", $1->lex_str, "] "
                                                                                    "[T_OPENPAR (] "
                                                                                    "[T_CLOSEPAR )] "
                                                                                "]");
                                                        }
                ;

listadenomes    : T_NAME                                {
                                                            /* Copy pointer of the terminal */
                                                            $$ = $1;
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, "[T_NAME ", $1->lex_str, "]");
                                                        }
                | listadenomes T_COMMA T_NAME           {
                                                            /* Initialize the list with no childs  */
                                                            allocateTokenAndChilds(&$$, TI_LISTADENOMES, 0);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 4, $1->token_str, " [T_COMMA ,] [T_NAME ", $3->lex_str, "]");
                                                            
                                                            /* Concatenate the first elements with the actual node */
                                                            concatenateChildTokens($$, &$1);
                                                            
                                                            /* Check if there are errors with the actual node */
                                                            if($$ == NULL){
                                                                fprintf(stderr, "[ERROR] FATAL ERROR BUG ON CHILD LIST!\n");
                                                                return EXIT_FAILURE;
                                                            }
                                                            
                                                            /* Add the last nodes */
                                                            listAddToken($$->child_list, $2);
                                                            listAddToken($$->child_list, $3);
                                                        }
                ;

listaexp        : exp                                   {
                                                            /* Copy pointer of the terminal or last token since this token is intermediary */
                                                            $$ = $1;
                                                        }
                | listaexp T_COMMA exp                  {
                                                            /* Initialize the list with no childs  */
                                                            allocateTokenAndChilds(&$$, TI_LISTAEXP, 0);
                                                            
                                                            /* Allocate a concatenation of token text strings */
                                                            allocateTokenText($$, 3, $1->token_str, " [T_COMMA ,] ", $3->token_str);
                                                            
                                                            /* Concatenate the first elements with the actual node */
                                                            concatenateChildTokens($$, &$1);
                                                            
                                                            /* Check if there are errors with the actual node */
                                                            if($$ == NULL){
                                                                fprintf(stderr, "[ERROR] FATAL ERROR BUG ON CHILD LIST!\n");
                                                                return EXIT_FAILURE;
                                                            }
                                                            
                                                            /* Add the last nodes */
                                                            listAddToken($$->child_list, $2);
                                                            listAddToken($$->child_list, $3);
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
