#ifndef SYS_VAR_H
#define SYS_VAR_H
extern int sys_error;
extern unsigned short sys_errorcode;
extern unsigned char Blocking_Serial_Out;
extern unsigned long _time_seconds;
extern unsigned short _time_ticks;
extern unsigned short _quantum_count;
extern unsigned short context_srsave;
extern int __MYPID;
#define SYS_ERR_KRNLINBUF_OVF 0x1
#define SYS_ERR_RDIOINBUF_OVF 0x2
#endif
