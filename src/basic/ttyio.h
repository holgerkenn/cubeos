#ifndef _TTYIO_H
#define _TTYIO_H
extern char console_echo;
void conecho_on ();
void conecho_off ();
char inchar(void);
void outchar (char byte);
void init_tty (void);
#endif
