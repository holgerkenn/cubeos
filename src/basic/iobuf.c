#include <iobuf.h>


void iobuf_init (struct iobuf *x)
{
	x->head = 0;
	x->tail = 0;
	x->cnt = 0;
}

int put (struct iobuf *x, char c)
{
	if (x->cnt < BUFLEN) {
		x->data[x->head] = c;
		x->head = (x->head + 1) % BUFLEN;
		x->cnt++;
		return (0);
	} else {
		return (-1);
	}
}

char get (struct iobuf *x)
{
	char c;

	while (x->cnt <= 0);	/* Busy waiting... Not nice... */
	c = x->data[x->tail];
	x->tail = (x->tail + 1) % BUFLEN;
	x->cnt--;
	return (c);

}
