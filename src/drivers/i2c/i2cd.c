#include <cubeos.h>
#include <mc68332.h>
#include <ivtab.h>
#include <i2cd.h>
#include <pcf8584.h>
#include <stdio.h>

struct i2c i2c[2];

static void GenerateStop (struct i2c *p, unsigned char mess)
{
	p->m_state = MASTER_IDLE;
	p->m_buff[p->m_tail]->status = mess;	/* still ptr of finished msg */
	p->m_buff[p->m_tail]->procBytes = p->m_dcnt;
	if (++p->m_tail >= MBUFLEN)
		p->m_tail = 0;
	p->m_cnt--;
	sem_signal (p->m_sem);

	/* check for further entries in output buffer */

	if (p->m_cnt == 0) {
		p->mode = I2C_SLAVE_MODE;
	} else {
		(p->m_state_handler) (p);
	}
}

void i2c_master_handler (struct i2c *p)
{
	struct i2c_device *dev;
	struct i2cmess *msg;
	unsigned long timeoutcnt;
	unsigned char dummy;

	dev = p->ioaddr;
	msg = p->m_buff[p->m_tail];

	switch (p->m_state) {
	case MASTER_IDLE:
		timeoutcnt = I2C_TIMEOUT_VAL;
		while (!(dev->control & BB) && timeoutcnt--);	/* wait until bus
								   free */
		if (timeoutcnt) {
			p->m_dcnt = 0;
			p->m_state = (msg->address & 1) ? MASTER_WAIT_ACK : MASTER_SEND;
			dev->data = msg->address;
			dev->control = PIN | ESO | ENI | STA | ACK;	/* generate
									   start */
		} else {
			GenerateStop (p, I2C_TIME_OUT);
		}
		return;
	case MASTER_SEND:
		if (dev->control & LAB) {	/* arbitration was lost */
			p->mode = I2C_SLAVE_MODE;
			(p->s_state_handler) (p);	/* check if addressed as slave */
			p->mode = I2C_MASTER_MODE;
			GenerateStop (p, I2C_ARBITRATION_LOST);		/* leave the bus */
			return;
		}
		if (dev->control & LRB) {
			dev->control = PIN | ESO | ENI | STO | ACK;
			GenerateStop (p, I2C_NACK_ON_DATA);	/* last byte not
								   ack'ed */
		} else if (p->m_dcnt < msg->nrBytes)
			dev->data = msg->buf[p->m_dcnt++];	/* send next byte */
		else {
			dev->control = PIN | ESO | ENI | STO | ACK;
			GenerateStop (p, I2C_OK);	/* transfer ready */
		}
		break;
	case MASTER_WAIT_ACK:
		if (dev->control & LAB) {	/* arbitration was lost */
			p->mode = I2C_SLAVE_MODE;
			(p->s_state_handler) (p);	/* check if addressed as slave */
			p->mode = I2C_MASTER_MODE;
			GenerateStop (p, I2C_ARBITRATION_LOST);		/* leave the bus */
			return;
		}
		if (dev->control & LRB) {
			dev->control = PIN | ESO | ENI | STO | ACK;
			GenerateStop (p, I2C_NACK_ON_ADDRESS);
		} else {
			if (msg->nrBytes == 1) {
				p->m_state = MASTER_RECV_LAST;
				dev->control = ESO | ENI;
			} else {
				p->m_state = MASTER_RECV;
			}
			dummy = dev->data;	/* start generation of clock pulses for the
						   first byte to read        */
		}
		break;
	case MASTER_RECV:
		if (p->m_dcnt + 2 == msg->nrBytes) {
			dev->control = ESO | ENI;	/* clear ACK */
			p->m_state = MASTER_RECV_LAST;
		}
		msg->buf[p->m_dcnt++] = dev->data;
		break;
	case MASTER_RECV_LAST:
		dev->control = PIN | ESO | ENI | STO | ACK;
		msg->buf[p->m_dcnt++] = dev->data;
		GenerateStop (p, I2C_OK);	/* transfer ready */
		break;
	default:		/* impossible, just to be sure */
		GenerateStop (p, I2C_ERR);
		break;
	}
}

void i2c_slave_handler (struct i2c *p)
{
	register struct i2c_device *dev;

	dev = p->ioaddr;

	switch (p->s_state) {
	case SLAVE_IDLE:
		if (dev->control & AAS) {	/* addressed as slave ? */
			if (dev->data & 1) {	/* slave transmitter */
				p->s_state = SLAVE_SEND;
			} else {	/* slave receiver */
				if (p->s_icnt >= SBUFLEN) {
					p->s_status |= I2C_SLAVE_IBUF_OVERFLOW;
					dev->control = PIN | ESO | ENI;
					sem_signal (p->s_isem);
				} else {
					p->s_state = SLAVE_RECV;
				}
			}
		} else {	/* clear interupt */
			dev->control = PIN | ESO | ENI;
		}
		return;
	case SLAVE_SEND:
		if (dev->control & LRB) {	/* no ack from master */
			p->s_state = SLAVE_IDLE;
			sem_signal (p->s_osem);
		}
		if (p->s_ocnt == 0) {
			dev->data = 0xff;
			p->s_status |= I2C_SLAVE_OBUF_EMPTY;
		} else {
			dev->data = p->s_obuff[p->s_optr++];
			p->s_ocnt--;
		}
		return;
	case SLAVE_RECV:
		if (dev->control & STS) {	/* STOP detected */
			p->s_state = SLAVE_IDLE;
			dev->control = PIN | ESO | ENI | ACK;
			sem_signal (p->s_isem);
		} else if (p->s_icnt == SBUFLEN) {
			dev->control = PIN | ESO | ENI;		/* clear ACK */
			p->s_state = SLAVE_IDLE;
			sem_signal (p->s_isem);
		} else {
			p->s_ibuff[p->s_iptr++] = dev->data;
			p->s_icnt++;
		}
		return;
	default:
		dev->control = PIN | ESO | ENI | ACK;	/* clear interrupt */
		p->s_status |= I2C_SLAVE_ERR;
		return;
	}
}


void i2c_int_handler (unsigned long minor)
{
	struct i2c *p;

	p = &i2c[minor];
	if (p->mode == I2C_MASTER_MODE)
		(p->m_state_handler) (p);	/* Master Mode */
	else
		(p->s_state_handler) (p);	/* Slave Mode  */
}

void i2c_init (int which, unsigned long ioaddr)
{
	register struct i2c *iptr;
	register struct i2c_device *chip;
//    unsigned long ivtab;
	unsigned short v;

	/* set up interrupt vector and interrupt dispatch table */
//    asm ("movec %vbr,%d0");
	//  asm ("movel %%d0,%0":"=m" (ivtab));
	if (which == I2CA) {
//      *(unsigned long *) (ivtab + 27 * 4) = (unsigned long) &i2ca_int;
		IVTab_setvector (I2CA_VECTORNUM, i2ca_int);

#ifdef I2C_INITCS
		writeshort (SIM_CSBAR4, ioaddr >> 8);	/* CS4: (2k) -- I2C-A */
		writeshort (SIM_CSOR4, 0x7ffa);		/* ASYNC,H+L,R/W,DS,DSACK
							   EXT,S+U SP */
		v = readshort (SIM_CSPAR0);
		v |= 0xc00;
		v &= 0xfbff;
		writeshort (SIM_CSPAR0, v);	/* enables CS4 as 8 bit port */
#endif
		iptr = &(i2c[0]);
	} else if (which == I2CB) {
//      *(unsigned long *) (ivtab + 28 * 4) = (unsigned long) &i2cb_int;
		IVTab_setvector (I2CB_VECTORNUM, i2cb_int);

#ifdef I2C_INITCS
		writeshort (SIM_CSBAR3, ioaddr >> 8);	/* CS3: (2k) -- I2C-B */
		writeshort (SIM_CSOR3, 0x7ffa);		/* ASYNC,H+L,R/W,DS,DSACK
							   EXT,S+U SP */
		v = readshort (SIM_CSPAR0);
		v |= 0x300;
		v &= 0xfeff;
		writeshort (SIM_CSPAR0, v);	/* enables CS3 as 8 bit port */
#endif
		iptr = &(i2c[1]);
	} else
		return;

	/* fill i2c control blk       */
	iptr->ioaddr = (struct i2c_device *) ioaddr;	/* chip address */
	iptr->mode = I2C_SLAVE_MODE;
	iptr->m_head = iptr->m_tail = 0;	/* empty input queue */
	/* iptr->m_isem = screate(0); */
	iptr->m_sem = MBUFLEN;
	iptr->m_cnt = 0;
	iptr->m_dcnt = 0;
	iptr->m_state_handler = &i2c_master_handler;
	iptr->m_state = MASTER_IDLE;

	iptr->s_iptr = 0;
	iptr->s_isem = 0;
	iptr->s_icnt = 0;
	iptr->s_optr = 0;
	iptr->s_osem = 0;
	iptr->s_ocnt = 0;
	iptr->s_state_handler = &i2c_slave_handler;
	iptr->s_state = SLAVE_IDLE;
	iptr->s_status = 0;

	/* set up the chip */
	chip = iptr->ioaddr;
	chip->control = 0x80;	/* software reset            */
	if (which == I2CA)
		chip->data = I2CA_SLAVE_ADDR;	/* write own slave address   */
	else
		chip->data = I2CB_SLAVE_ADDR;	/* write own slave address   */
	chip->control = 0xa0;	/* write clock register      */
	chip->data = I2C_SPEED;
	chip->control = PIN | ESO | ENI | ACK;	/* enable i2c bus interface  */
}

/************* Interface functions *******************/

int Start_I2C_Transfer (unsigned char which, struct i2cmess *msg)
{
	struct i2c *hdl;
	int timeout;

	if (which == I2CA)
		hdl = &i2c[0];
	else
		hdl = &i2c[1];

	/* insert message in queue    */
	msg->status = I2C_NOT_PROCESSED;
	sem_wait (hdl->m_sem);
	hdl->m_buff[hdl->m_head++] = msg;
	++hdl->m_cnt;
	if (hdl->m_head >= MBUFLEN)
		hdl->m_head = 0;

	/* Start Transfer if necessary */
	if ((hdl->mode == I2C_SLAVE_MODE) && (hdl->s_state == SLAVE_IDLE)) {
		hdl->mode = I2C_MASTER_MODE;
		disable ();
		(hdl->m_state_handler) (hdl);	/* handler returns here */
		enable ();
	}
	timeout = 0;
	while ((msg->status == I2C_NOT_PROCESSED) && (++timeout < 1000000));
	if (timeout > 0)
		return (-1);

	switch (msg->status) {
	case I2C_OK:
		return (0);
		break;
	case I2C_NACK_ON_DATA:
		return (-1);
		break;
	case I2C_NACK_ON_ADDRESS:
		return (-1);
		break;
	case I2C_ARBITRATION_LOST:
		return (-1);
		break;
	case I2C_TIME_OUT:
		return (-1);
		break;
	case I2C_ERR:
		return (-1);
		break;
	default:
		return (-1);
		break;
	}

}

int Process_I2C_Slave (unsigned char which, struct i2cmess *msg)
{
	struct i2c *hdl;
	int i;

	if (which == I2CA)
		hdl = &i2c[0];
	else
		hdl = &i2c[1];

	/* insert message in queue    */
	msg->status = I2C_NOT_PROCESSED;

	if (msg->nrBytes != SBUFLEN)
		return (-1);

	if (msg->address & 1) {	/* read */
		sem_wait (hdl->s_isem);
		disable ();
		for (i = 0; i < hdl->s_icnt; i++)
			msg->buf[i] = hdl->s_ibuff[i];
		msg->procBytes = hdl->s_icnt;
		hdl->s_icnt = 0;
		hdl->s_iptr = 0;
		if (hdl->s_status == 0)
			msg->status = I2C_OK;
		if (hdl->s_status & I2C_SLAVE_IBUF_OVERFLOW) {
			msg->status = I2C_ERR;
			hdl->s_status &= ~I2C_SLAVE_IBUF_OVERFLOW;
		}
		enable ();
	} else {		/* write */
		disable ();
		for (i = 0; i < SBUFLEN; i++)
			hdl->s_obuff[i] = msg->buf[i];
		hdl->s_ocnt = SBUFLEN;
		hdl->s_optr = 0;
		enable ();
		sem_wait (hdl->s_osem);
		disable ();
		msg->procBytes = hdl->s_ocnt;
		hdl->s_ocnt = 0;
		hdl->s_optr = 0;
		if (hdl->s_status == 0)
			msg->status = I2C_OK;
		if (hdl->s_status & I2C_SLAVE_OBUF_EMPTY) {
			msg->status = I2C_ERR;
			hdl->s_status &= ~I2C_SLAVE_OBUF_EMPTY;
		}
		enable ();
	}
	return (0);
}

int process_i2c (unsigned char whichBus, unsigned char whichQueue, struct i2cmess *msg)
{
	if (whichQueue == I2C_MASTER)
		return Start_I2C_Transfer (whichBus, msg);
	else
		return Process_I2C_Slave (whichBus, msg);
}

int i2c_messagestatus (struct i2cmess *msg)
{
	switch (msg->status) {
	case I2C_OK:
		return (0);
		break;
	case I2C_NACK_ON_DATA:
		printf ("I2C: transfer interupted\n");
		return (-1);
		break;
	case I2C_NACK_ON_ADDRESS:
		printf ("I2C: device not present\n");
		return (-1);
		break;
	case I2C_ARBITRATION_LOST:
		printf ("I2C: arbitration lost\n");
		return (-1);
		break;
	case I2C_TIME_OUT:
		printf ("I2C: time-out\n\r");
		return (-1);
		break;
	case I2C_ERR:
		printf ("I2C: master handler in strange state\n");
		return (-1);
		break;
	default:
		printf ("I2C: unknown status (%d)\n\r", msg->status);
		return (-1);
		break;
	}

}

int i2c_scanbus (unsigned char bus)
{

	struct i2cmess msg;
	unsigned char buf[10];
	int i, j;
	int count = 0;
	int ret;

	for (i = 0; i < 127; i++) {

		msg.address = i << 1;
		msg.nrBytes = 1;
		msg.buf = buf;
		for (j = 0; j < 10; j++)
			buf[j] = 0;

		ret = process_i2c (bus, I2C_MASTER, &msg);

		switch (msg.status) {
		case I2C_OK:
			printf ("found device at adress 0x%x \n", msg.address);
			count++;
			break;
		case I2C_TIME_OUT:
		case I2C_NACK_ON_ADDRESS:
		case I2C_NACK_ON_DATA:
			printf (".");
			fflush (stdout);
			break;

		default:
			i2c_messagestatus (&msg);
		}

	}
	return (count);
}
