#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include "compiler.h" 
#include <conio.h>

/* Global variable */
token_t token;
FILE * infile;

int main(int argc, char *argv[]){
    initToken(argv[1]);
    program();
    printf("Congratulations: No error is found\n");
}

void program(void){
    getToken();
	printf("token %3d %3d %s \n", token.attr, token.value, token.charvalue);
	// getch();
    if (!tokenIsProgram(token)){
        printf("Error : Reserved Word 'program' is expected\n");
        exit(-1);
    }

    getToken();
	printf("token %3d %3d %s \n", token.attr, token.value, token.charvalue);
	// getch();
    if (!tokenIsIdentifier(token)){
        printf("Error : Identifier is expected after 'program' \n");
        exit(-1);
    }

    getToken();
	printf("token %3d %3d %s \n", token.attr, token.value, token.charvalue);
    if (!tokenIsSemicolon(token)){
        printf("Error : Symbol ';' is expected\n");
        exit(-1);
    }

    getToken();
    statement();
	
    getToken();
	printf("token %3d %3d %s \n", token.attr, token.value, token.charvalue);
    if (!tokenIsPeriod(token)){
        printf("Error : Symbol '.' is expected\n");
        exit(-1);
    }
}

void statement(void){
    if(tokenIsBegin(token)){
		getToken();
		printf("token %3d %3d %s \n", token.attr, token.value, token.charvalue);
		statement();

		getToken();
		printf("token %3d %3d %s \n", token.attr, token.value, token.charvalue);
		while(tokenIsSemicolon(token)){
			getToken();
			printf("token %3d %3d %s \n", token.attr, token.value, token.charvalue);
			statement();
			
			getToken();
			printf("token %3d %3d %s \n", token.attr, token.value, token.charvalue);
		}
		
		if(!tokenIsEnd(token)){
			printf("Error : Reserved Word 'end' is expected\n");
			exit(-1);
		}
		
	}else if(tokenIsNumber(token)){
		getToken();
		printf("token %3d %3d %s \n", token.attr, token.value, token.charvalue);
		
		if(!tokenIsOperator(token)){
			printf("Error : Reserved symbol 'operator' is expected\n");
			exit(-1);
		}
		
		getToken();
		printf("token %3d %3d %s \n", token.attr, token.value, token.charvalue);
		
		if(!tokenIsNumber(token)){
			printf("Error : Number is expected\n");
			exit(-1);
		}
	}else{
		printf("Error : Reserved word or number expected\n");
		exit(-1);
	}
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
	while(i < RWORDS_SIZE && !stringIsEqual(rwords[i], chars)){  
		i++;
	}
	return i; 
}

int checkSymbol(char * chars){
	char symbols[][2] = {"+", "-", "*", "(", ")", "=", ",", ".", ";", ":=", "<", "<=", "<>", ">", ">=", "[", "]", "..", ":"};
	int i = 0; 
	while(i < SYMBOLS_SIZE && !stringIsEqual(symbols[i], chars)){ 
		i++;
	}
	return i; 
}

void clearToken(void){
	setStringNull(token.charvalue, 50);
	setVarNull(token.attr);
	setVarNull(token.value);
}

int getToken(void){
	clearToken();
	int i = 0;
	static char tempVal;
	char c1 = fgetc(infile);
	if(charIsWhiteSpace(c1)){
		getToken();
	}else if(charIsSymbol(c1)){
		char chtemp[2];
        token.charvalue[0] = c1;
		token.attr = SYMBOL;
		char c2 =  fgetc(infile);
		if(c2 == '=' || c2 == '>' || c2 == '.'){
			chtemp[0] = c1;
			chtemp[1] = c2;
		}
		tempVal = checkSymbol(chtemp);
		if(stringIsSymbol(tempVal)){
			copyString(token.charvalue, chtemp);
			token.value = tempVal;
		}else{
			token.value = checkSymbol(token.charvalue);
			if(!charIsEOF(c2)){
            	moveFileCursor(infile, -1);
			}
		}
		return c1;
	}else{
		do{
			if(charIsEOF(c1)){
				return c1;
			}else{
				token.charvalue[i] = tolower(c1);
				i++;
				c1 = fgetc(infile);
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
			tempVal = token.value = atoi(token.charvalue);
		}else{
			tempVal = checkRWord(token.charvalue);
			if(stringIsRword(tempVal)){
				token.attr = RWORD;
			}else{
				token.attr = IDENTIFIER;
			}
			token.value = tempVal;
		}
        moveFileCursor(infile, -1);
	}
}
