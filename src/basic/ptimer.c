#include <ptimer.h>
#include <cubeos.h>
#include <sys_var.h>
#include <io_duart.h>

/*
 * The periodic timer is a internal device of the 68332
 */
//char ptimer_clears_wdt;

int ptimer_int (void)
{

//      if (ptimer_clears_wdt)
	//              clearWDT();

	/* Advance the system clock */
	if ((++_time_ticks) == TICKS_PER_SECOND) {
		_time_seconds++;
		_time_ticks = 0;
	}
	duart_bugfix ();
	if (++_quantum_count == QUANTUM) {
		_quantum_count = 0;
		return (1);	/* and call scheduler */
	}
	return (0);		/* don't call scheduler */
}


void init_ptimer (void)
{
	_time_seconds = 0;
	_time_ticks = 0;
	writeshort (SIM_PITR, PTIMER_PITR_VAL);

	writeshortpos (SIM_PICR, PTIMER_VECTOR, 0xff, 0x0);
	writeshortpos (SIM_PICR, PTIMER_IRQ_LEVEL, 0x7, 0x8);

//      ptimer_clears_wdt = 1; /* clear WDT on timer int */


// the software watchdog timeout period is determined by 
	//      XTAL_FREQ: The clock frequency of the VCO/external clock source
	//      SIM_SYPCR.SWP: Clock Prescaler, 1: div 512 2: no prescaler
	//      SIM_SYPCR.SWT0/1





}

unsigned long _gettimeofday ()
{
	return _time_seconds;
}

int gettimeofday (struct timeval *tv, void *ignore)
{

// tv->tv_sec = _time_seconds; 

// tv->tv_usec = (_time_ticks*1000/TICKS_PER_SECOND); 
	return 0;
}

unsigned long _settimeofday (unsigned long val)
{
	writeshort (SIM_PITR, 0);	/* Disable timer interrupt */
	_time_ticks = 0;
	_time_seconds = val;	/* set clock */
	writeshort (SIM_PITR, PTIMER_PITR_VAL);		/* and return it */
	return _time_seconds;
}
