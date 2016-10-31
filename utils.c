#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "token_struct.h"
#include "utils.h"

/* Global Instruction queue */
extern InstructionQueue *instruction_queue;

/**
 * Utility to allocate a token and token childs
 * 
 * @param token_node A pointer to a token pointer structure.
 * @param token_type Type of the given root token.
 * @param no_params Number of variadic params.
 * @param ... A infinite list of child nodes parameters. 
 * @return true if there's no error on execution and false otherwise. 
 */
bool allocateTokenAndChilds(ptrTokenNode *token_node, int token_type, int no_params, ...){
    int i;
    va_list _params_list;
    TokenNode *_token_param;
    TokenNode *_new_token_node;
    
    /* Initialize arguments list */
    va_start(_params_list, no_params);
    
    /* Check if token_node is not null */
    if(token_node == NULL){
        fprintf(stderr, "[ERROR] TOKEN NODE IS NULL!\n");
        return false;
    }
    
    /* First initialize token_node */
    _new_token_node = newTokenNode(token_type);
    
    /* Check if the new token node is null */
    if(_new_token_node == NULL){
        fprintf(stderr, "[ERROR] ERROR ALLOCATING NEW TOKEN NODE!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Insert token childs */
    for(i = 0; i < no_params; i++){
        /* Get next token */
        _token_param = va_arg(_params_list, TokenNode*);
        
        /* Check if param is null */
        if(_token_param == NULL){
            fprintf(stderr, "[ERROR] TOKEN PARAMETER IS NULL!\n");
            return false;
        }
        
        /* Add new token as a root token */
        nodeAddRootToken(_token_param, _new_token_node);
        
        /* Add current token as a child */
        listAddToken(_new_token_node->child_list, _token_param);
    }
    
    /* Clean argument list */
    va_end(_params_list);
    
    /* Store the new token node */
    (*token_node) = _new_token_node;
    
    /* Return success */
    return true;
}

/**
 * Utility to allocate text of tokens
 * 
 * @param token_node A token node parameter.
 * @param token_format A format string.
 * @param no_params Number of variadic params.
 * @param ... A infinite list of child nodes.
 * @return true if there's no error on execution and false otherwise. 
 */
//bool allocateTokenText(TokenNode *token_node, const char *token_format, int no_params, ...){
//    
//}