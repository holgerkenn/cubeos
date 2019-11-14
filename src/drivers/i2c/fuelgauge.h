#ifndef _FUELGAUGE_H
#define _FUELGAUGE_H

#include <sai_cube.h>

#define FG_COUNT01 RoboCube.i2cb.addr[0]
#define FG_COUNT23 RoboCube.i2cb.addr[1]
#define FG_STATUS RoboCube.i2cb.addr[2]
#define FG_COMP01 RoboCube.i2cb.addr[0]
#define FG_COMP23 RoboCube.i2cb.addr[1]

// setGaugeStatus input bits
#define OCLO 		0x0001
#define OCHI		0x0002
#define ODLO		0x0004
#define ODHI		0x0008
#define COMPENABLE	0x0010
#define OFFSETMEAS	0x0020
#define SETCOUNT	0x0040
#define CLRINT		0x0080
#define CLRCOUNTER	0x0100
#define SOFTSHDN	0x0200
#define DIRINTENABLE	0x0400

// Minimal needed word sent in setGaugeStatus
#define FG_MODE_WORD 	0xF800

struct fg_status
{
	unsigned char dirChange;
	unsigned char chargeStatus;
	unsigned char shdnStatus;
	unsigned char countStatus;
	unsigned char compStatus;
	unsigned char ocStatus;
	unsigned char odStatus;
};

int getGaugeStatus(struct fg_status *status);
int setGaugeStatus(int status);
void printGaugeStatus(struct fg_status *status);
int getCounterValue();
int setCompValue(int value);

#endif
