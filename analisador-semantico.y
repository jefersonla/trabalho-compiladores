%{
int symbols[100];
#include "lex.yy.h"
/* Needed by bison */
void yyerror(char *);
%}

%union {
    int num; char id;
}

%start line
%token print
%token exit_command
%token <num> number
%token <id> identifier
%type <num> line exp term
%type <id> assingment

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

%%

/* New main will come there */

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(){
    return yyparse();
}
