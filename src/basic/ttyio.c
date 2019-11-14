#include <mc68681.h>
#include <cubeos.h>
#include <sys_var.h>
#include <iobuf.h>
#include <kerror.h>
#include <io_sci.h>
#include <io_duart.h>


void outchar (char byte);

char console_echo;

char contty;

void setcontty (char tty)
{
	contty = tty;
}

void conecho_on ()
{
	console_echo = 1;
}

void conecho_off ()
{
	console_echo = 0;
}

char inchar (void)
{
	if (contty == 0)
		return kinchar ();
	return sinchar ();
}



void outchar (char byte)
{
	if (contty == 0) {
		koutchar (byte);
		return;
	}
	soutchar (byte);
}


void init_tty (void)
{
//#ifdef CUBE3
	//      setcontty (1);          /* DUART */
	//#else
	setcontty (0);		/* DUART */
//#endif

	conecho_on ();
	init_duart ();
	init_sci ();

}
