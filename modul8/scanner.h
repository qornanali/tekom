typedef struct _token{
	char attr;				
	char charvalue[30];		
	int value;			
} token_t;

//tools
#define TRUE 1
#define FALSE 0
#define RWORDS_SIZE 15
#define SYMBOLS_SIZE 19
#define stringIsEqual(A, B) (strcmp((A), (B)) == 0)
#define stringIsContain(A, B) (strstr((A), (B)) != NULL)
#define charIsAlphabet(X) (((X) >= 'a' && (X) <= 'z') || ((X) >= 'A' && (X) <= 'Z'))
#define charIsNumber(X)  ((X) >= '0' && (X) <= '9')
#define charIsWhiteSpace(X) ((X) == ' ' || (X) == '\n' || (X) == '\t')
#define charIsSymbol(X) (!charIsNumber((X)) && !charIsAlphabet((X)) && !charIsWhiteSpace((X)))
#define charIsEOF(X) ((X) == EOF)
#define setCharNull(X) (X) = '\0'
#define setVarNull(X) (X) = NULL
#define setStringNull(X, n) memset(X,'\0', n)
#define stringIsSymbol(X) ((X) < SYMBOLS_SIZE && (X) >= 0)
#define stringIsRword(X) ((X) < RWORDS_SIZE && (X) >= 0)
#define copyString(A, B) strcpy(A, B)
#define varIsNull(X) ((X) == '\0' || (X) == NULL)
#define stringIsEmpty(X) (strlen(X) == 0)
#define moveFileCursor(F, X) fseek(F, X, SEEK_CUR)
#define printToken(I, X) printf("token#%d %3d %3d %s \n", I, X.attr, X.value, X.charvalue)

int getToken(void);
int checkRWord(char * chars);
int checkSymbol(char * chars);
void clearToken(void);