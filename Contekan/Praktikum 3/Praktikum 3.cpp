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
#include <string.h>
#include "scanner.h"
#include "compiler.h" 

FILE *infile;
int tab=0;
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
					printf("%s \n",token.charvalue);
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
				printf("%s \n",token.charvalue);
				return 0;
			}
		}
	}
	return EOF;
}

void statement (void){
	if(isBegin){
		
		get_token();
		statement();
		
		get_token();
		while(isSemicolon){
			
			get_token();
			statement();
			
			get_token();
		}
		
		if(!isEnd){
			printf("Error : Reserved Word 'end' is expected\n");
			exit(-1);
		}
		
	}else if(isNumbr){
		
		get_token();
		if(!isNumberOperator){
			printf("Error : Reserved symbol 'number operator' is expected\n");
			exit(-1);
		}
		
		get_token();
		if(!isNumbr){
			printf("Error : Reserved token type number is expected\n");
			exit(-1);
		}
	}else{
		printf("Error\n");
		exit(-1);
	}
}

void program(void){
	get_token();
	if(!isProgram){
		printf("Error : Reserved Word 'program' is expected\n");
		exit(0);
	}
	
	get_token();
	if(!isIdentifier){
 		printf("Error : Identifier is expected after 'program' \n");
		exit(0);
	}
	
	get_token();
	if(!isSemicolon){
		printf("Error : Symbol ';' is expected\n");
		exit(0);
	}
	
	get_token();
	statement();
	
	get_token();
	if(!isPeriod){
		printf("Error : Symbol '.' is expected\n");
		exit(0);
	}
}

int main(int argc, char * argv[]){
	
	char namafile[65] = "Praktikum 3.txt";
	
	printf("%s\n\n",namafile);
	
	if((infile = fopen(namafile, "r")) == NULL){
		printf("File tak dapat dibuka!\r\n");
		exit(1);
	}

	program();
	
	printf("Congratulations : No error is found\n"); 
}
