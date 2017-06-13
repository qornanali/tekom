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
#define isNull(X) ((X) == '\0' || (X) == NULL)
#define isIdentifier(X) (checkIdentifier(X) == strlen(X))

/* Global variable */
FILE * infile;
token_t token;

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
	char rwords[][10] = {"begin","div","do", "else", "end", "if", "procedure", "program", "then", "var", "while", "read", "write", "forward", "function"};
	int i = 0;
	while(i < 15 && !isStringEqual(rwords[i], chars)){  //iterasi utnuk ngebandingin apakah ada dalam daftar rword atau engga
		i++;
	}
	return i; //jika i >= 15 berarti tidak ada di dalam daftar rword
}

int checkSymbol(char * chars){
	char symbols[19][2] = {"+", "-", "*", "(", ")", "=", ",", ".", ";", ":=", "<", "<=", "<>", ">", ">=", "[", "]", "..", ":"};
	int i = 0; 
	while(i < 19 && !isStringEqual(symbols[i], chars)){ //iterasi utnuk ngebandingin apakah ada dalam daftar symbol atau engga
		i++;
	}
	return i; //jika i >= 19 berarti tidak ada di dalam daftar symbol
}


void clearToken(void){
	int i = 0;
	while(!isNull(token.charvalue[i])){ //printf itu sampai menemukan \0 jadi pembuatan null ini lebih efesien
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
			rword_value = val;
		}
	}else if(token.attr == IDENTIFIER){
		token.value = rword_value;
	}
}

int checkIdentifier(char * chars){
	int x = 0;
    while(x < strlen(chars)) {
           if(!isdigit(*(chars+x))) 
           x++;
    }
    return x;
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
			printf("%3d %3d %s \n", token.attr, token.value, token.charvalue);
			return EOF;
		}else if(!isWhiteSpace(ch)){
			if(isSymbol(ch)){
				chAttr = SYMBOL;
			}else if(isNumber(ch)){
				chAttr = NUMBER;
			}
			if(token.attr != chAttr && !isNull(token.attr)){ //jika bertemu dengan karakter yang attr nya berbeda
				if(isRWordFound((checkRWord(token.charvalue)))){ //mengecek apakah termasuk di daftar kata rword
					token.attr = RWORD;
					found(new_token);
					fseek(infile, -1, SEEK_CUR);
				}else if(token.attr != SYMBOL || token.attr == IDENTIFIER){ //identifier
					if(isAlphabet(ch) || (isNumber(ch) && i > 0)){	//identifier terdiri dari alfabet dan angka
						token.attr = IDENTIFIER;
						token.charvalue[i] = ch;
						i++;
					}else{ //token akan ditemukan jika bertemu selain alfabet dan angka
						found(new_token);
						fseek(infile, -1, SEEK_CUR);
					}
				}else{ // untuk symbol dan angka
					found(new_token);
					fseek(infile, -1, SEEK_CUR);
				}
			}else{ //mengumpulkan isi token
				token.attr = chAttr;
				token.charvalue[i] = ch;
				i++;
				if(isSymbol(ch)){ //simbol bisa terdiri dari 2 karakter jadi harus dilakukan pengecekan
					if(isSymbolFound(checkSymbol(token.charvalue))){ //kalau ada di daftar simbol maka token ditemukan
						found(new_token);
					}else{ //jika tidak, yang dimasukkan tadi kosongkan, dan kursor file dimundurkan satu langkah
						setNull(token.charvalue[i]);
						i--;
						fseek(infile, -1, SEEK_CUR);
					}
				}
			}
		}else if(i > 0 && isWhiteSpace(ch)){ //supaya gak tiap whitespace melakukan pengecekan lagi
			if(token.attr == SYMBOL){
				int symbolId = checkSymbol(token.charvalue);
				if(symbolId != 5 && symbolId != 7 && symbolId != 10 && symbolId != 13){ //bukan <,>,.,=
					found(new_token);
				} 
			}else{
				if(isRWordFound((checkRWord(token.charvalue)))){ //mengecek apakah rword
					token.attr = RWORD;
				}else if (isIdentifier(token.charvalue)){ // mengecek apakah identifier
					token.attr = IDENTIFIER;
				}	
				found(new_token); //token ditemukan ketika number,identifier,rword ketemu whitespace
			}
		}
	}while(!isEOF(ch) && !isFound(new_token)); //loop selama bukan eof  dan token tidak ditemukan
	setValueToken(); //mengisi value token
}
