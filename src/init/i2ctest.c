#include <stdio.h>
#include <cubeos.h>
#include <i2cd.h>
#include <analog.h>
#include <digital.h>

int I2CTest ()
{
	char a;
	int i, j;

	printf ("Calling I2C_INIT for Channel A:\n");
	printf (" Base 0x%x, IV %d", I2CA_BASE, I2CA_VECTORNUM);

	i2c_init (I2CA, I2CA_BASE);


	printf ("Calling I2C_INIT for Channel B...\n");
	printf (" Base 0x%x, IV %d", I2CB_BASE, I2CB_VECTORNUM);
	i2c_init (I2CB, I2CB_BASE);

	printf ("Seems to have worked...\n");


	printf ("Scanning i2c bus A...\n");

	printf ("found %d devices\n", i2c_scanbus (I2CA));

	printf ("Scanning i2c bus B...\n");

	printf ("Found %d devices\n", i2c_scanbus (I2CB));


	printf ("init_analog...\n");
	init_analog ();
	printf ("OK\n");

	for (j = 0; j < 5; j++) {

		printf ("reading analog at adress 0x%x, channel 0 : ", ad_data[0].address);

		a = ReadAnalogIn (0, 0);

		if (i2c_messagestatus (&analog_m))
			return (-1);

		printf ("%d OK\n", a);
		for (i = 1; i < 100000; i++);
	}
	printf ("init_digital...\n");

	init_digital ();

	printf ("OK\n");

	for (j = 0; j < 5; j++) {
		printf ("reading digital at adress 0x%x : ", bin_data[0].address);

		a = ReadBinIn (0);

		if (i2c_messagestatus (&digital_m))
			return (-1);

		printf ("0x%x OK\n", a);
		for (i = 1; i < 100000; i++);
	}


	return (0);
}
