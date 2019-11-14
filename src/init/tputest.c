#include <stdio.h>
#include <cubeos.h>
#include <tpu.h>
#include <tpud.h>
#include "testmak.h"

int tpuirqtest;

void tpuisrw ();		/* wrapper code */

void tpu_int ()
{
	int dummy;

	dummy = getCISR (6);
	clearCISR (6);
	tpuirqtest++;

}

int TPUTest ()
{
	int i, j;

	printf ("Calling TPU_INIT\n");
	TPU_init ();
	printf ("Making Channel 6 PWM\n");
	TPU_makepwm (6);

	printf ("Sweeping Duty cycle...\n");
	for (i = 0; i <= 100; i++) {
		TPU_setpwmdc (6, i);
		for (j = 0; j < 3000; j++);
	}
	printf ("and down...\n");
	for (i = 100; i > 0; i--) {
		TPU_setpwmdc (6, i);
		for (j = 0; j < 3000; j++);
	}

	printf ("ok\n");
	printf ("Sweeping frequency...\n");
	for (i = 20875; i > 0; i--) {
		TPU_setpwmperiod (6, i);
		TPU_setpwmdc (6, 50);
//        for (j = 0; j < 10; j++) ;
	}

	TPU_setpwmperiod (6, 10000);
	TPU_setpwmdc (6, 50);
	printf ("ok\n");
	printf ("Testing irq callback\n");
	TPU_initchannel (6);

	tpuirqtest = 0;

	failif (TPU_setisr (6, tpuisrw), "could not set TPU ISR\n");

	TPU_makepwm (6);
	TPU_setpwmperiod (6, TCR1FREQ / 100);
	TPU_setpwmdc (6, 0);
	setCIER (6, 0x1);
	setHSRR (6, 2);

	for (i = 1; i < 100000; i++);

	setCIER (6, 0x0);
	setHSRR (6, 2);

	printf ("seen %d isr calls\n", tpuirqtest);

	TPU_initchannel (6);

	failif (TPU_clearisr (6, tpuisrw), "could not clear TPU ISR\n");

	printf ("ok\n\r");

	failif (tpuirqtest == 0, "No ISR callbacks seen");

	return (0);

}
