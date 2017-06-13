/*
	File name : scanner.c
	Vers : 0.01a
	Written by : Setiadi Rachmat
	Date : Fri Aug 28 10:08:03 WIT 2009
	
	Modified by : Bicky Eric Kantona
	Date : 5 Maret 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "scanner.h"

#define isDelimiter (a == ' ' || a == '\n' || a == '\t')
#define isCharacter ((a >=65 && a <= 90) || (a >= 97 && a <= 122))
#define isNumber (a >= 48 && a <= 57)
#define isSymbol (!isNumber && !isCharacter)

FILE *infile;
token_t token;

void checkRWord(){
	char rWord[][30] = {("begin"),("div"),("do"),("else"),("end"),("if"),("procedure"),("program"),("then"),("var"),("while"),("read"),("write"),("forward"),("function")};
	int i=0;
	
	while(i<15){
		if(strcmp(token.charvalue, rWord[i]) == 0){
			token.attr = 3;
			token.value = i;
			break;
		}else{
			i++;
		}
	}	
}

void checkSymbol(){
	char symbol[][30] = {("+"),("-"),("*"),("("),(")"),("="),(","),("."),(";"),(":="),("<"),("<="),("<>"),(">"),(">="),("["),("]"),(".."),(":")};
	int i=0;
	
	while(i<19){
		if(strcmp(token.charvalue, symbol[i]) == 0){
			token.value = i;
			break;
		}else{
			i++;
		}
	}
}

void hapusToken(void){
	int i = 0;
	while(token.charvalue[i] != NULL){
		token.charvalue[i] = '\0';
		i++;
	}
	token.attr = NULL;
	token.value = NULL;
}

void tempAttr(char a){
	if(isNumber){
		token.attr = NUMBER;
	}else if(isCharacter){
		token.attr = IDENTIFIER;
	}else if(isSymbol){
		token.attr = SYMBOL;
	}
}

bool symbolAvailable(char a){
	char symbol[][3] = {(":="),("<="),("<>"),(">="),("..")};
	for(int i=0; i<=5; i++){
		if(token.charvalue[0] == symbol[i][0]){
			if(a == symbol[i][1]){
				return true;
			}
		}
	}
	return false;
}

int get_token(void){
	hapusToken();
	int i = 0;
	char a;
	while( ( a = fgetc(infile) ) != EOF){
		if(i==0){
			if(!isDelimiter){
				tempAttr(a);
				token.charvalue[i] = a;
				i++;
			}
		}else{
			if(token.attr == IDENTIFIER && (isCharacter || isNumber)){
				token.charvalue[i] = a;
				i++;
			}else if(token.attr == SYMBOL && isSymbol){
				if(symbolAvailable(a)){
					token.charvalue[i] = a;
					i++;
				}else{
					fseek(infile,-1,1);
					checkSymbol();
					return 0;
				}
			}else if(token.attr == NUMBER && isNumber){
				token.charvalue[i] = a;
				i++;
			}else{
				fseek(infile,-1,1);
				if(token.attr == SYMBOL){
					checkSymbol();
				}else{
					checkRWord();
				}
				return 0;
			}
		}
	}
	return EOF;
}

int main(int argc, char * argv[]){
	
	char namafile[65] = "Praktikum 2.txt";
	
	printf("%s\n\n",namafile);
	
	if((infile = fopen(namafile, "r")) == NULL){
		printf("File tak dapat dibuka!\r\n");
		exit(1);
	}

	while (get_token() != EOF){
 		printf("%3d %3d %s \n",token.attr, token.value, token.charvalue);
	}
	
	return 0;
}
