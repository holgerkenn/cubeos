#include <cubeos.h>
#include <global_def.h>
#include <mc68332.h>
#include <ivtab.h>
#include <qsm.h>
#include <io_sci.h>

extern void qsmWrapper ();

int init_qsm ()
{
	unsigned short v;

	IVTab_setvector (QSM_VECTORNUM, qsmWrapper);

	writeshort (QSM_QSMCR, 0x8e);
	/* Bit15: STOP  (QSM clock stopped)
	   Bit14: FRZ1  (Halt QSM)
	   Bit7 : SUPV  (all QSM registers in supervisor space)
	   Bit3..0: IARB (of the QSM module)
	 */
	writebyte (QSM_QIVR, QSM_VECTORNUM);
	/* Bit7..0  INTV (interrupt vector for the QSM module) */
	v = readbyte (QSM_QILR);
	writebyte (QSM_QILR, (v & 0xf8) | 0x5);
	/* Bit13..11: ILQSPI (internal iarb level of QSPI)
	   Bit10..8 : ILSCI  (internal iarb level of SCI)
	 */
	return 0;
}

void qsm_int ()
{

	sci_int ();

}
