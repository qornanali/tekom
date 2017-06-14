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
#include "stack.h"

token_t token;
FILE *infile;
ListStack varTable;


void program(void);
void statement(void);
void init_token(char *name);
int get_token(void);
void statement_list(); //gadipake
void statement_arithmetic(); //gadipake
void expression(void);
void term(void);
void factor(void);
void error(int i);
void condition();
int paramlist();
void outblock();
void inblock();
int isbalance();
int count_begin=0;
int balance=-1;

int main(int argc, char *argv[])
{
	int c=0;
  CreateListStack(&varTable);
  init_token(argv[1]);
  program();
  PrintInfoStack(varTable);
  printf("Congratulations : No error is found\n");
	return 0;
}

void program(void) 
{
	get_token();
  /*start checking*/
    printf("\n\nMASUK MODUL PROGRAM\n");
    printf("CURRENT TOKEN : %s\n",token.charvalue);
    printf("CHECKING IS PROGRAM\n");
  /*end checking*/
	if(!isprogram) 
	{
		printf("Error : Reserved Word 'program' is expected\n");
		exit(-1);
	}

	get_token();
  /*start checking*/
    printf("CURRENT TOKEN : %s\n",token.charvalue);
    printf("CHECKING IS IDENRIFIER\n");
  /*end checking*/
	if (!isidentifier)
	{
		printf("Error : Identifier is expected after 'program' \n");
		exit(-1);
	}

	get_token();
   /*start checking*/
    printf("CURRENT TOKEN : %s\n",token.charvalue);
    printf("CHECKING IS SEMICOLON\n");
  /*end checking*/
	if(!issemicolon)
	{
		printf("Error : Symbol ';' is expected\n");
		exit(-1);
	}
	get_token();
   /*start checking*/
    printf("CURRENT TOKEN : %s\n",token.charvalue);
    printf("CHECKING FROM PROGRAM To OUTBLOCK\n");
  /*end checking*/
	outblock();

    /*start checking*/
    printf("CURRENT TOKEN : %s\n",token.charvalue);
    printf("CHECKING PERIOD\n");
  /*end checking*/
	if (!isperiod)
	{
		printf("Error : Symbol '.' is expected\n");
		exit(-1);
	}
}


void statement ()
{
    /*start checking*/
    printf("\n\nMASUK KE MODUL STATEMENT\n");
    printf("CURRENT TOKEN : %s\n",token.charvalue);
    printf("CHECKING IN STATEMENT : PILIH NODE\n");
  /*end checking*/
   if(isidentifier) {
     addressStack resultSearch= NULL;
     int nparam=0;
     resultSearch=search(varTable,token.charvalue);  
     if(resultSearch == NULL) {
        printf("Fatal : Syntax Error, Indentifier '%s' is undefined\n", token.charvalue);
        printf("Fatal: Compilation Aborted\n");
        exit(-1);
     }
      get_token();
       /*start checking*/
        printf("CURRENT TOKEN : %s\n",token.charvalue);
        printf("CHECKING IN STATEMENT : ISBECOMENS BUKAN ?\n");
      /*end checking*/
      if(isbecomes) {
          get_token();
          /*start checking*/
            printf("CURRENT TOKEN : %s\n",token.charvalue);
            printf("CHECKING FROM STATEMENT to EXPRESSION\n");
          /*end checking*/
           expression();
      }else {
         /*start checking*/
            printf("CURRENT TOKEN : %s\n",token.charvalue);
            printf("CHECKING FROM STATEMENT to PARAMLIST\n");
          /*end checking*/
           /*start checking*/
            printf("CURRENT TOKEN : %s\n",token.charvalue);
            printf("CHECKING FROM STATEMENT to PARAMLIST\n");
          /*end checking*/
        nparam=paramlist();
        if(nparam != resultSearch->info.nparam) {
          if(nparam > resultSearch->info.nparam) {
              printf("Fatal : Too Many Actual Parameters\n", token.charvalue);
              printf("Fatal: Compilation Aborted\n");
              exit(-1);
          }else {
              printf("Fatal : Not Enough Actual Parameters\n", token.charvalue);
              printf("Fatal: Compilation Aborted\n");
              exit(-1);
          }
        }
      }
   }else if(isbegin) {
         do{
              get_token();
               /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("CHECKING IN STATEMENT  -> BEGIN\n");
              /*end checking*/
              statement();
               /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("CHECKING AFTER STATEMENT  -> BEGIN\n");
              /*end checking*/
          }while(issemicolon);
          if(!isend) {
                printf("Fatal : Syntax Error, Reserved Word 'End' is expected, but %s found\n", token.charvalue);
                printf("Fatal: Compilation Aborted\n");
                exit(-1);
          }
          get_token();
   }else if(iswhile) {
          get_token();
          condition();
          if(!isdo) {
              printf("Fatal : Syntax Error, Reserved Word 'do' is expected, but %s found\n", token.charvalue);
              printf("Fatal: Compilation Aborted\n");
              exit(-1);
          }
          get_token();
          statement();
   }else if(isif) {
          get_token();
          condition();
          if(!isthen) {
              printf("Fatal : Syntax Error, Reserved Word 'then' is expected, but %s found\n", token.charvalue);
              printf("Fatal: Compilation Aborted\n");
              exit(-1);
          }
          get_token();
          statement();
          if(iselse) {
            get_token();
            statement();
          }
   }else if(isread) {
          get_token();
          if(islparen) {
              do {
                  get_token();
                  if(!isidentifier) {
                      printf("Fatal : Syntax Error, Indentifier is expected, but %s found\n", token.charvalue);
                      printf("Fatal: Compilation Aborted\n");
                      exit(-1);
                  }
                  get_token();
              }while(iscomma);
              if(!isrparen) {
                  printf("Fatal : Syntax Error, ')' is expected, but %s found\n", token.charvalue);
                  printf("Fatal: Compilation Aborted\n");
                  exit(-1);
              }
              get_token();
          }else {
              printf("Fatal : Syntax Error, '(' is expected, but %s found\n", token.charvalue);
              printf("Fatal: Compilation Aborted\n");
              exit(-1);
          }
   }else if(iswrite) {
       get_token();
          if(islparen) {
              do {
                  get_token();
                  expression();
              }while(iscomma);
              if(!isrparen) {
                  printf("Fatal : Syntax Error, '(' is expected, but %s found\n", token.charvalue);
                  printf("Fatal: Compilation Aborted\n");
                  exit(-1);
              }
              get_token();
          }else {
              printf("Fatal : Syntax Error, '(' is expected, but %s found\n", token.charvalue);
              printf("Fatal: Compilation Aborted\n");
              exit(-1);
          }

   }else {
        if(isend) {
             return;
        }else {
               printf("Fatal : Syntax Error,statement expected, but %s found\n", token.charvalue);
              printf("Fatal: Compilation Aborted\n");
              exit(-1);
        }
            
   }   
         
}



void outblock() 
{
    infotype X;
   /*start checking*/
    printf("\n\nMASUK KE MODUL OUTBLOCK\n");
     printf("CURRENT TOKEN : %s\n",token.charvalue);
     printf("CHECKING VAR in OUTBLOCK\n");
  /*end checking*/
   if(!isvar) {
   	 printf("Fatal : Syntax Error, Reserved Word 'var' is expected\n");
     printf("Fatal: Compilation Aborted\n");
     exit(-1);
   }
   /*start */
      printf("TEST BENAR - INI VAR\n");
  /*end checking*/
   do{
      get_token();
       /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("CHECKING IDENTIFIER in OUTBLOCK dalam DO-WHILE\n");
      /*end checking*/
     if(!isidentifier) {
        printf("Fatal : Syntax Error, Identifier is expected, but %s found\n", token.charvalue);
        printf("Fatal: Compilation Aborted\n");
        exit(-1);
     }
     //PUSH IDENTIFIER GLOBAL
      strcpy(X.key, token.charvalue);
      X.type=VGLOBAL;
      X.address=0;
      X.nparam=0;
      Push(&varTable,&X);

      /*start */
      printf("TEST BENAR - INI IDENTIFIER\n");
  /*end checking*/
     get_token();
      /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("ULANGI LAGI NGGA VARNYA ? sama dengan COMMA ?\n");
     /*end checking*/
   }while(iscomma);

     /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("CHECKING SEMICOLON BENER?\n");
     /*end checking*/
   if(!issemicolon) {
        printf("Fatal : Syntax Error, ';' expected, but %s found\n", token.charvalue);
        printf("Fatal: Compilation Aborted\n");
        exit(-1);
   }
   get_token();
    /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("CHECKING MASUK KE PENGULANGAN PROCEDURE NGGA ?\n");
     /*end checking*/

   while(isprocedure || isfunction) {
        get_token();
        /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("CHECKING MASUK KE PENGULANGAN-> CEK INDENTIFIER ?\n");
     /*end checking*/
        if(!isidentifier) {
            printf("Fatal : Syntax Error, Identifier is expected, but %s found\n", token.charvalue);
            printf("Fatal: Compilation Aborted\n");
            exit(-1);
        }

          strcpy(X.key, token.charvalue);
          if(isprocedure) {
            X.type=PNAME;
          }else{
            X.type=FNAME;
          }
         
          X.address=0;
          X.nparam=0;
          Push(&varTable,&X);

        get_token();
          /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("MASUK KE INBLOCK\n");
          /*end checking*/
        inblock();
        PopVarLocal(&varTable);
       
            /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("BARU KELUAR DARI INBLOCK CEK SEMICOLON\n");
     /*end checking*/
        
        if(!issemicolon) {
            printf("Fatal : Syntax Error, ';' expected, but %s found\n", token.charvalue);
            printf("Fatal: Compilation Aborted\n");
            exit(-1);
        }
        get_token();
         /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("MASUK KE STATEMENT diAKHIR OUTBLOCK\n");
     /*end checking*/
   }
   statement();
}

void inblock() {
    infotype X;
   addressStack addrFuncOrProc = varTable.TOP;
    int nparam=0;
    if(islparen) {
     
      do{
        get_token();
        if(!isidentifier) {
              printf("Fatal : Syntax Error, Identifier is expected, but %s found\n", token.charvalue);
              printf("Fatal: Compilation Aborted\n");
              exit(-1);
        }
          strcpy(X.key, token.charvalue);
          X.type=VLOCAL;
          X.address=0;
           X.nparam=0;
           nparam++;
          Push(&varTable,&X);
        get_token();
      }while(iscomma);

      if(!isrparen) {
            printf("Fatal : Syntax Error, ')' expected, but %s found\n", token.charvalue);
            printf("Fatal: Compilation Aborted\n");
            exit(-1);
      }
      addrFuncOrProc->info.nparam=nparam;
      get_token();
    }
    if(!issemicolon) {
        printf("Fatal : Syntax Error, ';' expected, but %s found\n", token.charvalue);
        printf("Fatal: Compilation Aborted\n");
        exit(-478);
    }

    get_token();
    if(isvar) {
    
     do{
          get_token();
        if(!isidentifier) {
            printf("Fatal : Syntax Error, Identifier is expected, but %s found\n", token.charvalue);
            printf("Fatal: Compilation Aborted\n");
            exit(-488);
        }
          strcpy(X.key, token.charvalue);
          X.type=VLOCAL;
          X.address=0;
          X.nparam=0;
          Push(&varTable,&X);

        get_token();
     }while(iscomma);
      if(!issemicolon) {
            printf("Fatal : Syntax Error, ';' expected, but %s found\n", token.charvalue);
            printf("Fatal: Compilation Aborted\n");
            exit(-495);
      }
       get_token(); 
    }
    
    statement();
}

void expression()
{
  /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("INI AWAL MODUL EXPRESSION\n");
     /*end checking*/
  if(isplus || isminus) 
  {
    /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("INI + || -\n");
     /*end checking*/
    get_token();
    /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("MASUK KE MODUL TERM\n");
     /*end checking*/
  }

  term();
 /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("DIULANG KARENA + || - ?? \n");
     /*end checking*/
  while(isplus || isminus)
  {
   
    get_token();
    term();
   
  }
}

void term() 
{
   /*start checking*/
                printf("\n\nMasuk Modul Term\n");   
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                 printf("Masuk Modul Term to Factor\n"); 
     /*end checking*/
  factor();
   /*start checking*/
               printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("CEK PENGULANGAN MODUL FACTOR\n");   
               
     /*end checking*/
  while(istimes || isdiv)
  {
    get_token();
    factor();
  }
}


void factor()
{
  /*start checking*/
                printf("\n\nIni Awal Modul Factor\n");   
                printf("CURRENT TOKEN : %s\n",token.charvalue);
      
     /*end checking*/

  if(isnumbr)
  {
    get_token();
    return;
  }else if (isidentifier){
          addressStack result=search(varTable,token.charvalue);
          int nparam=-1;
      if(result != NULL && result->info.type == FNAME) {
                    get_token();
                    nparam=paramlist();
                    if(nparam != result->info.nparam) {
                      if(nparam > result->info.nparam) {
                          printf("Fatal : Too Many Actual Parameters\n", token.charvalue);
                          printf("Fatal: Compilation Aborted\n");
                          exit(-1);
                      }else {
                          printf("Fatal : Not Enough Actual Parameters\n", token.charvalue);
                          printf("Fatal: Compilation Aborted\n");
                          exit(-1);
                      }
                    }
                    return;
      }else{
            if(search(varTable,token.charvalue) == NULL) {
                printf("Fatal : Syntax Error, Indentifier '%s' is undefined\n", token.charvalue);
                printf("Fatal: Compilation Aborted\n");
                exit(-1);
            }
            /*start checking*/
                        printf("CURRENT TOKEN : %s\n",token.charvalue);
                        printf("DAPAT IDENTIFIER \n");
            /*end checking*/
              get_token();
              return;
        }
  }else if(islparen) {
       /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("DAPAT LPAREN ( \n");
     /*end checking*/
         
          get_token();
            printf("CURRENT TOKEN : %s\n",token.charvalue);
           printf("DAPAT LPAREN (  MASUK LAGI EXPRESSION\n");
          expression();
     /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("CHECKING RPARENT IN EXPRESSION\n");
     /*end checking*/
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
             exit(-582);
        break;
    case 2:  printf("Fatal: Syntax Error ')' expected but '%s'found\n", token.charvalue);
              printf("Fatal: Compilation Aborted\n");
              exit(-586);
        break;
  }
}

int paramlist() 
{ 
      int nparam=0;
       /*start checking*/
                printf("AWAL MASUK PARAMLIST\n");
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("CHECK LPAREN BUKAN\n");
     /*end checking*/
  if(islparen) {
      nparam=1;
      get_token();
      expression();
      while(iscomma) {
        get_token();
        nparam++;
        expression();
      }
      if(!isrparen) {
             printf("Fatal: Syntax Error, ')' expected but '%s'found\n", token.charvalue);
              printf("Fatal: Compilation Aborted\n");
              exit(-1);
      }
      get_token();
      return nparam;
  }
  return 0;
}

void condition() {
  expression();
  if(iseql == 0 && isnoeql == 0  && isgrtrthan == 0 && islessthan == 0 && isgrtreql == 0 && islesseql == 0) {
             printf("Fatal: Syntax Error, 'condition' expected but '%s'found\n", token.charvalue);
              printf("Fatal: Compilation Aborted\n");
              exit(-1);
  }
  get_token();
  expression();
   /*start checking*/
                printf("CURRENT TOKEN : %s\n",token.charvalue);
                printf("KELUAR DARI CONDITION\n");
     /*end checking*/
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