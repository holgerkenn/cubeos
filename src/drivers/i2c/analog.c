#include <stdlib.h>
#include <stdio.h>
#include <analog.h>
#include <i2cd.h>


struct ad_datas ad_data[16];
int adnum;

char analog_mbuf[MAXI2CMESSLENGTH];
struct i2cmess analog_m;

int ReadAnalogIn (int chip, int channel)
{

	if (!ad_data[chip].active)
		return (0);	/* inactive a/d are 0 */

/* set a/d channel */
	analog_m.address = ad_data[chip].address;
	analog_m.nrBytes = 1;
	analog_m.buf = analog_mbuf;
	analog_mbuf[0] = (ad_data[chip].status & 0x70) | (channel & 0x03);	/* no autoincrement */
	process_i2c (ad_data[chip].bus, I2C_MASTER, &analog_m);

/* read value */
	analog_m.address = ad_data[chip].address | 0x1;		/* Read adress */
	analog_m.nrBytes = 2;	/* Read one false byte, then read value */
	analog_mbuf[0] = 0;	/* clear buffer */
	analog_mbuf[1] = 0;
	analog_m.buf = analog_mbuf;


	process_i2c (ad_data[chip].bus, I2C_MASTER, &analog_m);

	return ((int) (((unsigned int) analog_mbuf[1]) & 0xff));

}

int ConfigureAnalog (int chip, char how)
{

	if (!ad_data[chip].active)
		return (-1);

	ad_data[chip].status = how & 0x70;

/* set status */
	analog_m.address = ad_data[chip].address;
	analog_m.nrBytes = 1;
	analog_m.buf = analog_mbuf;
	analog_mbuf[0] = (ad_data[chip].status);
	process_i2c (ad_data[chip].bus, I2C_MASTER, &analog_m);

	return (analog_m.status == I2C_OK);
}

int WriteAnalogOut (int chip, int value)
{

	if (!ad_data[chip].active)
		return (-1);


/* set status */
	analog_m.address = ad_data[chip].address;
	analog_m.nrBytes = 2;
	analog_m.buf = analog_mbuf;
	analog_mbuf[0] = (ad_data[chip].status);
	analog_mbuf[1] = value & 0xff;
	process_i2c (ad_data[chip].bus, I2C_MASTER, &analog_m);

	return (analog_m.status == I2C_OK);
}

int init_analog ()
{
	int i;
	adnum = 0;

	for (i = 0; i < 15; i++)
		ad_data[i].active = 0;

// later this should come from the HWinfo struct 
	for (i = 0; i < 15; i++) {

		if (i < 8) {
			ad_data[adnum].bus = I2CA;
		} else {
			ad_data[adnum].bus = I2CB;
		}

		ad_data[adnum].address = ((i & 0x07) << 1) | 0x90;

		ad_data[adnum].status = 0x00;	// 4x single ended, analog output off 

		// this is the power-on reset state

		analog_m.address = ad_data[adnum].address | 0x1;	/* Read adress */
		analog_m.nrBytes = 1;	/* Read one false byte, then read value */
		analog_mbuf[0] = 0;	/* clear buffer */
		analog_m.buf = analog_mbuf;
		process_i2c (ad_data[adnum].bus, I2C_MASTER, &analog_m);


		switch (analog_m.status) {
		case I2C_OK:
			printf ("analog device %d at adress 0x%x \n", adnum, ad_data[adnum].address);
			ad_data[adnum].active = 1;
			adnum++;
			break;
		case I2C_TIME_OUT:
		case I2C_NACK_ON_ADDRESS:
			break;

		default:
			i2c_messagestatus (&analog_m);
		}


	}

	return (adnum > 0 ? 0 : 1);

}
