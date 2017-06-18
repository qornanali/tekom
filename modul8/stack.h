#ifndef stack_H
#define	stack_H
#define Nil NULL
#define VGLOBAL 1
#define VLOCAL 2
#define PNAME 3
#define FNAME 4

typedef struct {
    char key[50];
    char type;
    int  address;
    int  nparam;
} infotype;
typedef struct tElmtListStack *addressStack;
typedef struct tElmtListStack {
    infotype info;
    addressStack  next;
    } ElmtListStack;    
typedef struct {
     addressStack TOP;	
} ListStack;

void CreateListStack (ListStack * L);
addressStack AlokasiStack (infotype *X);
void DeAlokasiStack (addressStack P);
void Push (ListStack * L, infotype *X);
void InsertTOP (ListStack * L, addressStack P);
void Pop (ListStack * L);
void PrintInfoStack (ListStack L);
addressStack search(ListStack L, char key[]);
void PopVarLocal(ListStack *L);


#endif
