#include <cubeos.h>
#include <stdio.h>
#include <softreset.h>

int ResetTest ()
{
	int i;
	Blocking_Serial_Out = 1;
	printf ("Calling softreset");
	fflush (stdout);
	outchar (' ');

	softreset ();

	while (1) {
		i++;
		if (!(i % 10000))
			outchar ('.');
	}

	return 0;
}
