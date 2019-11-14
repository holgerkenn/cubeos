#include <cubeos.h>
#include <global_def.h>
#include <tpu.h>
#include <tpud.h>
#include <sai.h>
#include <sai_cube.h>
#include <sai_motor.h>
#include <stdio.h>

int motor_readhandler (struct iop *iop)
{
	if (iop->major != MAJ_TPU) {
		/* error, wrong major, init problem? */
		return SA_FAIL;
	}
	return SA_FAIL;
}

int motor_writehandler (struct iop *iop, int value)
{
	if (iop->major != MAJ_TPU) {
		/* error, wrong major, init problem. */
		return SA_FAIL;
	}
	return (0);

}

int motor_controlhandler (struct iop *iop, int control, int value)
{
	if (iop->major != MAJ_TPU) {
		/* error, tpu is on cpu bus */
		return SA_FAIL;
	}
	printf ("this is tpu_control, minor is %d\n\r", (((struct iop_tpu *) iop)->minor));

	return (SA_FAIL);
}




int init_sai_motor ()
{
/* init functions go here */
	int i;


	return 0;

}
