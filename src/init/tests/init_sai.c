#include <stdio.h>
#include <global_def.h>
#include <tpu.h>
#include <tpud.h>
#include <sai_cube.h>

#define MOTOR1 &RoboCube.tpu[6]
#define MOTOR1PERIOD &RoboCube.tpu[6],CTL_TPU_SETPWMPERIOD

int main (int argc, char *argv[])
{
	int i, j;
	unsigned long *adr;

	printf ("\r\nCubeOS SAItest V1.0\r\n");

	printf ("Calling init_sai_cube\n\r");
	init_sai_cube ();

	printf ("Making Channel 6 PWM\n\r");

	sa_control (&RoboCube.tpu[6], CTL_TPU_SETFUNCTION, TPU_FKT_PWM);

	sa_control (MOTOR1PERIOD, 32000);


	sa_write (MOTOR1, 16000);

	return (0);

	printf ("Sweeping Duty cycle...\n");
	for (i = 0; i <= 32000; i++) {
		sa_write (MOTOR1, i);
	}
	printf ("and down...\n");
	for (i = 32000; i > 0; i--) {
		sa_write (MOTOR1, i);
	}

	printf ("ok\n\r");


	return (0);
}
