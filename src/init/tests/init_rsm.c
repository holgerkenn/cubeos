#include <stdio.h>
#include <ptimer.h>
#include <rsm.h>

struct object {
	short a:16;
	short x:16;
	short y:16;
} __attribute__ ((packed));

struct world {
	struct object p[5];
	struct object o[5];
	struct object b;
	short e[3];
} __attribute__ ((packed));


int main (int argc, char *argv[])
{
	unsigned long saveclock;
	int i;
	union {
		char buffer[sizeof (struct world)];
		struct world w;
	} u;

	printf ("\r\nCubeOS V0.4.6 RSM-Test\r\n");
	printf ("ok\n\r");
	init_rsm ();
	while (1) {
//      saveclock = _gettimeofday () + 1;
		//      printf ("Clock: %lu\n\r", saveclock);
		//      printf ("rsm_good = %d\n rsm_bad = %d \n rsm_ugly = %d at %d\n",
		//              rsm_good, rsm_bad, rsm_ugly, rsm_uglycount);
		if (!rsm_get_frame (u.buffer)) {
//          for (i = 0; i < 5; i++)
			//              printf ("p%d x%hd y%hd a%hd\n", i + 1, u.w.p[i].a, u.w.p[i].x, u.w.p[i].y);
			//          for (i = 0; i < 5; i++)
			//              printf ("o%d a %hd x%hd y%hd\n", i + 1, u.w.o[i].a,u.w.o[i].x, u.w.o[i].y);
			printf ("b x%hd y%hd", u.w.b.x, u.w.b.y);
			printf ("e 1:%hd 2:%hd 3:%hd\n", u.w.e[0], u.w.e[1], u.w.e[2]);
		}
//      printf (".\n");
		//      while (saveclock >= _gettimeofday ()) ;
	}

	return (0);
}
