/*
 File name : compiler.h
 Vers : 0.01a
 Written by : Setiadi Rachmat
 Date : Fri Aug 28 10:08:03 WIT 2009
*/

#define isIdentifier (token.attr == IDENTIFIER)
#define isNumbr (token.attr == NUMBER)

#define isProgram (token.attr == RWORD && token.value == PROGRAM)
#define isBegin (token.attr == RWORD && token.value == BEGIN)
#define isEnd (token.attr == RWORD && token.value == END)
#define isVar (token.attr == RWORD && token.value == VAR)
#define isProcedure (token.attr == RWORD && token.value == PROCEDURE)
#define isWhile (token.attr == RWORD && token.value == WHILE)
#define isIf (token.attr == RWORD && token.value == IF)
#define isDo (token.attr == RWORD && token.value == DO)
#define isThen (token.attr == RWORD && token.value == THEN)
#define isElse (token.attr == RWORD && token.value == ELSE)
#define isRead (token.attr == RWORD && token.value == READ)
#define isWrite (token.attr == RWORD && token.value == WRITE)

#define isDelimiter (a == ' ' || a == '\n' || a == '\t')
#define isCharacter ((a >=65 && a <= 90) || (a >= 97 && a <= 122))
#define isNumber (a >= 48 && a <= 57)
#define isSymbol (!isNumber && !isCharacter)

#define isPlus (token.attr == SYMBOL && token.value == PLUS)
#define isMinus (token.attr == SYMBOL && token.value == MINUS)
#define isTimes (token.attr == SYMBOL && token.value == TIMES)
#define isLParen (token.attr == SYMBOL && token.value == LPAREN)
#define isRParen (token.attr == SYMBOL && token.value == RPAREN)
#define isSemicolon (token.attr == SYMBOL && token.value == SEMICOLON)
#define isPeriod (token.attr == SYMBOL && token.value == PERIOD)
#define isComma (token.attr == SYMBOL && token.value == COMMA)
#define isBecomes (token.attr == SYMBOL && token.value == BECOMES)
