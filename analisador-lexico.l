/* Flex Definitions */

/* Allow read of only one file */
%option noyywrap
/* Enable Warnings */
%option warn
/* Enable line number */
%option yylineno
/* Make flex create a header file */
%option header-file="lex.yy.h"

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
/** gcc -o compilador lex.yy.c -lfl **/
/**       Ou... make lexical        **/
/** ******************************* **/

/** ******************************* **/
/**       LEIA O README PARA        **/
/**        MAIS INFORMAÇÔES         **/
/** ******************************* **/

/* External Definitions*/
//#include <stdlib.h>
//#include <stdio.h>
//#include <stdint.h>
//#include <string.h>
//#include <math.h>
/* These includes are already defined on generated source */

/* Language definitions and types */
#include "lexical.defs.h"

/* Include table file if compilation mode is parse */
#ifndef LEXICAL_ANALYSER
/* Change name of the import if debug mode is enabled */
#ifdef DEBUG_MODE
#include "debug.y.tab.h"
/* Otherwise just include the default table file */
#else
#include "y.tab.h"
#endif
#endif

/* Common define */
#define yyerrorfound -1
#define yycheckstate(VAR) (VAR != yyerrorfound)

#ifdef LEXICAL_ANALYSER
/* General Macros */
#define FPRINTF_PRINTF(OUT, STR, VAR)	do{fprintf(OUT, STR, VAR);printf(STR "\n", VAR);}while(0)
#define PUT_TOKEN(TOKEN) 				do{FPRINTF_PRINTF(output_file, "[" TOKEN ", \"%s\"], ", yytext);}while(0)
#define PUT_LITERAL(TOKEN) 				do{yytext[strlen(yytext)-1]=0;FPRINTF_PRINTF(output_file, "[" TOKEN ", \"%s\"], ", &yytext[1]);}while(0)
#define PUT_CONTENT(STR)				fprintf(output_file, STR)
#define PRINT_COMMENT_DEBUG()			printf("%s\n", yytext)

/* External Variables */
char output_filename[MAX_OUTPUT_FILENAME];
char temp[MAX_TEMP_STRING];
FILE *output_file;
#else
/* General Macros */
#define PUT_TOKEN(TOKEN) 				do{ \
                                            printf("[" TOKEN ", \"%s\"], ", yytext); \
                                        }while(0)
#define PUT_LITERAL(TOKEN) 				do{ \
                                            yytext[strlen(yytext)-1]=0; \
                                            printf("[" TOKEN ", \"%s\"], ", &yytext[1]); \
                                        }while(0)
#define PRINT_COMMENT_DEBUG()			printf("%s\n", yytext)
#endif

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

*/

/* External macro */
/**
 * Display error message, close current output file, and after that remove file
 *
 * @param error_msg	Message that will represent the error
 */
#ifdef LEXICAL_ANALYSER
#define PARSE_ERRO(ERROR_MSG) 	do{ 												        \
									printf("[ERROR] %s at line %d\n", ERROR_MSG, yylineno);	\
                                    fclose(output_file); 							        \
									remove(output_filename); 						        \
								}while(0)
#else
#define PARSE_ERRO(ERROR_MSG) 	do{ 												        \
									printf("[ERROR] %s at line %d\n", ERROR_MSG, yylineno);	\
								}while(0)
#endif
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

BC  		\[\[
EC 			\]\]

MULTILINE	{BC}[^{EC}]*{EC}

	/*** MATCHES FOR COMMENTS ***/

WS_COMMENT	--(\[\[[^\]\]]*\]\]|.*)|#.*

	/*** MATCHES TO DIFFERENTS TYPES OF STRING ***/

ESC			\\
SQ			\'
DQ			\"

    /*** POSIX DEFAULT TO LITERAL CONTENT ***/
T_LITERAL   \"[^\"\\]*(\\(.|\n)[^\"\\]*)*\"|\'[^\'\\]*(\\(.|\n)[^\'\\]*)*\'

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
{T_LITERAL}     { PUT_LITERAL("T_LITERAL"); }

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
    /** ROUTINES TO CLOSE AND FINISH FILE **/
    /** TODO UNCLOSED LITERAL AND MULTILINE COMMENTS **/

<<EOF>>	{
            /* Only close file if it's being compiled the lexical analyser */
			#ifdef LEXICAL_ANALYSER
            // Close output file, and reopen then with read and write mode
            fclose(output_file);
			output_file = fopen(output_filename, "r+");
			// Remove last token comma and put a new newline char
            fseek(output_file, -2, SEEK_END);
			fputc('\n', output_file);
            fclose(output_file);
            // Reopen output file as only read
            output_file = fopen(output_filename, "r");
            char *copy_tmp_filename = ".out_copy.tmp";
            FILE *copy_tmp_file = fopen(copy_tmp_filename, "w");
            // First copy all content of the output file to tmp file
            char read_char;
            do{
                read_char = fgetc(output_file);
                if(read_char == '\n') break;
                fputc(read_char, copy_tmp_file);
            }while(read_char != EOF);
            // After that close and reopen output file erasing previous content
            fclose(output_file);
            fclose(copy_tmp_file);
            output_file = fopen(output_filename, "w");
            copy_tmp_file = fopen(copy_tmp_filename, "r");
            do{
                read_char = fgetc(copy_tmp_file);
                if(read_char == EOF) break;
                fputc(read_char, output_file);
            }while(read_char != EOF);
            // Then close all open files, remove temp file and exit program
            fclose(output_file);
            fclose(copy_tmp_file);
            remove(copy_tmp_filename);
            // Print Complete message
            printf("\n::: LEX PARSER PROCESS END WITHOUT ERRORS! :::\n\n");
            return EXIT_SUCCESS;
            #else
            /* Just tell that the program has ended */
            // TODO!
            #endif
		}

	/*** DEFAULT BEHAVIOR ***/
    /** THROW ERROR IF THERE'S NO RULE TO MATCH **/

.	{
		PARSE_ERRO("undefinied token");
        switch(yytext[0]){
            case UNCLOSED_MULTILINE_COMMENT:
                printf("UNCLOSED MULTILINE COMMENT!\n");
                break;
            case UNCLOSED_LITERAL_DQUOTE:
            case UNCLOSED_LITERAL_SQUOTE:
                printf("UNCLOSED LITERAL!\n");
                break;
            default:
                printf(">> %s\n", yytext);
                printf("INVALID CHARACTER!\n");
        }
        // Stop Application
        return yyerrorfound;
	}

%%

	/*** **** C Code Section **** ***/

/* Main is only necessary when lexical analyser is being compiled */
#ifdef LEXICAL_ANALYSER
int main( int argc, char **argv ) {
	++argv, --argc; /* skip over program name */

    /* Code Begin Message */
	printf( " ::: LUA MIPS Compiler - Lexical Analyser ::: \n"  \
            "Version 0.1 - Pre Alpha - LUA 5.3 Compilant\n"     \
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

    // While there are tokens to process
    int yylex_state;
    for(yylex_state = yylex() ; (yylex_state) && (yylex_state != yyerrorfound) ; yylex_state = yylex());

    // If no errors has been related EXIT_SUCCESS
	return yycheckstate(yylex_state);
}
#endif
