#ifndef CUBEOS_H

#define CUBEOS_H
#include <global_def.h>
#include <mc68332.h>
#include <sys_var.h>
#include <schedule.h>
#include <taskconst.h>

static inline void disable()
{
asm("ori.w #0x0700,%sr");  /* Disable Interrupts */
}

static inline void enable()
{
/*asm("move.w %%sr,%%d0;
     andi #0xf8ff,%%d0;
     or %%d0,%0;
     move.w %%d0,%%sr": :"m"((ptable[__MYPID].irq)<<8): "d0" );*/
     asm("move.w #0x2000,%sr"); 
}

#ifdef MULTI_SEM
/* CubeOS/POSIX Semaphores */
typedef struct sem_s {
	int init;
	int c;
	int task;
	} sem_t;
/* complex semaphores in multithreading */
static inline void sem_post(sem_t x)
{
	if (!x.init) return;
	disable();
	x.c++;
	if (x.task!=NO_TASK)
	{
		wakeup(x.task); // this resets the task state, 
						// but we're still in disable()d state!
 		x.task=ptable[x.task].next_sem;
	}
	enable();
}
#define sem_signal(x) sem_post(x) /* for thomas */
static inline void sem_wait(sem_t x)
{
	if (!x.init) return;
	disable();	
	x--;
	if (x<0){
		// put ourself to sleep
		if (x.task = NO_TASK)
		{
			ptable[getpid()].next_sem=NO_TASK;
		} 
		else
		{
			ptable[getpid()].nest_sem=x.task;
		}
		x.task=getpid();
		enable();
		sleep();
	} 
	else 
	{
		enable(); // the semaphore was free
	}
}

#define sem_trywait(x) (x.c<0) /* might this block? */
#define sem_getvalue(x,i) (*i=x)

/* posix conventions. Quite Useless for CubeOS, but anyway */
static inline int sem_init(sem_t x,int i,int v)
{
	if (x.init) return (-1);
	disable();
	x.c=v;
	x.task=NO_TASK;
	x.enabled=1;
	enable();
}

static inline void sem_destroy(sem_t x)
{
	if (!x.init) return (-1);
	disable();
	while(x.task!=NO_TASK)
		{
			wakeup(x.task); // this resets the task state, 
							// but we're still in disable()d state!
 			x.task=ptable[x.task].next_sem;
		}
	enable();
}
#else

/* CubeOS/POSIX Semaphores */
typedef int sem_t;
/* simple semaphores with spin lock*/
#define sem_post(x) x++;
#define sem_signal(x) sem_post(x) /* for thomas */
#define sem_wait(x) x--;while(x<0) /* does not return a propper value ?!?*/
/* are signal and wait atomic with this ? */

#define sem_trywait(x) (x<0) /* might this block? */
#define sem_getvalue(x,i) (*i=x)

/* posix conventions. Quite Useless for CubeOS, but anyway */
#define sem_init(x,i,v) (x.c=v)
#define sem_destroy(x) (0)

#endif

/*  mutex */
/* Should be posix too. */
#define mutex sem_t; 
/* A mutex is a semaphore that starts out eqal 1 */
#define mutex_init(i) sem_init(i,1,1)
#define mutex_enter(i) sem_wait(i)
#define mutex_leave(i) sem_signal(i)
/* Or should we have a hw-dependant mutex implementation 
   with test&set instruction? */




#define readbyte(x)    (*(volatile char *)x)
#define writebyte(x,y) *(char *)(x) = y
#define readshort(x)    (*(short *)(void *)(x))
#define writeshort(x,y) *(short *)(x) = y
#define readint(x)    (*(int *)(void *)(x))
#define writeint(x,y) *(int *)(x) = y

#define writeshortpos(x,y,mask,pos) writeshort(x,(readshort(x)\
				      &(~((mask)<<(pos)))) \
				      |(((mask)&y)<<(pos)))
/* Writes y at x, shifted left by pos, masked with mask */
/* example writeshortpos(TPU_CFSR3,0x9,0xf,0x8) */
/*  writes function code 0x9 (4 bits wide, so mask 0xf) into TPU */
/*  channel function select register for TPU Channel 2 (bits 8-11) */  

#define readshortpos(x,mask,pos) ((readshort(x)>>(pos))&(mask))
/* This reads... */


static inline void init_LED()
{
writebyte(SIM_PORTE,0x0);
writebyte(SIM_DDRE,0x8);
writebyte(SIM_PEPAR,0xf7);
}

static inline void LED_ON()
{
writebyte(SIM_PORTE,0x8);
}

static inline void LED_OFF()
{
writebyte(SIM_PORTE,0x0);
}

#endif
