#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

FILE * infile;
token_t token;

int getToken(void);
void clearToken(void);
void initToken(char * name);
int checkRWord(char * chars);
int checkSymbol(char * chars);
int checkAttr(char * chars);

int main(int argc, char *argv[]){
	printf("%d", atoi("ali"));
	// initToken(argv[1]);
	// while(getToken() != EOF) printf("%3d %3d %s \n", token.attr, token.value, token.charvalue);
	return 0;
}

int checkAttr(char * chars){
	int value = checkRWord(chars);
	if(value != RWORDS_SIZE){
		token.attr = RWORD;
	}
}

void initToken(char * name){
	if((infile = fopen(name, "r")) == NULL){
		printf("Error : Can't open source code %s'");
		exit(-1);
	}else return;
}

int checkRWord(char * chars){
	char rwords[][10] = {"begin","div","do", "else", "end", "if", "procedure", "program", "then", "var", "while", "read", "write", "forward", "function"};
	int i = 0;
	while(i < RWORDS_SIZE && !isStringEqual(rwords[i], chars)){
		i++;
	}
	return i;
}

int checkSymbol(char * chars){
	char symbols[19][2] = {"+", "-", "*", "(", ")", "=", ",", ".", ";", ":=", "<", "<=", "<>", ">", ">=", "[", "]", "..", ":"};
	int i = 0;
	while(i < SYMBOLS_SIZE && !isStringEqual(symbols[i], chars)){
		i++;
	}
	return i;
}

void clearToken(void){
	int i = 0;
	while(token.charvalue[i] != '\0'){
		setNull(token.charvalue[i]);
		i++;
	}
	setNull(token.attr);
	setNull(token.value);
}


int getToken(void){
	clearToken();
	int i = 0;
	char ch;
	int tokenFounded = FALSE;
	do{
		ch = (char) fgetc(infile);
		if(isEOF(ch)){
			printf("%3d %3d %s \n", token.attr, token.value, token.charvalue);
			return EOF;
		}else if()
		// if(!isWhiteSpace(ch)){
		// 	token.charvalue[i] = ch;
		// 	i++;
		// }else{
		// 	tokenFounded = TRUE;
		// 	return tokenFounded;
		// }
	}while(!isEOF(ch) && tokenFounded == FALSE);
}
