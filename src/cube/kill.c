/* kill.c -- signal to a process.

 */

#include "glue.h"		/* for Cygnus stuff */
#include <sys_var.h>
#include <schedule.h>

/*
 * kill 
 */
int _DEFUN (kill, (pid, sig),
	    int pid _AND
	    int sig)
{

	if (pid > MAX_PROCESSNUM)
		return (-1);
	if (pid < 0)
		return (-1);
	if (ptable[pid].state == STATE_EMPTY)
		return (-1);
	if (sig < 0)
		return (-1);
	if (sig > 31)
		return (-1);
	ptable[pid].signal |= (1 << sig);
	return 0;
}
