/* This is my first scheduler */
#include <stddef.h>
#include <stdio.h>
#include <signal.h>
#include <cubeos.h>
#include <sys_var.h>
#include <context.h>
#include <schedule.h>
#include <kerror.h>
#include <malloc.h>

#define MAGIC 0xbabababa;


struct process ptable[MAX_PROCESSNUM];	/* The process table */



void taskend ()
{
	ptable[__MYPID].signal |= SIGKILL;
	schedule ();
}

regptr
init_context (regptr context, short sr, void *sp, void *function)
{
	/* initializes context so that it looks like function is just called */

	int i;

	void *stack;

	stack = sp;

	for (i = 0; i < PNREGS; i++)
		(*context).regs[i] = 0;		/* Clear everything, just in case... */

	(*context).regs[POS_SR] = sr;	/* set up SR */


	(*context).regs[POS_PC] = (unsigned long) function;	/* set up PC */

/*      The context switch is called in schedule(). The initial stack setup has to
   simulate this origin. The Stack that we're pointing to must look like this:

   *contextsw()
   *schedule()
   *function()
 */

	/* Stack Bottom */
	writeint (stack, (unsigned int) MAGIC);
	stack -= 4;
	writeint (stack, (unsigned int) taskend);


	(*context).regs[POS_SSP] = (unsigned long) stack;	/* set up SSP */


	return context;
}

void init_reent (struct _reent *ptr)
{
	ptr->_errno = 0;
	ptr->_stdin = &(ptr->__sf[0]);
	ptr->_stdout = &(ptr->__sf[1]);
	ptr->_stderr = &(ptr->__sf[2]);
	ptr->_inc = 0;
	ptr->_emergency[0] = 0;	/* Empty string */
	ptr->_current_category = 0;
	ptr->_current_locale = "C";
}

int create (void *function)
{				/* no arguments yet */
	int i, j;
	short sr;
	void *stack;

/* MUTEX! */

	/* find empty process slot */
	i = 1;
	while ((i < MAX_PROCESSNUM) && (ptable[i].state != STATE_EMPTY))
		i++;
	if (i == MAX_PROCESSNUM) {	/* No processes left. */
		complain (ERR_EMERG, "process table full");
		/* errno=ERR_NO_MORE_PROCESSES; */
		return (-1);
	}
	for (j = 0; j < PNREGS; j++)
		ptable[i].regs.regs[j] = 0;	/* Zero out Slot i */
	/* Slot i is now ready to use */

	stack = (void *) malloc (TASK_INIT_STACKSIZE);	/* Get us a stack */
	if (stack == NULL) {
		complain (ERR_EMERG, "no room for stack");
		return (-1);
	}
	sr = TASK_INIT_SR;	/* Supervisor state */
	ptable[i].irq = TASK_INIT_IRQLVL;	/* standard IRQ mask */

	ptable[i].stack = stack;	/* for freeing the stack later */

	init_context (&(ptable[i].regs), sr, (stack + TASK_INIT_STACKSIZE), function);
	init_reent (&ptable[i].reent);
	ptable[i].ppid = getpid ();	/* Parent ID */

	ptable[i].state = STATE_READY;

/* MUTEX END */

	return i;
}

void schedule (void)
{
	int old, new;

/* prepare to switch context */

      asm ("move.w %%sr,%0":"=m" (context_srsave));
	asm ("ori.w #0x0700,%sr");	/* Disable Interrupts */


	old = getpid ();
	if (ptable[old].state != STATE_RUNNING) {
		/* We're not the running task ?!? */
		complain (ERR_PANIC, "scheduler not called from running task");
		return;		// we're in panic, so this will not be called 

	}
	if ((ptable[old].signal & (1 << SIGKILL))) {
		free (ptable[old].stack);
		ptable[old].state = STATE_EMPTY;
	}
	new = old;
	while ((new < MAX_PROCESSNUM) && (ptable[new].state != STATE_READY))
		new++;
	if (new == MAX_PROCESSNUM) {	/* wrap around */
		new = 0;
		while ((new < old) && (ptable[new].state != STATE_READY))
			new++;
	}
	if (old == new) {	/* Nobody else ready to run */
		if (ptable[old].state == STATE_EMPTY) {
			/* We've just killed the last task ?!? */
			complain (ERR_PANIC, "AIEEE, just killed my last task !");
			while (1);
			return;	// we're in panic, so this will not be called 

		}
	      asm ("move.w %0,%%sr": :"m" (context_srsave));
		return;
	}
	if (ptable[old].state != STATE_EMPTY)
		ptable[old].state = STATE_READY;

	__MYPID = new;

	ptable[new].state = STATE_RUNNING;

/*      print("\n\rcontext\n\rStack:");putnum(ptable[new].regs.regs[POS_SSP]);print("\n\r");
   print("\n\rFramep:");putnum(ptable[new].regs.regs[POS_A6]);print("\n\r"); */

	_impure_ptr = &(ptable[__MYPID].reent);

	contextsw (&(ptable[old].regs), &(ptable[new].regs));

	/* now we're the new task ! */
/*
   asm("move.l %%sp,%0":"m=" (stackp): );

   printf("Stack:");putnum(stackp);print("\n\r");
   asm("move.l %%fp,%0":"m=" (stackp): );

   printf("Framep:");putnum(stackp);print("\n\r");
 */

	return;

}


int init_scheduler ()
{
	int i, j;

	__MYPID = 0;

	/* empty process slots */
	for (i = 0; i < MAX_PROCESSNUM; i++) {
		ptable[i].state = STATE_EMPTY;
		for (j = 0; j < PNREGS; j++)
			ptable[i].regs.regs[j] = 0;
		ptable[i].stack = (void *) 0;
		ptable[i].signal = 0;
		ptable[i].ppid = 0;
	}

	ptable[0].state = STATE_RUNNING;
	init_reent (&ptable[0].reent);
	_impure_ptr = &(ptable[0].reent);	/* initialise libc reentrance */
	return 0;

}
