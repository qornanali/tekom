/*
 File name : scanner.h
 Vers : 0.01a
 Written by : Setiadi Rachmat
 Date : Fri Aug 28 10:08:03 WIT 2009
*/
typedef struct _token {
	char attr; /* attribute type */
	char charvalue[30]; /* string */
	int value; /* string value */
} token_t;

/* token type */
#define IDENTIFIER 1
#define NUMBER 2
#define RWORD 3
#define SYMBOL 4 
/* reserved word definition */
#define BEGIN 0
#define DIV 1
#define DO 2
#define ELSE 3
#define END 4
#define IF 5
#define PROCEDURE 6
#define PROGRAM 7
#define THEN 8
#define VAR 9
#define WHILE 10
#define READ 11
#define WRITE 12
#define FORWARD 13
#define FUNCTION 14
/* symbol definition */
#define PLUS 0
#define MINUS 1 /* - */
#define TIMES 2 /* * */
#define LPAREN 3 /* ( */
#define RPAREN 4 /* ) */
#define EQL 5 /* = */
#define COMMA 6 /* , */
#define PERIOD 7 /* . */
#define SEMICOLON 8 /* ; */
#define BECOMES 9 /* := */
#define LESSTHAN 10 /* < */
#define LESSEQL 11 /* <= */
#define NOTEQL 12 /* <> */
#define GRTRTHAN 13 /* > */
#define GRTREQL 14 /* >= */
#define LBRACE 15 /* [ */
#define RBRACE 16 /* ] */
#define INTERVAL 17 /* .. */
#define COLON 18 /* : */


void hapusToken(token_t *a){
	int i = 0;
	while(a->charvalue[i] != NULL){
		a->charvalue[i] = '\0';
		i++;
	}
	a->attr = NULL;
	a->value = NULL;
}

void checkRWord(token_t *a){
	char rWord[][30] = {("begin"),("div"),("do"),("else"),("end"),("if"),("procedure"),("program"),("then"),("var"),("while"),("read"),("write"),("forward"),("function")};
	int i=0;
	
	while(i<15){
		if(strcmp(a->charvalue, rWord[i]) == 0){
			a->attr = 3;
			a->value = i;
			break;
		}else{
			i++;
		}
	}	
}

void checkSymbol(token_t *a){
	char symbol[][30] = {("+"),("-"),("*"),("("),(")"),("="),(","),("."),(";"),(":="),("<"),("<="),("<>"),(">"),(">="),("["),("]"),(".."),(":")};
	int i=0;
	
	while(i<19){
		if(strcmp(a->charvalue, symbol[i]) == 0){
			a->value = i;
			break;
		}else{
			i++;
		}
	}
}

bool symbolAvailable(char a,char b){
	char symbol[][3] = {(":="),("<="),("<>"),(">="),("..")};
	for(int i=0; i<=5; i++){
		if(a == symbol[i][0]){
			if(b == symbol[i][1]){
				return true;
			}
		}
	}
	return false;
}
