#ifndef _IO_DUART_H
#define _IO_DUART_H


extern struct iobuf krnlin, krnlout;

extern struct iobuf rdioin, rdioout;

void setDIMR (unsigned char bit);
void clearDIMR (unsigned char bit);
void setOPCR (unsigned char bit);
void clearOPCR (unsigned char bit);
void duart_int (void);
void duart_bugfix(void);
char kinchar (void);
char rdioget (void);
void koutchar (char byte);
void rdioput (char byte);
void rdio_enable_rx ();
void rdio_disable_rx ();
void init_duart (void);
#endif

