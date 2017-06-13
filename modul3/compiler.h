#define isCompilerProgram(X) (X.attr == RWORD && X.value == PROGRAM)
#define isCompilerBegin(X) (X.attr == RWORD && X.value == BEGIN)
#define isCompilerEnd(X) (X.attr == RWORD && X.value == END)
#define isCompilerIdentifier(X) (X.attr == IDENTIFIER)
#define isCompilerNumber(X) (X.attr == NUMBER)
#define isCompilerSemicolon(X) (X.attr == SYMBOL && X.value == SEMICOLON)
#define isCompilerPeriod(X) (X.attr == SYMBOL && X.value == PERIOD) 

#define isCompilerOperatorPlus(X) (X.attr == SYMBOL && X.value == PLUS)
#define isCompilerOperatorMinus(X) (X.attr == SYMBOL && X.value == MINUS)
#define isCompilerOperatorTimes(X) (X.attr == SYMBOL && X.value == TIMES)
#define isCompilerOperator(X) (isCompilerOperatorPlus(X) || isCompilerOperatorMinus(X) || isCompilerOperatorTimes(X))

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
#define isStringEqual(A, B) (strcmp((A), (B)) == 0)
#define isStringContain(A, B) (strstr((A), (B)) != NULL)
#define isAlphabet(X) (((X) >= 'a' && (X) <= 'z') || ((X) >= 'A' && (X) <= 'Z'))
#define isNumber(X)  ((X) >= '0' && (X) <= '9')
#define isWhiteSpace(X) ((X) == ' ' || (X) == '\n' || (X) == '\t')
#define isSymbol(X) (!isNumber((X)) && !isAlphabet((X)) && !isWhiteSpace((X)))
#define setNull(X) (X) = '\0'
#define isEOF(X) ((X) == EOF)
#define isSymbolFound(X) ((X) < SYMBOLS_SIZE && (X) >= 0)
#define isRWordFound(X) ((X) < RWORDS_SIZE && (X) >= 0)
#define copyString(A, B) strcpy(A, B)
#define isFound(X) ((X) == TRUE)
#define found(X) (X) = TRUE
#define isNull(X) ((X) == '\0' || (X) == NULL)
#define isIdentifierFound(X) (checkIdentifier(X) == TRUE)
#define isStringEmpty(X) (strlen(X) == 0)

/* Prototype */
void setValueToken(void);
int getToken(void);
int checkRWord(char * chars);
int checkSymbol(char * chars);
int checkIdentifier(char * chars);
void initToken(char * name);
void program(void);
void statement(void);
