#ifndef PTIMER_H
#define PTIMER_H

#include <cubeos.h>

#define PTIMER_VECTOR 64
#define PTIMER_IRQ_LEVEL 4

#define XTAL_FREQ 32768 /* Hz */

#define PTIMER_PITR_VAL 0x0008	/* 977 uSec */
#define TICKS_PER_SECOND 1024
#define QUANTUM TICKS_PER_SECOND/8 /* every 64 ticks = 62.5msec */
//#define PTIMER_PITR_VAL 0x0110 /* 1Sec */
//#define TICKS_PER_SECOND 1
//#define QUANTUM TICKS_PER_SECOND
//#define PTIMER_PITR_VAL 0x0101        /* 62.5 mSec */
//#define TICKS_PER_SECOND 16
//#define QUANTUM TICKS_PER_SECOND

int ptimer_int(void);
void init_ptimer(void);
unsigned long _gettimeofday();
unsigned long _settimeofday(unsigned long val);
//extern char ptimer_clears_wdt;

//static inline void clearWDT()
//{
//	writebyte(SIM_SWSR,0x55);
//	writebyte(SIM_SWSR,0xAA);
//}

#endif
