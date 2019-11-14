#include <stdio.h>
#include <ptimer.h>





int main (int argc, char *argv[])
{
	int i, j;
	unsigned long *adr;
	unsigned long saveclock;

	printf ("\r\nCubeOS V1.0\r\n");
	printf ("ok\n\r");

	printf ("Watching Clock ticks\n\r");
	while (1) {
		saveclock = _gettimeofday ();
		printf ("Clock: %lu\n\r", saveclock);
		while (saveclock == _gettimeofday ());
	}

	return (0);
}
