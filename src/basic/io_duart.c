#include <mc68681.h>
#include <cubeos.h>
#include <sys_var.h>
#include <iobuf.h>
#include <kerror.h>
#include <ttyio.h>
#include <rsm.h>
#include <rdio.h>
#include <ivtab.h>
#include <softreset.h>

/* 
 * The DUART is mapped into the IDP address space in an unusual 
 * manner.  The mc68681 is an 8 bit device located on the least
 * significant byte (byte0) of the data bus.  Bytes 3, 2, and 
 * one have nothing in them and writes to these locations are
 * not valid.
 */
unsigned char duart_opcrsave;


unsigned char duart_imrsave;

unsigned char duart_isr;

struct iobuf krnlin, krnlout;
struct iobuf rdioin, rdioout;

void ioutchar (char byte);

void setDIMR (unsigned char bit)
{
	duart_imrsave |= bit;
	writebyte (DUART_IMR, duart_imrsave);
}


void clearDIMR (unsigned char bit)
{
	duart_imrsave &= (0xff - bit);
	writebyte (DUART_IMR, duart_imrsave);
}

void setOPCR (unsigned char bit)
{
	duart_opcrsave |= bit;
	writebyte (DUART_OPCR, duart_opcrsave);
}


void clearOPCR (unsigned char bit)
{
	duart_opcrsave &= (0xff - bit);
	writebyte (DUART_OPCR, duart_opcrsave);
}



void duart_int (void)
{
	char c;

//      duart_isr = readbyte(DUART_ISR);

//      if (duart_isr & 0x20) {
	//      if (readbyte(DUART_ISR) & 0x20) {

	while (readbyte (DUART_SRB) & (char) 0x1) {
		if (krnlin.cnt < krnlin.buflen) {
			c = readbyte (DUART_RBB);
			if (c == RESET_CHAR)
				softreset ();
			krnlin.data[krnlin.head] = c;
			krnlin.head = (krnlin.head + 1) % krnlin.buflen;
			krnlin.cnt++;
		} else {
			sys_error |= SYS_ERR_KRNLINBUF_OVF;
			c = readbyte (DUART_RBB);
			if (c == RESET_CHAR)
				softreset ();
			krnlin.data[krnlin.head] = c;
		}
	}
//      return;
	//      }

//      if (duart_isr & 0x10) {
	//      if (readbyte(DUART_ISR) & 0x10) {

	while ((krnlout.cnt > 0) && (readbyte (DUART_SRB) & 4)) {
		writebyte (DUART_TBB, krnlout.data[krnlout.tail]);
		krnlout.tail = (krnlout.tail + 1) % krnlout.buflen;
		krnlout.cnt--;
	}
	/* hold possibly pending transmitter interrupt */
	if (krnlout.cnt == 0)
		clearDIMR (0x10);	/* writebyte(DUART_IMR, 0x20); */
//      return;
	//      }
	/* radio */
//      if (duart_isr & 0x02) {
	//      if (readbyte(DUART_ISR) & 0x02) {
	while (readbyte (DUART_SRA) & (char) 0x1) {
		if (!rsm_enabled) {
			if (rdioin.cnt < rdioin.buflen) {
				rdioin.data[rdioin.head] = readbyte (DUART_RBA);
				rdioin.head = (rdioin.head + 1) % rdioin.buflen;
				rdioin.cnt++;
			} else {
				sys_error |= SYS_ERR_RDIOINBUF_OVF;
				rdioin.data[rdioin.head] = readbyte (DUART_RBA);
			}
		} else {
			process_rsm (readbyte (DUART_RBA));
		}
	}

//      return;
	//      }

//      if (duart_isr & 0x01) {
	//      if (readbyte(DUART_ISR) & 0x01) {
	while ((rdioout.cnt > 0) && (readbyte (DUART_SRA) & 4)) {
		writebyte (DUART_TBA, rdioout.data[rdioout.tail]);
		rdioout.tail = (rdioout.tail + 1) % rdioout.buflen;
		rdioout.cnt--;
	}
	/* hold possibly pending transmitter interrupt */
	if (rdioout.cnt == 0) {
		clearDIMR (0x01);
	} else {
		setDIMR (0x01);
	}
//      return;
	//      }

//      duart_isr = readbyte(DUART_ISR);
}


void duart_bugfix (void)
{
/* try to determine if we are in a bugfix condition */
	if (
		   ((rdioout.cnt > 0) && (readbyte (DUART_SRA) & 4)) ||
		   ((readbyte (DUART_SRA) & (char) 0x1))
		)
		duart_int ();	/* then call the duart interrupt */
}

void koutchar (char byte);

char kinchar (void)
{
	char c;

	/* Block until char is there */
	while (krnlin.cnt == 0);
	disable ();
	c = krnlin.data[krnlin.tail];
	krnlin.tail = (krnlin.tail + 1) % krnlin.buflen;
	krnlin.cnt--;
	if (krnlin.cnt > krnlin.buflen) {
		complain (ERR_PANIC, "More than krnlin.buflen bytes in the buffer");
	}
	enable ();
	if (console_echo)
		koutchar (c);
	return (c);
}

char rdioget (void)
{
	char c;

	while (rdioin.cnt == 0);	/* Block until char is there */
	disable ();
	c = rdioin.data[rdioin.tail];
	rdioin.tail = (rdioin.tail + 1) % rdioin.buflen;
	rdioin.cnt--;
	enable ();
	return (c);
}


void koutchar (char byte)
{
	disable ();

	if (Blocking_Serial_Out) {
		writebyte (DUART_IMR, 0x20);
		while (krnlout.cnt > 0) {
			while (!(readbyte (DUART_SRB) & 4));
			writebyte (DUART_TBB, krnlout.data[krnlout.tail]);
			krnlout.tail = (krnlout.tail + 1) % krnlout.buflen;
			krnlout.cnt--;
		}
		while (!(readbyte (DUART_SRB) & 4));
		writebyte (DUART_TBB, byte);
	} else {

		if (krnlout.cnt == krnlout.buflen) {
			setDIMR (0x10);		/* writebyte (DUART_IMR, 0x30); */
			enable ();
			return;
		}		/* Ignore overflow */
		krnlout.data[krnlout.head] = byte;
		krnlout.head = (krnlout.head + 1) % krnlout.buflen;
		krnlout.cnt++;

		/* enable pending transmitter interrupt */
		if (krnlout.cnt == 1)
			setDIMR (0x10);		/* writebyte (DUART_IMR, 0x30); */
	}
	enable ();

}

void ioutchar (char byte)
{

	if (krnlout.cnt == krnlout.buflen) {
		return;
	}			/* Ignore overflow */
	krnlout.data[krnlout.head] = byte;
	krnlout.head = (krnlout.head + 1) % krnlout.buflen;
	krnlout.cnt++;

	/* enable pending transmitter interrupt */
	if (krnlout.cnt >= 1)
		setDIMR (0x10);	/* writebyte (DUART_IMR, 0x30); */

}


int rdioput (char byte)
{
	disable ();

	if (rdioout.cnt == rdioout.buflen) {
		setDIMR (0x1);
		enable ();
		return -1;
	}			/* Ignore overflow */
	rdioout.data[rdioout.head] = byte;
	rdioout.head = (rdioout.head + 1) % rdioout.buflen;
	rdioout.cnt++;

/* enable pending transmitter interrupt */
	if (rdioout.cnt > 0)
		setDIMR (0x1);	/* writebyte (DUART_IMR, 0x30); */

	enable ();
	return (0);
}




void rdio_enable_rx ()
{
	setDIMR (0x02);
}

void rdio_disable_rx ()
{
	clearDIMR (0x02);
}


void init_duart (void)
{
	unsigned char temp;

	iobuf_init (&krnlin, BUFLEN);
	iobuf_init (&krnlout, BUFLEN);
	iobuf_init (&rdioin, BUFLEN);
	iobuf_init (&rdioout, 128);

	conecho_on ();
//      IVTab_setvector(AVEC5,duartWrapper);

	writebyte (DUART_IMR, 0);
	writebyte (DUART_OPCR, 0);
	duart_opcrsave = 0;	/* Maybe we shouldn't do this */
	writebyte (DUART_CRA, 0x2a);
	writebyte (DUART_CRB, 0x2a);
	writebyte (DUART_CRA, 0x3a);
	writebyte (DUART_CRB, 0x3a);
	writebyte (DUART_ACR, 0xE0);	// 19.200
	// writebyte (DUART_ACR, 0x60); // 9.600

	writebyte (DUART_CTUR, 0);
	writebyte (DUART_CTLR, 2);
	temp = readbyte (DUART_STRTCC);
	writebyte (DUART_CSRA, 0xcc);
	writebyte (DUART_CSRB, 0xdd);
	writebyte (DUART_MR1A, 0x13);
	writebyte (DUART_MR1B, 0x13);
	writebyte (DUART_MR2A, 0x7);
	writebyte (DUART_MR2B, 0x7);
	writebyte (DUART_OPRRST, 0xff);
	writebyte (DUART_CRA, 5);
	writebyte (DUART_CRB, 5);
	writebyte (DUART_IMR, 0x20);
	duart_imrsave = 0x20;
/* now, radio reception is disabled by not setting the RxRDYA bit */
/* the tx irq is set via the rdioput(char) routine */

}
