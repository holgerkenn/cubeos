int TPU_init();
int TPU_makepwm(unsigned char nr);
int TPU_setpwmperiod(unsigned char nr, unsigned short period);
int TPU_setpwmdc(unsigned char nr, unsigned char hightime);
int TPU_makepac(unsigned char nr);
unsigned short TPU_getpac(unsigned char nr);
