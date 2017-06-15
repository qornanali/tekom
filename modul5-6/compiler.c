#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <conio.h>
#include "scanner.h"
#include "compiler.h" 

/* Global variable */
token_t token;
FILE * infile;

int t = 0;

int main(int argc, char *argv[]){
    initToken(argv[1]);

	while(getToken() != EOF){
		test(FALSE);
	}
    // program();
    // printf("Congratulations: No error is found\n");
}

void program(void){
	printf("program\n");
	getToken();test(FALSE);
	if(!tokenIsProgram(token)){
		error(1, "program");
	}

	getToken();test(FALSE);
	if(!tokenIsIdentifier(token)){
		error(2, "program");
	}

	getToken();test(FALSE);
	if(!tokenIsSemicolon(token)){
		error(3, ";");
	}

	outblock();

	if(!tokenIsPeriod(token)){
		error(3, ".");
		exit(-1);
	}
}

void outblock(void){
	printf("outblock\n");
	getToken();test(FALSE);
	if(!tokenIsVar(token)){
		error(1, "var");
	}

	do{
		getToken();test(FALSE);
		if(!tokenIsIdentifier(token)){
			error(2, "var");
		}
		getToken();test(FALSE);
	}while(tokenIsComma(token));

	if(!tokenIsSemicolon(token)){
		error(3, ";");
	}
	
	getToken();test(FALSE);
	while(tokenIsProcedure(token)){
		getToken();test(FALSE);
		if(!tokenIsIdentifier(token)){
			error(2, "procedure");
		}

		inblock();

		getToken();test(FALSE);
		if(!tokenIsSemicolon(token)){
			error(3, ";");
		}
	}

	statement();
}

void inblock(void){
	printf("inblock\n");
	getToken();test(FALSE);
	if(tokenIsLParen(token)){
		do{
			getToken();test(FALSE);
			if(!tokenIsIdentifier(token)){
				error(2, "(");
			}
			getToken();test(FALSE);
		}while(tokenIsComma(token));

		if(!tokenIsRParen(token)){
			error(3, ")");
		}
		
		getToken();test(FALSE);
	}else{
		error(3, "(");
	}
	
	if(!tokenIsSemicolon(token)){
		error(3, ";");
	}

	getToken();test(FALSE);
	if(tokenIsVar(token)){
		do{
			getToken();test(FALSE);
			if(!tokenIsIdentifier(token)){
				error(2, "var");
			}
			getToken();test(FALSE);
		}while(tokenIsComma(token));
		if(!tokenIsSemicolon(token)){
			error(3, ";");
		}

		getToken();test(FALSE);
	}

	statement();
}

void statement(void){
	printf("statement\n");
	if(tokenIsIdentifier(token)){
		getToken();test(FALSE);
		if(tokenIsBecomes(token)){
			expression();
		}else {
			paramList();
		}
	}else if(tokenIsBegin(token)){
		do{
			getToken();test(FALSE);
			statement();
			getToken();test(FALSE);
		}while(tokenIsSemicolon(token));

		if(!tokenIsEnd(token)){
			error(1, "end");
		}
	}else if(tokenIsWhile(token)){
		condition();

		getToken();test(FALSE);
		if(!tokenIsDo(token)){
			error(1, "do");
		}
		
		getToken();test(FALSE);
		statement();
	}else if(tokenIsIf(token)){
		condition();

		getToken();test(FALSE);
		if(!tokenIsThen(token)){
			error(1, "then");
		}

		getToken();test(FALSE);
		statement();

		getToken();test(FALSE);
		if(tokenIsElse(token)){
			getToken();test(FALSE);
			statement();
		}
	}
}

void expression(void){
	printf("expression\n");
	getToken();test(FALSE);
	if(tokenIsPlus(token) || tokenIsMinus(token)){
		getToken();test(FALSE);
	}

	do{
		term();
		getToken();test(FALSE);
	}while(tokenIsPlus(token) || tokenIsMinus(token));
}

void condition(void){
	printf("condition\n");
	expression();

	if(tokenIsEql(token) || tokenIsNotEql(token) ||
	tokenIsLessThan(token) || tokenIsGrtrThan(token) ||
	tokenIsLessEql(token) || tokenIsGrtrEql(token)){
		expression();
	}
}

void paramList(void){
	printf("paramlist\n");
	getToken();test(FALSE);
	if(tokenIsLParen(token)){
		do{
			expression();
		}while(tokenIsComma(token));

		if (tokenIsRParen(token)) {
			error(3, ")");
		}
	}
}

void term(void){
	printf("term\n");
	do{
		factor();
		getToken();test(FALSE);
	}while(tokenIsTimes(token) || tokenIsDiv(token));
}

void factor(void){
	printf("factor\n");
	if (tokenIsLParen(token)) {
		expression();
		
		if (tokenIsRParen(token)) {
			getToken();test(FALSE);
		} else error(3, ")");
	} else if(!tokenIsNumber(token) && !tokenIsIdentifier(token)){
		error(0, "Number or Identifier or '('");
	} 
}

void error(int errId, char * chars){
	switch(errId){
		case 0 :
			printf("Error : %s is expected", chars);
		break;
		case 1 :
			printf("Error : Reserved Word '%s' is expected", chars);
		break;
		case 2 :
			printf("Error : Identifier is expected after '%s'", chars);
		break;
		case 3 :
			printf("Error : Symbol '%s' is expected", chars);
		break;
		case 4 :
			printf("Error : Can't open %s", chars);
		break;
		default :
			printf("Error : Unindentified");
		break;
	}
	puts("");
    exit(-1);
}

void test(int pause){
	t++;
	printf("%d %3d %3d %s \n", t, token.attr, token.value, token.charvalue);
	if(pause == TRUE){
		getch();
	}
}

void initToken(char * name){
	if((infile = fopen(name, "r")) == NULL){
		printf("Error : Can't open source code %s'");
		exit(-1);
	}else return;
}

int checkRWord(char * chars){
	char rwords[RWORDS_SIZE][10] = {"begin","div","do", "else", "end", "if", "procedure", "program", "then", "var", "while", "read", "write", "forward", "function"};
	int i = 0;
	while(i < RWORDS_SIZE && !stringIsEqual(rwords[i], chars)){  
		i++;
	}
	return i; 
}

int checkSymbol(char * chars){
	char symbols[SYMBOLS_SIZE][3] = {"+", "-", "*", "(", ")", "=", ",", ".", ";", ":=", "<", "<=", "<>", ">", ">=", "[", "]", "..", ":"};
	int i = 0; 
	while(i < SYMBOLS_SIZE && !stringIsEqual(symbols[i], chars)){ 
		i++;
	}
	return i; 
}

void clearToken(void){
	setStringNull(token.charvalue, 50);
	setVarNull(token.attr);
	setVarNull(token.value);
}

int getToken(void){
	clearToken();
	int i = 0;
	static char tempVal;
	char c1 = fgetc(infile);
	printf("%c ",c1);
	puts("gettoken");
	if(charIsWhiteSpace(c1)){
		puts("A");
		getToken();
	}else if(charIsSymbol(c1)){
		puts("B");
		char chtemp[2];
        token.charvalue[0] = c1;
		token.attr = SYMBOL;
		char c2 =  fgetc(infile);
		if(c2 == '=' || c2 == '>' || c2 == '.'){
			puts("BA");
			chtemp[0] = c1;
			chtemp[1] = c2;
		}
		tempVal = checkSymbol(chtemp);
		if(stringIsSymbol(tempVal)){
			puts("BB.1");
			copyString(token.charvalue, chtemp);
			token.value = tempVal;
		}else{
			puts("BB.2");
			token.value = checkSymbol(token.charvalue);
			if(!charIsEOF(c2)){
				puts("BB.2A");
            	moveFileCursor(infile, -1);
			}
		}
		return c1;
	}else{
		puts("C");
		do{
			puts("CA");
			if(charIsEOF(c1)){
				puts("CA.1");
				return c1;
			}else{
				puts("CA.2");
				token.charvalue[i] = tolower(c1);
				i++;
				c1 = fgetc(infile);
			}
		}while(charIsAlphabet(c1) || charIsNumber(c1));

		int j = 0, isNumber = TRUE;
		while(!varIsNull(token.charvalue[j]) && isNumber == TRUE){
			puts("CB");
			if(!isdigit(token.charvalue[j])){
				puts("CB.1");
				isNumber = FALSE;
			}
			j++;
		}

		if(isNumber == TRUE){
			puts("CC.1");
			token.attr = NUMBER;
			tempVal = token.value = atoi(token.charvalue);
		}else{
			puts("CC.2");
			tempVal = checkRWord(token.charvalue);
			if(stringIsRword(tempVal)){
				puts("CC.3A1");
				token.attr = RWORD;
			}else{
				puts("CC.3A2");
				token.attr = IDENTIFIER;
			}
			token.value = tempVal;
		}
        moveFileCursor(infile, -1);
		return c1;
	}
}
