/* Global defines */
#define MAX_OUTPUT_FILENAME     350

/**
 * Allocate and store a string of a desired size.
 *
 * @param place Where to put text content.
 * @param str String that will be stored.
 */
#define allocateToken(place,token_str)  do{ \
                                            place.str_val = (char *) malloc(sizeof(char) * (strlen(token_str) + 2)); \
                                            strcpy(place.str_val, token_str); \
                                        }while(0)

/* Apply a macro to a list of Variable Arguments */
#define APPLY0(t, dummy)
#define APPLY1(t, a)                t(a)
#define APPLY2(t, a, b)             t(a) + t(b)
#define APPLY3(t, a, b, c)          t(a) + t(b) + t(c)
#define APPLY4(t, a, b, c, d)       t(a) + t(b) + t(c) + t(d)
#define APPLY5(t, a, b, c, d, e)    t(a) + t(b) + t(c) + t(d) + t(e)
#define APPLY6(t, a, b, c, d, e, f) t(a) + t(b) + t(c) + t(d) + t(e) + t(f)

#define NUM_ARGS_H1(dummy, x6, x5, x4, x3, x2, x1, x0, ...) x0
#define NUM_ARGS(...) NUM_ARGS_H1(dummy, ##__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define APPLY_ALL_H3(t, n, ...) APPLY##n(t, __VA_ARGS__)
#define APPLY_ALL_H2(t, n, ...) APPLY_ALL_H3(t, n, __VA_ARGS__)
#define APPLY_ALL(t, ...) APPLY_ALL_H2(t, NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

/**
 * Strlen of a parameter.
 *
 * @param parameter Parmeter string.
 */
#define parameterLen(token_str) (strlen(token_str))
#define paramenterLenAll(...)   (0 + APPLY_ALL(parameterLen, __VA_ARGS__))

/**
 * Allocate and store multiple tokens.
 *
 * @param place Where to put text content.
 * @param str String format that will be stored.
 * @param ... A list of arguments that will be stored.
 */

#ifdef DEBUG
#define allocateMultipleTokens(place , token_str, ...)   while(0){ \
                                                            place.str_val = (char *) malloc(sizeof(char) * (strlen(token_str) + paramenterLenAll(__VA_ARGS__) + 2)); \
                                                            sprintf(place.str_val, token_str, __VA_ARGS__); \
                                                        }
#define allocateTokenNum(place , token_str, token)  do{ \
                                                        fprintf(stderr, token_str "\n", token); \
                                                        place.str_val = (char *) malloc(sizeof(char) * sizeof(token_str) + 13); \
                                                        sprintf(place.str_val, token_str, token); \
                                                    }while(0)                                                       
#define allocate1Token(place , token_str, token1)   do{ \
                                                        fprintf(stderr, token_str "\n", token1); \
                                                        place.str_val = (char *) malloc(sizeof(char) * \
                                                                        (   strlen(token_str) + \
                                                                            parameterLen(token1) + \
                                                                        2)); \
                                                        sprintf(place.str_val, token_str, token1); \
                                                    }while(0)
#define allocate2Tokens(place , token_str, token1, token2) do{ \
                                                        fprintf(stderr, token_str "\n", token1, \
                                                                token2); \
                                                        place.str_val = (char *) malloc(sizeof(char) * \
                                                                        (   strlen(token_str) + \
                                                                            parameterLen(token1) + \
                                                                            parameterLen(token2) + \
                                                                        2)); \
                                                        sprintf(place.str_val, token_str, token1, \
                                                                token2); \
                                                    }while(0)
#define allocate3Tokens(place , token_str, token1, token2, token3) do{ \
                                                        fprintf(stderr, token_str "\n", token1, \
                                                                token2, token3); \
                                                        place.str_val = (char *) malloc(sizeof(char) * \
                                                                        (   strlen(token_str) + \
                                                                            parameterLen(token1) + \
                                                                            parameterLen(token2) + \
                                                                            parameterLen(token3) + \
                                                                        2)); \
                                                        sprintf(place.str_val, token_str, token1, \
                                                                token2, token3); \
                                                    }while(0)
#define allocate4Tokens(place , token_str, token1, token2, token3, token4) do{ \
                                                        fprintf(stderr, token_str "\n", token1, \
                                                                token2, token3, token4); \
                                                        place.str_val = (char *) malloc(sizeof(char) * \
                                                                        (   strlen(token_str) + \
                                                                            parameterLen(token1) + \
                                                                            parameterLen(token2) + \
                                                                            parameterLen(token3) + \
                                                                            parameterLen(token4) + \
                                                                        2)); \
                                                        sprintf(place.str_val, token_str, token1, \
                                                                token2, token3, token4); \
                                                    }while(0)
#define allocate5Tokens(place , token_str, token1, token2, token3, token4, token5) do{ \
                                                        fprintf(stderr, token_str "\n", token1, \
                                                                token2, token3, token4,\
                                                                token5); \
                                                        place.str_val = (char *) malloc(sizeof(char) * \
                                                                        (   strlen(token_str) + \
                                                                            parameterLen(token1) + \
                                                                            parameterLen(token2) + \
                                                                            parameterLen(token3) + \
                                                                            parameterLen(token4) + \
                                                                            parameterLen(token5) + \
                                                                        2)); \
                                                        sprintf(place.str_val, token_str, token1, \
                                                                token2, token3, token4,\
                                                                token5); \
                                                    }while(0)
#define allocate6Tokens(place , token_str, token1, token2, token3, token4, token5, token6) do{ \
                                                        fprintf(stderr, token_str "\n", token1, \
                                                                token2, token3, token4,\
                                                                token5, token6); \
                                                        place.str_val = (char *) malloc(sizeof(char) * \
                                                                        (   strlen(token_str) + \
                                                                            parameterLen(token1) + \
                                                                            parameterLen(token2) + \
                                                                            parameterLen(token3) + \
                                                                            parameterLen(token4) + \
                                                                            parameterLen(token5) + \
                                                                            parameterLen(token6) + \
                                                                        2)); \
                                                        sprintf(place.str_val, token_str, token1, \
                                                                token2, token3, token4,\
                                                                token5, token6); \
                                                    }while(0)
#else
#define allocateMultipleTokens(place , token_str, ...)   while(0){ \
                                                            place.str_val = (char *) malloc(sizeof(char) * (strlen(token_str) + paramenterLenAll(__VA_ARGS__) + 2)); \
                                                            sprintf(place.str_val, token_str, __VA_ARGS__); \
                                                        }
#define allocateTokenNum(place , token_str, token)  do{ \
                                                        place.str_val = (char *) malloc(sizeof(char) * sizeof(token_str) + 13); \
                                                        sprintf(place.str_val, token_str, token); \
                                                    }while(0)                                                       
#define allocate1Token(place , token_str, token1)   do{ \
                                                        place.str_val = (char *) malloc(sizeof(char) * \
                                                                        (   strlen(token_str) + \
                                                                            parameterLen(token1) + \
                                                                        2)); \
                                                        sprintf(place.str_val, token_str, token1); \
                                                    }while(0)
#define allocate2Tokens(place , token_str, token1, token2) do{ \
                                                        place.str_val = (char *) malloc(sizeof(char) * \
                                                                        (   strlen(token_str) + \
                                                                            parameterLen(token1) + \
                                                                            parameterLen(token2) + \
                                                                        2)); \
                                                        sprintf(place.str_val, token_str, token1, \
                                                                token2); \
                                                    }while(0)
#define allocate3Tokens(place , token_str, token1, token2, token3) do{ \
                                                        place.str_val = (char *) malloc(sizeof(char) * \
                                                                        (   strlen(token_str) + \
                                                                            parameterLen(token1) + \
                                                                            parameterLen(token2) + \
                                                                            parameterLen(token3) + \
                                                                        2)); \
                                                        sprintf(place.str_val, token_str, token1, \
                                                                token2, token3); \
                                                    }while(0)
#define allocate4Tokens(place , token_str, token1, token2, token3, token4) do{ \
                                                        place.str_val = (char *) malloc(sizeof(char) * \
                                                                        (   strlen(token_str) + \
                                                                            parameterLen(token1) + \
                                                                            parameterLen(token2) + \
                                                                            parameterLen(token3) + \
                                                                            parameterLen(token4) + \
                                                                        2)); \
                                                        sprintf(place.str_val, token_str, token1, \
                                                                token2, token3, token4); \
                                                    }while(0)
#define allocate5Tokens(place , token_str, token1, token2, token3, token4, token5) do{ \
                                                        place.str_val = (char *) malloc(sizeof(char) * \
                                                                        (   strlen(token_str) + \
                                                                            parameterLen(token1) + \
                                                                            parameterLen(token2) + \
                                                                            parameterLen(token3) + \
                                                                            parameterLen(token4) + \
                                                                            parameterLen(token5) + \
                                                                        2)); \
                                                        sprintf(place.str_val, token_str, token1, \
                                                                token2, token3, token4,\
                                                                token5); \
                                                    }while(0)
#define allocate6Tokens(place , token_str, token1, token2, token3, token4, token5, token6) do{ \
                                                        place.str_val = (char *) malloc(sizeof(char) * \
                                                                        (   strlen(token_str) + \
                                                                            parameterLen(token1) + \
                                                                            parameterLen(token2) + \
                                                                            parameterLen(token3) + \
                                                                            parameterLen(token4) + \
                                                                            parameterLen(token5) + \
                                                                            parameterLen(token6) + \
                                                                        2)); \
                                                        sprintf(place.str_val, token_str, token1, \
                                                                token2, token3, token4,\
                                                                token5, token6); \
                                                    }while(0)                                           
#endif