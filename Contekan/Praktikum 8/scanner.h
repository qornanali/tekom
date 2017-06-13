/*
 File name : scanner.h
 Vers : 0.01a
 Written by : Setiadi Rachmat
 Date : Fri Aug 28 10:08:03 WIT 2009
*/

#include"compiler.h"
typedef struct _token {
	char attr; /* attribute type */
	char charvalue[30]; /* string */
	int value; /* string value */
} token_t;

/* token type */
#define IDENTIFIER 1
#define NUMBER 2
#define RWORD 3
#define SYMBOL 4 

/* reserved word definition */
#define BEGIN 0
#define DIV 1
#define DO 2
#define ELSE 3
#define END 4
#define IF 5
#define PROCEDURE 6
#define PROGRAM 7
#define THEN 8
#define VAR 9
#define WHILE 10
#define READ 11
#define WRITE 12
#define FORWARD 13
#define FUNCTION 14

/* symbol definition */
#define PLUS 0
#define MINUS 1 /* - */
#define TIMES 2 /* * */
#define LPAREN 3 /* ( */
#define RPAREN 4 /* ) */
#define EQL 5 /* = */
#define COMMA 6 /* , */
#define PERIOD 7 /* . */
#define SEMICOLON 8 /* ; */
#define BECOMES 9 /* := */
#define LESSTHAN 10 /* < */
#define LESSEQL 11 /* <= */
#define NOTEQL 12 /* <> */
#define GRTRTHAN 13 /* > */
#define GRTREQL 14 /* >= */
#define LBRACE 15 /* [ */
#define RBRACE 16 /* ] */
#define INTERVAL 17 /* .. */
#define COLON 18 /* : */


// modul untuk mengecek apakah suatu token termasuk RWORD atau bukan
// membandingkan token yang menjadi input dengan daftar RWORD yang sudah didefinisikan
void checkRWord(token_t *a){
	
	// daftar RWORD yang terdefinisi yang telah disusun sesuai dengan urutan
	char rWord[][30] = {("begin"),("div"),("do"),("else"),("end"),("if"),("procedure"),("program"),("then"),("var"),("while"),("read"),("write"),("forward"),("function")};
	
	//digunakan untuk looping
	int i=0;
	
	//looping membandingkan token yang diinputkan dengan daftar RWORD
	while(i<15){
		
		//membandingkan token dengan salah satu RWORD
		if(strcmp(a->charvalue, rWord[i]) == 0){
			
			//jika suatu token adalah RWORD
			//maka attribut dari token dirubah menjadi 3 atau RWORD, value dari token diisi sesuai dengan urutan RWORD yang sama
			a->attr = 3;
			a->value = i;
			
			//setelah itu keluar dari modul
			break;
		}else{
			i++;
		}
	}	
}

// modul untuk mengecek apakah suatu token yang diinputkan adalah SYMBOL
// membandingkan token yang menjadi input dengan daftar SYMBOL yang sudah didefinisikan
void checkSymbol(token_t *a){
	
	// daftar SYMBOL yang terdefinisi yang telah disusun sesuai dengan urutan
	char symbol[][30] = {("+"),("-"),("*"),("("),(")"),("="),(","),("."),(";"),(":="),("<"),("<="),("<>"),(">"),(">="),("["),("]"),(".."),(":")};
	
	//digunakan untuk looping
	int i=0;
	
	//looping membandingkan token yang diinputkan dengan daftar SYMBOL
	while(i<19){
		
		//membandingkan token dengan salah satu SYMBOL
		if(strcmp(a->charvalue, symbol[i]) == 0){
			
			//jika suatu token adalah SYMBOL
			//maka value dari token diisi sesuai dengan urutan SYMBOL yang sama
			//catatan : attribut token tidak perlu diisi karena sudah diisi dengan SYMBOL atau angka 4 pada modul get token
			a->value = i;
			
			//setelah itu keluar dari modul
			break;
		}else{
			i++;
		}
	}
}


// modul untuk mengecek apakah dua buah symbol input yang digabungkan tersedia pada list SYMBOL yang didefinisikan
// variable a adalah symbol pertama, variable b adalah symbol kedua
bool symbolAvailable(char a,char b){
	
	// daftar SYMBOL yang terdiri dari 2 character yang terdefinisi
	char symbol[][3] = {(":="),("<="),("<>"),(">="),("..")};
	
	//looping untuk membandingkan dengan semua daftar SYMBOL
	for(int i=0; i<=5; i++){
		if(a == symbol[i][0]){
			if(b == symbol[i][1]){
				
				//jika gabungan symbol tersedia maka mengembalikan nilai TRUE
				return true;
			}
		}
	}
	
	//jika gabungan symbol tidak tersedia maka mengembalikan nilai FALSE
	return false;
}


// modul untuk menentukan attribut sementara suatu token
// variable a adalah character pertama dari token yang diinputkan
void tempAttr(token_t *token, char a){
	
	// jika character pertama adalah angka, maka dapat diketahui bahwa token tersebut adalah NUMBER
	if(isNumber){
		(*token).attr = NUMBER;
	
	// jika character pertama adalah huruf, maka token tersebut memiliki 2 kemungkinan yaitu IDENTIFIER atau RWORD
	// dalam kasus ini diisi IDENTIFIER karena nanti akan ada pengecekan dengan memanggil modul CheckRword();
	}else if(isCharacter){
		(*token).attr = IDENTIFIER;
		
	// jika character pertama adalah symbol, maka dapat diketahui bahwa token tersebut adalah SYMBOL
	}else if(isSymbol){
		(*token).attr = SYMBOL;
	}
}

