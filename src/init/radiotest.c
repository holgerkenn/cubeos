#include <stdio.h>
#include <cubeos.h>
#include <rdio.h>
#include <rsm.h>
#include <ptimer.h>

extern unsigned char duart_imrsave;

int RadioTestRx ()
{
	int i;
	char frame[RSM_FRAMELEN];

	printf ("\nCubeOS RSM-RX-Test\n");
	printf ("calling init_rsm_rx()\n");
	init_rsm_rx ();
	printf ("ok\n");
	printf ("rsm_enabled = %d\n", rsm_enabled);
	printf ("imrsave = %x\n", duart_imrsave);

	for (i = 0; i < 1000000; i++) {
		if (!rsm_get_frame (frame)) {
			printf (".");
			fflush (stdout);
		}
	}

	printf ("rsm_enabled = %d\n", rsm_enabled);
	printf ("imrsave = %x\n", duart_imrsave);

	deinit_rsm_rx ();

	printf ("rsm_enabled = %d\n", rsm_enabled);
	printf ("imrsave = %x\n", duart_imrsave);

	printf ("rsm_good = %d\nrsm_bad = %d \nrsm_ugly = %d at %d\nrsm_bytes = %d\n",
		rsm_good, rsm_bad, rsm_ugly, rsm_uglycount, rsm_bytes);


	return (0);
}

int RadioTestTx ()
{
	int i, j;
	char frame[RSM_FRAMELEN];

	printf ("\nCubeOS RSM-TX-Test\n");
	printf ("calling init_rsm_tx()\n");
	init_rsm_tx ();
	for (i = 1; i < 100000; i++);
	printf ("ok\n");

	printf ("Sending 100 test packets");
	fflush (stdout);
	for (i = 0; i < 100; i++) {
		sprintf (frame, "RSM-RX-Test Packet %d", i);
		for (j = strlen (frame); j < (RSM_FRAMELEN - 1); j++)
			frame[i] = 'X';
		frame[RSM_FRAMELEN - 1] = 0;
		rsm_send_frame (frame);
		printf (".");
		fflush (stdout);
	}


	deinit_rsm_tx ();
	printf ("\n");

	return (0);
}
