/* #define AS_FUNCTIONS */
#include <global_def.h>
#include <mc68681.h>		/* Probably not needed */
#include <tpu.h>
#include "hardware.h"

#define WFSR 1000		/* Wait For Service Request TimeOut limit */



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

	printf ("This is TPU_init()\n\r");

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



void TPU_smb2_init ()
{
	unsigned int i, j, failed, active_channels;	/* for whatever... */

/* The following initialisation sequence relies on the fact that no other
   process accessed before the TPU module.
   All frequencies on the assumption of 16.78MHz internal clock speed
   (==> TCR1 runs with 1048.75kHz)
 */


/* TP0: IR modulation frequency (40.3kHz) */


	setCFSR (AIR0, 9);


	setPAR (AIR0, 0, 0x8c);	/* Time Base TCR1, toggle on match */
	setPAR (AIR0, 2, 13);	/* High Time */
	setPAR (AIR0, 3, 26);	/* Period */
	setHSRR (AIR0, 2);	/* Request Init */
	setCIER (AIR0, 0);	/* Interrupts disabled */
	setCPR (AIR0, 2);	/* Activate with middle priority */

/* TP1: IR carrier frequency (157Hz; 75% high) */
	setCFSR (AIR1, 9);	/* 9 = PWM */
	setPAR (AIR1, 0, 0x8c);	/* Time Base TCR1, toggle on match */
	setPAR (AIR1, 2, 5000);	/* High Time */
	setPAR (AIR1, 3, 6666);	/* Period */
	setHSRR (AIR1, 2);	/* Request Init */
	setCIER (AIR1, 0);	/* Interrupts disabled */
	setCPR (AIR1, 2);	/* Activate with middle priority */

/* TP2: unused */

/* TP3: US, digital output */
	setCFSR (USOUT, 8);	/* 8 = Discrete I/O */
	setPAR (USOUT, 0, 0x80);	/* Output channel */
	setCIER (USOUT, 0);	/* Interrupts disabled */
	/* Pin remains deactivated */

/* TP4: Sound carrier frequency (502Hz) */
	setCFSR (SND0, 9);	/* 9 = PWM */
	setPAR (SND0, 0, 0x8c);	/* Time Base TCR1, toggle on match */
	setPAR (SND0, 2, 1045);	/* High Time */
	setPAR (SND0, 3, 2090);	/* Period */
	setHSRR (SND0, 2);	/* Request Init */
	setCIER (SND0, 0);	/* Interrupts disabled */
	setCPR (SND0, 3);	/* Activate with high priority */

/* TP5: Sound modulation frequency (50Hz) */
	setCFSR (SND1, 9);	/* 9 = PWM */
	setPAR (SND1, 0, 0x8c);	/* Time Base TCR1, toggle on match */
	setPAR (SND1, 2, 10400);	/* High Time */
	setPAR (SND1, 3, 20900);	/* Period */
	setHSRR (SND1, 2);	/* Request Init */
	setCIER (SND1, 0);	/* Interrupts disabled */
	setCPR (SND1, 1);	/* Activate with low priority */

/* TP6: Motor1 Pulse Width Modulation (base frequency 50.2Hz) */
	setCFSR (MT1P, 9);	/* 9 = PWM */
	setPAR (MT1P, 0, 0x8c);	/* Time Base TCR1, toggle on match */
	setPAR (MT1P, 2, 0);	/* High Time */
	setPAR (MT1P, 3, 20875);	/* Period */
	setHSRR (MT1P, 2);	/* Request Init */
	setCIER (MT1P, 0);	/* Interrupts disabled */
	setCPR (MT1P, 2);	/* Activate with middle priority */

/* TP7: Motor1 Pulse Accumulation */
	setCFSR (MT1A, 10);	/* 10 = ITC */
	setHSQR (MT1A, 1);	/* Continuous Mode, No links */
	setPAR (MT1A, 0, 0x4);	/* Input Channel, detect rising edge */
	setPAR (MT1A, 2, 0xffff);	/* max count */
	setHSRR (MT1A, 1);	/* Request Init */
	setCIER (MT1A, 0);	/* Interrupts disabled */
	setCPR (MT1A, 2);	/* Activate with middle priority */

/* TP8: Motor2 Pulse Width MOdulation (base frequency 50.2Hz) */
	setCFSR (MT2P, 9);	/* 9 = PWM */
	setPAR (MT2P, 0, 0x8c);	/* Time Base TCR1, toggle on match */
	setPAR (MT2P, 2, 0);	/* High Time */
	setPAR (MT2P, 3, 20875);	/* Period */
	setHSRR (MT2P, 2);	/* Request Init */
	setCIER (MT2P, 0);	/* Interrupts disabled */
	setCPR (MT2P, 2);	/* Activate with middle priority */

/* TP9: Motor2 Pulse Accumulation */
	setCFSR (MT2A, 10);	/* 10 = ITC */
	setHSQR (MT2A, 1);	/* Continuous Mode, No links */
	setPAR (MT2A, 0, 0x4);	/* Input Channel, detect rising edge */
	setPAR (MT2A, 2, 0xffff);	/* max count */
	setHSRR (MT2A, 1);	/* Request Init */
	setCIER (MT2A, 0);	/* Interrupts disabled */
	setCPR (MT2A, 2);	/* Activate with middle priority */

/* TP10: Motor3 Pulse Width MOdulation (base frequency 50.2Hz) */
	setCFSR (MT3P, 9);	/* 9 = PWM */
	setPAR (MT3P, 0, 0x8c);	/* Time Base TCR1, toggle on match */
	setPAR (MT3P, 2, 0);	/* High Time */
	setPAR (MT3P, 3, 20875);	/* Period */
	setHSRR (MT3P, 2);	/* Request Init */
	setCIER (MT3P, 0);	/* Interrupts disabled */
	setCPR (MT3P, 2);	/* Activate with middle priority */

/* TP11: Motor3 Pulse Accumulation */
	setCFSR (MT3A, 10);	/* 10 = ITC */
	setHSQR (MT3A, 1);	/* Continuous Mode, No links */
	setPAR (MT3A, 0, 0x4);	/* Input Channel, detect rising edge */
	setPAR (MT3A, 2, 0xffff);	/* max count */
	setHSRR (MT3A, 1);	/* Request Init */
	setCIER (MT3A, 0);	/* Interrupts disabled */
	setCPR (MT3A, 2);	/* Activate with middle priority */

/* TP12: US, Input Transition Counter, either edge detect */
	setCFSR (USITC, 10);	/* 10 = ITC */
	setHSQR (USITC, 1);	/* Continuous Mode, No links */
	setPAR (USITC, 0, 0xc);	/* Input Channel, detect either edge */
	setPAR (USITC, 2, 0);	/* max count */
	setCIER (USITC, 0);	/* Interrupts disabled */
	/* channel remains deactivated */

/* TP13: unused */

/* TP14: US, Output Capture, read TCR1, no activity on pin */
	setCFSR (USOC, 14);	/* 14 = Output Compare */
	setHSQR (USOC, 3);	/* only read TCR1/TCR2 */
	setPAR (USOC, 0, 0x83);	/* Time Base TCR1, Pin deactivated */
	setCIER (USOC, 0);	/* Interrupts disabled */
	/* channel remains deactivated */

/* TP15: PDL clock (40.2Hz) */
	setCFSR (PDLCK, 9);	/* 9 = PWM */
	setPAR (PDLCK, 0, 0x8c);	/* Time Base TCR1, toggle on match */
	setPAR (PDLCK, 2, 13046);	/* High Time */
	setPAR (PDLCK, 3, 26093);	/* Period */
	setHSRR (PDLCK, 2);	/* Request Init */
	setCIER (PDLCK, 0);	/* Interrupts disabled */
	setCPR (PDLCK, 2);	/* Activated with middle priority */

/* monitor, whether all initialisations have been performed */

	failed = 0;
	active_channels = (1 << AIR0) | (1 << AIR1) | (1 << SND0) |
		(1 << SND1) |
		(1 << MT1P) | (1 << MT1A) | (1 << MT2P) | (1 << MT2A) |
		(1 << MT3P) | (1 << MT3A) | (1 << PDLCK);
	for (j = 0; j < 16; j++) {
		if ((active_channels & (1 << j)) != 0) {
			i = 0;
			while ((getHSRR (j) != 0) && (i < WFSR))
				i++;
			if (i >= WFSR)
				failed |= (1 << j);
		}
	}
	if (failed == 0) {
		printf ("ok, ");
	} else {
		printf ("failed\r\n Flags: 0x%x\r\n", failed);
	}
}


int TPU_makepwm (unsigned char nr)
{

	printf ("This is TPU_makepwm() fo channel %d\n\r", nr);

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
		printf ("\r\nUndefined TPU channel %u\r\n", nr);
		return;
	}
	if (getCFSR (nr) != 9) {
		printf ("\r\nTPU channel %u not with PWM defined >%d< \r\n", nr, getCFSR (nr));
		return (-1);
	}
	i = 0;
	while ((getHSRR (nr) != 0) && (i < WFSR))
		i++;
	if (i >= WFSR) {
		printf ("\r\nTimeOut for Service Request TPU channel %u\r\n", nr);
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
		printf ("\r\nUndefined TPU channel %u\r\n", nr);
		return (-1);
	}
	if (getCFSR (nr) != 9) {
		printf ("\r\nTPU channel %u not with PWM defined >%d< \r\n", nr, getCFSR (nr));
		return (-1);
	}
	if (hightime > 100) {
		printf ("\r\nValue for High time on TPU channel %u is too large\r\n", nr);
		return (-1);
	}
	i = 0;
	while ((getHSRR (nr) != 0) && (i < WFSR))
		i++;
	if (i >= WFSR) {
		printf ("\r\nTimeOut for Service Request TPU channel %u\r\n", nr);
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
	printf ("This is TPU_makepac() fo channel %d\n\r", nr);
	setCFSR (nr, 10);	/* 10 = ITC */
	setHSQR (nr, 1);	/* Continuous Mode, No links */
	setPAR (nr, 0, 0x4);	/* Input Channel, detect rising edge */
	setPAR (nr, 2, 0xffff);	/* max count */
	setHSRR (nr, 1);	/* Request Init */
	setCIER (nr, 0);	/* Interrupts disabled */
	setCPR (nr, 2);		/* Activate with middle priority */
}

unsigned short TPU_getpac (unsigned char nr)
{
	unsigned short v;

	if (nr > 15) {
		printf ("\r\nUndefined TPU channel %u\r\n", nr);
		return;
	}
	if (getCFSR (nr) != 10) {
		printf ("\r\nTPU channel %u not with PWM defined >%d< \r\n", nr, getCFSR (nr));
		return (-1);
	}
	v = getPAR (nr, 3);	/* Read PAC */
	setHSRR (nr, 1);	/* Reset PAC */
	return (v);

}
