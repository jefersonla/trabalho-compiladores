#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "token_struct.h"
#include "utils.h"

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
 * @param no_params Number of variadic params.
 * @param ... A infinite list of strings.
 * @return true if there's no error on execution and false otherwise. 
 */
bool allocateTokenText(TokenNode *token_node, int no_params, ...){
    int i;
    char *_str_param;
    char *_new_token_string;
    va_list _params_list;
    size_t param_str_size;
    
    /* Initialize arguments list */
    va_start(_params_list, no_params);
    
    /* Check if token_node is not null */
    if(token_node == NULL){
        fprintf(stderr, "[ERROR] TOKEN NODE IS NULL!\n");
        return false;
    }
    
    /* Get size of the params list concatenated string */
    for(i = 0, param_str_size = 2; i < no_params; i++){
        /* Get next token */
        _str_param = va_arg(_params_list, char*);
        
        /* Check if param is null */
        if(_str_param == NULL){
            fprintf(stderr, "[ERROR] TOKEN STRING IS NULL!\n");
            return false;
        }
        
        /* Increase size of the token string */
        param_str_size += strlen(_str_param);
    }
    
    /* Initialize arguments list again */
    va_start(_params_list, no_params);
    
    /* Allocate string size */
    _new_token_string = (char *) malloc(sizeof(char) * param_str_size);
    
    /* Check if malloc failled */
    if(_new_token_string == NULL){
        fprintf(stderr,"[ERROR] CANNOT ALLOCATE NEW TOKEN STRING");
        return false;
    }
    
    /* Initialize the new empty string */
    _new_token_string[0] = 0;
    
    /* Concatenate the new string with the parameter list */
    for(i = 0; i < no_params; i++){
        /* Get next token */
        _str_param = va_arg(_params_list, char*);
        
        /* Check if param is null */
        if(_str_param == NULL){
            fprintf(stderr, "[ERROR] TOKEN STRING IS NULL!\n");
            return false;
        }
        
        /* Concatenate string */
        strcat(_new_token_string, _str_param);
    }
    
    /* Store the new string */
    token_node->token_str = _new_token_string;
    
    /* Clean argument list */
    va_end(_params_list);

    /* Return success */
    return true;
}

/**
 * Utility to concatenate list of tokens.
 * 
 * @param token_node_dest Destination token.
 * @param token_node_src  Source token.
 * @return true if there's no error on execution and false otherwise.
 */
bool concatenateChildTokens(TokenNode *token_node_dest, ptrTokenNode *token_node_src){
    int i;
    bool _comand_status;
    
    /* Check if dest token is null */
    if(token_node_dest == NULL){
        fprintf(stderr, "[ERROR] DESTINATION TOKEN IS NULL!\n");
        return false;
    }
    
    /* Check if src token is null */
    if(token_node_src == NULL){
        fprintf(stderr, "[ERROR] SOURCE TOKEN IS NULL!\n");
        return false;
    }
    
    /* Check if src token is null */
    if((*token_node_src) == NULL){
        fprintf(stderr, "[ERROR] SOURCE TOKEN IS NULL!\n");
        return false;
    }
    
    /* Can only concat if the dest is empty */
    if(token_node_dest->child_list->length != 0){
        fprintf(stderr, "[ERROR] DESTINATION TOKEN IS NOT EMPTY!\n");
        return false;
    }
    
    /* Concatenate the two tokens */
    for(i = 0; i < (*token_node_src)->child_list->length; i++){
        /* Add token a token from the src list */
        _comand_status = listAddToken(token_node_dest->child_list, listGetTokenByIndex((*token_node_src)->child_list, i + 1));
        
        /* Check if the add token method has worked */
        if(!_comand_status){
            fprintf(stderr, "[ERROR] DURING CONCATENATION!\n");
            return false;
        }
    }
    
    /* Delete src token */
    deleteTokenNode(token_node_src);
    
    /* Return success */
    return true;
}