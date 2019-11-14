#include <stdlib.h>
#include <fuelgauge.h>
#include <stdio.h>
#include <malloc.h>
#include <kerror.h>


int getGaugeStatus (struct fg_status *status)
{
	int tempStatus;

	tempStatus = sa_read (FG_STATUS);
	printf ("Gauge status = %X.\n", tempStatus);
	if (status == NULL)
		return tempStatus;

	status->dirChange = (tempStatus >> 1) & 0x0001;
	status->chargeStatus = (tempStatus >> 2) & 0x0001;
	status->shdnStatus = (tempStatus >> 3) & 0x0001;
	status->countStatus = (tempStatus >> 4) & 0x0001;
	status->compStatus = (tempStatus >> 5) & 0x0001;
	status->ocStatus = (tempStatus >> 6) & 0x0001;
	status->odStatus = (tempStatus >> 7) & 0x0001;

	return 0;
}

int setGaugeStatus (int status)
{
	printf ("Writing word %X.\n", status);
	return sa_write (FG_STATUS, status);
}

void printGaugeStatus (struct fg_status *status)
{
	if (status == NULL)
		return;

	printf ("\n");
	if (status->dirChange)
		printf ("Current flow changed direction.\n");
	if (status->chargeStatus)
		printf ("Charging current detected.\n");
	if (status->shdnStatus)
		printf ("Device in soft-shutdown.\n");
	if (status->countStatus)
		printf ("SETCOUNT bit is set.\n");
	else
		printf ("SETCOUNT bit is clear.\n");
	if (status->compStatus)
		printf ("COMPINT interrupt.\n");
	if (status->ocStatus)
		printf ("Overcurrent in charging direction.\n");
	if (status->odStatus)
		printf ("Overcurrent in discharging direction.\n");
}

int getCounterValue ()
{
	int value01 = 0, value23 = 0;

	value01 = sa_read (FG_COUNT01);
	value23 = sa_read (FG_COUNT23);
	return ((value23 << 16) + value01);
}

int setCompValue (int value)
{
	if (sa_write (FG_COMP01, value & 0xFFFF) == SA_FAIL) {
		printf ("First write has failed.\n");
		return SA_FAIL;
	}
	return (sa_write (FG_COMP23, (value >> 16) & 0xFFFF));
}


int init_fuelgauge ()
{
	struct iop_i2cc *i2cp;


// Fuel gauge

	if ((RoboCube.i2cb.addr[0] = malloc (sizeof (struct sa_entry)))) {
		complain (ERR_ALERT, "malloc for sai_init failed");
	} else {

		RoboCube.i2cb.addr[0]->readhandler = i2cc_readhandler;
		RoboCube.i2cb.addr[0]->writehandler = i2cc_writehandler;
		RoboCube.i2cb.addr[0]->controlhandler = i2cc_controlhandler;
		i2cp = (struct iop_i2cc *) &RoboCube.i2cb.addr[0]->iop;

		i2cp->major = MAJ_I2CB;
		i2cp->minor = 0x8E;
		i2cp->nrread = 2;
		i2cp->nrwrite = 2;
		i2cp->wcommand = 0x00;
		i2cp->rcommand = 0x82;
	}

	if ((RoboCube.i2cb.addr[1] = malloc (sizeof (struct sa_entry)))) {
		complain (ERR_ALERT, "malloc for sai_init failed");
	} else {

		RoboCube.i2cb.addr[1]->readhandler = i2cc_readhandler;
		RoboCube.i2cb.addr[1]->writehandler = i2cc_writehandler;
		RoboCube.i2cb.addr[1]->controlhandler = i2cc_controlhandler;
		i2cp = (struct iop_i2cc *) &RoboCube.i2cb.addr[1]->iop;
		i2cp->major = MAJ_I2CB;
		i2cp->minor = 0x8E;
		i2cp->nrread = 2;
		i2cp->nrwrite = 2;
		i2cp->wcommand = 0x01;
		i2cp->rcommand = 0x83;
	}

// Get status
	if ((RoboCube.i2cb.addr[2] = malloc (sizeof (struct sa_entry)))) {
		complain (ERR_ALERT, "malloc for sai_init failed");
	} else {


		RoboCube.i2cb.addr[2]->readhandler = i2cc_readhandler;
		RoboCube.i2cb.addr[2]->writehandler = i2cc_writehandler;
		RoboCube.i2cb.addr[2]->controlhandler = i2cc_controlhandler;
		i2cp = (struct iop_i2cc *) &RoboCube.i2cb.addr[2]->iop;
		i2cp->major = MAJ_I2CB;
		i2cp->minor = 0x8E;
		i2cp->nrread = 2;
		i2cp->nrwrite = 2;
		i2cp->wcommand = 0x04;
		i2cp->rcommand = 0x84;

	}

	return 0;

}
