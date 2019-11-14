#include <stdio.h>

int main (int argc, char *argv[])
{
	int i, j;
	unsigned long *adr;

	printf ("\r\nCubeOS Version 1.0\r\n");
	adr = (unsigned long *) (*(unsigned long *) 0x51c);
	printf ("stack at %lx\r\n", (*(unsigned long *) 0x51c));
/*  for(adr=0xf207fc;adr>0xf207a0;adr--) {
   printf("  %lx: %lx\r\n",adr,*adr);
   }
 */
	adr = (unsigned long *) (*(unsigned long *) 0x520);
	printf ("frame %lx [%lx]\r\n", (*(unsigned long *) 0x520), *adr);
	printf ("vbr   %lx\r\n", (*(unsigned long *) 0x518));

	for (i = 1; i < 100000; i++) {
		j = i;
	}
	return (0);
}
