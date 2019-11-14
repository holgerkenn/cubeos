#include <cubeos.h>
#include <kerror.h>
#include <fastbin.h>
#include <mc68681.h>
#include <mc68332.h>
#include <tpud.h>
#include <io_duart.h>		/* for OPCR */

#define MAXFB 16

struct fb_info {
	char type;
	char id;
};

struct fb_info fbs[MAXFB];

unsigned short ttlfastbin_state;

int init_fastbin ()
{
	int i;

	for (i = 1; i < MAXFB; i++) {
		fbs[i].type = FBTYPE_EMPTY;
		fbs[i].id = 0;
	}
	return 0;
}


int make_fastbin (int nr, char type, char id)
{
//      printf ("make_fastbin %d: ", nr);

	if (nr >= MAXFB)
		return (-1);
	fbs[nr].type = type;
	fbs[nr].id = id;

	switch (type) {
	case FBTYPE_DUART:
//              printf ("type DUART, id %d ", id);
		/* writebyte(DUART_OPCR,0); */
		switch (id) {
		case 2:
			clearOPCR (0);
			clearOPCR (1);
			break;
		case 3:
			clearOPCR (2);
			clearOPCR (3);
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			clearOPCR (id);
			break;
		default:
			fbs[nr].type = FBTYPE_EMPTY;
			return (-1);
		}
		break;
	case FBTYPE_TPU:
//              printf ("type TPU, id %d ", id);
		TPU_makedio (id);
		TPU_setdio (id, 0);
		break;
	case FBTYPE_TTL:
//              printf ("type TTL, id %d ", id);
		if (FASTBIN_BASE == 0) {
			fbs[nr].type = FBTYPE_EMPTY;
			complain (ERR_WARN, "Trying to use fastbin register without any register around");
		} else {
			/* clear all fastbins */
			writeshort (FASTBIN_BASE, 0);
			/* and save that fact */
			ttlfastbin_state = 0;
		}
		break;
	case FBTYPE_CPU:
//              printf ("type CPU, id %d ", id);
		switch (id) {
		case FBCPU_PCS1:
			writebyte (SPI_QPDR, readbyte (SPI_QPDR) & 0xEF);
			writebyte (SPI_QPAR, readbyte (SPI_QPAR) & 0xEF);
			writebyte (SPI_QDDR, readbyte (SPI_QDDR) | 0x10);
			break;
		default:
			fbs[nr].type = FBTYPE_EMPTY;
			return (-1);
		}
		break;
	default:
		return (-1);
	}
//      printf (" OK\n");
	return (0);


}

int set_fastbin (int i, char v)
{
	char type, id;

	type = fbs[i].type;
	id = fbs[i].id;
	switch (type) {
	case FBTYPE_DUART:
		switch (id) {
		case 2:
			if (v) {
				setDUARTOPR (2);
			} else {
				clearDUARTOPR (2);
			}
			break;
		case 3:
			if (v) {
				setDUARTOPR (3);
			} else {
				clearDUARTOPR (3);
			}
			break;
		case 4:
			if (v) {
				setDUARTOPR (4);
			} else {
				clearDUARTOPR (4);
			}
			break;
		case 5:
			if (v) {
				setDUARTOPR (5);
			} else {
				clearDUARTOPR (5);
			}
			break;
		case 6:
			if (v) {
				setDUARTOPR (6);
			} else {
				clearDUARTOPR (6);
			}
			break;
		case 7:
			if (v) {
				setDUARTOPR (7);
			} else {
				clearDUARTOPR (7);
			}
			break;
		default:
			return -1;

		}
		break;
	case FBTYPE_TPU:
		TPU_setdio (id, v);
		break;
	case FBTYPE_TTL:
		if (v) {
			ttlfastbin_state |= (1 >> id);
		} else {
			ttlfastbin_state &= (~(1 >> id));
		}
		writeshort (FASTBIN_BASE, ttlfastbin_state);
		break;
	case FBTYPE_CPU:
		switch (id) {
		case FBCPU_PCS1:
			if (v) {
				writebyte (SPI_QPDR, readbyte (SPI_QPDR) | 0x10);
			} else {
				writebyte (SPI_QPDR, readbyte (SPI_QPDR) & 0xEF);
			}
			break;
		default:
			return (-1);
		}
		break;
	default:
		return (-1);
	}
	return 0;
}
