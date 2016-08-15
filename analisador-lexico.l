/* Flex Definitions */

/* Allow read of only one file */
%option noyywrap
/* Enable Warnings */
%option warn
/* Enable line number */
%option yylineno

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
#include "types_lexical.h"

/* General Macros */
#define FPRINTF_PRINTF(OUT, STR, VAR)	do{fprintf(OUT, STR, VAR);printf(STR "\n", VAR);}while(0)
#define PUT_TOKEN(TOKEN) 				do{FPRINTF_PRINTF(output_file, "[" TOKEN ", \"%s\"], ", yytext);}while(0)
#define PUT_LITERAL(TOKEN) 				do{strcpy(temp, &yytext[1]);temp[strlen(temp)-1]=0;FPRINTF_PRINTF(output_file, "[" TOKEN ", \"%s\"], ", temp);}while(0)
#define PUT_CONTENT(STR)				fprintf(output_file, STR)
#define PRINT_COMMENT_DEBUG()			printf("%s\n", yytext)

/* External Variables */
char output_filename[MAX_OUTPUT_FILENAME];
char temp[MAX_TEMP_STRING];
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

/* External macro */
/**
 * Display error message, close current output file, and after that remove
 * 
 * @param error_msg	Message that will represent the error
 */
#define PARSE_ERRO(ERROR_MSG) 	do{ 												\
									printf("[ERROR] %s at line %d\n", ERROR_MSG, yylineno);	\
									fclose(output_file); 							\
									remove(output_filename); 						\
								}while(0)

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
T_COLON		\:
T_ASSIGN	\=
T_EQ 		\=\=
T_NEQ 		\~\=
T_LTEQ 		\<\=
T_GTEQ 		\>\=
T_LT 		\<
T_GT 		\>
T_SEP 		\.
T_CONCAT	\.\.
T_LIST 		\.\.\.
T_MOD		\%
T_EXP 		\^
T_FLOOR		\/\/
T_BIT_AND 	\&
T_BIT_OR 	\|
T_BIT_N_XOR \~
T_BIT_RSH 	\>\>
T_BIT_LSH 	\<\<
T_OPENBRACE  	\{
T_CLOSEBRACE 	\}
T_OPENBRACKET 	\[
T_CLOSEBRACKET  \]

	/*** MATCHES FOR NUMBERS ***/

D			[0-9]
E			[{E}e][+-]?{D}+

T_NUMBER	((?:{D}*\.)?{D}+)

	/*** MATCHS FOR NAMES ***/

L 			[a-zA-Z_]

T_NAME		{L}({L}|{D})*

	/*** MATCHES FOR LABELS ***/

T_LABEL 	\:\:{L}+\:\:

	/*** MATCHES FOR WHITESPACES ***/

WS			[\ \t\r\n\v\f]+

	/*** MATCHES FOR MULTILINE CONTENT ***/

MULTILINE	\[\[[^\]\]]*\]\]

	/*** MATCHES FOR COMMENTS ***/

WS_COMMENT	--({MULTILINE}|.*)|#.*

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

%%

	/*** **** Rules Section **** ***/   

	/*** COMENTARIOS ***/

{WS_COMMENT}	{ PRINT_COMMENT_DEBUG();	}

	/*** SYMBOLS ***/

{T_OPENPAR}		{ PUT_TOKEN("T_OPENPAR"); 	}
{T_CLOSEPAR}	{ PUT_TOKEN("T_CLOSEPAR"); 	}
{T_PLUS} 		{ PUT_TOKEN("T_PLUS"); 		}
{T_MINUS} 		{ PUT_TOKEN("T_MINUS"); 	}
{T_TIMES} 		{ PUT_TOKEN("T_TIMES"); 	}
{T_DIV} 		{ PUT_TOKEN("T_DIV"); 		}
{T_COMMA} 		{ PUT_TOKEN("T_COMMA"); 	}
{T_SEMICOL} 	{ PUT_TOKEN("T_SEMICOL"); 	}
{T_COLON} 		{ PUT_TOKEN("T_COLON"); 	}	
{T_ASSIGN} 		{ PUT_TOKEN("T_ASSIGN"); 	}
{T_EQ} 			{ PUT_TOKEN("T_EQ"); 		}
{T_NEQ} 		{ PUT_TOKEN("T_NEQ"); 		}
{T_LTEQ} 		{ PUT_TOKEN("T_LTEQ"); 		}
{T_GTEQ} 		{ PUT_TOKEN("T_GTEQ"); 		}
{T_LT} 			{ PUT_TOKEN("T_LT"); 		}
{T_GT} 			{ PUT_TOKEN("T_GT"); 		}
{T_SEP} 		{ PUT_TOKEN("T_SEP"); 		}
{T_CONCAT} 		{ PUT_TOKEN("T_CONCAT");	}
{T_MOD} 		{ PUT_TOKEN("T_MOD");		}
{T_EXP} 		{ PUT_TOKEN("T_EXP");		}
{T_FLOOR} 		{ PUT_TOKEN("T_FLOOR");		}
{T_BIT_AND} 	{ PUT_TOKEN("T_BIT_AND");	}
{T_BIT_OR} 		{ PUT_TOKEN("T_BIT_OR");	}
{T_BIT_N_XOR} 	{ PUT_TOKEN("T_BIT_N_XOR");	}
{T_BIT_RSH} 	{ PUT_TOKEN("T_BIT_RSH");	}
{T_BIT_LSH} 	{ PUT_TOKEN("T_BIT_LSH");	}
{T_OPENBRACE} 	 { PUT_TOKEN("T_OPENBRACE");	}
{T_CLOSEBRACE} 	 { PUT_TOKEN("T_CLOSEBRACE");	}
{T_OPENBRACKET}  { PUT_TOKEN("T_OPENBRACKET");	}
{T_CLOSEBRACKET} { PUT_TOKEN("T_CLOSEBRACKET");	}

	/*** SOME TYPES OF VALUES ***/

{T_NUMBER}		{ PUT_TOKEN("T_NUMBER"); 	}
{T_LITERAL_SQ}	{ PUT_LITERAL("T_LITERAL"); }
{T_LITERAL_DQ}	{ PUT_LITERAL("T_LITERAL"); }

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

	/*** VARIABLES, NAMES OR LABELS ***/

{T_NAME}		{ PUT_TOKEN("T_NAME"); 		}
{T_LABEL}		{ PUT_TOKEN("T_LABEL");		}


	/*** WHITESPACES ***/

{WS}			{ printf(""); 				}

	/*** END OF FILE ***/
	/*   CLOSE FILE, OPEN IT AGAIN AS A+   */
	/*   REMOVE COMMA OF THE LAST LINE     */
<<EOF>>	{
			fclose(output_file);
			output_file = fopen(output_filename, "r+");
			fseek(output_file, -2, SEEK_END);
			fputs("\n", output_file);
			fclose(output_file);
			return 0;
		}

	/*** DEFAULT BEHAVIOR ***/

.				{
					PARSE_ERRO("undefinied token");
					yyterminate();
				}

%%

	/*** **** C Code Section **** ***/

int main( int argc, char **argv ) {
	++argv, --argc; /* skip over program name */

	// Verify if there's a file, or receive code using stdin
	switch(argc){
		case 0:
			// Input stdin
			yyin = stdin;

			// The output will be a.out
			strcpy(output_filename, "stdin.out");
			break;
		case 1:
			// Input file for Flex
			yyin = fopen(argv[0], "r");
			
			// Output filename will be input_name+.out
			strcpy(output_filename, argv[0]);
			strcpy(&output_filename[strlen(output_filename)], ".out");
			
			break;
		case 2:
			// Input file for Flex
			yyin = fopen(argv[0], "r");
			
			// Output filename
			strcpy(output_filename, argv[1]);

			break;
		default:
			printf( "PLEASE USE THE CORRECT SYNTAX!!! \n "\
					"SYNTAX ./analisador-lexico [INPUT] [OUTPUT]");
			return EXIT_FAILURE;
	}
	
	// Output File for Flex
	output_file = fopen(output_filename, "w");

	yylex();
}
