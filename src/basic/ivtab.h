#ifndef _IVTAB_H
#define _IVTAB_H

#define AVEC5 0x29

extern void (*(*IVTab)[512]) (void);

extern void *IVTab_oldvector[512];

void IVTab_init ();
int IVTab_setvector (unsigned short vector, void (*isr) (void));
int IVTab_clearvector (unsigned short vector, void (*isr) (void));

#endif
