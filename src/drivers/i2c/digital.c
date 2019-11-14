#include <stdlib.h>
#include <stdio.h>
#include <digital.h>
#include <i2cd.h>


struct bin_datas bin_data[32];
int binnum;


unsigned char digital_mbuf[MAXI2CMESSLENGTH];
struct i2cmess digital_m;

unsigned char ReadBinIn (int chip)
{

	if (bin_data[chip].active == 0)
		return (0);

/* read value */
	digital_m.address = bin_data[chip].address | 0x1;	/* Read adress */
	digital_m.nrBytes = 1;	/* Read one byte */
	digital_mbuf[0] = 0;	/* clear buffer */
	digital_m.buf = digital_mbuf;


	process_i2c (bin_data[chip].bus, I2C_MASTER, &digital_m);

	return digital_mbuf[0];

}

int WriteBinOut (int chip, unsigned char value)
{

	if (bin_data[chip].active == 0)
		return (-1);

/* read value */
	digital_m.address = bin_data[chip].address;	/* Write adress */
	digital_m.nrBytes = 1;	/* Write one byte */
	digital_mbuf[0] = value;
	digital_m.buf = digital_mbuf;

	process_i2c (bin_data[chip].bus, I2C_MASTER, &digital_m);

	return (digital_m.status == I2C_OK);

}

int init_digital ()
{
	int i;

	binnum = 0;
	for (i = 0; i < 31; i++)
		bin_data[i].active = 0;

// later this should come from the HWinfo struct 
	for (i = 0; i < 31; i++) {

		if (i < 16) {
			bin_data[binnum].bus = I2CA;
		} else {
			bin_data[binnum].bus = I2CB;
		}

		if ((i & 0xf) < 8) {
			bin_data[binnum].address = ((i & 0x07) << 1) | 0x40;	// PCF8574

		} else {
			bin_data[binnum].address = ((i & 0x07) << 1) | 0x70;	// PCF8574A

		}
		bin_data[binnum].status = 0x00;		// maybe later...



		digital_m.address = bin_data[binnum].address | 0x1;	/* Read adress */
		digital_m.nrBytes = 1;	/* Read one false byte, then read value */
		digital_mbuf[0] = 0;	/* clear buffer */
		digital_m.buf = digital_mbuf;
		process_i2c (bin_data[binnum].bus, I2C_MASTER, &digital_m);


		switch (digital_m.status) {
		case I2C_OK:
			printf ("digital device %d at adress 0x%x \n", binnum, bin_data[binnum].address);
			bin_data[binnum].active = 1;
			binnum++;
			break;
		case I2C_TIME_OUT:
		case I2C_NACK_ON_ADDRESS:
			break;

		default:
			i2c_messagestatus (&digital_m);
		}


	}

	return (binnum > 0 ? 0 : 1);


}
