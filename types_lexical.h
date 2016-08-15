
/* Global defines */
#define MAX_OUTPUT_FILENAME		100
#define MAX_TEMP_STRING			100
#define WS 						0
#define T_NAME					100

/* Global Enums */
enum{
	T_OPENPAR,
	T_CLOSEPAR,
	T_PLUS,	
	T_MINUS,	
	T_TIMES,	
	T_DIV,	
	T_COMMA,	
	T_SEMICOL,
	T_ASSIGN,
	T_EQ,	
	T_NEQ,	
	T_LTEQ,	
	T_GTEQ,	
	T_LT,	
	T_GT,	
	T_NUMBER,
	T_AND,	
	T_DO,	
	T_ELSE,	
	T_ELSEIF,
	T_END,	
	T_FOR,	
	T_FUNCTION,
	T_IF,	
	T_LOCAL,	
	T_NIL,	
	T_NOT,	
	T_OR,	
	T_RETURN,
	T_THEN,	
	T_WHILE,	
	T_BREAK,	
	T_GLOBAL,
	T_IN,	
	T_REPEAT,
	T_TRUE,	
	T_FALSE,	
	T_UNTIL,
} T_TYPES;