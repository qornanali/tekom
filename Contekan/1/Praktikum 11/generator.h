#define INT "int"
#define LIT "lit"
#define LOD "lod"
#define STO "sto"
#define MVX "mvx"
#define OPR "opr"
#define CAL "cal"
#define RTN "rtn"
#define JMP "jmp"
#define JPC "jpc"
#define GET "get"
#define PUT "put"
#define HLT "hlt"

#define PLUSOP 0
#define MINUSOP 1
#define TIMESOP 3
#define DIVOP 4

void gencode(char *opcode, int b, int i, int a){
	FILE *OUTPUT_FILE;
	OUTPUT_FILE = fopen("generated_code.txt", "a+");
	
	fprintf(OUTPUT_FILE, "\n%s %d %d %d", opcode, b, i, a);
}
