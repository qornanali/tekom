/*
 File name : compiler.h
 Vers : 0.01a
 Written by : Setiadi Rachmat
 Date : Fri Aug 28 10:08:03 WIT 2009
*/
#define isProgram (token.attr == RWORD && token.value == PROGRAM)
#define isBegin (token.attr == RWORD && token.value == BEGIN)
#define isEnd (token.attr == RWORD && token.value == END)
#define isIdentifier (token.attr == IDENTIFIER)
#define isNumbr (token.attr == NUMBER)
#define isSemicolon (token.attr == SYMBOL && token.value == SEMICOLON)
#define isPeriod (token.attr == SYMBOL && token.value == PERIOD)

#define isDelimiter (a == ' ' || a == '\n' || a == '\t')
#define isCharacter ((a >=65 && a <= 90) || (a >= 97 && a <= 122))
#define isNumber (a >= 48 && a <= 57)
#define isSymbol (!isNumber && !isCharacter)

#define isPlus (token.attr == SYMBOL && token.value == PLUS)
#define isMinus (token.attr == SYMBOL && token.value == MINUS)
#define isTimes (token.attr == SYMBOL && token.value == TIMES)
#define isNumberOperator (isPlus || isMinus || isTimes)

#define isLParen (token.attr == SYMBOL && token.value == LPAREN)
#define isRParen (token.attr == SYMBOL && token.value == RPAREN)
