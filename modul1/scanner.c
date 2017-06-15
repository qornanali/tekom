/*
File name 	: scanner.c
Vers		: 0.01a
Written by	: Setiadi Rachmat
Date		: Fri Aug 28 2019

Modified by : Ali Qornan Jaisyurrahman
Date		: Fri May 19 2017
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <conio.h>

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

FILE * infile;
char token[50];

int getToken(void);
void clearToken(void);
void initToken(char *name);

int main(int argc, char *argv[]){
	initToken(argv[1]);
	while(getToken() != EOF) printf("%s \n",token);
}

void clearToken(void){
	setStringNull(token, 50);
}

void initToken(char *name){
	if((infile = fopen(name, "r")) == NULL){
		printf("Error : Can't open source code %s'");
		exit(-1);
	}else return;
}

int getToken(void){
	clearToken();
	int i = 0;
	int tempVal;
	char c1 = fgetc(infile);
	if(charIsWhiteSpace(c1)){
		getToken();
	}else if(charIsSymbol(c1)){
        token[0] = c1;
		return c1;
	}else{
		do{
			if(charIsEOF(c1)){
				return c1;
			}else{
				token[i] = tolower(c1);
				i++;
				c1 = fgetc(infile);
			}
		}while(charIsAlphabet(c1) || charIsNumber(c1));
        moveFileCursor(infile, -1);
		return c1;
	}
}