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
#include <windows.h>
#include "compiler.h"
#include "expression.h"
#include "list.h"
#include "generator.h"

void function(alamat);
void expression(void);
void term(void);
void factor(void);

FILE *infile;
token_t token;
Stack list1;

//modul untuk menghapus token, menghapus histori token sebelum mengambil token baru
void hapusToken(token_t *a){
	int i = 0;
	while(token.charvalue[i] != NULL){
		token.charvalue[i] = '\0';
		i++;
	}
	token.attr = NULL;
	token.value = NULL;
}


//modul untuk mengambil suatu token atau token selanjutnya
int get_token(token_t *token, FILE *infile){
	
	//memastikan bahwa variable token berisi NULL
	hapusToken(token);
	
	//digunakan untuk index character yang diambil
	int i = 0;
	
	//digunakan untuk menampung sebuah karakter yang diambil dari file input
	char a;
	
	//mengambil sebuah karakter dari file secara terus menerus sampai habis
	while( ( a = fgetc(infile) ) != EOF){
		
		//tanda apabila karekter yang diambil adalah karakter pertama untuk suatu token
		if(i==0){
			
			//sebuah token tidak boleh diawali oleh delimiter
			//jika karakter yang diambil adalah sebuah delimiter, maka harus mengambil karakter baru
			//selain itu, akan dimasukkan ke variable token
			if(!isDelimiter){
				
				//karakter pertama digunakan untuk menentukan attribut sementara sebuah token
				tempAttr(token, a);
				(*token).charvalue[i] = a;
				i++;
			}
		
		//algoritma untuk karakter kedua dan seterusnya...
		}else{
			
			//jika attribut sementara sebuah token adalah IDENTIFIER, maka karakter selanjutnya harus huruf atau angka
			if((*token).attr == IDENTIFIER && (isCharacter || isNumber)){
				(*token).charvalue[i] = a;
				i++;
			
			//jika attribut sementara sebuah token adalah SYMBOL, maka karakter selanjutnya harus symbol juga
			}else if((*token).attr == SYMBOL && isSymbol){
				
				//harus ada pengecekan apakah symbol pertama dan symbol kedua jika digabungkan membentuk sebuah SYMBOL yang terdefinisi atau tidak
				//memanggil sebuah modul untuk mengecek ketersedian SYMBOL yang terdiri dari 2 karakter
				//jika SYMBOL tersedia maka lanjut
				if(symbolAvailable((*token).charvalue[0], a)){
					(*token).charvalue[i] = a;
					i++;
				
				//jika membentuk sebuah SYMBOL yang tidak valid, maka symbol pertama dan symbol kedua adalah token yang berbeda dan harus ada pemisahan
				}else{
					
					//fseek digunakan untuk mundur satu langkah pembacaan karakter pada file input, sehingga symbol kedua akan dibaca lagi
					fseek(infile,-1,1);
					
					//menentukan hasil akhir dari sebuah token yang berisi SYMBOL
					checkSymbol(token);
					printf("%3d %3d %s \n",(*token).attr, (*token).value, (*token).charvalue);
					return 0;
				}
			
			//jika attribut sementara sebuah token adalah NUMBER, maka karakter selanjutnya harus angka juga
			}else if((*token).attr == NUMBER && isNumber){
				(*token).charvalue[i] = a;
				i++;
				
			//jika ketiga syarat sebuah token tidak terpenuhi, maka karakter yang diambil adalah karakter pertama sebuah token yang baru
			}else{
				
				//fseek digunakan untuk mundur satu langkah pembacaan karakter pada file input, sehingga karakter yang telah diambil akan diambil kembali
				fseek(infile,-1,1);
				
				//menenentukan hasil akhir dari sebuah token
				//jika attribut sementara sebuah token adalah SYMBOL
				if((*token).attr == SYMBOL){
					
					//menentukan hasil akhir dari sebuah token yang berisi SYMBOL
					checkSymbol(token);
				
				//jika attribut sementara sebuah token adalah IDENTIFIER, maka harus ada pengecekan apakah token tersebut sebuah RWORD
				}else{
					checkRWord(token);
				}
				
				printf("%3d %3d %s \n",(*token).attr, (*token).value, (*token).charvalue);
				return 0;
			}
		}
	}
	return EOF;
}

void function(alamat a){
	get_token(&token, infile);
	if(!isLParen){
		printf("Error : Symbol 'LPAREN' is expected");
		exit(0);
	}
	
	int jumlahParam = a->nparam;
	while(jumlahParam != 0){
		jumlahParam--;
		
		get_token(&token, infile);
		expression();
		
		if(jumlahParam != 0){
			if(!isComma){
				printf("Error : Symbol 'COMMA' is expected");
				exit(0);
			}
		}
	}
	
	if(!isRParen){
		printf("Error : Symbol 'RPAREN' is expected");
		exit(0);
	}
}

void expression(void){
	
	int sign = PLUSOP;
	
	if (isplus || isminus){
		if (isminus) sign = MINUSOP;
		get_token(&token, infile);
	}
	
	term();
	
	if (sign == MINUSOP){
		gencode(OPR,0,0,0);
	}
	
	while (isplus || isminus) {
		if(isplus){
			sign = PLUSOP;
		}else{
			sign = MINUSOP;
		}
		
		get_token(&token, infile);
		term();
		
		if(sign == MINUSOP) gencode(OPR,0,0,2);
		else gencode(OPR,0,0,1);
	}
}

void term(void){
	int sign = TIMESOP;
	factor();
	while(istimes || isdiv){
		if(isdiv){
			sign = DIVOP;
		}
		
		get_token(&token, infile);
		factor();
		
		if(sign == TIMESOP){
			gencode(OPR,0,0,TIMESOP);
		}else{
			gencode(OPR,0,0,DIVOP);
		}
	}
}

void factor(void){
	if (isNumbr || isIdentifier) {
		if(isIdentifier){
			alamat a = search(list1, token.charvalue);
			if( a  == NULL){
				printf("\nidentifier %s belum dideklarasikan!", token.charvalue);
				exit(0);
			}
			gencode(LOD,0,0,a->address);
			
			if(a->type == FNAME){
				function(a);
			}
		}else{
			gencode(LIT,0,0,toInteger(token.charvalue));
		}
		get_token(&token, infile);
		return;
	} else if (isLParen) {
		get_token(&token, infile);
		expression();
		if (isRParen) {
			get_token(&token, infile);
		return;
		} else printf("Error : Reserved Symbol ')' is expected");exit(-1);
	} else printf("Error : Expression is expected\n");exit(-1);
} 

void paramlist(void){//SETELAH MODUL PARAMLIST JANGAN MANGGIL GET_TOKEN!!!!
	if(isLParen){
		 
		 get_token(&token, infile);
		 expression();
		 
		 while(isComma){
		 	
		 	get_token(&token, infile);
		 	expression();
		 }
		 
		 if(!isRParen){
		 	printf("Error : Symbol RPAREN is expected\n");
			exit(0);
		 }
		 
		 get_token(&token, infile);
	}
}

void condition(void){
	expression();
	
	if(!isEql && !isNotEql && !isGrtrThan && !isLessThan && !isGrtrEql && !isLessEql ){
		printf("Error : Condition Operator is expected\n");
		exit(0);
	}
	
	get_token(&token, infile);
	expression();
}

void statement (void){
	if(isIdentifier){
		alamat P = search(list1, token.charvalue);
		if( P == NULL){
			printf("\nidentifier %s belum dideklarasikan!", token.charvalue);
			exit(0);
		}
		
		get_token(&token, infile);
		if(isBecomes){
			
			get_token(&token, infile);
			expression();
			
			gencode(STO,0,0,P->address);
		}else{
			paramlist();
		}
		
	}else if(isBegin){
		
		get_token(&token, infile);
		statement();
		
		while(isSemicolon){
			get_token(&token, infile);
			statement();
		}
		
		if(!isEnd){
			printf("Error : RWORD 'end' is expected\n");
			exit(0);
		}
		
		get_token(&token, infile);
	}else if(isWhile){
		
		get_token(&token, infile);
		condition();
		
		if(!isDo){
			printf("Error : RWORD 'do' is expected\n");
			exit(0);
		}
		
		get_token(&token, infile);
		statement();
	}else if(isIf){
		
		get_token(&token, infile);
		condition();
		
		if(!isThen){
			printf("Error : RWORD 'do' is expected\n");
			exit(0);
		}
		
		get_token(&token, infile);
		statement();
		
		if(isElse){
			
			get_token(&token, infile);
			statement();
		}
	}else if(isRead){
		
		get_token(&token, infile);
		if(!isLParen){
			printf("Error : Symbol '(' is expected\n");
			exit(0);
		}
		
		get_token(&token, infile);
		if(!isIdentifier){
			printf("Error : IDENTIFIER is expected\n");
			exit(0);
		}
		if( search(list1, token.charvalue) == NULL){
			printf("\nidentifier %s belum dideklarasikan!", token.charvalue);
			exit(0);
		}
		
		get_token(&token, infile);
		while(isComma){
			
			get_token(&token, infile);
			if(!isIdentifier){
				printf("Error : IDENTIFIER is expected\n");
				exit(0);
			}
			if( search(list1, token.charvalue) == NULL){
				printf("\nidentifier %s belum dideklarasikan!", token.charvalue);
				exit(0);
			}
			
			get_token(&token, infile);
		}
		
		if(!isRParen){
			printf("Error : Symbol ')' is expected\n");
			exit(0);
		}
		
		get_token(&token, infile);
	}else if(isWrite){
		
		get_token(&token, infile);
		if(!isLParen){
			printf("Error : Symbol '(' is expected\n");
			exit(0);
		}
		
		get_token(&token, infile);
		expression();
		
		while(isComma){
			
			get_token(&token, infile);
			expression();
		}
		
		if(!isRParen){
			printf("Error : Symbol ')' is expected\n");
			exit(0);
		}
		
		get_token(&token, infile);
	}else{
		printf("ERROR");
		exit(0);
	}
}

//modul untuk mengecek struktur inblock
//sebuah inblock dibentuk oleh :
// 1. jika diawali SYMBOL lparen '(', maka selanjutnya masuk syarat ke 1.a.
//    1.a. minimal satu IDENTIFIER dan dipisahkan oleh SYMBOL comma ','
//    1.b. SIMBOL rparen ')'
// 2. SYMBOL semicolon ';'
// 3. jika dilanjut oleh RWORD var, maka selanjutnya masuk syarat ke 3.a.
//    3.a. minimal satu IDENTIFIER dan dipisahkan oleh SYMBOL comma ','
//    3.b. SIMBOL semicolon ';''
// 4. Struktur Statement
//
// initial statenya variable token berisi token yang baru diambil
void inblock(void){
	
	// jika token yang diambil adalah SYMBOL lparen '(', maka harus masuk ke syarat 1.a.
	if(isLParen){
		
		//AMBIL TOKEN
		get_token(&token, infile);
		
		//token yang diambil harus IDENTIFIER
		if(!isIdentifier){
			printf("Error : Identifier is expected\n");
			exit(0);
		}else{
			
			//IDENTIFIER yang diambil harus dimasukkan kedalam stack sebagai variable lokal
			push(&list1, &token.charvalue[0], VLOCAL, NULL, NULL);
		}
		
		//untuk menghitung jumlah parameter
		int countParam = 1;
		
		//AMBIL TOKEN
		get_token(&token, infile);
		
		//pengecekan apakah terdiri dari 2 atau lebih parameter suatu function atau procedure ditandai dengan SYMBOL comma ','
		while(isComma){
			
			//AMBIL TOKEN
			get_token(&token, infile);
			
			//token yang diambil harus IDENTIFIER
			if(!isIdentifier){
				printf("Error : Identifier is expected\n");
				exit(0);
			}else{
				
				//IDENTIFIER yang diambil harus dimasukkan kedalam stack sebagai variable lokal
				push(&list1, &token.charvalue[0], VLOCAL, NULL, NULL);
			}
			
			//jumlah parameter bertambah satu
			countParam++;
			
			//AMBIL TOKEN
			get_token(&token, infile);
			
			//JIKA token yang diambil adalah SYMBOL comma ',' maka akan mengulan proses diatas
			//jika tidak, lanjut ke bawah TANPA AMBIL TOKEN LAGI
		}
		
		//mengubah jumlah parameter dari function atau procedure yang aktif dengan jumlah parameter yang telah dihitung
		ChangeParam(list1,countParam);
		
		
		//token yang diambil harus SYMBOL rparen ')'
		if(!isRParen){
			printf("Error : Symbol RPAREN is expected\n");
			exit(0);
		}
		
		//AMBIL TOKEN
		get_token(&token, infile);
		//LANJUT KE SYARAT 2.
	}
	
	//token yang diambil harus SYMBOL semicolon ';'
	if(!isSemicolon){
		printf("Error : Symbol SEMICOLON is expected\n");
		exit(0);
	}
	
	//AMBIL TOKEN
	get_token(&token, infile);
	
	//jika token yang diambil adalah RWORD var, maka lanjut ke syarat 3.a.
	if(isVar){
		
		//AMBIL TOKEN
		get_token(&token, infile);
		
		//token yang diambil harus IDENTIFIER
		if(!isIdentifier){
			printf("Error : Identifier is expected\n");
			exit(0);
		}else{
			
			//IDENTIFIER yang diambil harus dimasukkan kedalam stack sebagai variable lokal
			push(&list1, &token.charvalue[0], VLOCAL, NULL, NULL);
		}
		
		
		//AMBIL TOKEN
		get_token(&token, infile);
		
		//pengecekan apakah masih pendefinisian variable atau tidak
		//jika token yang diambil adalah comma, maka masih!, selain itu lanjut ke bawah
		while(isComma){
			
			
			//AMBIL TOKEN
			get_token(&token, infile);
			
			//token yang diambil harus IDENTIFIER
			if(!isIdentifier){
				printf("Error : Identifier is expected\n");
				exit(0);
			}else{
				
				//IDENTIFIER yang diambil harus dimasukkan kedalam stack sebagai variable lokal
				push(&list1, &token.charvalue[0], VLOCAL, NULL, NULL);
			}
			
			//AMBIL TOKEN
			get_token(&token, infile);
			
			//JIKA token yang diambil adalah SYMBOL comma ',' maka akan mengulan proses diatas
			//jika tidak, lanjut ke bawah TANPA AMBIL TOKEN LAGI
		}
		
		//token yang diammbil harus SYMBOL semicolon ';'
		if(!isSemicolon){
			printf("Error : Symbol SEMICOLON is expected\n");
			exit(0);
		}
		
		//AMBIL TOKEN
		get_token(&token, infile);
	}
	
	//MASUK Struktur STATEMENT
	statement();
}

//modul untuk mengecek struktur outblock
//sebuah outblock dibentuk oleh :
// 1. RWORD var
// 2. minimal satu IDENTIFIER dan dipisahkan oleh SYMBOL comma ','
// 3. SYMBOL semicolon ';'
// 4. boleh lebih dari satu atau tidak ada RWORD procedure maupun RWORD function, jika ada RWORD tersebut maka selanjutnya masuk ke syarat 4.a
//    4.a. IDENTIFIER
//    4.b. Stuktur INBLOCK
//    4.c. SYMBOL semicolon ';'
// 5. Struktur Statement
//
// initial statenya variable token berisi token yang baru diambil
void outblock(void){
	
	//token yang diambil harus RWORD var
	if(!isVar){
		printf("Error : Reserved Word 'var' is expected\n");
		exit(0);
	}
	
	int jumlahVariable = 0;
	
	//AMBIL TOKEN
	get_token(&token, infile);
	
	//token yang diambil harus IDENTIFIER
	if(!isIdentifier){
		printf("Error : Identifier is expected\n");
		exit(0);
	}else{
		
		//IDENTIFIER yang diambil harus dimasukkan kedalam stack sebagai variable GLOBAL
		push(&list1, &token.charvalue[0], VGLOBAL, NULL, jumlahVariable);
		jumlahVariable++;
	}
	
	//AMBIL TOKEN
	get_token(&token, infile);
	
	//jika token yang diambil adalah SYMBOL comma ',', maka proses masih dalam pendefinisian variable
	while(isComma){
		
		//AMBIL TOKEN
		get_token(&token, infile);
		
		//token yang diambil harus IDENTIFIER
		if(!isIdentifier){
			printf("Error : Identifier is expected\n");
			exit(0);
		}else{
			
			//IDENTIFIER yang diambil harus dimasukkan kedalam stack sebagai variable GLOBAL
			push(&list1, &token.charvalue[0], VGLOBAL, NULL, jumlahVariable);
			jumlahVariable++;
		}
		
		//AMBIL TOKEN
		get_token(&token, infile);
		
		//JIKA token yang diambil adalah SYMBOL comma ',' maka akan mengulan proses diatas
		//jika tidak, lanjut ke bawah TANPA AMBIL TOKEN LAGI
	}
	
	gencode(INT,0,0,jumlahVariable);
	
	//token yang diambil harus SYMBOL semicolon ','
	if(!isSemicolon){
		printf("Error : Symbol 'semicolon' is expected\n");
		exit(0);
	}
	
	//AMBIL TOKEN
	get_token(&token, infile);
	
	// pengecekan apakah masuk pendefinisan sebuah procedure atau function
	// jika iya maka melakukan pengecekan struktur nya
	// jika tidak, lanjut ke bawah
	while(isProcedure || isFunction){
		
		//tempat untuk menyimpan nama function atau procedure yang sedang dicek
		char temp[30];
		
		//AMBIL TOKEN
		get_token(&token, infile);
		
		//token yang diambil harus IDENTIFIER
		if(!isIdentifier){
			printf("Error : Identifier is expected\n");
			exit(0);
		}else{
			
			//copy nama procedure atau function agar hilang oleh token yang baru
			strcpy(temp,token.charvalue);
			
			//IDENTIFIER yang diambil harus dimasukkan kedalam stack sebagai FUNCTION
			push(&list1, &token.charvalue[0], FNAME, NULL, NULL);
			
		}
		
		//AMBIL TOKEN
		get_token(&token, infile);
		
		//MASUK STRUKTUR INBLOCK
		inblock();
		
		// TIDAK PERLU AMBIL TOKEN, karena final state dari inblock mengakibatkan token selanjutnya terambil
		// token yang diambil harus SYMBOL semicolon ';'
		if(!isSemicolon){
			printf("Error : Semicolon is expected\n");
			exit(0);
		}
		
		//menghapus semua variable lokal sebuah FUNCTION atau PROCEDURE
		while(strcmp(temp, list1.first->key) != 0){
			list1.first = list1.first->next;
		}
		
		//AMBIL TOKEN
		get_token(&token, infile);
		
		//JIKA token yang diambil adalah RWORD procedure atau function, maka akan mengulang proses diatas
		//jika tidak, lanjut ke bawah TANPA AMBIL TOKEN LAGI
	}
	
	statement();
}

//modul untuk mengecek struktur program
//sebuah program dibentuk oleh :
// 1. RWORD program
// 2. IDENTIFIER
// 3. SYMBOL semicolon ';'
// 4. Struktur OUTBLOCK
// 5. SYMBOL period '.'
void program(void){
	
	//AMBIL TOKEN!!!
	get_token(&token, infile);
	
	//token yang diambil harus RWORD program
	if(!isProgram){
		printf("Error : Reserved Word 'program' is expected\n");
		exit(0);
	}
	
	//ABMIL TOKEN
	get_token(&token, infile);
	
	//token yang diambil harus IDENTIFIER
	if(!isIdentifier){
 		printf("Error : Identifier is expected after 'program' \n");
		exit(0);
	}else{
		
		//identifier yang diambil harus dimasukkan kedalam stack sebagai variable GLOBAL
		push(&list1, &token.charvalue[0], VGLOBAL, NULL, NULL);
	}
	
	//AMBIL TOKEN
	get_token(&token, infile);
	
	// token yang diambil harus SYMBOL semicolon ';'
	if(!isSemicolon){
		printf("Error : Symbol ';' is expected\n");
		exit(0);
	}
	
	//AMBIL TOKEN
	get_token(&token, infile);
	outblock();
	
	// TIDAK PERLU AMBIL TOKEN, karena final state dari outblock mengakibatkan token selanjutnya terambil
	// token yang diambil harus SYMBOL period '.'
	if(!isPeriod){
		printf("Error : Symbol '.' is expected\n");
		exit(0);
	}
	gencode(HLT,0,0,0);
}

int main(int argc, char * argv[]){
	
	FILE *fp;
	fp=fopen("generated_code.txt", "w");
	
	//membuat sebuah list untuk menampung variable-variable, procedure, function
	CreateList(&list1);
	
	//nama file input
	char namafile[65] = "Praktikum 11.txt";
	
	printf("%s\n\n",namafile);
	
	if((infile = fopen(namafile, "r")) == NULL){
		printf("File tak dapat dibuka!\r\n");
		exit(1);
	}

	//memanggil modul program
	program();
	
	printf("Congratulations : No error is found\n"); 
}
