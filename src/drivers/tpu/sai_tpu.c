#include <cubeos.h>
#include <global_def.h>
#include <tpu.h>
#include <tpud.h>
#include <sai.h>
#include <sai_cube.h>
#include <sai_tpu.h>
#include <stdio.h>
#include <malloc.h>
#include <kerror.h>

int tpu_readhandler (struct iop *iop)
{
	if (iop->major != MAJ_TPU) {
		/* error, wrong major, init problem? */
		return SA_FAIL;
	}
	switch (((struct iop_tpu *) iop)->function) {
	case TPU_FKT_PPWA:
		break;
	case TPU_FKT_OC:
		break;
	case TPU_FKT_SM:
		break;
	case TPU_FKT_PSP:
		break;
	case TPU_FKT_PMA:
		break;
	case TPU_FKT_ITC:
		return getPAR (((struct iop_tpu *) iop)->minor, 3);
		break;
	case TPU_FKT_PWM:
		break;
	case TPU_FKT_DIO:
		break;
	case TPU_FKT_SPWM:
		break;
	case TPU_FKT_QDEC:
		break;
	}

	return SA_FAIL;
}

int tpu_writehandler (struct iop *iop, int value)
{
	if (iop->major != MAJ_TPU) {
		/* error, wrong major, init problem. */
		return SA_FAIL;
	}
	switch (((struct iop_tpu *) iop)->function) {
	case TPU_FKT_PPWA:
		break;
	case TPU_FKT_OC:
		break;
	case TPU_FKT_SM:
		break;
	case TPU_FKT_PSP:
		break;
	case TPU_FKT_PMA:
		break;
	case TPU_FKT_ITC:
		break;
	case TPU_FKT_PWM:
		printf ("calling setpwmdc(%d,%d)\n\r", (((struct iop_tpu *) iop)->minor), value);
		if (TPU_setpwmdc (((struct iop_tpu *) iop)->minor, value)) {
			printf ("failed!");
		}
		break;
	case TPU_FKT_DIO:
		break;
	case TPU_FKT_SPWM:
		break;
	case TPU_FKT_QDEC:
		break;
	default:
		return SA_FAIL;

	}
	return (0);

}

int tpu_controlhandler (struct iop *iop, int control, int value)
{
	if (iop->major != MAJ_TPU) {
		/* error, tpu is on cpu bus */
		return SA_FAIL;
	}
	printf ("this is tpu_control, minor is %d\n\r", (((struct iop_tpu *) iop)->minor));

	switch (control) {

/* TPU */
/* TPU init control */
	case CTL_TPU_SETFUNCTION:
		((struct iop_tpu *) iop)->function = value;
		setCFSR (((struct iop_tpu *) iop)->minor, value);
		switch (value) {
		case TPU_FKT_PPWA:
			break;
		case TPU_FKT_OC:
			break;
		case TPU_FKT_SM:
			break;
		case TPU_FKT_PSP:
			break;
		case TPU_FKT_PMA:
			break;
		case TPU_FKT_ITC:
			printf ("calling makepac(%d)\n\r", (((struct iop_tpu *) iop)->minor));
			TPU_makepac (((struct iop_tpu *) iop)->minor);
			break;
		case TPU_FKT_PWM:
			printf ("calling makepwm(%d)\n\r", (((struct iop_tpu *) iop)->minor));
			TPU_makepwm (((struct iop_tpu *) iop)->minor);
			break;
		case TPU_FKT_DIO:
			break;
		case TPU_FKT_SPWM:
			break;
		case TPU_FKT_QDEC:
			break;
		default:
			return SA_FAIL;
		}
		break;
/* TPU generic controls */
	case CTL_TPU_SETHSEQ:
		setHSQR (((struct iop_tpu *) iop)->minor, value);
		break;
	case CTL_TPU_SETHSRR:
		setHSRR (((struct iop_tpu *) iop)->minor, value);
		break;
	case CTL_TPU_SETPRIO:
		setCPR (((struct iop_tpu *) iop)->minor, value);
		break;
	case CTL_TPU_CLEARCISR:
		clearCISR (((struct iop_tpu *) iop)->minor);
		break;
	case CTL_TPU_SETPAR0:
	case CTL_TPU_SETPAR1:
	case CTL_TPU_SETPAR2:
	case CTL_TPU_SETPAR3:
	case CTL_TPU_SETPAR4:
	case CTL_TPU_SETPAR5:
	case CTL_TPU_SETPAR6:
	case CTL_TPU_SETPAR7:
		setPAR (((struct iop_tpu *) iop)->minor, control & 0x7, value);
/* TPU function dependent controls */

	case CTL_TPU_SETPWMDC:
		if ((((struct iop_tpu *) iop)->function) != TPU_FKT_PWM)
			return (SA_FAIL);
		printf ("calling setpwmdc(%d,%d)\n\r", (((struct iop_tpu *) iop)->minor), value);

		if (TPU_setpwmdc (((struct iop_tpu *) iop)->minor, value)) {
			printf ("failed!\n\r");
		}
		break;
	case CTL_TPU_SETPWMPERIOD:
		if ((((struct iop_tpu *) iop)->function) != TPU_FKT_PWM)
			return (SA_FAIL);
		printf ("calling setpwmperiod(%d,%d)\n\r", (((struct iop_tpu *) iop)->minor), value);
		if (TPU_setpwmperiod (((struct iop_tpu *) iop)->minor, value)) {
			printf ("failed!\n\r");
		}
		break;


	}
	return (0);

}




int init_sai_tpu ()
{
/* init functions go here */
	int i;
	struct iop_tpu *tpup;

/* TPU */
	TPU_init ();

	for (i = 0; i < 16; i++) {
		if ((RoboCube.tpu[i] = malloc (sizeof (struct sa_entry)))) {
			complain (ERR_ALERT, "malloc failed for init_sai_tpu()");
		} else {
			tpup = (struct iop_tpu *) &RoboCube.tpu[i]->iop;
			tpup->major = MAJ_TPU;
			tpup->minor = i;
			tpup->function = 0;	/* off */
			RoboCube.tpu[i]->readhandler = tpu_readhandler;
			RoboCube.tpu[i]->writehandler = tpu_writehandler;
			RoboCube.tpu[i]->controlhandler = tpu_controlhandler;
			TPU_initchannel (i);
		}
	}

	return 0;

}
