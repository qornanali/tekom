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
#include <string.h>
#include "scanner.h"
#include "compiler.h"
#include "expression.h"
#include "list.h"

void expression(void);
void term(void);
void factor(void);

FILE *infile;
int tab=0;
token_t token;
List list1;

void tempAttr(char a){
	if(isNumber){
		token.attr = NUMBER;
	}else if(isCharacter){
		token.attr = IDENTIFIER;
	}else if(isSymbol){
		token.attr = SYMBOL;
	}
}

int get_token(void){
	hapusToken(&token);
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
				if(symbolAvailable(token.charvalue[0], a)){
					token.charvalue[i] = a;
					i++;
				}else{
					fseek(infile,-1,1);
					checkSymbol(&token);
					printf("%3d %3d %s \n",token.attr, token.value, token.charvalue);
					return 0;
				}
			}else if(token.attr == NUMBER && isNumber){
				token.charvalue[i] = a;
				i++;
			}else{
				fseek(infile,-1,1);
				if(token.attr == SYMBOL){
					checkSymbol(&token);
				}else{
					checkRWord(&token);
				}
				printf("%3d %3d %s \n",token.attr, token.value, token.charvalue);
				return 0;
			}
		}
	}
	return EOF;
}

void expression(void){
	if (isplus || isminus) get_token();

	term();
	while (isplus || isminus) {
		get_token();
		term();
	}
}

void term(void){
	factor();
	while (istimes || isdiv) {
		get_token();
		factor();
	}
} 

void factor(void){
	if (isNumbr || isIdentifier) {
		if(isIdentifier){
			if( search(list1, token.charvalue) == NULL){
				printf("\nidentifier %s belum dideklarasikan!", token.charvalue);
				exit(0);
			}
		}
		get_token();
		return;
	} else if (isLParen) {
		get_token();
		expression();
	if (isRParen) {
		get_token();
	return;
	} else printf("Error : Reserved Symbol ')' is expected");exit(-1);
	} else printf("Error : Expression is expected\n");exit(-1);
} 

void paramlist(void){//SETELAH MODUL PARAMLIST JANGAN MANGGIL GET_TOKEN!!!!
	if(isLParen){
		 
		 get_token();
		 expression();
		 
		 while(isComma){
		 	
		 	get_token();
		 	expression();
		 }
		 
		 if(!isRParen){
		 	printf("Error : Symbol RPAREN is expected\n");
			exit(0);
		 }
		 
		 get_token();
	}
}

void condition(void){
	expression();
	
	if(!isEql && !isNotEql && !isGrtrThan && !isLessThan && !isGrtrEql && !isLessEql ){
		printf("%d %d",token.value, EQL);
		printf("Error : Condition Operator is expected\n");
		exit(0);
	}
	
	get_token();
	expression();
}

void statement (void){
	if(isIdentifier){
		if( search(list1, token.charvalue) == NULL){
			printf("\nidentifier %s belum dideklarasikan!", token.charvalue);
			exit(0);
		}
		
		get_token();
		if(isBecomes){
			
			get_token();
			expression();
		}else{
			paramlist();
		}
		
	}else if(isBegin){
		
		get_token();
		statement();
		
		while(isSemicolon){
			get_token();
			statement();
		}
		
		if(!isEnd){
			printf("Error : RWORD 'end' is expected\n");
			exit(0);
		}
		
		get_token();
	}else if(isWhile){
		
		get_token();
		condition();
		
		if(!isDo){
			printf("Error : RWORD 'do' is expected\n");
			exit(0);
		}
		
		get_token();
		statement();
	}else if(isIf){
		
		get_token();
		condition();
		
		if(!isThen){
			printf("Error : RWORD 'do' is expected\n");
			exit(0);
		}
		
		get_token();
		statement();
		
		if(isElse){
			
			get_token();
			statement();
		}
	}else if(isRead){
		
		get_token();
		if(!isLParen){
			printf("Error : Symbol '(' is expected\n");
			exit(0);
		}
		
		get_token();
		if(!isIdentifier){
			printf("Error : IDENTIFIER is expected\n");
			exit(0);
		}
		if( search(list1, token.charvalue) == NULL){
			printf("\nidentifier %s belum dideklarasikan!", token.charvalue);
			exit(0);
		}
		
		get_token();
		while(isComma){
			
			get_token();
			if(!isIdentifier){
				printf("Error : IDENTIFIER is expected\n");
				exit(0);
			}
			if( search(list1, token.charvalue) == NULL){
				printf("\nidentifier %s belum dideklarasikan!", token.charvalue);
				exit(0);
			}
			
			get_token();
		}
		
		if(!isRParen){
			printf("Error : Symbol ')' is expected\n");
			exit(0);
		}
		
		get_token();
	}else if(isWrite){
		
		get_token();
		if(!isLParen){
			printf("Error : Symbol '(' is expected\n");
			exit(0);
		}
		
		get_token();
		expression();
		
		while(isComma){
			
			get_token();
			expression();
		}
		
		if(!isRParen){
			printf("Error : Symbol ')' is expected\n");
			exit(0);
		}
		
		get_token();
	}else{
		printf("ERROR");
		exit(0);
	}
}

void inblock(void){
	if(isLParen){
		
		get_token();
		if(!isIdentifier){
			printf("Error : Identifier is expected\n");
			exit(0);
		}else{
			push(&list1, &token.charvalue[0], VLOCAL, NULL);
		}
		
		get_token();
		while(isComma){
			
			get_token();
			if(!isIdentifier){
				printf("Error : Identifier is expected\n");
				exit(0);
			}else{
				push(&list1, &token.charvalue[0], VLOCAL, NULL);
			}
			
			get_token();
		}
		
		if(!isRParen){
			printf("Error : Symbol RPAREN is expected\n");
			exit(0);
		}
	
		get_token();
	}
	
	if(!isSemicolon){
		printf("Error : Symbol SEMICOLON is expected\n");
		exit(0);
	}
	
	get_token();
	if(isVar){
		
		get_token();
		if(!isIdentifier){
			printf("Error : Identifier is expected\n");
			exit(0);
		}else{
			push(&list1, &token.charvalue[0], VLOCAL, NULL);
		}
		
		get_token();
		while(isComma){
			
			get_token();
			if(!isIdentifier){
				printf("Error : Identifier is expected\n");
				exit(0);
			}else{
				push(&list1, &token.charvalue[0], VLOCAL, NULL);
			}
			
			get_token();
		}
		
		if(!isSemicolon){
			printf("Error : Symbol SEMICOLON is expected\n");
			exit(0);
		}
		
		get_token();
	}
	
	statement();
}

void outblock(void){
	if(!isVar){
		printf("Error : Reserved Word 'var' is expected\n");
		exit(0);
	}
	
	get_token();
	if(!isIdentifier){
		printf("Error : Identifier is expected\n");
		exit(0);
	}else{
		push(&list1, &token.charvalue[0], VGLOBAL, NULL);
	}
	
	get_token();
	while(isComma){
		
		get_token();
		if(!isIdentifier){
			printf("Error : Identifier is expected\n");
			exit(0);
		}else{
			push(&list1, &token.charvalue[0], VGLOBAL, NULL);
		}
		
		get_token();
	}
	
	if(!isSemicolon){
		printf("Error : Symbol 'semicolon' is expected\n");
		exit(0);
	}
	
	get_token();
	while(isProcedure){
		char temp[30];
		
		get_token();
		if(!isIdentifier){
			printf("Error : Identifier is expected\n");
			exit(0);
		}else{
			strcpy(temp,token.charvalue);
			push(&list1, &token.charvalue[0], PNAME, NULL);
		}
		
		get_token();
		inblock();
		
		if(!isSemicolon){
			printf("Error : Semicolon is expected\n");
			exit(0);
		}
		
		while(strcmp(temp, list1.first->key) != 0){
			list1.first = list1.first->next;
		}
		
		get_token();
	}
	
	statement();
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
	}else{
		push(&list1, &token.charvalue[0], NULL, NULL);
	}
	
	get_token();
	if(!isSemicolon){
		printf("Error : Symbol ';' is expected\n");
		exit(0);
	}
	
	get_token();
	outblock();
	
//	get_token();
	if(!isPeriod){
		printf("Error : Symbol '.' is expected\n");
		exit(0);
	}
}

int main(int argc, char * argv[]){
	CreateList(&list1);
	
	char namafile[65] = "Praktikum 7.txt";
	
	printf("%s\n\n",namafile);
	
	if((infile = fopen(namafile, "r")) == NULL){
		printf("File tak dapat dibuka!\r\n");
		exit(1);
	}

	program();
	
	printf("Congratulations : No error is found\n"); 
}
