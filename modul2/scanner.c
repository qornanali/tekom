#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

#define TRUE 1
#define FALSE 0
#define isStringEqual(A, B) (strcmp((A), (B)) == 0)
#define isStringContain(A, B) (strstr((A), (B)) != NULL)
#define isLowerCharacter(X) ((X) >= 'a' && (X) <= 'z')
#define isUpperCharacter(X) ((X) >= 'A' && (X) <= 'Z')
#define isAlphabet(X) (isLowerCharacter(X) || isUpperCharacter(X))
#define isNumber(X)  ((X) >= 48 && (X) <= 57)
#define isWhiteSpace(X) ((X) == ' ' || (X) == '\n' || (X) == '\t')
#define isSymbol(X) (!isNumber((X)) && !isAlphabet((X)) && !isWhiteSpace((X)))
#define setNull(X) (X) = '\0'
#define isEOF(X) ((X) == EOF)
#define isSymbolFound(X) ((X) < 19 && (X) > 0)
#define isRWordFound(X) ((X) < 15 && (X) > 0)
#define copyString(A, B) strcpy(A, B)
#define isFound(X) ((X) == TRUE)
#define found(X) (X) = TRUE

/* Global variable */
char rwords[][10] = {"begin","div","do", "else", "end", "if", "procedure", "program", "then", "var", "while", "read", "write", "forward", "function"};
char symbols[19][2] = {"+", "-", "*", "(", ")", "=", ",", ".", ";", ":=", "<", "<=", "<>", ">", ">=", "[", "]", "..", ":"};
FILE * infile;
token_t token;

/* Prototype */
void clearToken(void);
void setValueToken(void);
void initToken(char * name);
int getToken(void);
int checkRWord(char * chars);
int checkSymbol(char * chars);
int checkIdentifier(char * chars);

int main(int argc, char *argv[]){
	initToken(argv[1]);
	while(getToken() != EOF) printf("%3d %3d %s \n", token.attr, token.value, token.charvalue);
	return 0;
}

void initToken(char * name){
	if((infile = fopen(name, "r")) == NULL){
		printf("Error : Can't open source code %s'");
		exit(-1);
	}else return;
}

int checkRWord(char * chars){
	int i = 0;
	while(i < 15 && !isStringEqual(rwords[i], chars)){
		i++;
	}
	return i;
}

int checkIdentifier(char * chars){
	int i = 0, j = 0;
	while(i < strlen(chars)){
		// if(i == 0 && isNumber())
		i++;
	}
}

int checkSymbol(char * chars){
	int i = 0;
	while(i < 19 && !isStringEqual(symbols[i], chars)){
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

void setValueToken(void){
	if(token.attr == SYMBOL){
		int val = checkSymbol(token.charvalue);
		if(isSymbolFound(val)){
			token.value = val;
		}
	}else if(token.attr == NUMBER){
		token.value = atoi(token.charvalue);
	}else if(token.attr == RWORD){
		int val = checkRWord(token.charvalue);
		if(isRWordFound(val)){
			token.value = val;
		}
	}else if(token.attr == IDENTIFIER){
		//IDENTIFIER VALUE
	}
}

int getToken(void){
	clearToken();
	int i = 0;
	int new_token;
	char ch;
	do{
		ch = (char) fgetc(infile);
		char chAttr = UNIDENTIFIED;
		if(isEOF(ch)){
			printf("%3d %3d %s \n", token.attr, token.value, token.charvalue);
			return EOF;
		}else if(isSymbol(ch)){
			chAttr = SYMBOL;
		}else if(isNumber(ch)){
			chAttr = NUMBER;
		}else if(isWhiteSpace(ch)){
			chAttr = WHITESPACE;
		}
		if(token.attr != chAttr && token.attr != NULL){
			if(isRWordFound((checkRWord(token.charvalue)))){
				token.attr = RWORD;
				found(new_token);
			}
			//
			// else if(token.attr == WHITESPACE){
			// 	clearToken();			
			// 	fseek(infile, -1, SEEK_CUR);
			// }else{
			// 	found(new_token);
			// } dunno why..
		}else{ 
			token.attr = chAttr;
			token.charvalue[i] = ch;
			i++;
		}
		if(new_token == TRUE){
			setValueToken();
			fseek(infile, -1, SEEK_CUR);
		}
	}while(!isEOF(ch) && !isFound(new_token));
}
