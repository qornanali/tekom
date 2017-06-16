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


int t = 0; int c = 0;

int main(int argc, char *argv[]){
    initToken(argv[1]);

	// while(!charIsEOF(getToken())){
	// 	t++;
	// 	printf("token#%d %3d %3d %s \n", t, token.attr, token.value, token.charvalue);
	// }

    program();
    printf("Congratulations: No error is found\n");
}

void program(void){
	// printf("program\n");
	getToken();t++;
	if(!tokenIsProgram(token)){
		error(1, "program");
	}

	getToken();t++;
	if(!tokenIsIdentifier(token)){
		error(2, "program");
	}

	getToken();t++;
	if(!tokenIsSemicolon(token)){
		error(3, ";");
	}

	getToken();t++;
	outblock();
	if(!tokenIsPeriod(token)){
		error(3, ".");
		exit(-1);
	}
}

void outblock(void){
	// printf("outblock\n");
	if(!tokenIsVar(token)){
		error(1, "var");
	}

	do{
		getToken();t++;
		if(!tokenIsIdentifier(token)){
			error(2, "var");
		}
		getToken();t++;
	}while(tokenIsComma(token));

	if(!tokenIsSemicolon(token)){
		error(3, ";");
	}
	
	getToken();t++;
	while(tokenIsProcedure(token)){
		getToken();t++;
		if(!tokenIsIdentifier(token)){
			error(2, "procedure");
		}
		
		getToken();t++;
		inblock();

		if(!tokenIsSemicolon(token)){
			error(3, ";");
		}

		getToken();t++;
	}

	statement();
}

void inblock(void){
	// printf("inblock\n");
	if(tokenIsLParen(token)){
		do{
			getToken();t++;
			if(!tokenIsIdentifier(token)){
				error(2, "(");
			}
			getToken();t++;
		}while(tokenIsComma(token));

		if(!tokenIsRParen(token)){
			error(3, ")");
		}
		
		getToken();t++;
	}
	
	if(!tokenIsSemicolon(token)){
		error(3, ";");
	}

	getToken();t++;
	if(tokenIsVar(token)){
		do{
			getToken();t++;
			if(!tokenIsIdentifier(token)){
				error(2, "var");
			}
			getToken();t++;
		}while(tokenIsComma(token));

		if(!tokenIsSemicolon(token)){
			error(3, ";");
		}

		getToken();t++;
	}

	statement();
}

void statement(void){
	// printf("statement\n");
	if(tokenIsIdentifier(token)){
		getToken();t++;
		if(tokenIsBecomes(token)){
          	getToken(); t++;
			expression();
		}else {
			paramList();
		}
	}else if(tokenIsBegin(token)){
		do{
			getToken();t++;
			statement();
		}while(tokenIsSemicolon(token));

		if(!tokenIsEnd(token)){
			error(1, "end");
		}

        getToken();t++;
	}else if(tokenIsWhile(token)){
		getToken();t++;
		condition();

		if(!tokenIsDo(token)){
			error(1, "do");
		}
		
		getToken();t++;
		statement();
	}else if(tokenIsIf(token)){
		getToken();t++;
		condition();
	
		if(!tokenIsThen(token)){
			error(1, "then");
		}

		getToken();t++;
		statement();

		if(tokenIsElse(token)){
			getToken();t++;
			statement();
		}
	}else if(tokenIsRead(token)){
		getToken();t++;
		if(tokenIsLParen(token)){
			do{
				getToken();t++;
				if(!tokenIsIdentifier(token)){
					error(2, "read");
				}
				getToken();t++;
			}while(tokenIsComma(token));

			if(!tokenIsRParen(token)){
				error(3, ")");
			}

			getToken();t++;
		}else{
			error(3, "(");
		}
	}else if(tokenIsWrite(token)){
		getToken();t++;
		if(tokenIsLParen(token)){
			do{
				getToken();
				expression();
			}while(tokenIsComma(token));

			if(!tokenIsRParen(token)){
				error(3, ")");
			}

			getToken();t++;
		}else{
			error(3, "(");
		}
	}else if(!tokenIsEnd(token)){
		error(2, "end");
	}
}

void expression(void){
	// printf("expression\n");
	if(tokenIsPlus(token) || tokenIsMinus(token)){
		getToken();t++;
	}

	term();
	while(tokenIsPlus(token) || tokenIsMinus(token)){
		getToken();t++;
		term();
	}
}

void condition(void){
	// printf("condition\n");
	expression();

	if(tokenIsEql(token) || tokenIsNotEql(token) ||
	tokenIsLessThan(token) || tokenIsGrtrThan(token) ||
	tokenIsLessEql(token) || tokenIsGrtrEql(token)){
		getToken();t++;
		expression();
	}else{
		error(0,"condition");
	}
}

void paramList(void){
	// printf("paramlist\n");
	if(tokenIsLParen(token)){
		getToken();t++;
		expression();
		while(tokenIsComma(token)) {
			getToken();t++;
			expression();
		}
		if (tokenIsRParen(token)) {
			error(3, ")");
		}
		getToken();t++;
	}
}

void term(void){
	// printf("term\n");
	factor(); 
	while(tokenIsTimes(token) || tokenIsDiv(token)){
		getToken();t++;
		factor();
	}
}

void factor(void){
	// printf("factor\n");
	if(tokenIsNumber(token) || tokenIsIdentifier(token)){
		getToken();t++;
	}else if (tokenIsLParen(token)) {
		getToken();t++;
		expression();
		
		if (tokenIsRParen(token)) {
			getToken();t++;
		} else {
			error(3, ")");
		} 
	} else{
		error(0, "Number or Identifier or '('");
	} 
}

void error(int errId, char * chars){
	printf("token#%d %3d %3d %s \n", t, token.attr, token.value, token.charvalue);
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
	printf(" on %d\n", c);
    exit(-1);
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
	int tempVal;
	char c1 = fgetc(infile);
	c++;
	if(charIsWhiteSpace(c1)){
		getToken();t++;
	}else if(charIsSymbol(c1)){
		char chtemp[3];
		setStringNull(chtemp, 3);
        token.charvalue[0] = c1;
		token.attr = SYMBOL;
		char c2;
		do{
			c2 = fgetc(infile);
			c++;
			if(charIsSymbol(c2) && (c2 == '=' || c2 == '>' || c2 == '.')){
				chtemp[0] = c1;
				chtemp[1] = c2;
				chtemp[2] = '\0';	
			}
		}while(!charIsSymbol(c2) && !charIsNumber(c2) && !charIsAlphabet(c2));
		tempVal = checkSymbol(chtemp);
		if(stringIsSymbol(tempVal)){
			copyString(token.charvalue, chtemp);
			token.value = tempVal;
		}else{
			token.value = checkSymbol(token.charvalue);
			if(!charIsEOF(c2)){
				c--;
            	moveFileCursor(infile, -1);
			}
		}
		return c1;
	}else{
		do{
			if(charIsEOF(c1)){
				return c1;
			}else{
				token.charvalue[i] = tolower(c1);
				i++;
				c1 = fgetc(infile);
			}
		}while(charIsAlphabet(c1) || charIsNumber(c1));

		int j = 0, isNumber = TRUE;
		while(!varIsNull(token.charvalue[j]) && isNumber == TRUE){
			if(!isdigit(token.charvalue[j])){
				isNumber = FALSE;
			}
			j++;
		}

		if(isNumber == TRUE){
			token.attr = NUMBER;
			tempVal = token.value = atoi(token.charvalue);
		}else{
			tempVal = checkRWord(token.charvalue);
			if(stringIsRword(tempVal)){
				token.attr = RWORD;
			}else{
				token.attr = IDENTIFIER;
			}
			token.value = tempVal;
		}
        moveFileCursor(infile, -1);
		return c1;
	}
}
