/* Global defines */
#define MAX_OUTPUT_FILENAME		100

/**
 * Allocate and store a string of a desired size.
 *
 * @param str String that will be stored.
 */
#define allocateToken(token_str)    do{ \
                                        $$ = malloc(sizeof(char) * (strlen(token_str) + 2)); \
                                        strcpy($$, token_str); \
                                    }while(0)

