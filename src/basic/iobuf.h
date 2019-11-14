#ifndef _IOBUF_H
#define _IOBUF_H

#define BUFLEN 1024


struct iobuf {
  unsigned short head;
  unsigned short tail;
  unsigned short cnt;
  unsigned short buflen;
  char data[BUFLEN];
};

static inline void iobuf_init(struct iobuf *x,short bl)
{

if ((bl<=0)||(bl>BUFLEN)) 
	{
		x->buflen=BUFLEN;
	}
else {
	x->buflen=bl;
}

x->head=0;
x->tail=0;
x->cnt=0;

}

static inline int put(struct iobuf *x,char c) 
{
if (x->cnt < x->buflen) {
	x->data[x->head]=c;
	x->head=(x->head+1) % x->buflen;
	x->cnt++;
	return(0);
} else {
	return(-1);
	}
}

static inline char get(struct iobuf *x)
{
char c;
while (x->cnt<=0); /* Busy waiting... Not nice...*/
c=x->data[x->tail];
x->tail=(x->tail +1) % x->buflen;
x->cnt--;
return (c);

}

#endif
