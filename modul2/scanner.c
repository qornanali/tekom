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
	// initToken("../pascal/program4.txt");
	while(getToken() != EOF) {
		printf("new token %3d %3d %s \n", token.attr, token.value, token.charvalue);
		// getch();	
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
	while(i < RWORDS_SIZE && !isStringEqual(rwords[i], chars)){  //iterasi utnuk ngebandingin apakah ada dalam daftar rword atau engga
		i++;
	}
	return i; //jika i >= RWORDS_SIZE berarti tidak ada di dalam daftar rword
}

int checkSymbol(char * chars){
	char symbols[][2] = {"+", "-", "*", "(", ")", "=", ",", ".", ";", ":=", "<", "<=", "<>", ">", ">=", "[", "]", "..", ":"};
	int i = 0; 
	while(i < SYMBOLS_SIZE && !isStringEqual(symbols[i], chars)){ //iterasi utnuk ngebandingin apakah ada dalam daftar symbol atau engga
		i++;
	}
	return i; //jika i >= SYMBOLS_SIZE berarti tidak ada di dalam daftar symbol
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
	 if(isdigit(*(chars+1))) {
		 return FALSE;
	 }else{
		 return TRUE;
	 }
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
			return EOF;
		}else if(!isWhiteSpace(ch)){
			puts("not whitespace");
			// getch();
			printf("iterate : %c", ch);
			if(isSymbol(ch)){
				chAttr = SYMBOL;
				puts(" SYMBOL");
			}else if(isNumber(ch)){
				chAttr = NUMBER;
				puts(" NUMBER");
			}else if(isAlphabet(ch)){
				chAttr = ALPHABET;
				puts(" ALPHABET");
			}
			// getch();
			if(token.attr != chAttr && i>0){ //jika bertemu dengan karakter yang attr nya berbeda
				puts("1");
				// getch();
				int rwordId = checkRWord(token.charvalue);
				if(isRWordFound(rwordId)){ //mengecek apakah termasuk di daftar kata rword
					puts("1.1");
					// getch();
					token.attr = RWORD;
					found(new_token);
					value = rwordId;
					fseek(infile, -1, SEEK_CUR);
				}else if(token.attr != SYMBOL){ //identifier
					puts("1.2");
					// getch();
					if(isAlphabet(ch) || (isNumber(ch) && i > 0)){	//identifier hanya bisa terdiri dari alfabet dan angka
						puts("1.2.1");
						// getch();
						token.attr = IDENTIFIER;
						token.charvalue[i] = ch;
						i++;
					}else{ //token akan ditemukan jika bertemu selain alfabet dan angka
						puts("1.2.2");
						// getch();
						if(token.attr == ALPHABET){
							token.attr = IDENTIFIER;
						}
						found(new_token);
						fseek(infile, -1, SEEK_CUR);
					}
				}else{ // untuk symbol dan angka
					puts("1.3");
					// getch();
					found(new_token);
					fseek(infile, -1, SEEK_CUR);
				}
			}else{ //mengumpulkan isi token
				puts("2.1");
				// getch();
				token.attr = chAttr;
				token.charvalue[i] = ch;
				i++;
				if(isSymbol(ch)){ //simbol bisa terdiri dari 2 karakter jadi harus dilakukan pengecekan
					puts("2.1.1");
					// getch();
					int symbolId = checkSymbol(token.charvalue);
					if(isSymbolFound(symbolId) ){ //kalau ada di daftar simbol maka token ditemukan
						puts("2.1.1.1");
						// getch();
						if(symbolId != 5 && symbolId != 7 && symbolId != 10 && symbolId != 13 && symbolId != 18){
							found(new_token);
							value = symbolId;
						}
					}else{ //jika tidak, yang dimasukkan tadi kosongkan, dan kursor file dimundurkan satu langkah
						puts("2.1.1.2");
						// getch();
						setNull(token.charvalue[i]);
						i--;
						// fseek(infile, -1, SEEK_CUR);
					}
				}
			}
		}else if(i > 0){ //supaya gak tiap whitespace melakukan pengecekan lagi
			puts(" whitespace");
			// getch();
			if(token.attr == SYMBOL){
				puts("1");
				// getch();
				int symbolId = checkSymbol(token.charvalue);
				if(symbolId != 5 && symbolId != 7 && symbolId != 10 && symbolId != 13 && symbolId != 18){ //bukan =,.,<,>,:
					puts("1.1");
					// getch();
					found(new_token);
					value = symbolId;
				} 
			}else{
				puts("2");
				// getch();
				int rwordId = checkRWord(token.charvalue);
				if(isRWordFound(rwordId)){ //mengecek apakah rword
					puts("2.1");
					// getch();
					token.attr = RWORD;
					value = rwordId;
				}else if (isIdentifier(token.charvalue)){ // mengecek apakah identifier
					puts("2.2");
					// getch();
					token.attr = IDENTIFIER;
				}
				found(new_token); //token ditemukan ketika number,identifier,rword ketemu whitespace
			}
		}else{
			puts("C");
			// getch(); 
		}
	}while(!isEOF(ch) && !isFound(new_token)); //loop selama bukan eof  dan token tidak ditemukan
	setValueToken(); //mengisi value token
}
