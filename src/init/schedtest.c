#include <stdio.h>
#include <signal.h>
#include <cubeos.h>
#include <schedule.h>
#include "testmak.h"

int thread1count;
int thread2count;

void thread_1 ()
{
	int i;
	while (1) {
		thread1count++;
		for (i = 1; i < 10000; i++);
		outchar ('1');
	}
}

void thread_2 ()
{
	int i;
	while (1) {
		thread2count++;
		for (i = 1; i < 10000; i++);
		outchar ('2');
	}
}

int SchedTest ()
{
	int handle1, handle2;
	int i;

	printf (" starting threads\n");
	fflush (stdout);
	handle1 = create (thread_1);
	handle2 = create (thread_2);

	for (i = 1; i < 500000; i++);

	printf ("\nkilling threads\n");
	fflush (stdout);
	kill (handle1, SIGKILL);
	kill (handle2, SIGKILL);

	for (i = 1; i < 100000; i++);	/* wait a bit to give time for the threads to react */

	printf ("\n");
	printf (" Thread1 counted %d\n", thread1count);
	printf (" Thread2 counted %d\n", thread2count);

	failif (thread1count == 0, "Thread 1 didn't run");
	failif (thread2count == 0, "Thread 2 didn't run");

	failif (ptable[handle1].state != STATE_EMPTY, "Thread 1 still running !");
	failif (ptable[handle2].state != STATE_EMPTY, "Thread 2 still running !");

	return (0);
}
