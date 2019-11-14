#include <stdio.h>
#include <ptimer.h>
#include <conio.h>
#include <schedule.h>
#include <sys/time.h>

void thread_1 ();
void thread_2 ();


struct timeval tvstruct;

int main (int argc, char *argv[])
{
	printf ("\r\nCubeOS V1.0\r\n");
	printf ("ok\n\r");
	create (thread_1);
	create (thread_2);
	printf (" All threads are ready\n\r");



	printf ("That's it !\n\r");
	while (1);

	return (0);
}


void thread_1 (void)
{
	while (1) {
		printf ("thread 1 %lu\n\r", _gettimeofday ());

	}

}

void thread_2 (void)
{
	while (1) {
		printf ("thread 2 \n\r");
	}

}
