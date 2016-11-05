#ifndef PARSER_DEFS_H
#define PARSER_DEFS_H

/* Non Terminals Defines */
#define TI_PROGRAMA             0xCAF1
#define TI_BLOCO                0xCAF2
#define TI_COMANDO              0xCAF3
#define TI_COMANDORET           0xCAF4
#define TI_EXP                  0xCAF5
#define TI_CHAMADADEFUNCAO      0xCAF6
#define TI_LISTADENOMES         0xCAF7
#define TI_LISTAEXP             0xCAF8
#define TI_COMANDO_LIST         0xCAF9
#define TI_TERM_ELSEIF          0xCAFA
#define TI_LABEL                0xCAFB

/* Extra Non Terminals */
#define TI_BLOCO_COMANDO        0xCAFC
#define TI_ASSIGN               0xCAFE
#define TI_WHILE                0XCAFF
#define TI_FOR                  0XCB00
#define TI_FOR_INC              0xCB01
#define TI_IF_ELSE              0xCB02
#define TI_IF                   0xCB03
#define TI_FUNCTION_PARAM       0xCB04
#define TI_FUNCTION             0xCB05
#define TI_LOCAL_DEFINE         0xCB06
#define TI_LOCAL_ASSIGN         0xCB07
#define TI_LIST_ELSEIF          0xCB08
#define TI_RETURN_EXPLIST       0xCB09
#define TI_RETURN               0xCB0A

/* Unary Operands */
#define IS_UNARY_OPERAND(X)     (X >= 0xCB0B && X <= 0xCB0D)
#define TI_UMINUS			    0xCB0B
#define TI_NOT				    0xCB0C
#define TI_BIT_NOT			    0xCB0D

/* Binary Operands */
#define IS_BINARY_OPERAND(X)    (X >= 0xCB0E && X <= 0xCB20)
#define TI_PLUS				    0xCB0E
#define TI_MINUS			    0xCB0F
#define TI_TIMES			    0xCB10
#define TI_DIV				    0xCB11
#define TI_FLOOR			    0xCB12
#define TI_MOD				    0xCB14
#define TI_BIT_AND			    0xCB15
#define TI_BIT_OR			    0xCB16
#define TI_BIT_N_XOR		    0xCB17
#define TI_BIT_LSH			    0xCB18
#define TI_BIT_RSH			    0xCB19
#define TI_CONCAT			    0xCB1A
#define TI_LT				    0xCB1B
#define TI_LTEQ				    0xCB1C
#define TI_GT				    0xCB1D
#define TI_GTEQ				    0xCB1E
#define TI_EQ				    0xCB1F
#define TI_NEQ				    0xCB20

/* Short-circuit evaluation for 'and' and 'or' */
#define IS_SHORT_CIRCUIT_OP(X)  (X >= 0xCB21 && X <= 0xCB22)
#define TI_AND				    0xCB21
#define TI_OR				    0xCB22

/* Call of functions */
#define TI_CALL_FUNCTION_PAR    0xCB23
#define TI_CALL_FUNCTION        0xCB24

/* Empty non terminal */
#define TI_EMPTY                0xCB25

#endif