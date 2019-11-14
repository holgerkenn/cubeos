// 
// Radio State machine for CubeOS
//

#include <rsm.h>
#include <rdio.h>
#include <cubeos.h>
#include <conio.h>
#include <stdio.h>

//
// A frame for the RSM looks like this:
//
// 00 ff 00 ff ... 00 ff SOH E1 E2 E1 E2 E1 E2 ... E1 E2 C1 C2
//
// There are exactly RMS_FRAMELEN E1/E2 pairs, each one encoding one byte
// The C1/C2 bytes represent the encoded 8-bit checksum of the frame
//
// At the moment, no escaping of the SOH byte is done, so the protocol
// may get stuck.
//

// States of the RSM

#define RSM_OFF 0
#define RSM_IDLE 1
#define RSM_INFRAME1 2
#define RSM_INFRAME2 3
#define RSM_CHECKSUM1 4
#define RSM_CHECKSUM2 5

// Start of Header
#define RSM_SOH 0xA5

#define RSM_PREHEADER 6		//Number of 0x00 0xff in preheader

//#define RSM_DEBUG

// variables

// buffers
char rsm_buffer0[RSM_FRAMELEN];
char rsm_buffer1[RSM_FRAMELEN];

// internal
unsigned char rsm_state = 0;
unsigned char rsm_enabled = 0;
short rsm_count;
char rsm_new;
char *rsm_writebuf;
char *rsm_getbuf;
unsigned char rsm_checksum;
unsigned char rsm_charcount;

// statistics
int rsm_good;
int rsm_bad;
int rsm_ugly;
int rsm_bytes;
int rsm_uglycount;

// function prototypes: user interface

int init_rsm_rx ();
int init_rsm_tx ();
int rsm_get_frame (unsigned char *buffer);
int rsm_send_frame (unsigned char *buffer);


// function prototypes: for internal use only

void doutchar (unsigned char c);
void douthex (unsigned char c);
void process_rsm (unsigned char c);
void rdioput_block (char byte);
unsigned char *rsm_rxframe (unsigned char *frame);


// function implementations

int init_rsm_rx ()
{
	rsm_writebuf = rsm_buffer0;
	rsm_getbuf = rsm_buffer1;
	rsm_state = RSM_IDLE;
	rsm_good = 0;
	rsm_bad = 0;
	rsm_ugly = 0;
	rsm_bytes = 0;
	rsm_uglycount = 0;
	rsm_new = 0;
	rsm_charcount = 0;
	rsm_enabled = 1;	/* redirect to process_rsm() */
	clearRTXSEL ();		/* disable transmitter */
	setRRXSEL ();		/* enable receiver */
	rdio_enable_rx ();	/* Enable reception IRQ */
	return 0;
}

int deinit_rsm_rx ()
{
	clearRRXSEL ();		/* disable receiver */
	rsm_enabled = 0;	/* redirect to process_rsm() */
	rsm_state = RSM_IDLE;	/* back to idle state */
	rdio_disable_rx ();	/* disable reception IRQ */
	return 0;
}

int init_rsm_tx ()
{
	clearRRXSEL ();		/* disable receiver */
	setRTXSEL ();		/* enable transmitter */
	rdio_disable_rx ();	/* disable RX ISR */
	rsm_enabled = 0;	/* and redirect to rdio queue */
	return 0;
}

int deinit_rsm_tx ()
{
	while (rdioout.cnt > 0) {
//              duart_int(); /* wait for empty queue*/
	}
	clearRTXSEL ();		/* disable transmitter */
	return 0;
}

#ifdef RSM_DEBUG

void doutchar (unsigned char c)
{
	ioutchar (c);
}

void douthex (unsigned char c)
{
	char l;
	char h;

	h = (c & 0xf0) >> 4;
	if (h < 10) {
		doutchar ('0' + h);
	} else {
		doutchar ('A' + h - 10);
	}
	l = (c & 0x0f);
	if (l < 10) {
		doutchar ('0' + l);
	} else {
		doutchar ('A' + l - 10);
	}
}

#else
void doutchar (unsigned char c)
{
}
void douthex (unsigned char c)
{
}

#endif

void process_rsm (unsigned char c)
{				/* this is called from the duart irq */
	static unsigned char save;
	unsigned char cx;

	rsm_bytes++;		// byte statistics

	switch (rsm_state) {
	case RSM_IDLE:
		if (c == RSM_SOH) {
			rsm_state = RSM_INFRAME1;
			rsm_count = 0;
			rsm_checksum = 0;
			doutchar ('X');
		}
		break;

	case RSM_INFRAME1:
		if ((c & 0xAA) != ((~((c << 1) | 0x55)) & 0xff)) {
			rsm_state = RSM_IDLE;
			rsm_uglycount = rsm_count;
			rsm_ugly++;
			doutchar ('u');
			break;
		}
		save = c;
		rsm_state = RSM_INFRAME2;
		break;

	case RSM_INFRAME2:
		if ((c & 0xAA) != ((~((c << 1) | 0x55)) & 0xff)) {
			rsm_state = RSM_IDLE;
			rsm_uglycount = rsm_count;
			rsm_ugly++;
			doutchar ('U');
			break;
		}
		cx = ((save & 0xAA) | (c & 0x55));
		rsm_writebuf[rsm_count++] = cx;
		rsm_checksum += cx;

		if (rsm_count >= RSM_FRAMELEN) {
			rsm_state = RSM_CHECKSUM1;
			break;
		}
		rsm_state = RSM_INFRAME1;
		break;

	case RSM_CHECKSUM1:
		save = c;
		rsm_state = RSM_CHECKSUM2;
		break;

	case RSM_CHECKSUM2:
		if (
		      ((save & 0xAA) != ((~((save << 1) | 0x55)) & 0xff)) ||
			   ((c & 0xAA) != ((~((c << 1) | 0x55)) & 0xff))
			) {
			rsm_state = RSM_IDLE;
			rsm_uglycount = rsm_count;
			rsm_ugly++;
			doutchar ('U');
			doutchar ('c');
			break;
		}
		cx = ((save & 0xAA) | (c & 0x55));
		if (cx == rsm_checksum) {
			doutchar ('G');
			douthex (cx);
			doutchar ('\n');
			rsm_good++;

//                      bufsave = rsm_getbuf;
			//                      rsm_getbuf = rsm_writebuf;
			//                      rsm_writebuf = bufsave;
			//                      rsm_new = 1;
			rsm_writebuf = rsm_rxframe (rsm_writebuf);
			if (rsm_writebuf == NULL) {	/* we did not get a new buffer, so we 
							   disable the reception */
				rdio_disable_rx ();
				doutchar ('N');
			}
		} else {
			rsm_bad++;
			doutchar ('B');
			douthex (cx);
			doutchar ('e');
			douthex (rsm_checksum);
			doutchar ('\n');
		}
		rsm_state = RSM_IDLE;
	}
}


// this implements a simple double buffer scheme. One gets written, the other 
// can be read

unsigned char *rsm_rxframe (unsigned char *rxframebuf)
{
	unsigned char *bufsave;

	bufsave = rsm_getbuf;
	rsm_getbuf = rxframebuf;	// this is the pointer to the frame just received

	rsm_new = 1;		// new frame available

	return (bufsave);	// we return the previous getbuf

}


int rsm_get_frame (unsigned char *buffer)
{
	int i;

	if (!rsm_new)
		return (-1);
	disable ();
	for (i = 0; i < RSM_FRAMELEN; i++)
		buffer[i] = rsm_getbuf[i];
	rsm_new = 0;
	enable ();

	return (0);

}


unsigned char *encode (unsigned char c)
{
	static unsigned char buf[2];

	unsigned char t, hi, lo;

	t = (c & 0xaa);
	hi = t | ((~(t >> 1)) & 0x55);
	t = (c & 0x55);
	lo = t | ((~(t << 1)) & 0xaa);
	*buf = hi;
	*(buf + 1) = lo;
	return buf;

}

void rdioput_block (char c)
{
	int i;
	i = 0;
	while (rdioput (c)) {
		i++;
		if (i > 1000) {
			//doutchar('B');
		}
	}

}

int rsm_send_frame (unsigned char *buffer)
{

	unsigned char *buf;
	int i;
	unsigned char checksum;


	for (i = 1; i < RSM_PREHEADER; i++);
	{
		rdioput_block (0x00);
		rdioput_block (0xff);
	}

	checksum = 0;

	rdioput_block (RSM_SOH);

	for (i = 0; i < RSM_FRAMELEN; i++) {
		buf = encode (buffer[i]);
		rdioput_block (buf[0]);
		rdioput_block (buf[1]);
		checksum += buffer[i];
	}

	buf = encode (checksum);

	rdioput_block (buf[0]);
	rdioput_block (buf[1]);
	return (0);

}
