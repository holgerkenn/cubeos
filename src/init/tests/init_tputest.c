#include <stdio.h>
#include <tpud.h>





int main (int argc, char *argv[])
{
	int i, j;
	unsigned long *adr;

	printf ("\r\nCubeOS TPUtest V1.0\r\n");

	printf ("Calling TPU_INIT\n\r");
	TPU_init ();
	printf ("Making Channel 6 PWM\n\r");
	TPU_makepwm (6);

	printf ("Sweeping Duty cycle...\n");
	for (i = 0; i <= 100; i++) {
		TPU_setpwmdc (6, i);
		for (j = 0; j < 10000; j++);
	}
	printf ("and down...\n");
	for (i = 100; i > 0; i--) {
		TPU_setpwmdc (6, i);
		for (j = 0; j < 10000; j++);
	}
	printf ("ok\n\r");
	printf ("Sweeping frequency...\n");
	for (i = 20875; i > 0; i--) {
		TPU_setpwmperiod (6, i);
		TPU_setpwmdc (6, 50);
		for (j = 0; j < 1000; j++);
	}

	for (i = 100; i > 0; i--) {
		TPU_setpwmperiod (6, i);
		TPU_setpwmdc (6, 50);
		for (j = 0; j < 100000; j++);
	}

	TPU_setpwmperiod (6, 3);
	TPU_setpwmdc (6, 50);

	printf ("ok\n\r");


	return (0);
}
