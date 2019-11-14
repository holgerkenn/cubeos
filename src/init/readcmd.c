#include <readcmd.h>
#include <cubeos.h>
#include <stdio.h>
#include <conio.h>

char commandbuf[MAXCLEN];

char *readcmd (char *prompt)
{
	char *iptr;		/* points to the location of the next insert */
	char c;
	int done;
	int processed;
	if (prompt)
		printf ("%s", prompt);
	fflush (stdout);
	conecho_off ();
	done = 0;

	iptr = commandbuf;

	while (!done) {
		processed = 0;
		c = inchar ();
		if (c == CRin) {
			done = 1;
			processed = 1;
		}
		if (c == BSin) {
			/* Backspace */
			if (iptr > commandbuf) {
				iptr--;
				printf (BSout);
				fflush (stdout);
				processed = 1;
			}
		}
		if (c >= 0x20 && c <= 0x7E) {
			/* usual char */
			if ((iptr - commandbuf) < MAXCLEN) {
				(*iptr++) = c;
				outchar (c);
				processed = 1;
			}
		}
		/* default */
		if (processed == 0) {
			printf (BELLout);
			fflush (stdout);
		}
	}

	(*iptr) = 0;		/* End of String */
	conecho_on ();
	return (commandbuf);
}
