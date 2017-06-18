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

#define tokenIsRword(X) (X.attr == RWORD)
#define tokenIsIdentifier(X) (X.attr == IDENTIFIER)
#define tokenIsNumber(X) (X.attr == NUMBER)
#define tokenIsSymbol(X) (X.attr == SYMBOL)
#define tokenIsProgram(X) (tokenIsRword(X) && X.value == PROGRAM)
#define tokenIsBegin(X) (tokenIsRword(X)&& X.value == BEGIN)
#define tokenIsEnd(X) (tokenIsRword(X) && X.value == END)
#define tokenIsDiv(X) (tokenIsRword(X) && token.value == DIV) 
#define tokenIsVar(X) (tokenIsRword(X) && token.value == VAR) 
#define tokenIsWhile(X) (tokenIsRword(X) && token.value == WHILE) 
#define tokenIsDo(X) (tokenIsRword(X) && token.value == DO) 
#define tokenIsIf(X) (tokenIsRword(X) && token.value == IF) 
#define tokenIsThen(X) (tokenIsRword(X) && token.value == THEN) 
#define tokenIsElse(X) (tokenIsRword(X) && token.value == ELSE) 
#define tokenIsProcedure(X) (tokenIsRword(X) && token.value == PROCEDURE) 
#define tokenIsRead(X) (tokenIsRword(X) && token.value == READ) 
#define tokenIsWrite(X) (tokenIsRword(X) && token.value == WRITE) 
#define tokenIsSemicolon(X) (tokenIsSymbol(X) && X.value == SEMICOLON)
#define tokenIsPeriod(X) (tokenIsSymbol(X) && X.value == PERIOD) 
#define tokenIsPlus(X) (tokenIsSymbol(X) && X.value == PLUS)
#define tokenIsMinus(X) (tokenIsSymbol(X) && X.value == MINUS)
#define tokenIsTimes(X) (tokenIsSymbol(X) && X.value == TIMES)
#define tokenIsLParen(X) (tokenIsSymbol(X) && X.value == LPAREN)
#define tokenIsRParen(X) (tokenIsSymbol(X) && X.value == RPAREN)
#define tokenIsComma(X) (tokenIsSymbol(X) && X.value == COMMA)
#define tokenIsBecomes(X) (tokenIsSymbol(X) && X.value == BECOMES)
#define tokenIsEql(X) (tokenIsSymbol(X) && X.value == EQL)
#define tokenIsNotEql(X) (tokenIsSymbol(X) && X.value == NOTEQL)
#define tokenIsLessThan(X) (tokenIsSymbol(X) && X.value == LESSTHAN)
#define tokenIsGrtrThan(X) (tokenIsSymbol(X) && X.value == GRTRTHAN)
#define tokenIsLessEql(X) (tokenIsSymbol(X) && X.value == LESSEQL)
#define tokenIsGrtrEql(X) (tokenIsSymbol(X) && X.value == GRTREQL)
#define tokenIsOperator(X) (tokenIsPlus(X) || tokenIsMinus(X) || tokenIsTimes(X))

/* Prototype */
void initToken(char * name);
void program(void);
void outblock(void);
void inblock(void);
void statement(void);
void expression(void);
void condition(void);
void paramList(void);
void term(void);
void factor(void);
void error(int errId, char * chars);
void identifier(char * chars);
void semicolon(void);
void end(void);
void rparen(void);