#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include "compiler.h" 
#include <conio.h>

/* Global variable */
token_t token;
FILE * infile;
int t = 0, c = 0, rwordValue;

int main(int argc, char *argv[]){
    initToken(argv[1]);
    program();
    printf("Success : Compilation completed with no error(s)\n");
}

void program(void){
    getToken();
	printToken(t, token);
    if (!tokenIsProgram(token)){
		error(1, "program");
    }

    getToken();
	printToken(t, token);
    if (!tokenIsIdentifier(token)){
		error(2, "program");
    }

    getToken();
	printToken(t, token);
    if (!tokenIsSemicolon(token)){
		error(3, ";");
    }

    statement();

    if (!tokenIsPeriod(token)){
		error(3, ".");
    }
}

void statement(void){
	getToken();
	printToken(t, token);
	if(tokenIsBegin(token)){
		do{		
			statement();
		}while(tokenIsSemicolon(token));

		if(!tokenIsEnd(token)){
			error(1, "end");
		}

		getToken();
		printToken(t, token);
	}else{
		expression();
	} 
}

void expression(void){
	if (tokenIsPlus(token) || tokenIsMinus(token)) {
		getToken();
		printToken(t, token);
	}

	term();
	
	while (tokenIsPlus(token) || tokenIsMinus(token)) {
		getToken();
		printToken(t, token);
		term();
	}
}

void term(void){
	factor();
	while (tokenIsTimes(token) || tokenIsDiv(token)){
		getToken();
		printToken(t, token);
		factor();
	}
}

void factor(void){
	if(tokenIsNumber(token)){
		getToken();
		printToken(t, token);
	}else if (tokenIsLParen(token)) {
		getToken();
		printToken(t, token);
		expression();
		if (tokenIsRParen(token)) {
			getToken();
			printToken(t, token);
		}else{
			error(3, ")");
		}
	}else {
		error(0, "Number");
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
		case 5 :
			printf("Error : Operator is expected after Number", chars);
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
		error(4, name);
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
}

int getToken(void){
	clearToken();
	int tempVal;
	char c1 = fgetc(infile);
	c++;
	if(charIsWhiteSpace(c1)){
		getToken();
	}else if(charIsSymbol(c1)){
		char chtemp[3];
        token.charvalue[0] = c1;
		token.attr = SYMBOL;
		char c2;
		do{
			c2 = fgetc(infile);
			c++;
			if(charIsSymbol(c2) && (c2 == '=' || c2 == '>' || c2 == '.')){
				chtemp[0] = c1;
				chtemp[1] = c2;
				setCharNull(chtemp[2]);
			}
		}while(!charIsSymbol(c2) && charIsWhiteSpace(c2));
		if(stringIsSymbol(tempVal = checkSymbol(chtemp))){
			copyString(token.charvalue, chtemp);
			token.value = tempVal;
		}else{
			token.value = checkSymbol(token.charvalue);
			if(!charIsEOF(c2)){
            	moveFileCursor(infile, -1);
				c--;
			}
		}
		setStringNull(chtemp, 3);
		t++;
		return c1;
	}else{
		int i = 0;
		do{
			if(charIsEOF(c1)){
				return c1;
			}else{
				token.charvalue[i] = c1;
				i++;
				c1 = fgetc(infile);
				c++;
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
			token.value = atoi(token.charvalue);
		}else{
			token.value = checkRWord(token.charvalue);
			if(stringIsRword(token.value)){
				rwordValue = token.value;
				token.attr  = RWORD;
			}else{
				token.value = rwordValue;
				token.attr = IDENTIFIER;
			}
		}
        moveFileCursor(infile, -1);
		c--;
		t++;
		return c1;
	}
}