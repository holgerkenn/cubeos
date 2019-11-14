#ifndef _TPUD_H
#define _TPUD_H

#define TPU_ILEVEL 0x4
#define TCR1FREQ 1000000
#define TCR2FREQ 1000000

int TPU_init();
int TPU_makepwm(unsigned char nr);
int TPU_initchannel(unsigned char nr);
int TPU_setpwmperiod(unsigned char nr, unsigned short period);
int TPU_setpwmdc(unsigned char nr, unsigned char hightime);
int TPU_makepac(unsigned char nr);
int TPU_makeqd(unsigned char ch1,unsigned char ch2);
unsigned short TPU_getpac(unsigned char nr);
short TPU_getqd(unsigned char nr);
short TPU_readqd(unsigned char nr);
int TPU_setisr (unsigned char ch, void (*isr) (void));
int TPU_clearisr (unsigned char ch, void (*isr) (void));
int TPU_makedio (unsigned char nr);
int TPU_setdio (unsigned char nr, unsigned char v);


#endif

