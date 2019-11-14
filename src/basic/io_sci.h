#ifndef _IO_SCI_H
#define _IO_SCI_H

#include <iobuf.h>

void sciecho_on ();
void sciecho_off ();
void sci_int (void);
char sinchar (void);
void soutchar (char byte);
void init_sci (void);

#endif
