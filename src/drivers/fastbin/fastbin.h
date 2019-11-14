#ifndef _FASTBIN_H
#define _FASTBIN_H


#define FBTYPE_EMPTY 0
#define FBTYPE_DUART 1
#define FBTYPE_TPU 2
#define FBTYPE_TTL 3
#define FBTYPE_CPU 4

#define FBCPU_PCS1 1

#define setDUARTOPR(i) writebyte(DUART_OPRSET,1<<i)
#define clearDUARTOPR(i) writebyte(DUART_OPRRST,1<<i)


int init_fastbin();
int make_fastbin(int nr,char type,char id);
int set_fastbin(int nr,char v);

#endif
