#include <charger.h>
#include <stdlib.h>
#include <sai.h>
#include <stdio.h>
#include <cubeos.h>
#include <malloc.h>
#include <kerror.h>
#include <i2cd.h>
#include <sai_i2c.h>

int setChargerStatus (int status)
{
	return sa_write (BC_SETMODE, status);
}

int setChargingVoltage (int voltage)
{
	return sa_write (BC_CHARGINGVOLTAGE, voltage);
}

int setChargingCurrent (int current)
{
	return sa_write (BC_CHARGINGCURRENT, current);
}

int getChargerStatus (struct ch_status *status)
{
	int tempStatus;

	tempStatus = sa_read (BC_STATUS);
	if (status == NULL)
		return tempStatus;
	printf ("Charger status = %X.\n", tempStatus & 0xFFFF);

	status->chargeInhibited = tempStatus & 0x0001;
	status->voltageNotReg = (tempStatus >> 2) & 0x0001;
	status->currentNotReg = (tempStatus >> 3) & 0x0001;
	status->currentOR = (tempStatus >> 6) & 0x0001;
	status->voltageOR = (tempStatus >> 7) & 0x0001;
	status->thermistorOR = (tempStatus >> 8) & 0x0001;
	status->thermistorCold = (tempStatus >> 9) & 0x0001;
	status->thermistorHot = (tempStatus >> 10) & 0x0001;
	status->thermistorUR = (tempStatus >> 11) & 0x0001;
	status->alarmInhibited = (tempStatus >> 12) & 0x0001;
	status->powerFail = (tempStatus >> 13) & 0x0001;
	status->batteryPresent = (tempStatus >> 14) & 0x0001;
	status->acPresent = (tempStatus >> 15) & 0x0001;

	return 0;
}

void printChargerStatus (struct ch_status *status)
{
	if (status == NULL)
		return;

	if (status->chargeInhibited)
		printf ("\nCharger is off.\n");
	else
		printf ("\nReady to charge a smart battery.\n");

	if (status->voltageNotReg)
		printf ("BATT voltage is less than V0.\n");
	else
		printf ("BATT voltage is limited at V0.\n");

	if (status->currentNotReg)
		printf ("Current is less than I0.\n");
	else
		printf ("Current is limited to I0.\n");

	if (status->currentOR)
		printf ("ChargingCurrent() value exceeds deliverable current.\n");
	else
		printf ("ChargingCurrent() value is valid.\n");

	if (status->voltageOR)
		printf ("ChargingVoltage() value exceeds deliverable voltage.\n");
	else
		printf ("ChargingVoltage() value is valid.\n");

	if (status->thermistorOR)
		printf ("THM voltage > 91%% of REF voltage.\n");
	else if (status->thermistorCold)
		printf ("THM voltage is between 75%% and 91%% of REF voltage.\n");
	else if (!status->thermistorHot)
		printf ("THM voltage is between 23%% and 75%% of REF voltage.\n");
	else if (!status->thermistorUR)
		printf ("THM voltage is between 5%% and 23%% of REF voltage.\n");
	else
		printf ("THM voltage is below 5%% of REF voltage.\n");

	if (status->alarmInhibited)
		printf ("Alarm inhibited set.\n");

	/* 
	   if (status->batteryPresent) printf("Battery is present.\n"); else
	   printf("No battery present.\n"); */

	if (status->acPresent)
		printf ("VL voltage > 4V.\n");
	else
		printf ("VL voltage < 4v.\n");

}


int init_charger ()
{

	struct iop_i2cc *i2cp;


// Battery charger

// Set charger mode
	if ((RoboCube.i2cb.addr[3] = malloc (sizeof (struct sa_entry)))) {
		complain (ERR_ALERT, "malloc for sai_init failed");
	} else {

		RoboCube.i2cb.addr[3]->readhandler = i2cc_readhandler;
		RoboCube.i2cb.addr[3]->writehandler = i2cc_writehandler;
		RoboCube.i2cb.addr[3]->controlhandler = i2cc_controlhandler;
		i2cp = (struct iop_i2cc *) &RoboCube.i2cb.addr[3]->iop;

		i2cp->major = MAJ_I2CB;
		i2cp->minor = 0x12;
		i2cp->nrread = 0;
		i2cp->nrwrite = 2;
		i2cp->wcommand = 0x12;
		i2cp->rcommand = 0x00;
	}

// Set charging voltage
	if ((RoboCube.i2cb.addr[4] = malloc (sizeof (struct sa_entry)))) {
		complain (ERR_ALERT, "malloc for sai_init failed");
	} else {

		RoboCube.i2cb.addr[4]->readhandler = i2cc_readhandler;
		RoboCube.i2cb.addr[4]->writehandler = i2cc_writehandler;
		RoboCube.i2cb.addr[4]->controlhandler = i2cc_controlhandler;
		i2cp = (struct iop_i2cc *) &RoboCube.i2cb.addr[4]->iop;

		i2cp->major = MAJ_I2CB;
		i2cp->minor = 0x12;
		i2cp->nrread = 0;
		i2cp->nrwrite = 2;
		i2cp->wcommand = 0x15;
		i2cp->rcommand = 0x00;
	}

// Set charging current
	if ((RoboCube.i2cb.addr[5] = malloc (sizeof (struct sa_entry)))) {
		complain (ERR_ALERT, "malloc for sai_init failed");
	} else {

		RoboCube.i2cb.addr[5]->readhandler = i2cc_readhandler;
		RoboCube.i2cb.addr[5]->writehandler = i2cc_writehandler;
		RoboCube.i2cb.addr[5]->controlhandler = i2cc_controlhandler;
		i2cp = (struct iop_i2cc *) &RoboCube.i2cb.addr[5]->iop;

		i2cp->major = MAJ_I2CB;
		i2cp->minor = 0x12;
		i2cp->nrread = 0;
		i2cp->nrwrite = 2;
		i2cp->wcommand = 0x14;
		i2cp->rcommand = 0x00;
	}

// Get charger status
	if ((RoboCube.i2cb.addr[6] = malloc (sizeof (struct sa_entry)))) {
		complain (ERR_ALERT, "malloc for sai_init failed");
	} else {


		RoboCube.i2cb.addr[6]->readhandler = i2cc_readhandler;
		RoboCube.i2cb.addr[6]->writehandler = i2cc_writehandler;
		RoboCube.i2cb.addr[6]->controlhandler = i2cc_controlhandler;
		i2cp = (struct iop_i2cc *) &RoboCube.i2cb.addr[6]->iop;

		i2cp->major = MAJ_I2CB;
		i2cp->minor = 0x12;
		i2cp->nrread = 2;
		i2cp->nrwrite = 0;
		i2cp->wcommand = 0x00;
		i2cp->rcommand = 0x13;

	}

	return 0;

}
