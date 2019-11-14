#include <stdarg.h>
#include <stdio.h>
#include "sys_var.h"


void exp_handler (unsigned long par1, unsigned long par2,...)
{
	va_list ap;
	char *e_name[36] =
	{"UNKNOWN", "BUS ERROR", "ADDRESS ERROR",
	 "ILLEGAL INSTRUCTION",
	 "DIVIDE BY ZERO", "CHK, CHK2", "TRAPCCV",
	 "PRIVILEGE VIOLATION", "TRACE", "UNUSED VECTOR",
	 "HARDWARE BREAKPOINT", "UNINITIALISED",
	 "SPURIOUS INTERRUPT", "AUTOVECTOR 1", "AUTOVECTOR 2",
	 "AUTOVECTOR 3", "AUTOVECTOR 4", "AUTOVECTOR 5",
	 "AUTOVECTOR 6", "AUTOVECTOR 7",
	 "TRAP0", "TRAP1", "TRAP2", "TRAP3", "TRAP4", "TRAP5", "TRAP6",
	 "TRAP7", "TRAP8", "TRAP9", "TRAP10", "TRAP11", "TRAP12",
	 "TRAP13", "TRAP14", "TRAP15"
	};

	unsigned short stat_reg, fmt_vector;
	unsigned long prg_ctr;
	unsigned int fmt, tmp1, tmp2, ssw;
	unsigned int my_errno;


	asm (" ori.w #0x0700,%sr");	/* disable all further interupts */

	Blocking_Serial_Out = 1;	/* wait until every char is output */
	stat_reg = (par1 >> 16);
	prg_ctr = (par2 >> 16) | ((par1 & 0xffff) << 16);
	fmt_vector = (par2 & 0xffff);

	my_errno = sys_errorcode;
	if (my_errno > 35)
		my_errno = 0;

	printf ("\r\n*******SYSTEM ERROR********\r\n");
	printf ("%s [%d] (vector offset %03x) @ %08lx\n\r", e_name[my_errno], sys_errorcode,
		(fmt_vector & 0xfff), prg_ctr);
	printf ("Statusregister: %04x\r\n", stat_reg);
	printf ("Trace mode: %1d\r\n", ((stat_reg & 0xc000) >> 14));
	if (stat_reg & 0x2000)
		printf ("Supervisor mode\r\n");
	else
		printf ("User mode\r\n");
	printf ("Interrupt priority mask: %1d\r\n", ((stat_reg & 0x700) >> 8));
	printf ("Flags: ");
	if (stat_reg & 0x10)
		printf ("Extend ");
	if (stat_reg & 0x8)
		printf ("Negative ");
	if (stat_reg & 0x4)
		printf ("Zero ");
	if (stat_reg & 0x2)
		printf ("Overflow ");
	if (stat_reg & 0x1)
		printf ("Carry");
	printf ("\r\n");

	fmt = (fmt_vector & 0xf000) >> 12;
	va_start (ap, par2);

	switch (fmt) {
	case (0):
		break;
	case (2):
		printf ("Faulted instr PC: %08lx\r\n", va_arg (ap, unsigned long));

		break;
	case (12):
		printf ("Faulted addr. PC: %08lx\r\n", va_arg (ap, unsigned long));
		tmp1 = va_arg (ap, unsigned long);
		tmp2 = va_arg (ap, unsigned long);
		printf ("Microcode rev. no.: %02x\n\r", va_arg (ap, unsigned char));
		printf ("Transfer count: %02x\n\r", va_arg (ap, unsigned char));
		ssw = va_arg (ap, unsigned int);

		printf ("SSW: ");
		if (ssw & 0x8000)
			printf ("TP ");
		if (ssw & 0x4000)
			printf ("MV ");
		if (ssw & 0x1000)
			printf ("TR ");
		if (ssw & 0x800)
			printf ("B1 ");
		if (ssw & 0x400)
			printf ("B0 ");
		if (ssw & 0x200)
			printf ("RR ");
		if (ssw & 0x100)
			printf ("RM ");
		if (ssw & 0x80)
			printf ("IN ");
		if (ssw & 0x40)
			printf ("RW ");
		if (ssw & 0x20)
			printf ("LG ");
		printf ("SIZ=%1d ", ((ssw & 18) >> 3));
		printf ("FCT=%1d ", (ssw & 7));
		printf ("\r\n");
		if (ssw & 0x8000) {
			printf ("Pre-exception SR: %04x\n\r", (tmp1 >> 16));
			printf ("Faulted exception format/vector: %04x\n\r", (tmp1 & 0xffff));
			printf ("Faulted exception PC: %08x\n\r", tmp2);
		} else {
			printf ("Data Buffer (DBUF): %08x\n\r", tmp1);
			printf ("Current instr. PC: %08x\n\r", tmp2);
		}
		break;
	default:
		printf ("Something strange happened: fmt=%2d\n\r", fmt);
	}

	printf ("*******SYSTEM HALTED*******\r\n");
	while (1);
}
