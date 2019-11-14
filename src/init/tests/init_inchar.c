#include <stdio.h>
#include <global_def.h>
#include <tpu.h>
#include <tpud.h>
#include <sai_cube.h>
#include <rdio.h>
#include <fuelgauge.h>


int main (int argc, char *argv[])
{
	int i, j;
	unsigned long *adr;
	char test[256];
	char c;


	printf ("\nCubeOS Test V1.0\n");


	while (1) {
		c = inchar ();

		printf (" Got %c: %d\n\r", c, c);

	}
	return (0);
}
