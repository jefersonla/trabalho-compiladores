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
/** Versão      0.2                 **/
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

/* Change name of the import if debug mode is enabled */
#ifdef DEBUG_MODE
#include "debug.y.tab.h"
/* Otherwise just include the default table file */
#else
#include "y.tab.h"
#endif

/* Common define */
#define yyerrorfound -1
#define yycheckstate(VAR) (VAR == yyerrorfound ? 1 : 0)

#ifdef LEXICAL_ANALYSER
/* General Macros */
#define FPRINTF_PRINTF(OUT, STR, VAR)	do{fprintf(OUT, STR, VAR);printf(STR "\n", VAR);}while(0)
#define PUT_TOKEN(TOKEN) 				do{FPRINTF_PRINTF(output_file, "[" TOKEN ", \"%s\"], ", yytext);}while(0)
#define PUT_LITERAL(TOKEN) 				do{\
                                            yytext++; \
                                            yytext[strlen(yytext)-1] = 0;\
                                            FPRINTF_PRINTF(output_file, "[" TOKEN ", \"%s\"], ", yytext);\
                                        }while(0)
#define PUT_NUMBER(TOKEN)               PUT_TOKEN(TOKEN)
#define PUT_SYMBOL(TOKEN)               PUT_TOKEN(TOKEN)
#define PUT_CONTENT(STR)				fprintf(output_file, STR)
#define PRINT_COMMENT_DEBUG()			printf("%s\n", yytext)

/* External Variables */
char output_filename[MAX_OUTPUT_FILENAME];
FILE *output_file;
#elif DEBUG
/* Store a list of all tokens */
extern char *all_tokens;
extern int last_char;
/* General Macros */
#define PUT_TOKEN(TOKEN) 				do{ \
                                            yylval.string_content = malloc(strlen(yytext)+1); \
                                            strcpy(yylval.string_content, yytext); \
                                            printf("[" TOKEN ", \"%s\"], ", yytext); \
                                            all_tokens = realloc(all_tokens, (strlen(all_tokens) + strlen(TOKEN) + strlen(yytext) + 10));\
                                            sprintf(&all_tokens[last_char],"[" TOKEN ", \"%s\"], ", yytext); \
                                            last_char = strlen(all_tokens); \
                                        }while(0)
#define PUT_SYMBOL(TOKEN)               do{ \
                                            printf("[" TOKEN ", \"%s\"], ", yytext); \
                                            all_tokens = realloc(all_tokens, (strlen(all_tokens) + strlen(TOKEN) + strlen(yytext) + 10));\
                                            sprintf(&all_tokens[last_char],"[" TOKEN ", \"%s\"], ", yytext); \
                                            last_char = strlen(all_tokens); \
                                        }while(0)
#define PUT_LITERAL(TOKEN) 				do{ \
                                            yytext++; \
                                            yytext[strlen(yytext)-1] = 0; \
                                            yylval.string_literal = malloc(strlen(yytext)+1); \
                                            strcpy(yylval.string_literal, yytext); \
                                            printf("[" TOKEN ", \"%s\"], ", yytext); \
                                            all_tokens = realloc(all_tokens, (strlen(all_tokens) + strlen(TOKEN) + strlen(yytext) + 10));\
                                            sprintf(&all_tokens[last_char],"[" TOKEN ", \"%s\"], ", yytext); \
                                            last_char = strlen(all_tokens); \

                                        }while(0)
#define PUT_NUMBER(TOKEN)               do{ \
                                            yylval.integer_number = atoi(yytext); \
                                            printf("[" TOKEN ", \"%s\"], ", yytext); \
                                            all_tokens = realloc(all_tokens, (strlen(all_tokens) + strlen(TOKEN) + strlen(yytext) + 10));\
                                            sprintf(&all_tokens[last_char],"[" TOKEN ", \"%s\"], ", yytext); \
                                            last_char = strlen(all_tokens); \
                                        }while(0)
#define PRINT_COMMENT_DEBUG()			printf("%s\n", yytext)
#else
/* Store a list of all tokens */
extern char *all_tokens;
extern int last_char;
/* General Macros */
#define PUT_TOKEN(TOKEN) 				do{ \
                                            yylval.string_content = malloc(strlen(yytext)+1); \
                                            strcpy(yylval.string_content, yytext); \
                                            all_tokens = realloc(all_tokens, (strlen(all_tokens) + strlen(TOKEN) + strlen(yytext) + 10));\
                                            sprintf(&all_tokens[last_char],"[" TOKEN ", \"%s\"], ", yytext); \
                                            last_char = strlen(all_tokens); \
                                        }while(0)
#define PUT_SYMBOL(TOKEN)               do{ \
                                            all_tokens = realloc(all_tokens, (strlen(all_tokens) + strlen(TOKEN) + strlen(yytext) + 10));\
                                            sprintf(&all_tokens[last_char],"[" TOKEN ", \"%s\"], ", yytext); \
                                            last_char = strlen(all_tokens); \
                                        }while(0)
#define PUT_LITERAL(TOKEN) 				do{ \
                                            yytext++; \
                                            yytext[strlen(yytext)-1] = 0; \
                                            yylval.string_literal = malloc(strlen(yytext)+1); \
                                            strcpy(yylval.string_literal, yytext); \
                                            all_tokens = realloc(&all_tokens[last_char], (strlen(all_tokens) + strlen(TOKEN) + strlen(yytext) + 10));\
                                            sprintf(all_tokens,"[" TOKEN ", \"%s\"], ", yytext); \
                                            last_char = strlen(all_tokens); \
                                        }while(0)
#define PUT_NUMBER(TOKEN)               do{ \
                                            yylval.integer_number = atoi(yytext); \
                                            all_tokens = realloc(all_tokens, (strlen(all_tokens) + strlen(TOKEN) + strlen(yytext) + 10));\
                                            sprintf(&all_tokens[last_char],"[" TOKEN ", \"%s\"], ", yytext); \
                                            last_char = strlen(all_tokens); \
                                        }while(0)
#define PRINT_COMMENT_DEBUG()
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
									fprintf(stderr,"[ERROR] %s at line %d\n", ERROR_MSG, yylineno);	\
                                    fclose(output_file); 							        \
									remove(output_filename); 						        \
								}while(0)
#else
#define PARSE_ERRO(ERROR_MSG) 	do{ 												        \
									fprintf(stderr,"[ERROR] %s at line %d\n", ERROR_MSG, yylineno);	\
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
T_VARARG    \.\.\.
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
T_LABEL 	\:\:

    /*** MATCHES FOR NUMBERS ***/

D           [0-9]
E           [{E}e][+-]?{D}+

T_NUMBER    ((?:{D}*\.)?{D}+)

    /*** MATCHS FOR NAMES ***/

L           [a-zA-Z_]

T_NAME      {L}({L}|{D})*

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

{WS_COMMENT}	{ PRINT_COMMENT_DEBUG();		                         }

	/*** SYMBOLS ***/

{T_OPENPAR}		{ PUT_SYMBOL("T_OPENPAR"); 		 return T_OPENPAR;		}
{T_CLOSEPAR}    { PUT_SYMBOL("T_CLOSEPAR");      return T_CLOSEPAR;     }
{T_PLUS} 		{ PUT_SYMBOL("T_PLUS"); 		 return T_PLUS;		 	}
{T_MINUS} 		{ PUT_SYMBOL("T_MINUS"); 		 return T_MINUS;		}
{T_TIMES} 		{ PUT_SYMBOL("T_TIMES"); 		 return T_TIMES;		}
{T_DIV} 		{ PUT_SYMBOL("T_DIV"); 			 return T_DIV;		 	}
{T_COMMA} 		{ PUT_SYMBOL("T_COMMA"); 		 return T_COMMA;		}
{T_SEMICOL} 	{ PUT_SYMBOL("T_SEMICOL"); 		 return T_SEMICOL;		}
{T_COLON} 		{ PUT_SYMBOL("T_COLON"); 		 return T_COLON;		}
{T_ASSIGN} 		{ PUT_SYMBOL("T_ASSIGN"); 		 return T_ASSIGN;		}
{T_EQ} 			{ PUT_SYMBOL("T_EQ"); 			 return T_EQ;		 	}
{T_NEQ} 		{ PUT_SYMBOL("T_NEQ"); 			 return T_NEQ;		 	}
{T_LTEQ} 		{ PUT_SYMBOL("T_LTEQ"); 		 return T_LTEQ;		 	}
{T_GTEQ} 		{ PUT_SYMBOL("T_GTEQ"); 		 return T_GTEQ;		 	}
{T_LT} 			{ PUT_SYMBOL("T_LT"); 			 return T_LT;		 	}
{T_GT} 			{ PUT_SYMBOL("T_GT"); 			 return T_GT;		 	}
{T_SEP} 		{ PUT_SYMBOL("T_SEP"); 			 return T_SEP;		 	}
{T_CONCAT} 		{ PUT_SYMBOL("T_CONCAT");		 return T_CONCAT;		}
{T_MOD} 		{ PUT_SYMBOL("T_MOD");			 return T_MOD;		 	}
{T_EXP} 		{ PUT_SYMBOL("T_EXP");			 return T_EXP;		 	}
{T_FLOOR} 		{ PUT_SYMBOL("T_FLOOR");		 return T_FLOOR;		}
{T_BIT_AND} 	{ PUT_SYMBOL("T_BIT_AND");		 return T_BIT_AND;		}
{T_BIT_OR} 		{ PUT_SYMBOL("T_BIT_OR");		 return T_BIT_OR;		}
{T_BIT_N_XOR} 	{ PUT_SYMBOL("T_BIT_N_XOR");	 return T_BIT_N_XOR;	}
{T_BIT_RSH} 	{ PUT_SYMBOL("T_BIT_RSH");		 return T_BIT_RSH;		}
{T_BIT_LSH} 	{ PUT_SYMBOL("T_BIT_LSH");		 return T_BIT_LSH;		}
{T_LABEL}       { PUT_SYMBOL("T_LABEL");         return T_LABEL;        }
{T_VARARG}      { PUT_SYMBOL("T_VARARG");        return T_VARARG;       }
{T_OPENBRACE}    { PUT_SYMBOL("T_OPENBRACE");    return T_OPENBRACE;    }
{T_CLOSEBRACE}   { PUT_SYMBOL("T_CLOSEBRACE");   return T_CLOSEBRACE;   }
{T_OPENBRACKET}  { PUT_SYMBOL("T_OPENBRACKET");  return T_OPENBRACKET;  }
{T_CLOSEBRACKET} { PUT_SYMBOL("T_CLOSEBRACKET"); return T_CLOSEBRACKET; }

	/*** SOME TYPES OF VALUES ***/

{T_NUMBER}		{ PUT_TOKEN("T_NUMBER");       return T_NUMBER;         }
{T_LITERAL}     { PUT_LITERAL("T_LITERAL"); 	return T_LITERAL; 		}

	/*** RESERVED WORDS ***/

{T_AND}			{ PUT_TOKEN("T_AND"); 			return T_AND; 			}
{T_DO}			{ PUT_TOKEN("T_DO"); 			return T_DO; 			}
{T_ELSE}		{ PUT_TOKEN("T_ELSE"); 			return T_ELSE; 			}
{T_ELSEIF}		{ PUT_TOKEN("T_ELSEIF"); 		return T_ELSEIF; 		}
{T_END}			{ PUT_TOKEN("T_END"); 			return T_END; 			}
{T_FOR}			{ PUT_TOKEN("T_FOR"); 			return T_FOR; 			}
{T_FUNCTION}	{ PUT_TOKEN("T_FUNCTION");		return T_FUNCTION; 		}
{T_IF}			{ PUT_TOKEN("T_IF"); 			return T_IF; 			}
{T_LOCAL}		{ PUT_TOKEN("T_LOCAL"); 		return T_LOCAL; 		}
{T_NIL}			{ PUT_TOKEN("T_NIL"); 			return T_NIL; 			}
{T_NOT}			{ PUT_TOKEN("T_NOT"); 			return T_NOT; 			}
{T_OR}			{ PUT_TOKEN("T_OR"); 			return T_OR; 			}
{T_RETURN}		{ PUT_TOKEN("T_RETURN"); 		return T_RETURN; 		}
{T_THEN}		{ PUT_TOKEN("T_THEN"); 			return T_THEN; 			}
{T_WHILE}		{ PUT_TOKEN("T_WHILE"); 		return T_WHILE; 		}

	/*** EXTRA RESERVED WORDS ***/

{T_BREAK}		{ PUT_TOKEN("T_BREAK"); 		return T_BREAK; 		}
{T_GLOBAL}		{ PUT_TOKEN("T_GLOBAL"); 		return T_GLOBAL; 		}
{T_IN}			{ PUT_TOKEN("T_IN"); 			return T_IN; 			}
{T_REPEAT}		{ PUT_TOKEN("T_REPEAT"); 		return T_REPEAT; 		}
{T_TRUE}		{ PUT_TOKEN("T_TRUE"); 			return T_TRUE; 			}
{T_FALSE}		{ PUT_TOKEN("T_FALSE"); 		return T_FALSE; 		}
{T_UNTIL}		{ PUT_TOKEN("T_UNTIL"); 		return T_UNTIL; 		}

	/*** VARIABLES AND NAMES ***/

{T_NAME}		{ PUT_TOKEN("T_NAME"); 			return T_NAME; 			}

	/*** WHITESPACES ***/

{WS}			{ ; }

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
            printf("\n::: LEX PARSER PROCESS END WITHOUT ERRORS! :::\n");
            #else
            /* Just tell that the program has ended */
            all_tokens[last_char - 2] = 0;
            #endif
            return EXIT_SUCCESS;
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
void showHelpUsage(){
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
}

int main( int argc, char **argv ) {
	++argv, --argc; /* skip over program name */

    /* Code Begin Message */
	printf( " ::: LUA MIPS Compiler - Lexical Analyser ::: \n"  \
            "Version 0.2 - Pre Alpha - LUA 5.3 Compilant\n"     \
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
            printf( "Incorrect number of parrameters used!\n\n");
            showHelpUsage();
			return EXIT_FAILURE;
	}

    // Message of starting compilation
    printf( ":: OUTPUT FILE WILL BE %s.out ::\n\n"
            "::: STARTING LEX PARSER PROCESS :::\n", output_filename);

	// Output File for Flex
	output_file = fopen(output_filename, "w");

    // While there are tokens to process
    int yylex_state;
    for(yylex_state = yylex() ; (yylex_state) && (yylex_state != yyerrorfound) ; yylex_state = yylex());

    // If no errors has been related EXIT_SUCCESS
	return yycheckstate(yylex_state);
}
#endif
