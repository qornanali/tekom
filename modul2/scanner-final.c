#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include <conio.h>

/* Global variable */
FILE * infile;
token_t token;

int value = 0;
int rword_value = 0;

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
	while(getToken() != EOF) {
		printf("%3d %3d %s \n", token.attr, token.value, token.charvalue);
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
	char rwords[][10] = {"begin","div","do", "else", "end", "if", "procedure", "program", "then", "var", "while", "read", "write", "forward", "function"};
	int i = 0;
	while(i < RWORDS_SIZE && !isStringEqual(rwords[i], chars)){  
		i++;
	}
	return i; 
}

int checkSymbol(char * chars){
	char symbols[][2] = {"+", "-", "*", "(", ")", "=", ",", ".", ";", ":=", "<", "<=", "<>", ">", ">=", "[", "]", "..", ":"};
	int i = 0; 
	while(i < SYMBOLS_SIZE && !isStringEqual(symbols[i], chars)){ 
		i++;
	}
	return i; 
}


void clearToken(void){
	int i = 0;
	while(!isNull(token.charvalue[i])){
		setNull(token.charvalue[i]);
		i++;
	} 
	setNull(token.attr);
	setNull(token.value);
}

void setValueToken(void){
	if(token.attr == SYMBOL){
		token.value = value;
	}else if(token.attr == NUMBER){
		token.value = atoi(token.charvalue);
	}else if(token.attr == RWORD){
		token.value = value;
		rword_value = token.value;
	}else if(token.attr == IDENTIFIER){
		token.value = rword_value;
	}
}

int checkIdentifier(char * chars){
	int i;
	while(i < strlen(chars)){
		if(isdigit(*(chars+i))){	
			return FALSE;
		}
		++i;
	}
	return TRUE;
}

int getToken(void){
	clearToken();
	int i = 0;
	int new_token;
	char ch;
	do{
		ch = (char) fgetc(infile);
		char chAttr;
		if(isEOF(ch)){
			if(!isStringEmpty(token.charvalue)){
				setValueToken();
				printf("%3d %3d %s \n", token.attr, token.value, token.charvalue);
			}
		}else if(!isWhiteSpace(ch)){
			if(isSymbol(ch)){
				chAttr = SYMBOL;
			}else if(isNumber(ch)){
				chAttr = NUMBER;
			}else if(isAlphabet(ch)){
				chAttr = ALPHABET;
			}
			if(token.attr != chAttr && i>0){ 
				int rwordId = checkRWord(token.charvalue);
				if(isRWordFound(rwordId)){
					token.attr = RWORD;
					found(new_token);
					value = rwordId;
					fseek(infile, -1, SEEK_CUR);
				}else if(token.attr != SYMBOL){ 
					if(isAlphabet(ch) || (isNumber(ch) && i > 0)){
						token.attr = IDENTIFIER;
						token.charvalue[i] = ch;
						i++;
					}else{ 
                        if(token.attr == ALPHABET){
							token.attr = IDENTIFIER;
						}
						found(new_token);
						fseek(infile, -1, SEEK_CUR);
					}
				}else{ 
					found(new_token);
					fseek(infile, -1, SEEK_CUR);
				}
			}else{ 
				token.attr = chAttr;
				token.charvalue[i] = ch;
				i++;
				if(isSymbol(ch)){ 
					int symbolId = checkSymbol(token.charvalue);
					if(isSymbolFound(symbolId)){
						value = symbolId;
                        if(symbolId != 5 && symbolId != 7 && symbolId != 10 && symbolId != 13 && symbolId != 18){
                            found(new_token);
                        }
					}else{ 
						setNull(token.charvalue[i]);
						i--;
					}
				}
			}
		}else if(i > 0){ 
			if(token.attr == SYMBOL){
				int symbolId = checkSymbol(token.charvalue);
				if(symbolId != 5 && symbolId != 7 && symbolId != 10 && symbolId != 13 && symbolId != 18){ 
					found(new_token);
					value = symbolId;
				} 
			}else if(token.attr == NUMBER){
				found(new_token);
			}else{
				int rwordId = checkRWord(token.charvalue);
				if(isRWordFound(rwordId)){
					token.attr = RWORD;
					value = rwordId;
				}else if (isIdentifier(token.charvalue)){ 
					token.attr = IDENTIFIER;
				}	
				found(new_token); 
			}
		}
	}while(!isEOF(ch) && !isFound(new_token)); 
	setValueToken(); 
	return ch;
}
