%expect 52
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

/* String Manipulation */
#include <string.h>

/* Definições do Flex */
#include "lex.yy.h"

/* Definições dos tipos */
#include "parser.defs.h"

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

%}

/* Definition of token iterator */
%union{
    /* Number Value */
    int integer_number;
    float float_number;
    /* Literal Value */
    char *string_literal;
    /* String Content */
    char *string_content;
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
%token T_VARARG
%token T_SEP
%token T_COLON
%token T_LABEL

/* Reserved Words */

/* Boolean Comparators */
%token <string_content> T_AND
%token <string_content> T_OR
%token <string_content> T_NOT
/* -- Precendence -- */
%left T_AND T_OR
/* Conditional Expression */
%token <string_content> T_IF
%token <string_content> T_ELSEIF
%token <string_content> T_ELSE
%token <string_content> T_THEN
/* Loop Expression */
%token <string_content> T_FOR
%token <string_content> T_WHILE
%token <string_content> T_DO
/* Close Expression */
%token <string_content> T_END
%token <string_content> T_RETURN
/* Variable and Functions Manipultion */
%token <string_content> T_FUNCTION
%token <string_content> T_LOCAL
%token <string_content> T_NIL

/* Extra Reserved Words */

/* Boolean Names */
%token <string_content> T_TRUE
%token <string_content> T_FALSE
/* Other Loop Expression */
%token <string_content> T_REPEAT
%token <string_content> T_UNTIL
%token <string_content> T_BREAK
/* Other Reserved Words */
%token <string_content> T_GLOBAL
%token <string_content> T_IN

/* Variable Types */
%token <integer_number> T_NUMBER
%token <string_literal> T_LITERAL
%token <string_content> T_NAME

/* Expression Types */
%type <string_content> programa
%type <string_content> bloco
%type <string_content> comando
%type <string_content> comandoret
%type <string_content> exp
%type <string_content> chamadadefuncao
%type <string_content> listadenomes
%type <string_content> listaexp
%type <string_content> opbin
%type <string_content> opunaria
%type <string_content> term_elseif
%type <string_content> label

/* Start Type */
%start programa

/* Enable Verbose Errors */
%error-verbose

/*
### Example of Type Definition for Token and Expressions ###
 -- TOKEN dá um tipo ao TOKEN
%token  <integer_number>        number
%token  <string_variable_name>  identifier
 -- TYPE dá um tipo a EXPRESSÃO
%type   <integer_number>        line exp term
%type   <string_variable_name>  assingment
*/

%%

    /*** * Language Definitions * ***/

/* -- Program Section  -- */
/*  > Store Application if success parsing */
programa        : bloco                                 {
                                                            /* Store output file result */
                                                            fprintf(output_file, "[programa%s]\n", $1);
                                                            /* Print Finished Result */
                                                            fprintf(stderr,
                                                                    "\n::: SYNTATIC/SEMANTIC ANALYSER :::\n"
                                                                    "[programa%s]\n\n"
                                                                    "::: LEXICAL PARSER :::\n"
                                                                    "%s",
                                                                    $1, all_tokens);
                                                            /* Close Output File */
                                                            fclose(output_file);
                                                        }
                ;

/* -- Block are sections of code -- */
bloco           : comando comandoret                    { allocate2Tokens($$, " [bloco %s %s]", $1, $2);                }
                | comandoret                            { allocate1Token($$, " [bloco %s]", $1);                        }
                | comando                               { allocate1Token($$, " [bloco %s]", $1);                        }
                | /* Empty */                           { allocateToken($$, "");                                        }
                ;

/* -- Commands belong to program, and represent all the actions that can occur -- */
comando         : comando comando                       { allocate2Tokens($$, "%s %s", $1, $2);                         }
                | T_SEMICOL                             { allocateToken($$, "[comando [T_SEMICOL ;]]");                 }
                | label                                 { allocate1Token($$, "[comando %s]", $1);                       }
                | T_BREAK                               { allocateToken($$, "[comando [T_BREAK break]]");               }
                | chamadadefuncao                       { allocate1Token($$, "[comando %s]", $1);                       }
                | listadenomes T_ASSIGN listaexp        { allocate2Tokens($$, "[comando [listadenomes %s] [T_ASSIGN =] [listaexp %s]]", $1, $3);  }
                | T_DO bloco T_END                                                      {
                                                                                            allocate1Token( $$,
                                                                                                            "[comando [T_DO do]%s "
                                                                                                            "[T_END end]]",
                                                                                                            $2);
                                                                                        }
                | T_WHILE exp T_DO bloco T_END                                          {
                                                                                            allocate2Tokens($$,
                                                                                                            "[comando [T_WHILE while] %s "
                                                                                                            "[T_DO do]%s [T_END end]]",
                                                                                                            $2, $4);
                                                                                        }
                | T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_COMMA exp T_DO bloco T_END    {
                                                                                            allocate5Tokens($$,
                                                                                                            "[comando "
                                                                                                            "[T_FOR for] [T_NAME %s] [T_ASSIGN =] %s "
                                                                                                            "[T_COMMA ,] %s [T_COMMA ,] %s "
                                                                                                            "[T_DO do]%s [T_END end]]",
                                                                                                            $2, $4, $6, $8, $10);
                                                                                        }
                | T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_DO bloco T_END                {
                                                                                            allocate4Tokens($$,
                                                                                                            "[comando "
                                                                                                            "[T_FOR for] [T_NAME %s] "
                                                                                                            "[T_ASSIGN =] %s [T_COMMA ,] %s "
                                                                                                            "[T_DO do]%s [T_END end]]",
                                                                                                            $2, $4, $6, $8);
                                                                                        }
                | T_IF exp T_THEN bloco term_elseif T_ELSE bloco T_END                  {
                                                                                            allocate4Tokens($$,
                                                                                                            "[comando "
                                                                                                            "[T_IF if] %s [T_THEN then]%s%s "
                                                                                                            "[T_ELSE else]%s [T_END end]]",
                                                                                                            $2, $4, $5, $7);
                                                                                        }
                | T_IF exp T_THEN bloco term_elseif T_END                               {
                                                                                            allocate3Tokens($$,
                                                                                                            "[comando "
                                                                                                            "[T_IF if] %s [T_THEN then]%s%s "
                                                                                                            "[T_END end]]",
                                                                                                            $2, $4, $5);
                                                                                        }
                | T_FUNCTION T_NAME T_OPENPAR listadenomes T_CLOSEPAR bloco T_END       {
                                                                                            allocate3Tokens($$,
                                                                                                            "[comando "
                                                                                                            "[T_FUNCTION function] [T_NAME %s] "
                                                                                                            "[T_OPENPAR (] [listadenomes %s] "
                                                                                                            "[T_CLOSEPAR )]%s [T_END end]]",
                                                                                                            $2, $4, $6);
                                                                                        }
                | T_FUNCTION T_NAME T_OPENPAR T_CLOSEPAR bloco T_END                    {
                                                                                            allocate2Tokens($$,
                                                                                                            "[comando "
                                                                                                            "[T_FUNCTION function] [T_NAME %s] "
                                                                                                            "[T_OPENPAR (] [T_CLOSEPAR )]%s "
                                                                                                            "[T_END end]]",
                                                                                                            $2, $5);
                                                                                        }
                | T_LOCAL listadenomes T_ASSIGN listaexp                                {
                                                                                            allocate2Tokens($$,
                                                                                                            "[comando "
                                                                                                            "[T_LOCAL local] [listadenomes %s] [T_ASSIGN =] "
                                                                                                            "[listaexp %s]]",
                                                                                                            $2, $4);
                                                                                        }
                | T_LOCAL listadenomes                                                  { allocate1Token($$, "[comando [T_LOCAL local] [listadenomes %s]]", $2);   }
                ;


label           : T_LABEL T_NAME T_LABEL                { allocate1Token($$, "[T_LABEL ::] [T_NAME %s] [T_LABEL ::]", $2);  }

term_elseif     : term_elseif T_ELSEIF exp T_THEN bloco { allocate3Tokens($$,"%s [T_ELSEIF elseif] %s [T_THEN then]%s", $1, $3, $5);          }
                | /* Empty */                           { allocateToken($$, "");                                            }
                ;

comandoret      : T_RETURN listaexp T_SEMICOL           { allocate1Token($$, "[comandoret [T_RETURN return] [listaexp %s] [T_SEMICOL ;]]", $2);     }
                | T_RETURN listaexp                     { allocate1Token($$, "[comandoret [T_RETURN return] [listaexp %s]]", $2);                   }
                | T_RETURN T_SEMICOL                    { allocateToken($$, "[comandoret [T_RETURN return] [T_SEMICOL ;]]");                        }
                | T_RETURN                              { allocateToken($$, "[comandoret [T_RETURN return]]");                                      }
                ;

exp             : T_NIL                                 { allocateToken($$, "[exp [T_NIL nil]]");                           }
                | T_VARARG                              { allocateToken($$, "[exp [T_VARARG ...]]");                        }
                | T_TRUE                                { allocateToken($$, "[exp [T_TRUE true]]");                         }
                | T_FALSE                               { allocateToken($$, "[exp [T_FALSE false]]");                       }
                | T_NUMBER                              { allocateTokenNum($$, "[exp [T_NUMBER %d]]", $1);                  }
                | T_LITERAL                             { allocateTokenNum($$, "[exp [T_LITERAL %s]]", $1);                 }
                | T_NAME                                { allocate1Token($$, "[exp [T_NAME %s]]", $1);                      }
                | chamadadefuncao                       { allocate1Token($$, "[exp %s]", $1);                               }
                | exp opbin exp                         { allocate3Tokens($$, "[exp %s %s %s]", $1, $2, $3);                }
                | opunaria exp                          { allocate2Tokens($$, "[exp %s %s]", $1, $2);            }
                | T_OPENPAR exp T_CLOSEPAR              { allocate1Token($$, "[exp [T_OPENPAR (] %s [T_CLOSEPAR )]]", $2);  }
                ;

chamadadefuncao : T_NAME T_OPENPAR listaexp T_CLOSEPAR  { allocate2Tokens($$,"[chamadadefuncao [T_NAME %s] [T_OPENPAR (] [listaexp %s] [T_CLOSEPAR )]]", $1, $3);   }
                | T_NAME T_OPENPAR T_CLOSEPAR           { allocate1Token($$,"[chamadadefuncao [T_NAME %s] [T_OPENPAR (] [T_CLOSEPAR )]]", $1);          }
                ;

listadenomes    : T_NAME                                { allocate1Token($$, "[T_NAME %s]", $1);                            }
                | listadenomes T_COMMA T_NAME           { allocate2Tokens($$,"%s [T_COMMA ,] [T_NAME %s]", $1, $3);         }
                ;

listaexp        : exp                                   { allocate1Token($$, "%s", $1);                                     }
                | listaexp T_COMMA exp                  { allocate2Tokens($$, "%s [T_COMMA ,] %s", $1, $3);                 }
                ;

opbin           : T_PLUS                                { allocateToken($$, "[opbin [T_PLUS +]]");           }
                | T_MINUS                               { allocateToken($$, "[opbin [T_MINUS -]]");          }
                | T_TIMES                               { allocateToken($$, "[opbin [T_TIMES *]]");          }
                | T_DIV                                 { allocateToken($$, "[opbin [T_DIV /]]");            }
                | T_FLOOR                               { allocateToken($$, "[opbin [T_FLOOR //]]");         }
                | T_EXP                                 { allocateToken($$, "[opbin [T_EXP ^]]");            }
                | T_MOD                                 { allocateToken($$, "[opbin [T_MOD %%]]");           }
                | T_BIT_AND                             { allocateToken($$, "[opbin [T_BIT_AND &]]");        }
                | T_BIT_OR                              { allocateToken($$, "[opbin [T_BIT_OR |]]");         }
                | T_BIT_N_XOR                           { allocateToken($$, "[opbin [T_BIT_XOR ~]]");        }
                | T_BIT_LSH                             { allocateToken($$, "[opbin [T_BIT_LSH <<]]");       }
                | T_BIT_RSH                             { allocateToken($$, "[opbin [T_BIT_RSH >>]]");       }
                | T_CONCAT                              { allocateToken($$, "[opbin [T_CONCAT ..]]");        }
                | T_LT                                  { allocateToken($$, "[opbin [T_LT <]]");             }
                | T_LTEQ                                { allocateToken($$, "[opbin [T_LTEQ <=]]");          }
                | T_GT                                  { allocateToken($$, "[opbin [T_GT >]]");             }
                | T_GTEQ                                { allocateToken($$, "[opbin [T_GTEQ >=]]");          }
                | T_EQ                                  { allocateToken($$, "[opbin [T_EQ ==]]");            }
                | T_NEQ                                 { allocateToken($$, "[opbin [T_NEQ ~=]]");           }
                | T_AND                                 { allocateToken($$, "[opbin [T_AND and]]");          }
                | T_OR                                  { allocateToken($$, "[opbin [T_OR or]]");            }
                ;

opunaria        : T_MINUS                               { allocateToken($$, "[opunaria [T_MINUS -]]");     }
                | T_NOT                                 { allocateToken($$, "[opunaria [T_NOT not]]");     }
                | T_BIT_N_XOR                           { allocateToken($$, "[opunaria [T_BIT_NOT ~]]");   }
                ;

%%

    /*** **** C Code Section **** ***/

/**
 * Print an error generated by bison.
 *
 * @param s String with error informated.
 */
void yyerror(const char *s) {
    fprintf(stdout, "%s\n", s);
    fclose(output_file);
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
    /* Debug Configuration */
#ifdef DEBUG
    extern int yydebug;
    yydebug = 1;
#endif

    ++argv, --argc; /* skip over program name */

    /* Code Begin Message */
    printf( " ::: LUA MIPS Compiler - Sintatical and Semantic Analyser ::: \n"  \
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
            printf( "Incorrect number of parrameters used!\n"   \
                    "\n"                                        \
            );
            showHelpUsage();

            return EXIT_FAILURE;
    }

    // Message of starting compilation
    printf( ":: OUTPUT FILE WILL BE %s.out ::\n\n"
            "::: STARTING SINTATIC AND SEMANTIC PROCESS :::\n", argv[0]);

    // Output File for Flex
    output_file = fopen(output_filename, "w");

    /* Token Data Initialization */
    last_char = 0;
    all_tokens = calloc(sizeof(char), 1);
    all_tokens[0] = '\0';

    /* Parse entire file */
    int yyparse_return = yyparse();
    printf("\n\n::: COMPILATION END :::\n");

    // Process entire file
    return yyparse_return;
}
