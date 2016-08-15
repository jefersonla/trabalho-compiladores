/* Flex Definitions */

/* Allow read of only one file */
%option noyywrap
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

/* Global Macros */
#define MAX_OUTPUT_FILENAME		100
#define WS 						0
#define T_NAME					100

/* Global Enums */
enum{
	T_OPENPAR,
	T_CLOSEPAR,
	T_PLUS,	
	T_MINUS,	
	T_TIMES,	
	T_DIV,	
	T_COMMA,	
	T_SEMICOL,
	T_ASSIGN,
	T_EQ,	
	T_NEQ,	
	T_LTEQ,	
	T_GTEQ,	
	T_LT,	
	T_GT,	
	T_NUMBER,
	T_AND,	
	T_DO,	
	T_ELSE,	
	T_ELSEIF,
	T_END,	
	T_FOR,	
	T_FUNCTION,
	T_IF,	
	T_LOCAL,	
	T_NIL,	
	T_NOT,	
	T_OR,	
	T_RETURN,
	T_THEN,	
	T_WHILE,	
	T_BREAK,	
	T_GLOBAL,
	T_IN,	
	T_REPEAT,
	T_TRUE,	
	T_FALSE,	
	T_UNTIL,
} T_TYPES;

/* General Macros */
#define FPRINTF_PRINTF(OUT, STR, VAR)	fprintf(OUT, STR, VAR);printf(STR, VAR)
#define PUT_TOKEN(TOKEN) 				FPRINTF_PRINTF(output_file, "[" TOKEN ", \"%s\"],\n", yytext)

/* External Variables */
char output_filename[MAX_OUTPUT_FILENAME];
FILE *output_file;

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

// NOT SURE ABOUT THIS
- T_TRUE
- T_FALSE
- T_BREAK
- T_GLOBAL
- T_IN
- T_REPEAT
- T_UNTIL

*/
	
%}

	/*** * Definitions Section * ***/  

	/*** MATCHES FOR SYMBOLS ***/

T_OPENPAR	\(
T_CLOSEPAR 	\)
T_PLUS		\+
T_MINUS		\-
T_TIMES		\*
T_DIV		\/
T_COMMA		\,
T_SEMICOL	\;
T_ASSIGN	\=
T_EQ 		\=\=
T_NEQ 		\~\=
T_LTEQ 		\<\=
T_GTEQ 		\>\=
T_LT 		\<
T_GT 		\>

	/*** MATCHES FOR NUMBERS ***/

D			[0-9]
E			[{E}e][+-]?{D}+

T_NUMBER	((?:{D}*\.)?{D}+)

	/*** MATCHS FOR NAMES ***/

L 			[a-zA-Z_]

T_NAME		{L}({L}|{D})*

	/*** MATCHES FOR WHITESPACES ***/

WS			[\ \t\r\n\v\f]+

	/*** MATCHES TO DIFFERENTS TYPES OF STRING ***/

ESC			\\
SQ			'
DQ			["]

T_LITERAL_SQ	{SQ}(?:[^{SQ}{ESC}]|{ESC}.)*{SQ}
T_LITERAL_DQ 	{DQ}(?:[^{DQ}{ESC}]|{ESC}.)*{DQ}

	/*** MATCHES FOR RESERVED WORDS ***/

T_AND		and
T_DO		do
T_ELSE		else
T_ELSEIF	elseif
T_END		end
T_FOR		for
T_FUNCTION	function
T_IF		if
T_LOCAL		local
T_NIL		nil
T_NOT		not
T_OR		or
T_RETURN	return
T_THEN		then
T_WHILE		while

	/*** EXTRA RESERVED WORD ***/

T_FALSE		false
T_BREAK		break
T_GLOBAL	global
T_IN		in
T_REPEAT	repeat
T_TRUE		true
T_UNTIL		until

	/*** MATCHES FOR COMMENTS ***/

WS_COMMENT	^--.+$

%%

	/*** **** Rules Section **** ***/   

	/*** COMENTARIOS ***/

{WS_COMMENT}	{ printf(""); 				}

	/*** SYMBOLS ***/

{T_OPENPAR}		{ PUT_TOKEN("T_OPENPAR"); 	}
{T_CLOSEPAR}	{ PUT_TOKEN("T_CLOSEPAR"); 	}
{T_PLUS} 		{ PUT_TOKEN("T_PLUS"); 		}
{T_MINUS} 		{ PUT_TOKEN("T_MINUS"); 	}
{T_TIMES} 		{ PUT_TOKEN("T_TIMES"); 	}
{T_DIV} 		{ PUT_TOKEN("T_DIV"); 		}
{T_COMMA} 		{ PUT_TOKEN("T_COMMA"); 	}
{T_SEMICOL} 	{ PUT_TOKEN("T_SEMICOL"); 	}	
{T_ASSIGN} 		{ PUT_TOKEN("T_ASSIGN"); 	}
{T_EQ} 			{ PUT_TOKEN("T_EQ"); 		}
{T_NEQ} 		{ PUT_TOKEN("T_NEQ"); 		}
{T_LTEQ} 		{ PUT_TOKEN("T_LTEQ"); 		}
{T_GTEQ} 		{ PUT_TOKEN("T_GTEQ"); 		}
{T_LT} 			{ PUT_TOKEN("T_LT"); 		}
{T_GT} 			{ PUT_TOKEN("T_GT"); 		}


	/*** SOME TYPES OF VALUES ***/

{T_NUMBER}		{ PUT_TOKEN("T_NUMBER"); 	}
{T_LITERAL_SQ}	{ PUT_TOKEN("T_STRING1"); 	}
{T_LITERAL_DQ}	{ PUT_TOKEN("T_STRING2"); 	}

	/*** RESERVED WORDS ***/

{T_AND}			{ PUT_TOKEN("T_AND"); 		}
{T_DO}			{ PUT_TOKEN("T_DO"); 		}
{T_ELSE}		{ PUT_TOKEN("T_ELSE"); 		}
{T_ELSEIF}		{ PUT_TOKEN("T_ELSEIF"); 	}
{T_END}			{ PUT_TOKEN("T_END"); 		}
{T_FOR}			{ PUT_TOKEN("T_FOR"); 		}
{T_FUNCTION}	{ PUT_TOKEN("T_FUNCTION");	}	
{T_IF}			{ PUT_TOKEN("T_IF"); 		}
{T_LOCAL}		{ PUT_TOKEN("T_LOCAL"); 	}
{T_NIL}			{ PUT_TOKEN("T_NIL"); 		}
{T_NOT}			{ PUT_TOKEN("T_NOT"); 		}
{T_OR}			{ PUT_TOKEN("T_OR"); 		}
{T_RETURN}		{ PUT_TOKEN("T_RETURN"); 	}
{T_THEN}		{ PUT_TOKEN("T_THEN"); 		}
{T_WHILE}		{ PUT_TOKEN("T_WHILE"); 	}

	/*** EXTRA RESERVED WORDS ***/

{T_BREAK}		{ PUT_TOKEN("T_BREAK"); 	}
{T_GLOBAL}		{ PUT_TOKEN("T_GLOBAL"); 	}
{T_IN}			{ PUT_TOKEN("T_IN"); 		}
{T_REPEAT}		{ PUT_TOKEN("T_REPEAT"); 	}
{T_TRUE}		{ PUT_TOKEN("T_TRUE"); 		}
{T_FALSE}		{ PUT_TOKEN("T_FALSE"); 	}
{T_UNTIL}		{ PUT_TOKEN("T_UNTIL"); 	}

	/*** VARIABLES OR NAMES ***/

{T_NAME}		{ PUT_TOKEN("T_NAME"); 		}

	/*** WHITESPACES ***/

{WS}			{ printf(""); 				}

	/*** END OF FILE ***/
	/*   CLOSE FILE, OPEN IT AGAIN AS A+   */
	/*   REMOVE COMMA OF THE LAST LINE     */
<<EOF>>	{
			fclose(output_file);
			output_file = fopen(output_filename, "a+");
			while(getc(output_file) != ',');
			fclose(output_file);
			return 0;
		}

%%

	/*** **** C Code Section **** ***/

int main( int argc, char **argv ) {
	++argv, --argc; /* skip over program name */

	// Verify if there's a file, or receive code using stdin
	if ( argc > 0 ){
		// Input file for Flex
		yyin = fopen(argv[0], "r");
		
		// Output filename will be input_name+.out
		strcpy(output_filename, argv[0]);
		strcpy(&output_filename[strlen(output_filename)], ".out");

		// Output File for Flex
		output_file = fopen(output_filename, "w");
	}
	else{
		// Input stdin
		yyin = stdin;
	}
	
	yylex();
}
