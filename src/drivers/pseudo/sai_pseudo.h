#ifndef _SAI_PSEUDO_H
#define _SAI_PSEUDO_H

#include <sai.h>

#define CUBEMAXPSEUDO 64
/* Sensor Actuator Interface */

/* A structured interface to the cube's sensos & actuators */


struct sa_pseudo
{
	sa_p dev[CUBEMAXPSEUDO]; 
};

struct iop_pseudo
{
char major; /*  */
int minor;  /* I2C Adress */
char nrread;
char nrwrite;
char fill[IOPARAMSIZE-7]; 
};

struct iop_i2cs
{
char major; /* =I2CB */
int minor;  /* I2C Adress */
char nrread;
char nrwrite;
char fill[IOPARAMSIZE-7]; 
};

struct iop_i2cc
{
char major; /* =I2CB */
int minor;  /* I2C Adress */
char nrread;
char nrwrite;
char rcommand;
char wcommand;
char fill[IOPARAMSIZE-7]; 
};




int i2cs_writehandler(struct iop * iop,int value);
int i2cs_readhandler(struct iop * iop);
int i2cs_controlhandler(struct iop * iop,int control, int value);
int i2cc_writehandler(struct iop * iop,int value);
int i2cc_readhandler(struct iop * iop);
int i2cc_controlhandler(struct iop * iop,int control, int value);
int init_sai_i2c();
#endif
