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

/* Types imported from flex */
/* Need to organize precende and values */
%token T_OPENPAR
%token T_CLOSEPAR
%token T_PLUS
%token T_MINUS
%token T_TIMES
%token T_DIV
%token T_COMMA
%token T_SEMICOL
%token T_COLON
%token T_ASSIGN
%token T_EQ
%token T_NEQ
%token T_LTEQ
%token T_GTEQ
%token T_LT
%token T_GT
%token T_SEP
%token T_CONCAT
%token T_MOD
%token T_EXP
%token T_FLOOR
%token T_BIT_AND
%token T_BIT_OR
%token T_BIT_N_XOR
%token T_BIT_RSH
%token T_BIT_LSH
%token T_OPENBRACE
%token T_CLOSEBRACE
%token T_OPENBRACKET
%token T_CLOSEBRACKET
%token T_NUMBER
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
%token T_BREAK
%token T_GLOBAL
%token T_IN
%token T_REPEAT
%token T_TRUE
%token T_FALSE
%token T_UNTIL

%token T_LABEL
%token T_NAME
%token T_LITERAL

/* ################ Começo do Exemplo ################ */
//%union {
//    int num;
//    char id;
//}

%start 	line
%token 	print
%token 	exit_command
%token 	<integer_number> 		number
%token 	<string_variable_name> 	identifier
%type 	<integer_number> 		line exp term
%type 	<string_variable_name> 	assingment

%%

/* Especificação da linguagem */

line        : assingment ';'        {;}
            | exit_command ';'      {;}
            ;

assingment  : identifier '=' exp    {;}

exp         : term
            | exp '+' term          {$$ = $1 + $3;}
            | exp '-' term          {$$ = $1 - $3;}

term        : number                {$$ = $1;}
            | identifier            {$$ = $1;}
            ;

/* ################ Fim do Exemplo ################ */

%%

/* New main will come there */

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(){
    return yyparse();
}
