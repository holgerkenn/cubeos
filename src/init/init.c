#include <stdio.h>
#include <global_def.h>
#include <tpu.h>
#include <tpud.h>
#include <sai_cube.h>
#include <rdio.h>
#include <fuelgauge.h>
#include <ptimer.h>

#include "readcmd.h"
#include "testmak.h"

/* tests */
#define MAXTESTS 16

int EmptyTest ();		// tests the test facility, defined below

int TimerTest ();		// tests the timer irq and the daytime clock
#include "tputest.h"		// tests TPU interface and irq callback
#include "i2ctest.h"		// tests i2c read-write
#include "schedtest.h"		// tests task scheduling/sleep/resume
#include "radiotest.h"		// tests radio module
#include "listtest.h"		// tests list manager
#include "hwinfot.h"		// dump hardware infostruct
#include "fbintest.h"		// test the TTL fast bins
//#include "semtest.h"   // tests semaphores between tasks
//#include "msgtest.h"   // tests message ipc
//#include "sigtest.h"   // tests signals
//#include "nettest.h"   // tests network
#include "resettest.h"		// tests soft reset

/* table of tests */
struct test {
	int (*entry) ();
	int interactive;
	char *name;
	int success;
	int failure;
};
struct test tests[MAXTESTS];
int testnr;
void newtest (int (*entry) (), int interactive, char *name);

/* add your tests here */
void init_tests ()
{
	newtest (EmptyTest, 0, "Empty test");
	newtest (ListTest, 0, "List test");
	newtest (TimerTest, 0, "Timer test");
	newtest (TPUTest, 0, "TPU test");
	newtest (HWInfoTest, 0, "Hardware Info test");
	newtest (I2CTest, 0, "I2C Test");
	newtest (FBinTest, 0, "TTL FastBin test");
	newtest (SchedTest, 0, "Scheduler test");
	newtest (RadioTestRx, 0, "Radiolink test (Rx)");
	newtest (RadioTestTx, 0, "Radiolink test (Tx)");
	newtest (ResetTest, 1, "Soft reset test");

}

/* don't change anything down here */

void newtest (int (*entry) (), int interactive, char *name)
{
	if (testnr == 0) {
		int i;
		printf ("Cleaning up tests...");
		for (i = 0; i < MAXTESTS; i++) {
			tests[i].entry = NULL;
			tests[i].name = NULL;
			tests[i].success = 0;
			tests[i].failure = 0;
		}
		printf ("done\n");
	}
	if (testnr < MAXTESTS) {
		tests[testnr].entry = entry;
		tests[testnr].interactive = interactive;
		tests[testnr].name = name;
		testnr++;
	}
}

void dotest (int i)
{
	int ret;
	if (tests[i].entry == NULL) {
		printf ("no such test %d\n", i);
		return;
	}
	printf ("\nRunning test %d:", i);
	if (tests[i].name) {
		printf ("%s\n", tests[i].name);
	} else {
		printf ("\n");
	}
	fflush (stdout);

	ret = tests[i].entry ();

	fflush (stdout);
	printf ("\n");

	if (ret) {
		printf ("test %d FAILED:%d ", i, ret);
		if (tests[i].name) {
			printf ("%s\n", tests[i].name);
		} else {
			printf ("\n");
		}
	} else {
		printf ("test %d SUCCEDED ", i);
		if (tests[i].name) {
			printf ("%s\n", tests[i].name);
		} else {
			printf ("\n");
		}
	}

}


int EmptyTest ()
{
	printf ("Empty Test\n");
	return 0;
}

int TimerTest ()
{
	int i;
	unsigned long oldtime;
	int success;

	success = 0;
	printf ("watching clockticks");
	fflush (stdout);
	oldtime = _gettimeofday ();

	for (i = 0; i < 1000000; i++) {
		if (oldtime != _gettimeofday ()) {
			oldtime = _gettimeofday ();
			printf (".");
			fflush (stdout);
			success++;
		}
	}

	printf ("seen %d ticks", success);
	failif (success == 0, "Clock is not ticking");
	return (0);

}


int main (int argc, char *argv[])
{
	int i, j;
	int quit = 0;
	unsigned long *adr;
	char test[256];
	char c;
	char *command;

	testnr = 0;
	init_LED ();
	LED_OFF ();
	for (i = 0; i < 100000; i++);
	LED_ON ();
	for (i = 0; i < 100000; i++);
	LED_OFF ();


	printf ("\nCubeOS Testsuite V1.0\n");

	printf ("NL/CR test:\nERROR\rOK   \n");

	printf ("please press any key to continue>");
	fflush (stdout);
	c = inchar ();
	printf ("\nGot %c: %d\n", c, c);
	init_LED ();

	init_tests ();


	do {
		printf ("\n\navailable tests:\n");
		for (i = 0; i < testnr; i++) {
			printf ("%2d:%c ", i, (tests[i].interactive ? 'I' : ' '));
			if (tests[i].name) {
				printf ("%s\n", tests[i].name);
			} else {
				printf ("\n");
			}
		}

		command = readcmd ("test(#/q/a)?");

		if (!strcmp (command, "q")) {
			quit = 1;
		} else {
			if (!strcmp (command, "a")) {
				printf ("\n\nRunning all noninteractive tests\n");
				LED_ON ();
				for (i = 0; i < testnr; i++)
					if (!tests[i].interactive)
						dotest (i);
				LED_OFF ();
			} else {

				j = sscanf (command, "%d", &i);

				if ((j == 1) && (i >= 0) && (i < testnr)) {
					LED_ON ();
					dotest (i);
					LED_OFF ();
				} else {
					printf ("no such test \n");
				}
			}

		}

	} while (!quit);


	return (0);
}
