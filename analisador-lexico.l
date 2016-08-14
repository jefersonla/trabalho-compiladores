%{

/** ****** Analisador Léxico ****** **/
/** Desenvolvido por Jeferson Lima  **/
/**              e   Jefferson Renê **/
/** Versão      1.0                 **/
/** Linguagem   LUA                 **/
/** Licença     MIT                 **/
/** Descrição:  Analisador léxico   **/
/**             para a linguagem    **/
/**             LUA. 1º Trabalho de **/
/**             COMPILADORES        **/
/** ******************************* **/

/* External Definitions*/
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <string>
#include <math.h>
extern "C" int yywrap() { }

/* The following part */

%}

DIGIT    [0-9]
ID       [a-z][a-z0-9]*

%%

{DIGIT}+    {
printf( "An integer: %s (%d)\n", yytext,
 atoi( yytext ) );
}

{DIGIT}+"."{DIGIT}*        {
printf( "A float: %s (%g)\n", yytext,
 atof( yytext ) );
}

if|then|begin|end|procedure|function        {
printf( "A keyword: %s\n", yytext );
}

{ID}        printf( "An identifier: %s\n", yytext );

"+"|"-"|"*"|"/"   printf( "An operator: %s\n", yytext );

"{"[\^{}}\n]*"}"     /* eat up one-line comments */

[ \t\n]+          /* eat up whitespace */

.           printf( "Unrecognized character: %s\n", yytext );

%%

/*
int main( int argc, char **argv ){
    ++argv, --argc;  // skip over program name
    
    if ( argc > 0 ){
        yyin = fopen( argv[0], "r" );
    }
    else
        yyin = stdin;

    yylex();
}
*/