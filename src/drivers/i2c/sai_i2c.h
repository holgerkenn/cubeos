#ifndef _SAI_I2C_H
#define _SAI_I2C_H

#include <sai.h>

/* Sensor Actuator Interface */

/* A structured interface to the cube's sensos & actuators */

#define I2CMAXDEV 64
#define MAXI2CMESSLENGTH 8

struct sa_i2c
{
	sa_p addr[I2CMAXDEV]; /* Too big. */
};

struct iop_i2c
{
char major; /* =I2CB */
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
