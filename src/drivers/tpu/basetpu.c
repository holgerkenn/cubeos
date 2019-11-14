/* #define AS_FUNCTIONS */
#include <cubeos.h>
#include <mc68681.h>		/* needed !! */
#include <tpu.h>
#include <stdio.h>		// should be removed when done debugging
				// sonar

#define WFSR 1000		/* Wait For Service Request TimeOut limit */
// #define FULLDEBUG


/* The TPU consists of two 16-bit time bases (TCR1,TCR2) and 16 independent
   timer channels. A set of preprogrammed functions is coded in the micro
   engine. The host interface consists of the following genral registers:
   TPUMCR (Module Configuration)
   STOP
   TCR1P (2): Prescaler1 Divide
   TCR2P (2): Prescaler2 Divide
   EMU: Emulation Mode
   T2CG: TCR2 Clock or Gate controlled
   STF: Stop flag
   SUPV: all registers supervisor controlled
   PSCK: Prescaler Clock for TCR1
   00
   IARB: Int. Arbitration Number
   TICR (TPU Interrupt Configuration)
   00000
   CIRL (3): Interrupt Request Level
   CIBV (4): Interrupt Base Vector
   Each of the 15 channels is defined within a certain position in a set of
   registers. In a sequence of registers (e.g. R0,R1,R2,R3) the lower order 
   bits (in the register with the highest number) defines channel 0.
   The registers are:
   CFSR: Channel Function Select
   HSQR: Host Sequence (subfunction)
   HSSR: Host Service Request (00 indicates host, that no service is being
   requested; a write signals the TPU a service request; the host
   should wait then until the request is processed; for a write all
   but the intended bits have to be 0!)
   CPR: Channel Priority (00=disabled,01=low,10=middle,11=high; it is
   recommended to perform an initialisation before setting an active
   priority)
   CIER: Channel Interrupt Enable
   CISR: Channel Interrupt Status (clear through read + write a 0)
   CHPAR: Channel Parameter (6 for channels 0-13,8 for channels 14+15)
 */

int TPU_init ()
{


	setTPUMCR (0x4acd);
/*   STOP=0
   TCR1P=10 (divide by 4)
   TCR2P=01 (divide by 2)
   EMU=0
   T2CG=1 (T2CLK is enable for int. CLK/8 --> source for Prescaler2)
   SUPV=1
   PSCK=1 (int. CK/4 --> source for Prescaler1)
   IARB=d
 */

	setTICR (0x0480);
/*   CIRL=4
   CIBV=8
 */

	writeshort (TPU_CIER, 0);	/* Disable all interrupts */

	return (0);

}


int TPU_initchannel (unsigned char nr)
{
	setCFSR (nr, 0);	/* 0 = off */
	setHSRR (nr, 0);	/* no host service request */
	setCIER (nr, 0);	/* no irq */
	setCPR (nr, 0);		/* and no priority */
	return (0);
}

int TPU_makepwm (unsigned char nr)
{


	setCFSR (nr, 9);	/* 9 = PWM */
	setPAR (nr, 0, 0x8c);	/* Time Base TCR1, toggle on match */
	setPAR (nr, 2, 0);	/* High Time */
	setPAR (nr, 3, 20875);	/* Period */
	setHSRR (nr, 2);	/* Request Init */
	setCIER (nr, 0);	/* Interrupts disabled */
	setCPR (nr, 2);		/* Activate with middle priority */
	return (0);

}



int TPU_setpwmperiod (unsigned char nr, unsigned short period)
{
	int i;

	if (nr > 15) {
		return (-1);
	}
	if (getCFSR (nr) != 9) {
		return (-1);
	}
	i = 0;
	while ((getHSRR (nr) != 0) && (i < WFSR))
		i++;
	if (i >= WFSR) {
		return (-1);
	}
	setPAR (nr, 3, period);	/* Par 3 = Period */
	setHSRR (nr, 1);	/* Immediate Update */
	return (0);
}

int TPU_setpwmdc (unsigned char nr, unsigned char hightime)
{
	int i;
	unsigned long t;

	if (nr > 15) {
		return (-1);
	}
	if (getCFSR (nr) != 9) {
		return (-1);
	}
	if (hightime > 100) {
		return (-1);
	}
	i = 0;
	while ((getHSRR (nr) != 0) && (i < WFSR))
		i++;
	if (i >= WFSR) {
		return (-1);
	}
	t = (getPAR (nr, 3) * hightime) / 100;	/* to avoid intermediate
						   overflow */
	setPAR (nr, 2, (unsigned short) t);	/* Par 2 = High Time */
	setHSRR (nr, 1);	/* Immediate Update */
	return (0);
}

int TPU_makepac (unsigned char nr)
{
	setCFSR (nr, 10);	/* 10 = ITC */
	setHSQR (nr, 1);	/* Continuous Mode, No links */
	setPAR (nr, 0, 0x4);	/* Input Channel, detect rising edge */
	setPAR (nr, 2, 0xffff);	/* max count */
	setHSRR (nr, 1);	/* Request Init */
	setCIER (nr, 0);	/* Interrupts disabled */
	setCPR (nr, 2);		/* Activate with middle priority */
	return (0);

}

unsigned short TPU_getpac (unsigned char nr)
{
	unsigned short v;

	if (nr > 15) {
		return (0);
	}
	if (getCFSR (nr) != 10) {
		return (0);
	}
	v = getPAR (nr, 3);	/* Read PAC */
	setHSRR (nr, 1);	/* Reset PAC */
	return (v);

}

///////////////////////////////// Temp stuff
int TPU_make40kwave (unsigned char nr)
{
	setCFSR (nr, 9);	/* 9 = PWM */
	setPAR (nr, 0, 0x8c);	/* Time Base TCR1, toggle on match */
	setPAR (nr, 2, 12);	/* High Time */
	setPAR (nr, 3, 24);	/* Period */
	setHSRR (nr, 2);	/* Request Init */
	setCIER (nr, 0);	/* Interrupts disabled */
	setCPR (nr, 2);		/* Activate with middle priority */
	return (0);
}

int TPU_makeHigh (unsigned char nr)
{
	setCFSR (nr, 9);	/* 9 = PWM */
	setPAR (nr, 0, 0x8c);	/* Time Base TCR1, toggle on match */
	setPAR (nr, 2, 100);	/* High Time */
	setPAR (nr, 3, 100);	/* Period */
	setHSRR (nr, 2);	/* Request Init */
	setCIER (nr, 0);	/* Interrupts disabled */
	setCPR (nr, 2);		/* Activate with middle priority */
	return (0);
}
