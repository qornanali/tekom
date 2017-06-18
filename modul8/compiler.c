#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <conio.h>
#include <limits.h>
#include <malloc.h>
#include <stdbool.h>
#include "stack.h"
#include "scanner.h"
#include "compiler.h" 

/* MAIN */
token_t token;
FILE * infile;
int t = 0, c = 0, rwordValue;
ListStack stack;

int main(int argc, char *argv[]){
	CreateListStack(&stack);

    initToken(argv[1]);
    program();
    printf("Success : Compilation completed with no error(s)\n");
}



/* COMPILER */

void pushTokenToStack(char type, int addr, int nparam){
	printf("\nPushing item to stack\n");
	infotype info;
	copyString(info.key, token.charvalue);
	info.address = addr;
	info.nparam = nparam;
	info.type = type;
	Push(&stack, &info);
	PrintInfoStack(stack);
}

void popTokenFromStack(void){
	printf("\nPopping item from stack\n");
	Pop(&stack);
	PrintInfoStack(stack);
}

void program(void){
	getToken();printToken(t, token);
	if(!tokenIsProgram(token)){
		error(1, "program");
	}

	getToken();printToken(t, token);
	identifier("program");

	getToken();printToken(t, token);
	semicolon();

	getToken();printToken(t, token);
	outblock();

	if(!tokenIsPeriod(token)){
		error(3, ".");
		exit(-1);
	}
}

void identifier(char * chars){
	if(!tokenIsIdentifier(token)){
		error(2, chars);
	}
}

void semicolon(void){
	if(!tokenIsSemicolon(token)){
		error(3, ";");
	}
}

void end(void){
	if(!tokenIsEnd(token)){
		error(1, "end");
	}
}

void rparen(void){
	if(!tokenIsRParen(token)){
		error(3, ")");
	}
}


void outblock(void){
	if(!tokenIsVar(token)){
		error(1, "var");
	}
	do{
		getToken();printToken(t, token);
		identifier("var");
		addressStack pSearch = search(stack, token.charvalue);
		if(!varIsNull(pSearch)){
			error(7, token.charvalue);
		}
		pushTokenToStack(VGLOBAL, 0, 0);
		getToken();printToken(t, token);
	}while(tokenIsComma(token));

	semicolon();
	
	getToken();printToken(t, token);
	while(tokenIsProcedure(token) || tokenIsFunction(token)){
		getToken();printToken(t, token);
		int type;
		char key[10];
		if(tokenIsProcedure(token)){
			copyString(key, "procedure");
			type = PNAME;
		}else{
			copyString(key, "function");
			type = FNAME;
		}
		identifier(key);
		addressStack pSearch = search(stack, token.charvalue);
		if(!varIsNull(pSearch)){
			error(7, token.charvalue);
		}
		pushTokenToStack(type, 0, 0);
		setStringNull(key, 10);

		getToken();printToken(t, token);
		inblock();
		popTokenFromStack();

		semicolon();

		getToken();printToken(t, token);
	}

	statement();
}

void inblock(void){
	addressStack pMethod = stack.TOP;
	if(tokenIsLParen(token)){
		do{
			getToken();printToken(t, token);
			identifier("(");
			pMethod->info.nparam++;
			addressStack pSearch = search(stack, token.charvalue);
			if(!varIsNull(pSearch)){
				error(7, token.charvalue);
			}
			pushTokenToStack(VLOCAL, pMethod, 0);

			getToken();printToken(t, token);
		}while(tokenIsComma(token));
		rparen();
		
		getToken();printToken(t, token);
	}
	
	semicolon();

	getToken();printToken(t, token);
	if(tokenIsVar(token)){
		do{
			getToken();printToken(t, token);
			identifier("var");
			addressStack pSearch = search(stack, token.charvalue);
			if(!varIsNull(pSearch)){
				error(7, token.charvalue);
			}
			pushTokenToStack(VLOCAL, pMethod, 0);

			getToken();printToken(t, token);
		}while(tokenIsComma(token));

		semicolon();

		getToken();printToken(t, token);
	}

	statement();
}

void statement(void){
	if(tokenIsIdentifier(token)){

		addressStack pSearch = search(stack, token.charvalue);
		if(varIsNull(pSearch)){
			error(6, token.charvalue);
		}

		getToken();printToken(t, token);
		if(tokenIsBecomes(token)){
          	getToken();printToken(t, token); t++;
			expression();
		}else {
			int nparam = paramList();
			if(nparam > pSearch->info.nparam){
				if(pSearch->info.type == PNAME){
					error(9, pSearch->info.key);
				}else{
					error(10, pSearch->info.key);
				}
			}else if(nparam < pSearch->info.nparam){
				if(pSearch->info.type == PNAME){
					error(8, pSearch->info.key);
				}else{
					error(11, pSearch->info.key);
				}
			}
			getToken();printToken(t, token);
		}
	}else if(tokenIsBegin(token)){
		do{
			getToken();printToken(t, token);
			statement();
		}while(tokenIsSemicolon(token));
		end();

        getToken();printToken(t, token);
	}else if(tokenIsWhile(token)){
		getToken();printToken(t, token);
		condition();

		if(!tokenIsDo(token)){
			error(1, "do");
		}
		
		getToken();printToken(t, token);
		statement();
	}else if(tokenIsIf(token)){
		getToken();printToken(t, token);
		condition();
	
		if(!tokenIsThen(token)){
			error(1, "then");
		}

		getToken();printToken(t, token);
		statement();

		if(tokenIsElse(token)){
			getToken();printToken(t, token);
			statement();
		}
	}else if(tokenIsRead(token)){
		getToken();printToken(t, token);
		if(tokenIsLParen(token)){
			do{
				getToken();printToken(t, token);
				identifier("(");
				addressStack pSearch = search(stack, token.charvalue);
				if(varIsNull(pSearch)){
					error(6, token.charvalue);
				}
				if(pSearch->info.type != VLOCAL && pSearch->info.type != VGLOBAL){
					if(pSearch->info.type == PNAME){
						error(12, "variabel");
					}else{
						error(13, "variabel");
					}
				}

				getToken();printToken(t, token);
			}while(tokenIsComma(token));
			rparen();

			getToken();printToken(t, token);
		}else{
			error(3, "(");
		}
	}else if(tokenIsWrite(token)){
		getToken();printToken(t, token);
		if(tokenIsLParen(token)){
			do{
				getToken();printToken(t, token);
				expression();
			}while(tokenIsComma(token));
			rparen();

			getToken();printToken(t, token);
		}else{
			error(3, "(");
		}
	}else {
		end();
	}
}

void expression(void){
	if(tokenIsPlus(token) || tokenIsMinus(token)){
		getToken();printToken(t, token);
	}

	term();
	while(tokenIsPlus(token) || tokenIsMinus(token)){
		getToken();printToken(t, token);
		term();
	}
}

void condition(void){
	expression();

	if(tokenIsEql(token) || tokenIsNotEql(token) ||
	tokenIsLessThan(token) || tokenIsGrtrThan(token) ||
	tokenIsLessEql(token) || tokenIsGrtrEql(token)){
		getToken();printToken(t, token);
		expression();
	}else{
		error(0,"Condition");
	}
}

int paramList(void){
	int i = 0;
	if(tokenIsLParen(token)){
		do{
			getToken();printToken(t, token);
			expression();
			i++;
		}while(tokenIsComma(token));
		rparen();
	}
	return i;
}

void term(void){
	factor(); 
	while(tokenIsTimes(token) || tokenIsDiv(token)){
		getToken();printToken(t, token);
		factor();
	}
}

void factor(void){
	if(tokenIsIdentifier(token)){
		addressStack pSearch = search(stack, token.charvalue);
		if(varIsNull(pSearch)){
			error(6, token.charvalue);
		}	
		if(pSearch->info.type == FNAME){
			getToken();printToken(t, token);
			int nparam = paramList();
			if(nparam > pSearch->info.nparam){
				error(11, pSearch->info.key);
			}else if(nparam < pSearch->info.nparam){
				error(10, pSearch->info.key);
			}
		}else if(pSearch->info.type == PNAME){
			error(12, "function or variabel");
		}
	}else if (tokenIsLParen(token)) {
		printf("kesini");
		getToken();printToken(t, token);
		expression();
		
		rparen();
	} else if(!tokenIsNumber(token)){
		error(0, "Expression");
	}
	getToken();printToken(t, token);
}

void error(int errId, char * chars){
	switch(errId){
		case 0 :
			printf("Error : %s is expected", chars);
		break;
		case 1 :
			printf("Error : Reserved Word '%s' is expected", chars);
		break;
		case 2 :
			printf("Error : Identifier is expected after '%s'", chars);
		break;
		case 3 :
			printf("Error : Symbol '%s' is expected", chars);
		break;
		case 4 :
			printf("Error : Can't open %s", chars);
		break;
		case 5 :
			printf("Error : Operator is expected after Number", chars);
		break;
		case 6 :
			printf("Error : '%s' undeclared", chars);
		break;
		case 7 :
			printf("Error : '%s' already declared", chars);
		break;
		case 8 :
			printf("Error : too few arguments to procedure '%s'", chars);
		break;
		case 9 :
			printf("Error : too much arguments to procedure '%s'", chars);
		break;
		case 10 :
			printf("Error : too few arguments to function '%s'", chars);
		break;
		case 11 :
			printf("Error : too much arguments to function '%s'", chars);
		break;
		case 12 :
			printf("Error : expected %s found procedure", chars);
		break;
		case 13 :
			printf("Error : expected %s found function", chars);
		break;
		case 14 :
			printf("Error : expected %s found variabel", chars);
		break;
		default :
			printf("Error : Unindentified");
		break;
	}
	printf(" on %d\n", c);
    exit(-1);
}

/* SCANNER */

void initToken(char * name){
	if((infile = fopen(name, "r")) == NULL){
		error(4, name);
	}else return;
}

int checkRWord(char * chars){
	char rwords[RWORDS_SIZE][10] = {"begin","div","do", "else", "end", "if", "procedure", "program", "then", "var", "while", "read", "write", "forward", "function"};
	int i = 0;
	while(i < RWORDS_SIZE && !stringIsEqual(rwords[i], chars)){  
		i++;
	}
	return i; 
}

int checkSymbol(char * chars){
	char symbols[SYMBOLS_SIZE][3] = {"+", "-", "*", "(", ")", "=", ",", ".", ";", ":=", "<", "<=", "<>", ">", ">=", "[", "]", "..", ":"};
	int i = 0; 
	while(i < SYMBOLS_SIZE && !stringIsEqual(symbols[i], chars)){ 
		i++;
	}
	return i; 
}

void clearToken(void){
	setStringNull(token.charvalue, 50);
}

int getToken(void){
	clearToken();
	int tempVal;
	char c1 = fgetc(infile);
	c++;
	if(charIsWhiteSpace(c1)){
		getToken();
	}else if(charIsSymbol(c1)){
		char chtemp[3];
        token.charvalue[0] = c1;
		token.attr = SYMBOL;
		char c2;
		do{
			c2 = fgetc(infile);
			c++;
			if(charIsSymbol(c2) && (c2 == '=' || c2 == '>' || c2 == '.')){
				chtemp[0] = c1;
				chtemp[1] = c2;
				setCharNull(chtemp[2]);
			}
		}while(!charIsSymbol(c2) && charIsWhiteSpace(c2));
		if(stringIsSymbol(tempVal = checkSymbol(chtemp))){
			copyString(token.charvalue, chtemp);
			token.value = tempVal;
		}else{
			token.value = checkSymbol(token.charvalue);
			if(!charIsEOF(c2)){
            	moveFileCursor(infile, -1);
				c--;
			}
		}
		setStringNull(chtemp, 3);
		t++;
		return c1;
	}else{
		int i = 0;
		do{
			if(charIsEOF(c1)){
				return c1;
			}else{
				token.charvalue[i] = c1;
				i++;
				c1 = fgetc(infile);
				c++;
			}
		}while(charIsAlphabet(c1) || charIsNumber(c1));

		int j = 0, isNumber = TRUE;
		while(!varIsNull(token.charvalue[j]) && isNumber == TRUE){
			if(!isdigit(token.charvalue[j])){
				isNumber = FALSE;
			}
			j++;
		}

		if(isNumber == TRUE){
			token.attr = NUMBER;
			token.value = atoi(token.charvalue);
		}else{
			token.value = checkRWord(token.charvalue);
			if(stringIsRword(token.value)){
				rwordValue = token.value;
				token.attr  = RWORD;
			}else{
				token.value = rwordValue;
				token.attr = IDENTIFIER;
			}
		}
        moveFileCursor(infile, -1);
		c--;
		t++;
		return c1;
	}
}

/* STACK */

void CreateListStack (ListStack *L)
{
  L->TOP= Nil;
}

addressStack AlokasiStack (infotype *X)
{
 addressStack P = (addressStack)malloc(sizeof(ElmtListStack));
			 if (P != Nil) 
			 {
					 strcpy(P->info.key,X->key);
					 P->info.type = X->type;
					 P->info.address = X->address;
					 P->info.nparam = X->nparam;
					 P->next = Nil;
			 }
			 
 return (P);
}

void DeAlokasiStack (addressStack P)
{
		 if (P != Nil)
		 {
		 	free (P);
		 }
} 

void Push (ListStack * L, infotype * X)
{
 addressStack P;
 P = AlokasiStack (&(*X));
 
 	if (P != Nil)
 	{ 			
	 		InsertTOP (&(*L), P); 
 	}
}

void InsertTOP (ListStack * L, addressStack P)
{
 P->next= L->TOP;
 L->TOP = P;
}

void Pop (ListStack *L)
{
 addressStack P;
 P = L->TOP;
 L->TOP= L->TOP->next;
 DeAlokasiStack (P);
} 

void PrintInfoStack (ListStack L)
{
	int i = 0;
 addressStack P;
 if (L.TOP == Nil)
 {
 		printf ("ListStack Kosong .... \a\n");
 }
  else 
	 {
	 		P = L.TOP;
			for (;;)
	 		{
				 if (P == Nil)
				 {
					 printf("\n");
					 break;
				 }
				 else 
					 {
						 printf("no : %d ; key : %s\n", i, P->info.key);
						 printf ("type : %d ; address : %d ; nparam : %d\n", P->info.type, P->info.address, P->info.nparam);
						 P = P->next;
						 i++;
					 }
			}
	 }
}

addressStack search(ListStack L, char key[]) {
	addressStack P=L.TOP;
	while(P != NULL) {
		if(strcmp(P->info.key, key)==0) {
			return P;
		}
		P=P->next;
	}
	return NULL;
} 

void PopVarLocal(ListStack *L) {
	addressStack P = L->TOP;
	while(P->info.type == VLOCAL) {
		Pop(&(*L));
		P=P->next;
	}
}