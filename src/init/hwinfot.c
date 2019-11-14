#include "hwinfot.h"
#include <stdio.h>
#include "hwinfo.h"


int HWInfoTest ()
{
	struct hw_info *hwinfo;

	hwinfo = (struct hw_info *) (0x00f2040c);

	printf ("Hardware InfoStruct:\n");

	printf (" trying to read...\n");
	fflush (stdout);

	printf (" Clock Speed: %d\n", hwinfo->clk);

	printf (" Board Type: %d\n", hwinfo->board_type);

	printf (" EPROM Base: 0x%x\n", hwinfo->eprom_base);

	printf (" EPROM Size: %d\n", hwinfo->eprom_size);

	printf (" RAM Base: 0x%x\n", hwinfo->ram_base);

	printf (" RAM Size: %d\n", hwinfo->ram_size);

	printf (" Duart available: %d\n", hwinfo->duart_avail);

	if (hwinfo->duart_avail) {

		printf ("   Duart information:\n");

		printf ("   Base: 0x%x\n", hwinfo->duart.base);

	}
	printf (" I2Ca available: %d\n", hwinfo->i2ca_avail);

	if (hwinfo->i2ca_avail) {

		printf ("   i2ca information:\n");

		printf ("   Base: 0x%x\n", hwinfo->i2ca.base);
		printf ("   ADDR: %d\n", hwinfo->i2ca.ownaddr);
		printf ("   IVR: %d\n", hwinfo->i2ca.ivr);


	}
	printf (" I2CB available: %d\n", hwinfo->i2cb_avail);

	if (hwinfo->i2cb_avail) {

		printf ("   i2cb information:\n");

		printf ("   Base: 0x%x\n", hwinfo->i2cb.base);
		printf ("   ADDR: %d\n", hwinfo->i2cb.ownaddr);
		printf ("   IVR: %d\n", hwinfo->i2cb.ivr);

	}
	return (0);


}
