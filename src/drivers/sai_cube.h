#ifndef _SAI_CUBE_H
#define _SAI_CUBE_H

#include <sai.h>
#include <sai_i2c.h>
#include <sai_tpu.h>
//#include <sai_pseudo.h>

/* Sensor Actuator Interface */
/* A structured interface to the cube's sensos & actuators */

#define MAJ_TPU 0
#define MAJ_I2CA 1
#define MAJ_I2CB 2
#define MAJ_SPI 3

#define MAJ_MOT 10


struct sa_cube
{
	struct sa_i2c i2ca;
	struct sa_i2c i2cb;
	sa_p tpu[16];
	sa_p bin[2];
//	struct sa_pseudo pseudo;
};

extern struct sa_cube RoboCube;

int init_sai_cube();

#endif
