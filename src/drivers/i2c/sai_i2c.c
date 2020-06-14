#include <cubeos.h>
#include <global_def.h>
#include <sai.h>
#include <sai_cube.h>
#include <i2cd.h>
#include <stdio.h>		/* for printf */
#include <malloc.h>
#include <kerror.h>



int i2cs_writehandler (struct iop *iop, int value)
{
	struct i2cmess m;
	char mbuf[MAXI2CMESSLENGTH];
	unsigned char bus;

	switch (((struct iop_i2c *) iop)->major) {	/* Select I2c Bus */
	case MAJ_I2CA:
		bus = I2CA;
		break;
	case MAJ_I2CB:
		bus = I2CB;
		break;
	default:
		return SA_FAIL;
	}

	switch (((struct iop_i2c *) iop)->nrwrite) {
	case 1:
		mbuf[0] = (unsigned char) (value & 0xff);
	case 2:
		mbuf[1] = (unsigned char) ((value >> 8) & 0xff);
	case 3:
		mbuf[2] = (unsigned char) ((value >> 16) & 0xff);
	case 4:
		mbuf[3] = (unsigned char) ((value >> 24) & 0xff);
		break;
	default:
		return SA_FAIL;
		break;
	}

	m.address = (((struct iop_i2c *) iop)->minor) + 1;
	/* select write adress */
	m.nrBytes = ((struct iop_i2c *) iop)->nrwrite;
	/* nr of bytes expected from target */
	m.buf = mbuf;
	process_i2c (bus, I2C_LEADER, &m);

	return SA_OK;
}

int i2cs_readhandler (struct iop *iop)
{
	struct i2cmess m;
	char mbuf[MAXI2CMESSLENGTH];
	unsigned char bus;

	switch (((struct iop_i2c *) iop)->major) {	/* Select I2c Bus */
	case MAJ_I2CA:
		bus = I2CA;
		break;
	case MAJ_I2CB:
		bus = I2CB;
		break;
	default:
		return SA_FAIL;
	}

	m.address = (((struct iop_i2c *) iop)->minor);
	/* select read adress */
	m.nrBytes = ((struct iop_i2c *) iop)->nrread;
	/* nr of bytes expected from target */
	m.buf = mbuf;
	process_i2c (bus, I2C_LEADER, &m);
	switch (((struct iop_i2c *) iop)->nrread) {
	case 1:
		return mbuf[0];
		break;
	case 2:
		return (mbuf[0] + (mbuf[1] << 8));
		break;
	case 4:
		return (mbuf[0] + (mbuf[1] << 8) + (mbuf[2] << 16) + (mbuf[3] << 24));
		break;
	default:
		break;
	}
	return SA_FAIL;
}

int i2cs_controlhandler (struct iop *iop, int control, int value)
{
	return SA_FAIL;
}



int i2cc_writehandler (struct iop *iop, int value)
{
	struct i2cmess m;
	char mbuf[MAXI2CMESSLENGTH];
	unsigned char bus;

	printf ("I2CC writehandler\n");
	printf ("iop->major is %d\n", iop->major);
	printf ("iop->minor is %d\n", ((struct iop_i2cc *) iop)->minor);
	printf ("iop->wcommand is %d\n", ((struct iop_i2cc *) iop)->wcommand);
	printf ("iop->nrwrite is %d\n", ((struct iop_i2cc *) iop)->nrwrite);


	switch (((struct iop_i2cc *) iop)->major) {	/* Select I2c Bus */
	case MAJ_I2CA:
		bus = I2CA;
		break;
	case MAJ_I2CB:
		bus = I2CB;
		break;
	default:
		return SA_FAIL;
	}

	mbuf[0] = (((struct iop_i2cc *) iop)->wcommand);

	switch (((struct iop_i2cc *) iop)->nrwrite) {
	case 1:
		mbuf[1] = (unsigned char) ((value >> 24) & 0xff);
	case 2:
		mbuf[2] = (unsigned char) ((value >> 16) & 0xff);
	case 3:
		mbuf[3] = (unsigned char) ((value >> 8) & 0xff);
	case 4:
		mbuf[4] = (unsigned char) ((value) & 0xff);
		break;
	default:
		return SA_FAIL;
		break;
	}

	m.address = (((struct iop_i2cc *) iop)->minor);
	/* select write adress */
	m.nrBytes = ((struct iop_i2cc *) iop)->nrwrite + 1;
	/* nr of bytes expected from target */
	m.buf = mbuf;

	printf ("I2C buffer = %X %X %X.\n", mbuf[0], mbuf[1], mbuf[2]);
	process_i2c (bus, I2C_LEADER, &m);
	printf ("I2C procBytes = %d...", m.procBytes);
	printf ("I2C status = %d.\n", m.status);


	return SA_OK;
}

int i2cc_readhandler (struct iop *iop)
{
	struct i2cmess m;
	char mbuf[MAXI2CMESSLENGTH];
	unsigned char bus;
	int i;

	printf ("I2CC readhandler\n");
	printf ("iop->major is %d\n", iop->major);
	printf ("iop->minor is %d\n", ((struct iop_i2cc *) iop)->minor);
	printf ("iop->rcommand is %d\n", ((struct iop_i2cc *) iop)->rcommand);
	printf ("iop->nrread is %d\n", ((struct iop_i2cc *) iop)->nrread);



	switch (((struct iop_i2cc *) iop)->major) {	/* Select I2c Bus */
	case MAJ_I2CA:
		bus = I2CA;
		break;
	case MAJ_I2CB:
		bus = I2CB;
		break;
	default:
		return SA_FAIL;
	}

	m.address = (((struct iop_i2cc *) iop)->minor) + 1;
	/* select write adress */
	m.nrBytes = 1;
	/* send one byte (command code) */
	m.buf = mbuf;
	mbuf[0] = (unsigned char) (((struct iop_i2cc *) iop)->rcommand);

	process_i2c (bus, I2C_LEADER, &m);
//printf("Write procBytes = %d, status = %d.\n", m.procBytes, m.status);

	printf ("Write succesfull, now reading\n");

	m.address = (((struct iop_i2cc *) iop)->minor) + 1;
	/* select read adress */
	m.nrBytes = ((struct iop_i2cc *) iop)->nrread;
	/* nr of bytes expected from target */

	m.procBytes = 0;
	for (i = 0; i < MAXI2CMESSLENGTH; i++)
		mbuf[i] = 1;

	m.buf = mbuf;
	process_i2c (bus, I2C_LEADER, &m);
//printf("I2C procBytes = %d.\n", m.procBytes);
	// printf("I2C mbuf = %X %X.\n", m.buf[0], m.buf[1]);
	printf ("I2C status after read = %d.\n", m.status);

	switch (((struct iop_i2c *) iop)->nrread) {
	case 1:
		return mbuf[0];
		break;
	case 2:
		return ((int) mbuf[0] + ((int) mbuf[1] << 8));
		break;
	case 4:
		return (mbuf[0] + (mbuf[1] << 8) + (mbuf[2] << 16) + (mbuf[3] << 24));
		break;
	default:
		break;
	}
	return SA_FAIL;
}

int i2cc_controlhandler (struct iop *iop, int control, int value)
{
	return SA_FAIL;
}





int init_sai_i2c ()
{
/* init functions go here */
	struct iop_i2cc *i2cp;


	i2c_init (I2CA, I2CA_BASE);
	i2c_init (I2CB, I2CB_BASE);

	return 0;

}
