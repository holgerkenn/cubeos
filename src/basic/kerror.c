#include <kerror.h>
#include <stdio.h>

void k_panic ()
{

	printf ("KERNEL PANIC\n\r");

	while (1);

/* flush the DUART queue */

}

void complain (int i, char *text)
{

	printf ("%d: %s\n\r", i, text);

	if (i == ERR_PANIC)
		k_panic ();

}
