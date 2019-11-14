#ifndef _DIGITAL_H
#define _DIGITAL_H

#define MAXI2CMESSLENGTH 8

struct bin_datas
{
char bus;
char address;
char status;
char active;
};

extern struct bin_datas bin_data[32];
extern unsigned char digital_mbuf[MAXI2CMESSLENGTH];
extern struct i2cmess digital_m;
extern int binnum;

unsigned char ReadBinIn(int chip);
int WriteBinOut(int chip,unsigned char value);

int init_digital();

#endif
