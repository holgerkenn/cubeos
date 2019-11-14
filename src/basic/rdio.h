/* This is the low-level interface to the radio hardware. An application
program is not supposed to see this. */

#ifndef _RDIO_H
#define _RDIO_H

#include <cubeos.h>
#include <mc68681.h>


extern struct iobuf rdioin, rdioout;

char rdioget(void);
int rdioput(char byte);
void rdio_enable_rx();
void rdio_disable_rx();

static inline void setRTXSEL() 
{
	writebyte(DUART_OPRSET,0x2); /*TXSEL = OP1 */
}

static inline void setRRXSEL()
{
	writebyte(DUART_OPRSET,0x1); /*RXSEL = OP0 */
}

static inline void clearRTXSEL()
{
	writebyte(DUART_OPRRST,0x2);
}

static inline void clearRRXSEL()
{ 
	writebyte(DUART_OPRRST,0x1); 
}

static inline char getRCD()
{
return  readshortpos(DUART_IP,0x1,0x0); /* IP0 = CD */
}

#endif

