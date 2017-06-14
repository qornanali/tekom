#ifndef checkObjectMachine_H
#define	checkObjectMachine_H

typedef struct {
    char opcode[4];
    int b;
    int i;
    int a;
}ReadOnlyMemory;


void initRand(int s[], int size);
void printStack(int s[], int size,int *R1, int *R2);
void getObjectCode(ReadOnlyMemory m[], int *size);
void printROM(ReadOnlyMemory m[], int size);
int get_token();
void init_token();
void execute(int *PC, int *R1, int *R2, int *R3, int *R4, ReadOnlyMemory ROM[], int stackMemory[], int size);
void printStepROM(ReadOnlyMemory m[], int size, int PC);
void printStep(int *PC, int *R1, int *R2, int *R3, int *R4, ReadOnlyMemory ROM[], int stackMemory[]);
void SetColorAndBackground(int ForgC, int BackC);
#endif