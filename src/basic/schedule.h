#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <context.h> /* for regs */
#include <reent.h>
/* Again, goes into global_defs later */
#define MAX_PROCESSNUM 16
#define NO_TASK 0xffff; /* marker for empty task */

#define STATE_EMPTY 0   /* Slot not used */
#define STATE_RUNNING 1 /* Is on CPU */
#define STATE_READY 2   /* Ready to Roll */
#define STATE_SUSPEND 3 /* Sleeping */
#define STATE_KILLED 4  /* Almost dead */

struct process {
	reglist regs;
	void *stack; 
	char state;
	char irq;
	int signal;
	int mask;
	int ppid;
	struct _reent reent;
	int message;
	struct sem_t * next_sem;
	int time_delta;
};
extern struct process ptable[MAX_PROCESSNUM]; /* The process table */
int init_scheduler();
int create(void * function);
void schedule(void);
int getpid();
//int pmsg_send(int pid,int message);
//int pmsg_receive();
#endif
