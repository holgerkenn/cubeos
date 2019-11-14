#include <cubeos.h>
#include <icontrol.h>
#include <tpu.h>
#include <tpud.h>
#include <sincos.h>
#include <math.h>
#include <fastbin.h>

int isrcount;

struct position pos;

unsigned short period;

void icontrolWrapper ();

int init_icontrol ()
{


	init_sincos ();
	init_fastbin ();
	TPU_init ();
	period = TCR1FREQ / PULSEFREQ;

	TPU_makepwm (LEFT_PWM);
	TPU_setpwmperiod (LEFT_PWM, period);
	TPU_makepwm (RIGHT_PWM);
	TPU_setpwmperiod (RIGHT_PWM, period);
	TPU_makeqd (LEFT_QD1, LEFT_QD2);
	TPU_makeqd (RIGHT_QD1, RIGHT_QD2);
	TPU_setisr (LEFT_PWM, icontrolWrapper);
	setCIER (LEFT_PWM, 0x1);	/* Now, icontrol_int will be called */
	setHSRR (LEFT_PWM, 2);	/* request init */
	setHSRR (RIGHT_PWM, 2);	/* request init */


	return (0);

}




void do_pfield (void *field, int xpos, int ypos, int *newleft, int *newright)
{






}

void icontrol_int ()
{
	short beta;
	short pl;
	short pr;
	float df;
	float betaf;

	int d;

/* get qd */
	char dummy;

	dummy = getCISR (LEFT_PWM);
	clearCISR (LEFT_PWM);
	isrcount++;

	pl = TPU_getqd (LEFT_QD2);
	pr = TPU_getqd (RIGHT_QD2);


/* calculate vector */
	df = (pl + pr) * (3141593.0 * WHEELDIA * 196.0) / (1000000.0 * 113.0 * COUNTSPERREV);
	d = df;
	betaf = (pl - pr) * (18000.0 * WHEELDIA) / (GEARRATIO * COUNTSPERREV * WHEELDIST);
	beta = betaf;
	if (!(isrcount % 1000)) {
		printf ("\n");
		printf ("pl = %d\n", pl);
		printf ("pr = %d\n", pl);
		printf ("betaf = %f\n", betaf);
		printf ("beta = %d\n", beta);
		printf ("df = %f\n", df);
		printf ("d = %d\n", d);
		printf ("\n");
		printf ("xpos = %d\n", pos.xpos);
		printf ("ypos = %d\n", pos.ypos);
		printf ("angle = %d\n", pos.angle);

	}
/* calculate new position, angle and normalize it */

	pos.xpos = pos.xpos + (mycos (beta) * d / 10000);
	pos.ypos = pos.ypos + (mysin (beta) * d / 10000);
	pos.angle = pos.angle + beta;
	if (pos.angle > 18000)
		pos.angle -= 36000;
	if (pos.angle <= -18000)
		pos.angle += 36000;



/*      do_pfield (field,pos.xpos, pos.ypos, &newleft, &newright);

   icontrol_set(newleft,newright); */

/*      TPU_setpwmdc (LEFT_PWM, newleft);
   TPU_setpwmdc (LEFT_PWM, newright); */
}

void icontrol_set (int newleft, int newright)
{

	if (newright > 0) {
		set_fastbin (LEFT_D0, 0);
		set_fastbin (LEFT_D1, 1);
	} else {
		set_fastbin (LEFT_D0, 1);
		set_fastbin (LEFT_D1, 0);
	}
	if (newleft < 0) {
		set_fastbin (RIGHT_D0, 0);
		set_fastbin (RIGHT_D1, 1);

	} else {
		set_fastbin (RIGHT_D0, 1);
		set_fastbin (RIGHT_D1, 0);
	}

	setPAR (LEFT_PWM, 2, abs (newleft) * period / 10000);
	setPAR (RIGHT_PWM, 2, abs (newright) * period / 10000);

}
