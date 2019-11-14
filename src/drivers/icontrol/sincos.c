#include <math.h>


short sintab[360];
short costab[360];


void init_sincos ()
{
	short i;


	for (i = 0; i < 360; i++) {
		sintab[i] = sin ((i / 360.0) * M_PI * 2) * 10000;
		costab[i] = cos ((i / 360.0) * M_PI * 2) * 10000;
		outchar ('.');
	}

//for (i=0;i<360;i++)
	// printf ("i=%d, \tsin=%d \tcos=%d\n",i,sintab[i],costab[i]);

}


/* mysin and mycos are normalized to +-10000 */

int mysin (int x)
{
	short y1;
	short y2;
	short where;

	x = (x % 36000);
	if (x < 0)
		x += 36000;

	where = x / 100;

	y1 = sintab[where];
	y2 = sintab[where + 1];

	return (y1 + (((x % 100) * (y2 - y1)) / 100));
}

int mycos (int x)
{

	short y1;
	short y2;
	short where;


	x = (x % 36000);
	if (x < 0)
		x += 36000;
	y1 = costab[where];
	y2 = costab[where + 1];

	return (y1 + (((x % 100) * (y2 - y1)) / 100));

}
