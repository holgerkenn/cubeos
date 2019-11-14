#include "fbintest.h"
#include <stdio.h>
#include "testmak.h"
#include <cubeos.h>
#include <fastbin.h>

int FBinTest ()
{

	int i;

	failif (FASTBIN_BASE == 0, "No fastbins around...");

	printf ("init_fastbin\n");

	init_fastbin ();

	printf ("Making fastbins...");
	fflush (stdout);
	for (i = 0; i < 15; i++) {
		printf ("%d ", i);
		checkif (make_fastbin (i, FBTYPE_TTL, i) == 0, "couldn't make fastbin");
	}
	printf ("\n");

	printf ("Enabling fastbins...");
	fflush (stdout);
	for (i = 0; i < 15; i++) {
		printf ("%d ", i);
		checkif (set_fastbin (i, 1) == 0, "couldn't enable fastbin");
	}
	printf ("\n");

	printf ("disabeling fastbins...");
	fflush (stdout);
	for (i = 0; i < 15; i++) {
		printf ("%d ", i);
		checkif (set_fastbin (i, 0) == 0, "couldn't disable fastbin");
	}
	printf ("\n");

	printf ("Flashing fastbins FAST");
	fflush (stdout);
	for (i = 1; i < 1000000; i++) {
		writeshort (FASTBIN_BASE, 0xffff);
		writeshort (FASTBIN_BASE, 0x0000);
	}
	printf ("\nOK\n");

	return (0);
}
