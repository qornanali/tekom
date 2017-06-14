#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <windows.h>
#include "scanner.h"
#include "checkObjectMachine.h"
#define MAX_SIZE 101
#define CURRENT_SIZE 7
token_t token;
FILE *infile;


int main(int argc, char *argv[]) {
        system("cls");
        SetColorAndBackground(7,0);
        int size;
        int PC=0;
        int R1=0;
        int R2=-1;
        int R3=0;
        int R4=0;
        ReadOnlyMemory ROM[MAX_SIZE];
        int stackMemory[MAX_SIZE];
        /* initialitation */
        initRand(stackMemory, CURRENT_SIZE);
        init_token(argv[1]);
        getObjectCode(ROM,&size);
        printROM(ROM,size);
        SetColorAndBackground(7,1);
        printf("INITIAL STATE\n");
        SetColorAndBackground(7,0);
        printStep(&PC,&R1,&R2,&R3,&R4,ROM,stackMemory);
        execute(&PC, &R1,&R2,&R3,&R4,ROM,stackMemory, size);
        
        return 0;
}

void initRand(int s[], int size) {
    int i;
    for(i=-1;i<size;i++) {
        s[i] = -999;
    }
}

void printStep(int *PC, int *R1, int *R2, int *R3, int *R4, ReadOnlyMemory ROM[], int stackMemory[]) {
    printf("+----------+-----------------+\n");
    printf("| Register | Current Value   |\n");
    printf("+----------+-----------------+\n");
    printf("|    R1    |      %3d        |\n", *R1);
    printf("|    R2    |      %3d        |\n", *R2);
    printf("|    R3    |      %3d        |\n", *R3);
    printf("|    R4    |      %3d        |\n", *R4);
    printf("|    PC    |      %3d        |\n", *PC);
    printf("+----------+-----------------+\n");
    printStack(stackMemory,CURRENT_SIZE,&(*R1),&(*R2));
    system("PAUSE");
    printf("\n\n");
}

void printStack(int s[], int size, int *R1, int *R2) {
 int i;
    printf("+-----------------+\n");
    printf("| STACK MEMORY    |\n");
    printf("+-------+---------+\n");
    printf("|address|  value  |\n");
    printf("+-------+---------+\n");
    for(i=size-1;i >= -1;i--) {
        if(*R2 == i){
              SetColorAndBackground(7,1);
        }
    printf("| %3d   |  %4d   |\n",i,s[i]);
      SetColorAndBackground(7,0);
        if(i != 0) {
            printf("+-----------------+\n");
        }else{
            printf("+-------+---------+\n");
        }
    }
   
}

void execute(int *PC, int *R1, int *R2, int *R3, int *R4, ReadOnlyMemory ROM[], int stackMemory[], int size) {
    while(1) {
        printStepROM(ROM,size, *PC);
        //   printf("+----------+-----------------------------------------+\n");
        //   printf("|  PC: %3d | EXECUTE INSTRUCTION:   %3s  %1d  %1d  %1d     |\n", *PC,ROM[*PC].opcode, ROM[*PC].b,ROM[*PC].i,ROM[*PC].a);
        //   printf("+----------+-----------------------------------------+\n");
       if(strcmp(ROM[*PC].opcode,"hlt") == 0){
        printStep(&(*PC),&(*R1),&(*R2),&(*R3),&(*R4),ROM,stackMemory);
        return;
       }else if(strcmp(ROM[*PC].opcode, "lit") == 0) {
            *R2=*R2+1;
            stackMemory[*R2]=ROM[*PC].a;
            *PC=*PC+1;
            printStep(&(*PC),&(*R1),&(*R2),&(*R3),&(*R4),ROM,stackMemory);
        }else if(strcmp(ROM[*PC].opcode, "lod") == 0) {
            *R2=*R2+1;
            stackMemory[*R2]=stackMemory[ROM[*PC].b+ROM[*PC].i+ROM[*PC].a];
              *PC=*PC+1;
              printStep(&(*PC),&(*R1),&(*R2),&(*R3),&(*R4),ROM,stackMemory);
        }else if(strcmp(ROM[*PC].opcode, "sto") == 0) {
            stackMemory[ROM[*PC].a]=stackMemory[*R2];
            *R2=*R2-1;
             *PC=*PC+1;
             printStep(&(*PC),&(*R1),&(*R2),&(*R3),&(*R4),ROM,stackMemory);
        }else if(strcmp(ROM[*PC].opcode, "opr") == 0){
             if(ROM[*PC].a == 0 ) {
                stackMemory[*R2]= -stackMemory[*R2];
                printStep(&(*PC),&(*R1),&(*R2),&(*R3),&(*R4),ROM,stackMemory);
             }else {
                 *R2=*R2-1;
                 switch(ROM[*PC].a) {
                     case 1: stackMemory[*R2]=stackMemory[*R2]+stackMemory[*R2+1];break;
                     case 2: stackMemory[*R2]=stackMemory[*R2]-stackMemory[*R2+1];break;
                     case 3: stackMemory[*R2]=stackMemory[*R2]*stackMemory[*R2+1];break;
                     case 4: stackMemory[*R2]=stackMemory[*R2]/stackMemory[*R2+1];break;
                     case 5:
                                if(stackMemory[*R2] == stackMemory[*R2+1]) {
                                    stackMemory[*R2]=1;
                                }else {
                                    stackMemory[*R2]=0;
                                }
                                break;
                    case 6 :
                                if(stackMemory[*R2] != stackMemory[*R2+1]) {
                                    stackMemory[*R2]=1;
                                }else {
                                    stackMemory[*R2]=0;
                                }
                                break;
                    case 7: 
                                if(stackMemory[*R2] < stackMemory[*R2+1]) {
                                    stackMemory[*R2]=1;
                                }else {
                                    stackMemory[*R2]=0;
                                }
                                break;

                    case 8: 
                                if(stackMemory[*R2] <= stackMemory[*R2+1]) {
                                    stackMemory[*R2]=1;
                                }else {
                                    stackMemory[*R2]=0;
                                }
                                break;
                    case 9: 
                                if(stackMemory[*R2] > stackMemory[*R2+1]) {
                                    stackMemory[*R2]=1;
                                }else {
                                    stackMemory[*R2]=0;
                                }
                                break;
                    case 10:  
                                if(stackMemory[*R2] >= stackMemory[*R2+1]) {
                                    stackMemory[*R2]=1;
                                }else {
                                    stackMemory[*R2]=0;
                                }
                                break;
                 }
             }
               *PC=*PC+1;
               printStep(&(*PC),&(*R1),&(*R2),&(*R3),&(*R4),ROM,stackMemory);
        }else if(strcmp(ROM[*PC].opcode, "int") == 0){
            *R2=*R2+ROM[*PC].a;
            *PC=*PC+1;
            printStep(&(*PC),&(*R1),&(*R2),&(*R3),&(*R4),ROM,stackMemory);
        }else if(strcmp(ROM[*PC].opcode, "jmp") == 0) {
                *PC=ROM[*PC].a;
                printStep(&(*PC),&(*R1),&(*R2),&(*R3),&(*R4),ROM,stackMemory);
        }else if (strcmp(ROM[*PC].opcode, "jpc") == 0){
             if(stackMemory[*R2] == 0) {
                *PC=ROM[*PC].a;
            }else {
             *PC=*PC+1;
            }
             *R2=*R2-1;
             printStep(&(*PC),&(*R1),&(*R2),&(*R3),&(*R4),ROM,stackMemory);
        }else if(strcmp(ROM[*PC].opcode, "get") == 0){
            *R2=*R2+1;
            SetColorAndBackground(7,1);
            printf("\n+--------------------\n");
            printf("| GET KEYBOARD : ");scanf("%4d", &stackMemory[*R2]);
            printf("+--------------------\n");
             SetColorAndBackground(7,0);
            *PC=*PC+1;
            printStep(&(*PC),&(*R1),&(*R2),&(*R3),&(*R4),ROM,stackMemory);
        }else if (strcmp(ROM[*PC].opcode, "put") == 0) {
              SetColorAndBackground(7,1);
            printf("\n--------------------\n");
            printf("PUT DISPLAY : ");
            printf("%d \n", stackMemory[*R2]);
            printf("--------------------\n");
              SetColorAndBackground(7,0);
            *R2=*R2-1;
             *PC=*PC+1;
             printStep(&(*PC),&(*R1),&(*R2),&(*R3),&(*R4),ROM,stackMemory);
        }
    }

}

void getObjectCode(ReadOnlyMemory m[], int *size) {
    int i=0;
    while(get_token() != EOF) {
        strcpy(m[i].opcode,token.charvalue);
        get_token();
        m[i].b=token.value;
        get_token();
        m[i].i=token.value;
        get_token();
        m[i].a=token.value;
        i++;
    }
    *size=i;
}

void printROM(ReadOnlyMemory m[], int size) {
      int i;
       printf("\nINSTRUCTION OBJECT CODE :\n");
       printf("+----------+-----------------------------+\n");
       printf("| address  |  instruction (object code)  |\n");
       printf("+----------+-----------------------------+\n");
      for(i=0;i < size;i++) {
       printf("|  %3d     |      %3s    %1d    %1d    %2d    |\n", i,m[i].opcode, m[i].b, m[i].i, m[i].a);
      }
       printf("+----------+-----------------------------+\n");

      system("PAUSE");
}

void printStepROM(ReadOnlyMemory m[], int size, int PC) {
    int i;
       printf("+----------------------------------------+\n");
       SetColorAndBackground(7,4);
       printf("|  PC = %3d                              |\n",PC);
      SetColorAndBackground(7,0);  
       printf("+----------+-----------------------------+\n");
     
       printf("| address  |  instruction (object code)  |\n");
       printf("+----------+-----------------------------+\n");
       for(i=0;i < size;i++) {
            if(i == PC){
               SetColorAndBackground(7,1);
            }
        printf("|  %3d     |      %3s    %1d    %1d     %2d   |\n", i,m[i].opcode, m[i].b, m[i].i, m[i].a);
            SetColorAndBackground(7,0);
        }
       printf("+----------+-----------------------------+\n");
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

void SetColorAndBackground(int ForgC, int BackC)
{
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
     return;
}