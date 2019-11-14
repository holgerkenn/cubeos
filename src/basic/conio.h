#ifndef _CONIO_H
#define _CONIO_H
#include <iobuf.h>

extern struct iobuf krnlin, krnlout;

void conecho_on();
void conecho_off();
char inchar (void);
void outchar (char byte);
void ioutchar(char byte);
void init_duart (void);

#endif
