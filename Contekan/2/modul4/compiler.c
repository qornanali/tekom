/*
 File name : compiler.c
 Vers : 0.01a
 Written by : Setiadi Rachmat
 Date : Fri Aug 28 10:08:03 WIT 2009
 Modified by : Mochamad Sohibul Iman - 151511016 - 2A - D3 Teknik Informatika

*/ 


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "scanner.h"
#include "compiler.h"

token_t token;
FILE *infile;

void program(void);
void statement(void);
void init_token(char *name);
int get_token(void);
void statement_list();
void statement_arithmetic();
void expression(void);
void term(void);
void factor(void);
void error(int i);
int isbalance();
int count_begin=0;
int balance=-1;

int main(int argc, char *argv[])
{
	int c=0;
	init_token(argv[1]);
  program();
  printf("Congratulations : No error is found\n");
	return 0;
}

void program(void) 
{

	get_token();
	if(!isprogram) 
	{
		printf("Error : Reserved Word 'program' is expected\n");
		exit(-1);
	}

	get_token();
	if (!isidentifier)
	{
		printf("Error : Identifier is expected after 'program' \n");
		exit(-1);
	}

	get_token();
	if(!issemicolon)
	{
		printf("Error : Symbol ';' is expected\n");
		exit(-1);
	}

	get_token();
	statement();
	if (!isperiod)
	{
		printf("Error : Symbol '.' is expected\n");
		exit(-1);
	}
}


void statement ()
{
       
       if(balance == -1)
       {
         balance=isbalance();
         if (!balance)
         {
                printf("Fatal: Syntax Error, Reserved Word 'Begin - End' is not balance\n");
                exit(-1);
         }
       }

      if(isbegin) 
      {
          count_begin++;
          get_token();
          statement();
         
          if(isend) 
          {
            count_begin--;
            get_token();
          }
        
          //ditengah-tengah
         if(count_begin>=1) 
          {       
              if(!issemicolon)
              {
                 char found_type[50];
                 switch(token.attr)
                 {
                  case 1: strcpy(found_type,"Identifier");break;
                  case 2: strcpy(found_type,"Number");break;
                  case 3: strcpy(found_type,"Reserved Word");break;
                  case 4: strcpy(found_type,"Symbol");break; 
                 }
                 printf("Fatal: Syntax Error, ';' expected but '%s %s' found\n",found_type,token.charvalue);
                 printf("Fatal: Compilation Aborted\n");
                 exit(-1);
              }else if(isidentifier)
                    {
                        printf("Fatal: Syntax Error, IDENTIFIER '%s' UNDEFINED\n", token.charvalue);
                        printf("Fatal: Compilation Aborted\n");
                        exit(-1);
                    } 
                    else 
                       {
                           get_token();
                           statement();
                        }   
          }  

    }else if(isidentifier)
                  {
                    printf("Fatal: Syntax Error, IDENTIFIER '%s' UNDEFINED\n", token.charvalue);
                    printf("Fatal: Compilation Aborted\n");
                    exit(-1);
                  } else if(!isend && !isperiod)
                         {
             
                            expression();
                            if(issemicolon)
                            {
                              get_token();
                              statement();
                            }
              
                        }  else if(count_begin == 0)
                                {

                                  if(!isnumbr)
                                  {
                                    printf("Fatal: Syntax Error, NUMBER  is expected\n");
                                    printf("Fatal: Compilation Aborted\n");
                                    exit(-1);
                                   }

                                }
       
}


void statement_arithmetic() {
              
              if(!isnumbr) 
              {
                printf("Fatal: Syntax Error, Operand-#1 is expected\n");
                printf("Fatal: Compilation Aborted\n");
                exit(-1);
              }

              get_token();
              if(token.value == PLUS  || token.value == MINUS || token.value == TIMES)
              {
                get_token();
                if(!isnumbr) 
                {
                  printf("Fatal : Syntax Error, Operand-#2 is expected\n");
                  printf("Fatal: Compilation Aborted\n");
                  exit(-1);
                 
                }

                get_token();
              }else 
                  {
                    printf("Fatal : Syntax Error Operator is expected\n");
                    printf("Fatal: Compilation Aborted\n");
                    exit(-1);
                  }

}


int isbalance () {
  int count_begin, count_end;
  count_end=count_begin=0;
  if(isbegin)
  {
    count_begin++;
  }
  while(get_token() != EOF)
  {
    if(isbegin)
    {
      count_begin++;
    }else if (isend)
          {
            count_end++;
          }
          // else if(isidentifier)
          //         {
          //           printf("Fatal: Syntax Error, IDENTIFIER '%s' UNDEFINED\n", token.charvalue);
          //           exit(-1);
          //         }

  }
 
  fseek(infile,0,SEEK_SET);
  int i;
  for (i = 1; i <= 4; ++i)
  {
    get_token();
  }
  if(count_begin == count_end)
  {
    return 1;
  }else {
          return 0;
        }

}


void expression()
{

  if(isplus || isminus) 
  {
    get_token();
  }

  term();

  while(isplus || isminus)
  {
   
    get_token();
    term();
   
  }
}

void term() 
{
  factor();
  while(istimes || isdiv)
  {
    get_token();
    factor();
  }
}


void factor()
{
  
  if(isnumbr)
  {
    get_token();
    return;
  }else if(islparen) 
        {
         
          get_token();
          expression();
          if(isrparen)
          {
            get_token();
            return;
          }
          else{
                error(2);
              }
        }
        else
            {
              error(1);
            }
}

void error(int i)
{
  switch(i)
  {
    case 1: printf("Fatal: Syntax Error '(' expected but '%s'found\n", token.charvalue);
             printf("Fatal: Compilation Aborted\n");
             exit(-1);
        break;
    case 2:  printf("Fatal: Syntax Error ')' expected but '%s'found\n", token.charvalue);
              printf("Fatal: Compilation Aborted\n");
              exit(-1);
        break;
  }
}


void init_token (char *name)
{
	if((infile = fopen(name,"r")) == NULL) 
	{
		printf("Error Cant't Open Source Code %s\n", name);
		exit(-1);
	} else return;

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