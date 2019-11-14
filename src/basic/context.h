/* This defines a context for a process or thread */
/* It's simply a block for the context switch to store data */
#ifndef CONTEXT_H
#define CONTEXT_H

#define PNREGS	19

#define POS_D0 0
#define POS_D1 1
#define POS_D2 2
#define POS_D3 3
#define POS_D4 4
#define POS_D5 5
#define POS_D6 6
#define POS_D7 7
#define POS_A0 8
#define POS_A1 9
#define POS_A2 10
#define POS_A3 11
#define POS_A4 12
#define POS_A5 13
#define POS_A6 14
#define POS_SSP 15
#define POS_SR 16
#define POS_PC 17


typedef struct regs *regptr;

typedef struct regs {
	long regs[PNREGS];
} reglist;

 void contextsw(regptr oldarea,regptr newarea);

#endif
