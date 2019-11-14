#include <cubeos.h>
#include <ivtab.h>

void (*(*IVTab)[512]) (void);

void *IVTab_oldvector[512];


void IVTab_init ()
{

	int i;

	asm ("movec %vbr,%d0");
      asm ("movel %%d0,%0":"=m" (IVTab));

	for (i = 0; i < 512; i++)
		IVTab_oldvector[i] = (void *) 0;

}

int IVTab_setvector (unsigned short vector, void (*isr) (void))
{

	if (IVTab_oldvector[vector]) {
		return (-1);
	} else {
		IVTab_oldvector[vector] = (*IVTab)[vector];
		(*IVTab)[vector] = isr;
		return (0);
	}

}

int IVTab_clearvector (unsigned short vector, void (*isr) (void))
{
	if ((*IVTab)[vector] != isr) {
		return (-1);
	} else {
		(*IVTab)[vector] = IVTab_oldvector[vector];
		IVTab_oldvector[vector] = 0;
		return (0);
	}

}
