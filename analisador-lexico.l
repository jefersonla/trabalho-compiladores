/* Flex Definitions */

/* Allow read of only one file */
%option noyywrap
/* Run in Debug mode */
%option debug
/* Run in Verbose mode */
%option verbose
/* Enable Warnings */
%option warn

%{

/** ****** Analisador Léxico ****** **/
/** Desenvolvido por Jeferson Lima  **/
/**              e   Jefferson Renê **/
/** Versão      1.0                 **/
/** Linguagem   LUA                 **/
/** Licença     MIT                 **/
/** Descrição:  Analisador léxico   **/
/**             para a linguagem    **/
/**             LUA.                **/
/**                                 **/
/**   1º TRABALHO DE COMPILADORES   **/
/** ******************************* **/

/** ******************************* **/
/**  Compilar este  projeto usando  **/
/**     flex analisador-lexico.l    **/
/** gcc -lfl -o compilador lex.yy.c **/
/**           Ou... make            **/
/** ******************************* **/

/* External Definitions*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/*

Types Allowed

T_NAME      T_NUMBER    T_AND       T_NOT 
T_OR        T_ELSEIF    T_WHILE     T_DO 
T_FUNCTION  T_END       T_FOR       T_ELSE 
T_IF        T_THEN      T_RETURN    T_LOCAL 
T_NIL       T_PLUS      T_MINUS     T_TIMES 
T_DIV       T_COMMA     T_OPENPAR   T_CLOSEPAR 
T_SEMICOL   T_ASSIGN    T_EQ        T_NEQ   
T_LTEQ      T_GTEQ      T_LT        T_GT

DONE

T_NUMBER -


TODO

T_NAME      T_AND       T_NOT 
T_OR        T_ELSEIF    T_WHILE     T_DO 
T_FUNCTION  T_END       T_FOR       T_ELSE 
T_IF        T_THEN      T_RETURN    T_LOCAL 
T_NIL       T_PLUS      T_MINUS     T_TIMES 
T_DIV       T_COMMA     T_OPENPAR   T_CLOSEPAR 
T_SEMICOL   T_ASSIGN    T_EQ        T_NEQ   
T_LTEQ      T_GTEQ      T_LT        T_GT

*/
	
%}

	/*** * Definitions Section * ***/  

	/*** MATCHES FOR NUMBERS ***/

D	[0-9]
E	[{E}e][+-]?{D}+
L	[a-zA-Z_]

T_NUMBER	({D}+|{D}*.{D}+|{D}+.{D}*){E}?

	/*** MATCHES FOR WHITESPACES ***/

WS	[\ \t\r\n\v\f]+

	/*** MATCHES TO DIFFERENTS TYPES OF STRING ***/

ESC	\\
SQ	'
DQ	["]

STRX	{SQ}(?:[^{SQ}{ESC}]|{ESC}.)*{SQ}
STRY	{DQ}(?:[^{DQ}{ESC}]|{ESC}.)*{DQ}

	/*** MATCHES FOR RESERVED WORDS ***/

T_AND	and
T_BREAK	break
T_DO	do
T_ELSE	else
T_ELSEIF	elseif
T_END	end
T_FALSE	false
T_FOR	for
T_FUNCTION	function
T_GLOBAL	global
T_IF	if
T_IN	in
T_LOCAL	local
T_NIL	nil
T_NOT	not
T_OR	or
T_REPEAT	repeat
T_RETURN	return
T_THEN	then
T_TRUE	true
T_UNTIL	until
T_WHILE	while

%%

	/*** **** Rules Section **** ***/   

{T_NUMBER}	{ printf("[T_NUMBER],\n"); 		}
{STRX}		{ printf("[T_STRING_1],\n"); 	}
{STRY}		{ printf("[T_STRING_2],\n"); 	}
{WS}		{ }

	/*** RESERVED WORDS ***/
{T_AND}			{ printf("[T_AND],\n"); 	}
{T_BREAK}		{ printf("[T_BREAK],\n"); 	}
{T_DO}			{ printf("[T_DO],\n"); 		}
{T_ELSE}		{ printf("[T_ELSE],\n"); 	}
{T_ELSEIF}		{ printf("[T_ELSEIF],\n"); 	}
{T_END}			{ printf("[T_END],\n"); 	}
{T_FALSE}		{ printf("[T_FALSE],\n"); 	}
{T_FOR}			{ printf("[T_FOR],\n"); 	}
{T_FUNCTION}	{ printf("[T_FUNCTION],\n");}
{T_GLOBAL}		{ printf("[T_GLOBAL],\n"); 	}
{T_IF}			{ printf("[T_IF],\n"); 		}
{T_IN}			{ printf("[T_IN],\n"); 		}
{T_LOCAL}		{ printf("[T_LOCAL],\n"); 	}
{T_NIL}			{ printf("[T_NIL],\n"); 	}
{T_NOT}			{ printf("[T_NOT],\n"); 	}
{T_OR}			{ printf("[T_OR],\n"); 		}
{T_REPEAT}		{ printf("[T_REPEAT],\n"); 	}
{T_RETURN}		{ printf("[T_RETURN],\n"); 	}
{T_THEN}		{ printf("[T_THEN],\n"); 	}
{T_TRUE}		{ printf("[T_TRUE],\n"); 	}
{T_UNTIL}		{ printf("[T_UNTIL],\n"); 	}
{T_WHILE}		{ printf("[T_WHILE],\n"); 	}

%%

	/*** **** C Code Section **** ***/

int main( int argc, char **argv ) {
	++argv, --argc; /* skip over program name */
	
	// Verify if there's a file, or receive code using stdin
	if ( argc > 0 )
		yyin = fopen(argv[0], "r");
	else
		yyin = stdin;
	
	yylex();
}
