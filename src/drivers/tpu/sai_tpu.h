#ifndef _SAI_TPU_H
#define _SAI_TPU_H

#include <sai.h>
/* Sensor Actuator Interface */
/* TPU */
#define CTL_TPU_SETFUNCTION	0x00000101
#define CTL_TPU_SETHSEQ		0x00000102
#define CTL_TPU_SETHSRR		0x00000103
#define CTL_TPU_SETPRIO		0x00000104
#define CTL_TPU_CLEARCISR	0x00000105
#define CTL_TPU_SETPAR0		0x00000110
#define CTL_TPU_SETPAR1		0x00000111
#define CTL_TPU_SETPAR2		0x00000112
#define CTL_TPU_SETPAR3		0x00000113
#define CTL_TPU_SETPAR4		0x00000114
#define CTL_TPU_SETPAR5		0x00000115
#define CTL_TPU_SETPAR6		0x00000116
#define CTL_TPU_SETPAR7		0x00000117
#define CTL_TPU_SETPWMDC	0x00000220 /* = write */
#define CTL_TPU_SETPWMPERIOD	0x00000221

#define CTL_TPU_GETFUNCTION	0x10000101
#define CTL_TPU_GETHSEQ		0x10000102
#define CTL_TPU_GETHSRR		0x10000103
#define CTL_TPU_GETPRIO		0x10000104
#define CTL_TPU_GETCISR		0x10000105
#define CTL_TPU_GETPARA0	0x10000110
#define CTL_TPU_GETPARA1	0x10000111
#define CTL_TPU_GETPARA2	0x10000112
#define CTL_TPU_GETPARA3	0x10000113
#define CTL_TPU_GETPARA4	0x10000114
#define CTL_TPU_GETPARA5	0x10000115
#define CTL_TPU_GETPARA6	0x10000116
#define CTL_TPU_GETPARA7	0x10000117

struct iop_tpu
{
char major; /* =0 for TPU */
int  minor; /* tpu channel number */
char function; /* function code for channel, copy of TPU register */
char fill[IOPARAMSIZE-6]; 
};


int tpu_readhandler(struct iop * iop);
int tpu_writehandler(struct iop* iop, int value);
int tpu_controlhandler(struct iop * iop, int control, int value);
int init_sai_tpu();

#endif
