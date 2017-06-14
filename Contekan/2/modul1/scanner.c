/*
 File name : scanner.c
 Vers : 0.01a
 Written by : Setiadi Rachmat
 Date : Fri Aug 28 10:08:03 WIT 2009
 Modified by : Mochamad Sohibul Iman - 151511016
 Date : Friday, 3 March 2017 13:39 WIB
*/ 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

FILE *infile;
char token[50];

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
	 	printf("%s\n", token);
	}
	return 0;
}


int get_token(void) 
{
    int i=0;
    char c;
  	memset(token,'\0',50);
  	c=getc(infile);
    if (isspace(c) || iscntrl(c) || isblank(c)) {
       get_token();
  	}
  	else if(ispunct(c)) {
		  token[0]=c;
		  return c;
  	}
  	else {
	  	    do{
	  		    if(c == EOF) {
			  		return EOF;
			  	}else {
			  		token[i] = c;
			  		i++;
			  		c= getc(infile);
			  	}
	  		}while(isalnum(c));
  			fseek(infile, -1, SEEK_CUR);
  	}
}