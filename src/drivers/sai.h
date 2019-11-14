/* Sensor Actuator Interface */

#ifndef _SAI_H
#define _SAI_H

#define IOPARAMSIZE 30

#define SA_FAIL -1
#define SA_OK 0

struct iop
{
char major;
char fill[IOPARAMSIZE-1];
};


struct sa_entry 
{ 
char * name;
int (*writehandler)(struct iop*,int value);
int (*readhandler)(struct iop*);
int (*controlhandler)(struct iop*,int function,int value);
struct iop iop;
};


typedef struct sa_entry * sa_p;

int sa_write(sa_p device,int value);
int sa_read(sa_p device);
int sa_control(sa_p device,int control,int value);
/* later on:
int sa_control(struct sa_entry * device,int control,...);
*/

#endif
