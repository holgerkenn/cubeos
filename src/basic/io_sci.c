#include <cubeos.h>
#include <sys_var.h>
#include <iobuf.h>
#include <kerror.h>
#include <ttyio.h>
#include <mc68332.h>
#include <ivtab.h>
#include <qsm.h>

struct iobuf sciin, sciout;

void sciWrapper ();

void sci_int (void)
{
	/* SCSR:
	   Bit8: TDRE (transmit data reg empty)
	   Bit7: TC   (transmit complete)
	   Bit6: RDRF (receive data reg full)
	   Bit5: RAF  (receiver busy)
	   Bit4: IDLE (idle line detected)
	   Bit3: OR   (overrun error)
	   Bit2: NF   (noise error)
	   Bit1: FE   (framing error)
	   Bit0: PF   (parity error)
	 */
	while (readshort (SCI_SCSR) & 0x40) {
		if (sciin.cnt < BUFLEN) {
			sciin.data[sciin.head] = (char) (readshort (SCI_SCDR) & 0xff);
			sciin.head = (sciin.head + 1) % BUFLEN;
			sciin.cnt++;
		} else {
//      sys_error |= SYS_ERR_TTY0BUF_OVF;
			sciin.data[sciin.head] = (char) (readshort (SCI_SCDR) & 0xff);
		}
	}
	while ((sciout.cnt > 0) && (readshort (SCI_SCSR) & 0x100)) {
		writeshort (SCI_SCDR, ((unsigned short) sciout.data[sciout.tail]) & 0xff);
		sciout.tail = (sciout.tail + 1) % BUFLEN;
		sciout.cnt--;
	}
	/* hold possibly pending transmitter interrupt */
	if (sciout.cnt == 0)
		writeshort (SCI_SCCR1, readshort (SCI_SCCR1) & 0xff7f);

}

void soutchar (char byte);

char sinchar (void)
{
	char c;

	/* Block until char is there */
	while (sciin.cnt == 0);
	disable ();
	c = sciin.data[sciin.tail];
	sciin.tail = (sciin.tail + 1) % BUFLEN;
	sciin.cnt--;
	if (sciin.cnt > BUFLEN) {
		complain (ERR_PANIC, "More than BUFLEN bytes in the buffer");
	}
	enable ();
	if (console_echo)
		soutchar (c);
	return (c);
}

void soutchar (char byte)
{
	disable ();

	if (Blocking_Serial_Out) {
		writeshort (SCI_SCCR1, readshort (SCI_SCCR1) & 0xff5f);
		while (sciout.cnt > 0) {
			while (!(readshort (SCI_SCSR) & 0x100));
			writeshort (SCI_SCDR, ((unsigned short) sciout.data[sciout.tail]) & 0xff);
			sciout.tail = (sciout.tail + 1) % BUFLEN;
			sciout.cnt--;
		}
		while (!(readshort (SCI_SCSR) & 0x100));
		writeshort (SCI_SCDR, ((unsigned short) byte) & 0xff);
	} else {

		if (sciout.cnt == BUFLEN) {
			enable ();
			return;
		}		/* Ignore overflow */
		sciout.data[sciout.head] = byte;
		sciout.head = (sciout.head + 1) % BUFLEN;
		sciout.cnt++;

		/* enable pending transmitter interrupt */
		if (sciout.cnt == 1)
			writeshort (SCI_SCCR1, readshort (SCI_SCCR1) | 0x80);

	}
	enable ();

}


void init_sci (void)
{

	iobuf_init (&sciin, BUFLEN);
	iobuf_init (&sciout, BUFLEN);

	init_qsm ();

	writeshort (SCI_SCCR0, 9);	/* 58200 baud @ 16.77MHz */
	/* Bit12..0: SCBR  (baud rate divider 1 = 1/32(CK)) */
	writeshort (SCI_SCCR1, 0x2c);
	/* 
	   Bit14: LOOPS (feedback loop mode)
	   Bit13: WOMS  (TXD in wired-or mode)
	   Bit12: ILT   (idle line detect type [long,short])
	   Bit11: PT    (parity type [odd,even])
	   Bit10: PE    (parity enable)
	   Bit9 : M     (mode select: [9,8] data bits)
	   Bit8 : WAKE  (wake up by address mark)
	   Bit7 : TIE   (transmit interupt enable)
	   Bit6 : TCIE  (transmit complete interupt enable)
	   *Bit5 : RIE   (receiver interupt enable)
	   Bit4 : ILIE  (idle line interupt enable)
	   *Bit3 : TE    (transmitter enable)
	   *Bit2 : RE    (receiver enable)
	   Bit1 : RWU   (call receiver wakeup function)
	   Bit0 : SBK   (send break)
	 */

}
