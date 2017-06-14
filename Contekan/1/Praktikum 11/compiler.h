/*
 File name : compiler.h
 Vers : 0.01a
 Written by : Setiadi Rachmat
 Date : Fri Aug 28 10:08:03 WIT 2009
*/

#define isIdentifier (token.attr == IDENTIFIER)
#define isNumbr (token.attr == NUMBER)

#define isRWord (token.attr == RWORD)
#define isBegin (isRWord && token.value == BEGIN)
#define isDo (isRWord && token.value == DO)
#define isElse (isRWord && token.value == ELSE)
#define isEnd (isRWord && token.value == END)
#define isFunction (isRWord && token.value == FUNCTION)
#define isIf (isRWord && token.value == IF)
#define isProcedure (isRWord && token.value == PROCEDURE)
#define isProgram (isRWord && token.value == PROGRAM)
#define isRead (isRWord && token.value == READ)
#define isThen (isRWord && token.value == THEN)
#define isVar (isRWord && token.value == VAR)
#define isWhile (isRWord && token.value == WHILE)
#define isWrite (isRWord && token.value == WRITE)

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
