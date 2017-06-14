#define VGLOBAL 1 /* global variable */
#define VLOCAL 2 /* local variable */
#define PNAME 3 /* procedure name */
#define FNAME 4 /* function name */ 
#define Next(P) (P)->next
#define First(L) (L).first

typedef struct _stack *alamat;
typedef struct _stack {
	char key[50]; /* string of token */
	char type; /* VGLOBAL | VLOCAL | PNAME | FNAME */
	int address; /* reserved for code generator */
	int nparam; /* parameter number(s) if type := PNAME | FNAME */
	alamat next;
} stack_t;

typedef struct{
	alamat first;
}List;

void CreateList(List * L);
alamat Alokasi(char *key, char type, int addr);
alamat Search(List L, char *key);

void CreateList(List *L){
	First(*L) = NULL;
}

alamat Alokasi(char key[30], char type, int addr){
	alamat P;
	P = (alamat)malloc(sizeof(stack_t));
	if(P != NULL){
		Next(P) = NULL;
		for(int i=0; i<30; i++){
			if(key[i] != NULL || key[i] != '\0'){
				P->key[i] = key[i];
			}else{
				P->key[i] = '\0';
				break;
			}
		}
		P->type = type;
		P->address = addr;
	}
	return (P);
}

alamat search(List L, char key[30]){
	alamat P;
	bool found = false;
	P = First(L);
	while(!found && P!=NULL){
		if(strcmp(key, P->key) == 0){
			return P;
		}
		P = P->next;
	}
	return NULL;
}

void print(List L){
	if(L.first == NULL){
		printf("kosong!");
	}else{
		alamat p = L.first;
		
		while(p != NULL){
			printf("%s ", p->key);
			p = p->next;
		}
	}
}

void push (List *L, char *key, char type, int addr){
	alamat P;
	P = Alokasi(key, type, addr);
	Next(P) = First(*L);
	First(*L) = P;
}

void pop(List *L){
	(*L).first = (*L).first->next;
}
