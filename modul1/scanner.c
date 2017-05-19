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

FILE * infile;
char token[50];

int get_token(void);
void delete_token();
void init_token(char *name);

int main(int argc, char *argv[]){
	init_token(argv[1]);
	while(get_token() != EOF) printf("%s \n",token);
}

void init_token(char *name){
	if((infile = fopen(name, "r")) == NULL){
		printf("Error : Can't open source code %s'");
		exit(-1);
	}else return;
}

int get_token(void){
	int j;
	for(j < 0; j < 50; j++){
		token[j] = '\0';
	}
	int i;
	int token_type = 0;
	char ch;
	i = 0;
	do{
		ch = (char) fgetc(infile);
		if(ch == EOF){
			printf("%s\n",token);
			return EOF;
		}else if((ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a') || (ch <= '9' && ch >= '0')){
				if(token_type == 2){
					i = 0;
				}
				if(i > 0 && token_type != 1){
					token_type = 0;
					fseek(infile, -1, SEEK_CUR);
				}else{
					token_type = 1;
					token[i] = ch;	
					i++;
				}	
		}else if(ch == ' ' || ch == '\n' || ch == '\t'){
			if(i > 0 && token_type != 2){
				token_type = 0;
			}else{
				token_type = 2;	
				i++;
			}
		}else{
			if(token_type == 2){
					i = 0;
			}
			if(i > 0 && token_type != 3){
				token_type = 0;
				fseek(infile, -1, SEEK_CUR);
			}else{
				token_type = 3;
				token[i] = ch;	
				i++;
			}
		} 
	}while((ch != EOF) && token_type != 0);
}
