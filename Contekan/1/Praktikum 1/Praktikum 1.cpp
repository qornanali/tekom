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

#define isDelimiter (a == ' ' || a == '\n' || a == '\t')
#define isCharacter ((a >=65 && a <= 90) || (a >= 97 && a <= 122))
#define isNumber (a >= 48 && a <= 57)
#define isSymbol (!isNumber && !isCharacter)

FILE *infile;
char token[50];

void init_token(char *name){
	if((infile = fopen(name,"r")) == NULL){
		printf("ERROR");
		exit(-1);
	} else return;
}

void hapusToken(void){
	int i = 0;
	while(token[i] != NULL){
		token[i] = '\0';
		i++;
	}
}

int get_token(void){
	hapusToken();
	int i = 0;
	char a;
	while( ( a = fgetc(infile) ) != EOF){
		if(i == 0 && isDelimiter){
		}else if(isDelimiter){
			return i;
		}else if(isSymbol && i != 0){
			printf("%s\r\n",token);
			hapusToken();
			i = 0;
			token[i] = a;
			return i;
		}else {
			token[i] = a;
			i++;
		}
	}
	return EOF;
}

int main(int argc, char * argv[]){
	
	char namafile[65] = "Praktikum 1.txt";
	
	printf("%s\n\n",namafile);
	
	if((infile = fopen(namafile, "r")) == NULL){
		printf("File tak dapat dibuka!\r\n");
		exit(1);
	}

	while (get_token() != EOF){
		printf("%s\n",token);
	}
	
	return 0;
}
