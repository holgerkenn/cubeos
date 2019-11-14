
#include <stdio.h>

/* Interface routines for OS */
#define	OK	1
#define	CONSOLE	0

void outchar (char c);
char inchar ();


/* Internals */

inline char inbyte ()
{
	return (inchar ());
}

inline void outbyte (char c)
{
	outchar (c);
}

void outstring (char *str)
{
	char *ptr;

	ptr = str;
	while (ptr[0] != (char) 0)
		outchar (*ptr++);

}


void _exit (int status)
{
	printf ("\n\r **** PROGRAM EXIT ****");
	printf ("\n\r **** EXIT CODE %d ****\n\r", status);
}
