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
        printError("TOKEN NODE IS NULL!");
        return false;
    }
    
    /* First initialize token_node */
    _new_token_node = newTokenNode(token_type);
    
    /* Check if the new token node is null */
    if(_new_token_node == NULL){
        printFatalError("ERROR ALLOCATING NEW TOKEN NODE!");
        exit(EXIT_FAILURE);
    }
    
    /* Insert token childs */
    for(i = 0; i < no_params; i++){
        /* Get next token */
        _token_param = va_arg(_params_list, TokenNode*);
        
        /* Check if param is null */
        if(_token_param == NULL){
            printError("TOKEN PARAMETER IS NULL!");
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
    
    /* Check if token_node is not null */
    if(token_node == NULL){
        printError("TOKEN NODE IS NULL!");
        return false;
    }
    
    /* Initialize arguments list */
    va_start(_params_list, no_params);

    /* Initialize params size */
    param_str_size = SECURE_BUFFER_SIZE;
    
    for(i = 0; i < no_params; i++){
        /* Get next token */
        _str_param = va_arg(_params_list, char*);
        
        /* Get size of the printed string */
        param_str_size += snprintf(NULL, 0, "%s", _str_param);
    }
    
    /* Initialize arguments list again */
    va_start(_params_list, no_params);
    
    /* Allocate string size */
    _new_token_string = malloc(param_str_size);
    
    /* Check if malloc failled */
    if(_new_token_string == NULL){
        printError("CANNOT ALLOCATE NEW TOKEN STRING");
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
            printError("TOKEN STRING IS NULL!");
            return false;
        }
        
        /* Concatenate string */
        strncat(_new_token_string, _str_param, param_str_size);
    }
    
    /* Free the old string if it exists */
    if(token_node->token_str != NULL){
        secureFree(token_node->token_str);
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
        printError("DESTINATION TOKEN IS NULL!");
        return false;
    }
    
    /* Check if src token is null */
    if(token_node_src == NULL){
        printError("SOURCE TOKEN IS NULL!");
        return false;
    }
    
    /* Check if src token is null */
    if((*token_node_src) == NULL){
        printError("SOURCE TOKEN IS NULL!");
        return false;
    }
    
    /* Can only concat if the dest is empty */
    if(token_node_dest->child_list->length != 0){
        printError("DESTINATION TOKEN IS NOT EMPTY!");
        return false;
    }
    
    /* Concatenate the two tokens */
    for(i = 0; i < (*token_node_src)->child_list->length; i++){
        /* Add token a token from the src list */
        _comand_status = listAddToken(token_node_dest->child_list, listGetTokenByIndex((*token_node_src)->child_list, i + 1));
        
        /* Check if the add token method has worked */
        if(!_comand_status){
            printError("DURING CONCATENATION!");
            return false;
        }
    }
    
    /* Delete src token */
    deleteTokenNode(token_node_src, false);
    
    /* Return success */
    return true;
}

/**
 * Utility to allocate formatted instructions.
 * 
 * @param format_string String with the format of the instruction.
 * @param ... A list of parameters used in format string.
 * @return A pointer to a string with the formated instruction.
 */
char* formatedInstruction(const char *format_string, ...){
    va_list params;
    char *_new_formated_instruction;

    /* Initialize list of params */
    va_start (params, format_string);
    
    /* Get size of the printed string */
    size_t str_len = snprintf(NULL, 0, format_string, params);
    str_len += SECURE_BUFFER_SIZE;

    /* Check if the size received is invalid */
    if(str_len <= 0){
        printError("ERROR GETING FORMATED INSTRUCTION SIZE!");
        return NULL;
    }
    
    /* Allocate formated string buffer */
    _new_formated_instruction = malloc(str_len);
    
    /* Check if the malloc hasn't failed */
    if(_new_formated_instruction == NULL){
        printError("ERROR ALLOCATING NEW FORMATED INSTRUCTION BUFFER!");
        return NULL;
    }
    
    /* Store the formated instruction */
    va_start (params, format_string);
    vsnprintf (_new_formated_instruction, str_len, format_string, params);
    _new_formated_instruction[str_len - 1] = 0;
    va_end (params);
    
    /* Return the new formated instruction */
    return _new_formated_instruction;
}
