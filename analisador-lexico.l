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

%}

	/*** * Definitions Section * ***/  


%%

	/*** **** Rules Section **** ***/   


%%

	/*** **** C Code Section **** ***/