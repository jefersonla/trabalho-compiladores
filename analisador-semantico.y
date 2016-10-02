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

%left T_TIMES T_DIV
%left T_PLUS T_MINUS
/* Boolean Operators */
%token T_EQ
%token T_NEQ
%token T_LTEQ
%token T_GTEQ
%token T_LT
%token T_GT
%token T_SEP

%left T_EQ T_NEQ T_LTEQ T_GTEQ T_LT T_GT
/* Separators and assign */
%token T_COMMA
%token T_SEMICOL
%token T_COLON
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
%token 	<integer_number> 		number
%token 	<string_variable_name> 	identifier
%type 	<integer_number> 		line exp term
%type 	<string_variable_name> 	assingment
*/

%%

/* Especificação da linguagem */

/*line        : assingment ';'        {;}
            | exit_command ';'      {;}
            ;

assingment  : identifier '=' exp    {;}

exp         : term
            | exp '+' term          {$$ = $1 + $3;}
            | exp '-' term          {$$ = $1 - $3;}

term        : number                {$$ = $1;}
            | identifier            {$$ = $1;}
            ;
*/

/* ################ Fim do Exemplo ################ */

programa 	: bloco					        {;}
			;

bloco 		: comando comandoret            {;}
            | comando                       {;}
            | /* Empty */                   {;}
            ;

comando     : comando comando               {;}
            | ','                           {;}
            | listadenomes '=' listaexp     {;}
            | chamadadefuncao               {;}
            | T_DO bloco T_END              {;}
            | T_WHILE exp T_DO bloco T_END  {;}
            | T_FOR T_NAME '=' exp ',' exp ',' exp T_DO bloco T_END     {;}
            | T_FOR T_NAME '=' exp ',' exp T_DO bloco T_END             {;}
            | T_IF exp T_THEN bloco term_elseif T_ELSE bloco T_END      {;}
            | T_IF exp T_THEN bloco term_elseif T_END                   {;}
            | T_FUNCTION T_NAME '(' listadenomes ')' bloco T_END        {;}
            | T_FUNCTION T_NAME '(' ')' bloco T_END                     {;}
            | T_LOCAL listadenomes '=' listaexp                         {;}
            | T_LOCAL listadenomes          {;}
            ;

term_elseif : /* Empty */                           {;}
            | T_ELSEIF exp T_THEN bloco term_elseif {;}
            ;

comandoret  : T_RETURN listaexp ';'         {;}
            | T_RETURN listaexp             {;}
            | T_RETURN ';'                  {;}
            | T_RETURN                      {;}
            ;

exp         : T_NUMBER                      {;}
            | T_NAME                        {;}
            | T_NIL                         {;}
            | chamadadefuncao               {;}
            | exp opbin exp                 {;}
            | opunaria exp                  {;}
            | '(' exp ')'                   {;}
            ;

chamadadefuncao : T_NAME '(' listaexp ')'   {;}
                | T_NAME '(' ')'            {;}
                ;

listadenomes    : T_NAME listanomes         {;}
                ;

listanomes      : ',' T_NAME listanomes     {;}
                |  /* Empty */              {;}
                ;

listaexp        : exp lista_expr            {;}
                ;

lista_expr      : ',' exp lista_expr        {;}
                |  /* Empty */              {;}
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

/* New main will come there */

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(){
    return yyparse();
}
