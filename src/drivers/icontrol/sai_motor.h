#ifndef _SAI_TPU_H
#define _SAI_TPU_H

#include <sai.h>
/* Sensor Actuator Interface */
/* motor */

#define CTL_MOT_SETMODE	0x00000001



struct iop_motor
{
char major; /* =10 for Motor */
int  minor; /* Motor number */
struct sa_entry * pwmout;
struct sa_entry * qdec1;
struct sa_entry * qdec2;
struct sa_entry * dir0;
struct sa_entry * dir1;
char fill[IOPARAMSIZE-25]; 
};


int motor_readhandler(struct iop * iop);
int motor_writehandler(struct iop* iop, int value);
int motor_controlhandler(struct iop * iop, int control, int value);
int init_sai_motor();

#endif
