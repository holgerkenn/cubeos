#ifndef _ANALOG_H
#define _ANALOG_H

#define MAXI2CMESSLENGTH 8

#define ANALOG_4SE    0x00;
#define ANALOG_3DI    0x10;
#define ANALOG_2S2D   0x20;
#define ANALOG_2DI    0x30;
#define ANALOG_DACOE  0x40;


struct ad_datas
{
char bus;
char address;
char ad[3];
char da;
char status;
char active;
};

extern struct ad_datas ad_data[16];
extern char analog_mbuf[MAXI2CMESSLENGTH];
extern struct i2cmess analog_m;
extern int adnum;

int ReadAnalogIn(int chip,int port);
int WriteAnalogOut(int chip,int value);
int ConfigureAnalog(int chip,char how);
int init_analog();

#endif
