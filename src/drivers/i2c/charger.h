#ifndef _CHARGER_H
#define _CHARGER_H

#include <sai_cube.h>

// Battery charger functions
#define BC_SETMODE RoboCube.i2cb.addr[3]
#define BC_CHARGINGVOLTAGE RoboCube.i2cb.addr[4]
#define BC_CHARGINGCURRENT RoboCube.i2cb.addr[5]
#define BC_STATUS RoboCube.i2cb.addr[6]

// Battery charger status bits, used in calls to setChargerMode()
#define INHIBIT_CHARGE 		0x0001
#define POR_RESET 		0x0004
#define BATTERY_PRESENT_MASK 	0x0020
#define POWER_FAIL_MASK 	0x0040
#define HOT_STOP 		0x0400

// Basic minimal data that has to be sent to battery charger in
// setChargerMode()
#define BC_MODE_WORD 0xFB90 
                                                                    

struct ch_status
{
	unsigned char chargeInhibited;
	unsigned char voltageNotReg;
	unsigned char currentNotReg;
	unsigned char currentOR;
	unsigned char voltageOR;
	unsigned char thermistorOR;
	unsigned char thermistorCold;
	unsigned char thermistorHot;
	unsigned char thermistorUR;
	unsigned char alarmInhibited;
	unsigned char powerFail;
	unsigned char batteryPresent;
	unsigned char acPresent;
};

int setChargerMode(int modeWord);
int setChargingVoltage(int voltage);
int setChargingCurrent(int current);
int getChargerStatus(struct ch_status *status);
void printChargerStatus(struct ch_status *status);

#endif

