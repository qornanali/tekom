/*
File name 	: scanner.c
Vers		: 0.01a
Written by	: Ali Qornan Jaisyurrahman
Date		: Fri May 19 2017
*/


#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include "scanner.h" 
#include <conio.h>

/* Global variable */
token_t token;
FILE * infile;

int t = 0, rwordValue;

int main(int argc, char *argv[]){
	initToken(argv[1]);
	while(getToken() != EOF) {
		printf("token #%d %3d %3d %s \n", t, token.attr, token.value, token.charvalue);
	} 	
	return 0;
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
}

int getToken(void){
	clearToken();
	int tempVal;
	char c1 = fgetc(infile);
	if(charIsWhiteSpace(c1)){
		getToken();
	}else if(charIsSymbol(c1)){
		char chtemp[3];
        token.charvalue[0] = c1;
		token.attr = SYMBOL;
		char c2;
		do{
			c2 = fgetc(infile);
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
		t++;
		return c1;
	}
}