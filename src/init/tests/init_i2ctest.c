#include <i2cd.h>

struct i2cmess ma, sl;
char mvalue[10], svalue[10];

int main ()
{

	printf ("Hallo Thomas\r\n");

	i2c_init (I2CA, 0xffe000);
	i2c_init (I2CB, 0xffe800);

	printf ("Initialization done\r\n");

	ma.address = 0xac;
	ma.nrBytes = 6;
	ma.procBytes = 0;
	ma.buf = mvalue;
	mvalue[0] = 'H';
	mvalue[1] = 'a';
	mvalue[2] = 'l';
	mvalue[3] = 'l';
	mvalue[4] = 'o';
	mvalue[5] = '\0';
	process_i2c (I2CA, I2C_MASTER, &ma);
	printf ("write to I2CB: %s (%d)\r\n", mvalue, ma.procBytes);

	sl.address = 0x1;
	sl.nrBytes = 10;
	sl.procBytes = 0;
	sl.buf = svalue;
	svalue[0] = 'n';
	svalue[0] = 'o';
	svalue[0] = 'p';
	svalue[0] = '\0';
	process_i2c (I2CB, I2C_SLAVE, &sl);
	printf ("read slave I2CB: %s (%d)\r\n", svalue, sl.procBytes);

	printf ("End program\r\n");
	return (0);
}
