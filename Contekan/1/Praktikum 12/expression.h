#define isplus (token.attr == SYMBOL && token.value == PLUS)
#define isminus (token.attr == SYMBOL && token.value == MINUS)
#define istimes (token.attr == SYMBOL && token.value == TIMES)
#define isdiv (token.attr == RWORD && token.value == DIV)

#define isEql (token.attr == SYMBOL && token.value == EQL)
#define isNotEql (token.attr == SYMBOL && token.value == NOTEQL)
#define isGrtrThan (token.attr == SYMBOL && token.value == GRTRTHAN)
#define isLessThan (token.attr == SYMBOL && token.value == LESSTHAN)
#define isGrtrEql (token.attr == SYMBOL && token.value == GRTREQL)
#define isLessEql (token.attr == SYMBOL && token.value == LESSEQL)


int pangkat(int a){
	if(a>0){
		return 10*pangkat(a-1);
	}else{
		return 1;
	}
}

int toInteger(char input[30]){
	int hasil = 0, p = 0;
	
	for(int i=0;i<=29;i++){
		if(input[29-i] !='\0'){
			hasil = hasil + ((input[29-i] - 48) * pangkat(p));
			p++;
		}
	}
	
	return hasil;
}
