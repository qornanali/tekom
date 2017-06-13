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
}Stack;

void CreateList(Stack * L);
alamat Alokasi(char *key, char type, int addr);
alamat Search(Stack L, char *key);


// modul untuk membuat sebuah stack
void CreateList(Stack *L){
	First(*L) = NULL;
}

// modul untuk mengubah jumlah parameter sebuah function atau procedure yang telah terdaftar
void ChangeParam(Stack L,int param){
	alamat P = First(L);
	for(int i=0;i<param; i++){
		P= P->next;
	}
	P->nparam = param;
}

// modul untuk membuat sebuah node atau alokasi memori sebuah node
alamat Alokasi(char key[30], char type, int param){
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
		P->address = NULL;
		P->nparam = param;
	}
	return (P);
}

//modul untuk mencari variable pada stack
alamat search(Stack L, char key[30]){
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

//modul untuk menamplkan isi dari stack
void print(Stack L){
	if(L.first == NULL){
		printf("kosong!");
	}else{
		alamat p = L.first;
		
		while(p != NULL){
			printf("%3d %s \n",p->type, p->key);
			p = p->next;
		}
	}
}


//modul untuk push stack
void push (Stack *L, char *key, char type, int param){
	alamat P;
	P = Alokasi(key, type, param);
	Next(P) = First(*L);
	First(*L) = P;
}


//modul untuk pop stack
void pop(Stack *L){
	(*L).first = (*L).first->next;
}
