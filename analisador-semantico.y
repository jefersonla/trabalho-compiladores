%{
/** **** Analisador  Semantico **** **/
/** Desenvolvido por Jeferson Lima  **/
/**              e   Jefferson Renê **/
/** Versão      1.0                 **/
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

/* Definições do Flex */
#include "lex.yy.h"

/* Definições dos tipos */
#include "parser.defs.h"

/* Needed by bison */
void yyerror(char *);

/* Variables for File Manipulation */
char output_filename[MAX_OUTPUT_FILENAME];
FILE *output_file;

/* Contain actual token */
char *string_addr;

%}

/* Definition of token iterator */
%union{
	/* Number Value */
	int integer_number;
	float float_number;
	/* Literal Value */
	char *string_literal;
	/* Label Definition */
	char *string_label;
	/* Variable Name */
	char *string_variable_name;
	/* Pointer to a token node */
	// TokenNode *ptr_token_node;
}

    /*** * Definitions Section * ***/

/* Symbols */

/* Math Operators */
%token T_PLUS
%token T_MINUS
%token T_TIMES
%token T_DIV
%token T_MOD
%token T_EXP
%token T_FLOOR
/* -- Precendence -- */
%left T_TIMES T_DIV
%left T_PLUS T_MINUS
/* Boolean Operators */
%token T_EQ
%token T_NEQ
%token T_LTEQ
%token T_GTEQ
%token T_LT
%token T_GT
/* -- Precendence -- */
%left T_EQ T_NEQ T_LTEQ T_GTEQ T_LT T_GT
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
%token T_LIST
%token T_SEP
%token T_COLON

/* Reserved Words */

%token T_AND
%token T_DO
%token T_ELSE
%token T_ELSEIF
%token T_END
%token T_FOR
%token T_FUNCTION
%token T_IF
%token T_LOCAL
%token T_NIL
%token T_NOT
%token T_OR
%token T_RETURN
%token T_THEN
%token T_WHILE
/* -- Precendence -- */
%left T_AND T_OR

/* Extra Reserved Words */

%token T_BREAK
%token T_GLOBAL
%token T_IN
%token T_REPEAT
%token T_TRUE
%token T_FALSE
%token T_UNTIL

/* Variable Types */

%token T_NUMBER
%token T_LABEL
%token T_NAME
%token T_LITERAL

/* Start Type */
%start programa

/*
 -- TOKEN dá um tipo ao TOKEN
%token 	<integer_number> 		number
%token 	<string_variable_name> 	identifier
 -- TYPE dá um tipo a EXPRESSÃO
%type 	<integer_number> 		line exp term
%type 	<string_variable_name> 	assingment
*/

%%

    /*** * Language Definitions * ***/

programa 	: bloco					        {;}
			;

bloco 		: comando comandoret            {;}
            | comando                       {;}
            | /* Empty */                   {;}
            ;

comando     : comando comando               {;}
            | T_COMMA                           {;}
            | listadenomes T_ASSIGN listaexp     {;}
            | chamadadefuncao               {;}
            | T_DO bloco T_END              {;}
            | T_WHILE exp T_DO bloco T_END  {;}
            | T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_COMMA exp T_DO bloco T_END     {;}
            | T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_DO bloco T_END             {;}
            | T_IF exp T_THEN bloco term_elseif T_ELSE bloco T_END      {;}
            | T_IF exp T_THEN bloco term_elseif T_END                   {;}
            | T_FUNCTION T_NAME T_OPENPAR listadenomes T_CLOSEPAR bloco T_END        {;}
            | T_FUNCTION T_NAME T_OPENPAR T_CLOSEPAR bloco T_END                     {;}
            | T_LOCAL listadenomes T_ASSIGN listaexp                         {;}
            | T_LOCAL listadenomes          {;}
            ;

term_elseif : /* Empty */                           {;}
            | T_ELSEIF exp T_THEN bloco term_elseif {;}
            ;

comandoret  : T_RETURN listaexp T_SEMICOL   {;}
            | T_RETURN listaexp             {;}
            | T_RETURN T_SEMICOL            {;}
            | T_RETURN                      {;}
            ;

exp         : T_NUMBER                      {;}
            | T_NAME                        {;}
            | T_NIL                         {;}
            | chamadadefuncao               {;}
            | exp opbin exp                 {;}
            | opunaria exp                  {;}
            | T_OPENPAR exp T_CLOSEPAR      {;}
            ;

chamadadefuncao : T_NAME T_OPENPAR listaexp T_CLOSEPAR	{;}
                | T_NAME T_OPENPAR T_CLOSEPAR           {;}
                ;

listadenomes    : T_NAME listanomes         {;}
                ;

listanomes      : T_COMMA T_NAME listanomes     {;}
                |  /* Empty */              	{;}
                ;

listaexp        : exp lista_expr            	{;}
                ;

lista_expr      : T_COMMA exp lista_expr        {;}
                |  /* Empty */              	{;}
                ;

opbin 		: T_PLUS                        {;}
		| T_MINUS                       {;}
		| T_TIMES                       {;}
		| T_DIV                         {;}
		| T_LT                          {;}
		| T_LTEQ                        {;}
		| T_GT                          {;}
		| T_GTEQ                        {;}
		| T_EQ                          {;}
		| T_NEQ                         {;}
		| T_AND                         {;}
		| T_OR                          {;}
		;

opunaria 	: T_MINUS                       {;}
		| T_NOT                         {;}
		;

%%

    /*** **** C Code Section **** ***/

/**
 * Print an error generated by bison.
 * @param s String with error informated.
 */
void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

/* Main Execution Code */
int main(int argc, char *argv[]){
    ++argv, --argc; /* skip over program name */

    /* Code Begin Message */
	printf( " ::: LUA MIPS Compiler - Sintatical and Semantic Analyser ::: \n"  \
            "Version 0.2 - Pre Alpha - LUA 5.3 Compilant\n"     \
            "Developed by Jeferson Lima and Jefferson Rene\n"   \
            "\n");

    // If you have one or more arguments and they start with --
    if((argc == 1) && (argv[0][0] == '-' && argv[0][1] == '-')){
        printf( "Help usage!\n"                                                             \
                "\n"                                                                        \
                "usage:\tcompilador [<input_file>] [<output_file>]\n"                       \
                "\t<input_file>\tFile to be compiled\n"                                     \
                "\t<output_file>\tCompiled file result\n"                                   \
                "\n"                                                                        \
                "Both <input_file> and <output_file> parameters are optional if no\n"       \
                "parameter is specified input file will be stdin and the result content\n"  \
                "will be available both in stdout and file 'stdout.out'. The default\n"     \
                "behavior when there's no <output_file> is create a file in the following\n"\
                "form: '<input_file>.out'. Code generated is printed in stdout by default.\n\n");

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
            printf( ":: SPECIFIED INPUT FILE MODE SELECTED ::\n"
                    ":: OUTPUT FILE WILL BE %s.out ::", argv[0]);

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
            printf( "Incorrect number of parrameters used!\n"                                   \
                    "\n"                                                                        \
                    "usage:\tcompilador [<input_file>] [<output_file>]\n"                       \
                    "\t<input_file>\tFile to be compiled\n"                                     \
                    "\t<output_file>\tCompiled file result\n"                                   \
                    "\n"                                                                        \
                    "Both <input_file> and <output_file> parameters are optional if no\n"       \
                    "parameter is specified input file will be stdin and the result content\n"  \
                    "will be availabyyerror_le both in stdout and file 'stdout.out'. The default\n"     \
                    "behavior when there's no <output_file> is create a file in the following\n"\
                    "form: '<input_file>.out'. Code generated is printed in stdout by default.\n\n");

			return EXIT_FAILURE;
	}

    // Message of starting compilation
    printf("::: STARTING LEX PARSER PROCESS :::\n\n");

	// Output File for Flex
	output_file = fopen(output_filename, "w");

    // Process entire file
    return yyparse();
}
