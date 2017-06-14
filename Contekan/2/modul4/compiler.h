/*
 File name : scanner.c
 Vers : 0.01a
 Written by : Setiadi Rachmat
 Date : Fri Aug 28 10:08:03 WIT 2009
 Modified by : Mochamad Sohibul Iman - 151511016 - 2A-D3 Teknik Informatika
 Date : Sunday, March 19, 2017 WIB
*/ 
#define isprogram			(token.attr == RWORD && token.value == PROGRAM)
#define isbegin				(token.attr == RWORD && token.value == BEGIN)
#define isend				(token.attr == RWORD && token.value == END)

#define isidentifier 		(token.attr == IDENTIFIER)
#define isnumbr				(token.attr == NUMBER) 

#define issemicolon			(token.attr == SYMBOL && token.value == SEMICOLON)
#define isperiod			(token.attr == SYMBOL &&  token.value == PERIOD)

#define isplus				(token.attr == SYMBOL && token.value == PLUS)
#define isminus				(token.attr == SYMBOL && token.value == MINUS)
#define istimes				(token.attr == SYMBOL && token.value == TIMES)
#define isdiv				(token.attr == RWORD  && token.value ==DIV)
#define islparen			(token.attr == SYMBOL && token.value == LPAREN)
#define isrparen			(token.attr == SYMBOL && token.value == RPAREN)