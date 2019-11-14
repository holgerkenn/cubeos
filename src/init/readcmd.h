#ifndef _READCMD_H
#define _READCMD_H

#define MAXCLEN 60  /* for command line */
#define BSin   '\b' /* Backspace send from terminal */
#define CRin   '\r' /* CR send from terminal */
#define BSout  "\b \b" /* Backspace char send to terminal, destructive */
#define NLout  "\n" /* NewLine */
#define BELLout  "\007" /* NewLine */

char *readcmd (char * prompt);

#endif
