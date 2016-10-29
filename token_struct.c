#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token_struct.h"

/*------------ Token Structure Methods ------------ */

/**
 * Creates a new token node.
 * 
 * @param root_token Father Token.
 * @param token_type Type of the Token;
 * @return A new token node.
 */
TokenNode* newTokenNode(TokenNode *root_token, int token_type){
    TokenNode *_new_token_node;
    
    /* Malloc new token node */
    _new_token_node = (TokenNode*) malloc(sizeof(TokenNode));
    
    /* Give a falta error if malloc has errors */
    if(_new_token_node == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNO'T ALLOCATE A NEW TOKEN!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Initialize new token node */
    _new_token_node->root_token = root_token;
    _new_token_node->token_type = token_type;
    _new_token_node->token_str = NULL;
    _new_token_node->lex_str = NULL;
    _new_token_node->child_list = newTokenList();
    
    /* Return pointer to the new token */
    return _new_token_node;
}

/**
 * Add Token String.
 *
 * @param token_node Token which lex_str will be appended.
 * @param token_str Token string value to be copied to TokenNode.
 * @return true if there's no error on execution and false otherwise.
 */
bool nodeAddTokenStr(TokenNode *token_node, char *token_str){
    char *_new_token_str;
    
    /* Allocate new token_str */
    _new_token_str = (char *) malloc(sizeof(char) * (strlen(token_str) + 1));
    
    /* Check if malloc has succeed */
    if(_new_token_str == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNO'T ALLOCATE STRING FOR TOKEN!\n");
        return false;
    }
    
    /* Store the new token_str array pointer */
    token_node->token_str = _new_token_str;
    
    /* Copy content from token_str to the new token */
    strcpy(token_node->token_str, token_str);
    
    /* Return success */
    return true;
}

/**
 * Add Lexical String.
 * 
 * @param token_node Token which lex_str will be appended.
 * @param lex_str Lex string value to be copied to TokenNode.
 * @return true if there's no error on execution and false otherwise.
 */
bool nodeAddLexStr(TokenNode *token_node, char *lex_str){
    char *_new_lex_str;
    
    /* Allocate new lex_str */
    _new_lex_str = (char *) malloc(sizeof(char) * (strlen(lex_str) + 1));
    
    /* Check if malloc has succeed */
    if(_new_lex_str == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNO'T ALLOCATE STRING FOR TOKEN!\n");
        return false;
    }
    
    /* Store the new lex_str array pointer */
    token_node->lex_str = _new_lex_str;
    
    /* Copy content from lex_str to the new token */
    strcpy(token_node->lex_str, lex_str);
    
    /* Return success */
    return true;
}

/* --------------- Token List Methods ------------- */

/**
 * Initialize a list of tokens.
 * 
 * @return Return a new token list.
 */
TokenList* newTokenList(){
    TokenList *_new_token_list;
    TokenNode *_items_list;
    
    /* Try allocate a new token list */
    _new_token_list = (TokenList*) malloc(sizeof(TokenList));
    
    /* Give a falta error if malloc has errors */
    if(_new_token_node == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNO'T ALLOCATE TOKEN LIST!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Try to allocate an array of tokens */
    _items_list = (TokenNode**) malloc(sizeof(TokenNode*) * 1);
    
    /* Give a falta error if malloc has errors */
    if(_items_list == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNO'T ALLOCATE ITEMS OF THE TOKEN LIST!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Initialize token list */
    _new_token_list->length = 0;
    _new_token_list->items = _items_list;
    
    /* Return the new token list */
    return _new_token_list;
}

/**
 * Add an item to a list of tokens.
 * 
 * @param token_list A pointer to a token list structure.
 * @param token Token to be added.
 * @return true if there's no error on execution and false otherwise.
 */
bool listAddToken(TokenList *token_list, TokenNode *token){
    TokenNode **_reallocated_items;
    
    /* Check if either token or token_list isn't null */
    if(token_list == NULL || token == NULL){
        fprintf(stderr, "[ERROR] TOKEN OR TOKEN LIST IS NULL!\n");
        return false;
    }
    
    /* Try reallocate array of token_list items */
    _reallocated_items = (TokenNode**) realloc(token_list->items, (token_list->length + 1) * sizeof(TokenNode *));
    
    /* Give a falta error if malloc has errors */
    if(_reallocated_items == NULL){
        fprintf(stderr, "[ERROR] FATAL ERROR CANNO'T REALLOCATE TOKEN LIST ITEMS!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Store pointer of the new token on items list */
    _reallocated_items[token_list->length] = token;
    
    /* Store the pointer to the new items list array */
    token_list->items = _reallocated_items;
    
    /* Increment list length */
    token_list->length += 1;
    
    /* Return success */
    return true;
}

/**
 * Retrieve a token by it's given index, this function assume that valid index is on range from 1 to n.
 * 
 * @param token_list A pointer to a token list structure.
 * @param index Index of the item retrieved.
 * @return Pointer to the token at the given index.
 */
TokenNode* listGetTokenByIndex(TokenList *token_list, int index){
    int i;
    
    /* Check if either token or token_list isn't null */
    if(token_list == NULL){
        fprintf(stderr, "[ERROR] TOKEN LIST IS NULL!\n");
        return NULL;
    }
    
    /* Invalid index used */
    if(index <= 0){
        fprintf(stderr, "[ERROR] INVALID INDEX!\n");
        return NULL;
    }
    
    /* Out of bounds access */
    if(index > token_list->length){
        fprintf(stderr, "[ERROR] INDEX OUT OF BOUNDS!\n");
        return NULL;
    }
    
    /* return Token pointer */
    return token_list->items[index - 1];
}

/**
 * Retrieve a list of tokens by it's given type.
 * 
 * @param token_list A pointer to a token list structure.
 * @param token_type Type of the wanted tokens.
 * @return Pointer to a generated list of tokens of a given type.
 */
TokenList* listGetTokensByType(TokenList *token_list, int token_type){
    TokenList *_tokens_found;
    TokenNode *_token_i;
    
    /* Create a new token list */
    _tokens_found = newTokenList();
    
    /* Search for the given types */
    for(i = 0; i < token_list->length; i++){
        /* Pick the token at index i */
        _token_i = listGetTokenByIndex(token_list, i + 1);
        
        /* Check if token_list item has the type wanted, and add this item to the found list */
        if(_token_i->token_type == token_type){
            listAddToken(_tokens_found, _token_i);
        }
    }
    
    /* Return token founds */
    return _tokens_found;
}

/* ------------------------------------------------ */