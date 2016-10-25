#ifndef TOKEN_STRUCT_H
#define TOKEN_STRUCT_H

/* Token Structure */
typedef struct strToken{
    /* Root token of the given node */
    struct strToken *root_token;
    
    /* String value */
    char *str_val;
} TokenNode, *ptrTokenNode;

#endif