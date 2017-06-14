/*
 File name : scanner2.c
 Vers : 0.01a
 Written by : Setiadi Rachmat
 Date : Fri Aug 28 10:08:03 WIT 2009
 Modified by : Mochamad Sohibul Iman - 151511016 - 2A - D3 Teknik Informatika
 Date : Friday, 3 March 2017 13:39 WIB
*/ 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "scanner.h"

FILE *infile;
token_t token;

int get_token(void);

void init_token (char *name)
{
	if((infile = fopen(name,"r")) == NULL) 
	{
		printf("Error Cant't Open Source Code %s\n", name);
		exit(-1);
	} else return;

}

int main(int argc, char *argv[])
{
	init_token(argv[1]);
   while(get_token() != EOF) 
	{
	 	printf("%3d %3d %s \n", token.attr, token.value, token.charvalue);
	}
	return 0;
}


int get_token(void) 
{
    int i=0;
    char c;
    char c2;
    static char temp;
  	memset(token.charvalue,'\0',50);
    token.value=0;
    token.attr=0;
  	c=getc(infile);
    if (isspace(c) || iscntrl(c) || isblank(c)) {
       get_token();
  	} 
    else if(ispunct(c)) {
          token.attr = SYMBOL;
          c2 = getc(infile);
          if((c2 == '=' || c2 == '>' || c2 == '.') && (c == '<' || c=='>' || c =='.' || c == ':')) {
               token.charvalue[0]= tolower(c);
               token.charvalue[1]= tolower(c2);
               if(strcmp(token.charvalue, ":=") == 0){
                temp=token.value=BECOMES;
               }else if (strcmp(token.charvalue, "<=")==0) {
                temp=token.value=LESSEQL;
               }else if(strcmp(token.charvalue, "<>") == 0) {
                temp=token.value=NOTEQL;
               }else if(strcmp(token.charvalue, ">=")== 0) {
                temp=token.value=GRTREQL;
               }else if (strcmp(token.charvalue,"..")==0) {
                temp=token.value=INTERVAL;
               }
               return c;
          }else {
            token.charvalue[0]= tolower(c);
            if(c2 != EOF) {
              fseek(infile, -1, SEEK_CUR);
            }
            switch(c) {
              case '+' : temp=token.value = PLUS;
                         break;
              case '-' : temp=token.value = MINUS;
                         break;
              case '*' : temp=token.value = TIMES;
                         break;
              case '(' : temp=token.value = LPAREN;
                         break;
              case ')' : temp=token.value = RPAREN;
                         break;
              case '=' : temp=token.value = EQL;
                         break;
              case ',' : temp=token.value = COMMA;
                         break;
              case '.' : temp=token.value = PERIOD;
                         break;
              case ';' : temp=token.value = SEMICOLON;
                         break;
              case '<' : temp=token.value = LESSTHAN;
                         break;
              case '>' : temp=token.value = GRTRTHAN;
                         break;
              case '[' : temp=token.value = LBRACE;
                         break;
              case ']' : temp=token.value = RBRACE;
                         break;
              case ':' : temp=token.value = COLON;
                         break;
              default:
                        temp=token.value=-1;
                        break;
            }
            return c;
          }
    }
  	else {
  			
      	  		do{
        	  			if(c == EOF) {
        			  		return EOF;
        			  	}else {
        			  		token.charvalue[i] = tolower(c);
        			  		i++;
        			  		c= getc(infile);
        			  	}
      	  		}while(isalnum(c));

              int index=0;
              int isnumber=1;
              while(token.charvalue[index] != '\0') {
                if(!isdigit(token.charvalue[index])) {
                  isnumber=0;
                }
                index++;
              }

              if(isnumber == 1) {
                 token.attr=NUMBER;
                 temp=token.value= atoi(token.charvalue); 
              }
              else {
                    token.attr=RWORD;
                    if (strcmp(token.charvalue, "begin")==0) {
                    temp=token.value=0;
                   }else if(strcmp(token.charvalue, "div") == 0) {  
                    temp=token.value=1;
                   }else if(strcmp(token.charvalue, "do")== 0) {
                    temp=token.value=2;
                   }else if (strcmp(token.charvalue,"else")==0) {
                    temp=token.value=3;
                   }else if (strcmp(token.charvalue, "end")==0) {
                    temp=token.value=4;
                   }else if(strcmp(token.charvalue, "if") == 0) {
                    temp=token.value=5;
                   }else if(strcmp(token.charvalue, "procedure")== 0) {
                    temp=token.value=6;
                   }else if (strcmp(token.charvalue,"program")==0) {
                    temp=token.value=7;
                   }else if (strcmp(token.charvalue, "then")==0) {
                    temp=token.value=8;
                   }else if(strcmp(token.charvalue, "var") == 0) {
                    temp=token.value=9;
                   }else if(strcmp(token.charvalue, "while")== 0) {
                    temp=token.value=10;
                   }else if (strcmp(token.charvalue,"read")==0) {
                    temp=token.value=11;
                   }else if (strcmp(token.charvalue, "write")==0) {
                    temp=token.value=12;
                   }else if(strcmp(token.charvalue, "forward") == 0) {
                    temp=token.value=13;
                   }else if(strcmp(token.charvalue, "function")== 0) {
                    temp=token.value=14;
                   }else {
                      token.attr=IDENTIFIER;
                      token.value=temp;
                   }
              } 
    			fseek(infile, -1, SEEK_CUR);
  	}
}