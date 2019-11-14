#include <cubeos.h>

void softreset ()
{

	writeshort (SIM_MCR, readshort (SIM_MCR) | 0x8000);

//asm("movl #0xf00004,%%a0": : :"a0");
	//asm("movl (%%a0),%%a0": :);
	//asm("movl #0xf00000,%%a1": : :"a1");
	//asm("adda  %%a0,%%a1": : :"a0");
	//asm("jmp (%%a1)": :);

}
