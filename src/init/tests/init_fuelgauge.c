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

	printf ("\r\nCubeOS SAItest V1.0\r\n");

	printf ("Calling init_sai_cube\n\r");
	init_sai_cube ();

	printf ("Trying to write fuelgauge\n\r");
	sa_write (FG_STATCONF, 0x0000);


	printf ("Trying to read fuelgauge\n\r");
	printf ("Result is %d", sa_read (FG_COUNT01));


	printf ("ok\n\r");


	return (0);
}
