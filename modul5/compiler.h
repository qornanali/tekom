typedef struct _token{
	char attr;				
	char charvalue[30];		
	int value;			
} token_t;

/* token type */

#define ALPHABET 	0
#define IDENTIFIER 	1
#define NUMBER 		2
#define RWORD 		3
#define SYMBOL 		4
#define WHITESPACE	5

/* RESERVED WORD DEFINITION */
#define BEGIN		0
#define DIV			1	
#define DO			2
#define ELSE		3
#define END			4
#define IF			5
#define PROCEDURE	6
#define PROGRAM		7
#define THEN		8
#define	VAR			9
#define	WHILE		10
#define READ		11
#define WRITE		12
#define FORWARD		13
#define FUNCTION	14

/* symbol definition */

#define PLUS		0 	/* + */
#define MINUS		1	/* - */
#define TIMES		2	/* * */
#define LPAREN		3	/* ( */
#define RPAREN		4	/* ) */
#define EQL			5	/* = */
#define COMMA		6	/* , */
#define PERIOD		7	/* . */
#define SEMICOLON	8 	/* ; */
#define BECOMES		9	/* := */
#define	LESSTHAN	10  /* < */
#define LESSEQL		11	/* <= */ 
#define	NOTEQL		12	/* <> */
#define	GRTRTHAN	13	/* > */
#define GRTREQL		14	/* >= */
#define LBRACE		15	/* [  */
#define RBRACE		16	/* ]  */
#define INTERVAL	17	/* .. */
#define COLON		18	/* : */

//modified
#define TRUE 1
#define FALSE 0
#define RWORDS_SIZE 15
#define SYMBOLS_SIZE 19
#define stringIsEqual(A, B) (strcmp((A), (B)) == 0)
#define stringIsContain(A, B) (strstr((A), (B)) != NULL)
#define charIsAlphabet(X) (((X) >= 'a' && (X) <= 'z') || ((X) >= 'A' && (X) <= 'Z'))
#define charIsNumber(X)  ((X) >= '0' && (X) <= '9')
#define charIsWhiteSpace(X) ((X) == ' ' || (X) == '\n' || (X) == '\t')
#define charIsSymbol(X) (!charIsNumber((X)) && !charIsAlphabet((X)) && !charIsWhiteSpace((X)))
#define charIsEOF(X) ((X) == EOF)
#define setCharNull(X) (X) = '\0'
#define setVarNull(X) (X) = NULL
#define setStringNull(X, n) memset(X,'\0', n)
#define stringIsSymbol(X) ((X) < SYMBOLS_SIZE && (X) >= 0)
#define stringIsRword(X) ((X) < RWORDS_SIZE && (X) >= 0)
#define copyString(A, B) strcpy(A, B)
#define varIsNull(X) ((X) == '\0' || (X) == NULL)
#define stringIsEmpty(X) (strlen(X) == 0)
#define moveFileCursor(F, X) fseek(F, X, SEEK_CUR)

#define tokenIsRword(X) (X.attr == RWORD)
#define tokenIsProgram(X) (tokenIsRword(X) && X.value == PROGRAM)
#define tokenIsBegin(X) (tokenIsRword(X)&& X.value == BEGIN)
#define tokenIsEnd(X) (tokenIsRword(X) && X.value == END)
#define tokenIsDiv(X) (tokenIsRword(X) && token.value == DIV) 
#define tokenIsIdentifier(X) (X.attr == IDENTIFIER)
#define tokenIsNumber(X) (X.attr == NUMBER)
#define tokenIsSymbol(X) (X.attr == SYMBOL)
#define tokenIsSemicolon(X) (tokenIsSymbol(X) && X.value == SEMICOLON)
#define tokenIsPeriod(X) (tokenIsSymbol(X) && X.value == PERIOD) 
#define tokenIsPlus(X) (tokenIsSymbol(X) && X.value == PLUS)
#define tokenIsMinus(X) (tokenIsSymbol(X) && X.value == MINUS)
#define tokenIsTimes(X) (tokenIsSymbol(X) && X.value == TIMES)
#define tokenIsLParen(X) (tokenIsSymbol(X) && X.value == LPAREN)
#define tokenIsRParen(X) (tokenIsSymbol(X) && X.value == RPAREN)
#define tokenIsOperator(X) (tokenIsPlus(X) || tokenIsMinus(X) || tokenIsTimes(X))

/* Prototype */
int getToken(void);
int checkRWord(char * chars);
int checkSymbol(char * chars);
void clearToken(void);
void initToken(char * name);
void program(void);
void statement(void);
void expression(void);
void term(void);
void factor(void);
void error();
