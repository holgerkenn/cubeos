#ifndef GLOBAL_DEF_H

#define GLOBAL_DEF_H



#ifdef CUBE3

#define EPROM_BASE       0xe00000  // -- 0xf1ffff
#define CPURAM_BASE      0xfff000  // -- 0xf207ff
#define DUART_BASE       0xffe400  // -- 0xffdfff

#define SIM_BASE         0xfffa00
#define PTIMER_VECTORNUM 64
#define CPURAMCTRL_BASE  0xfffb00
#define SCI_BASE         0xfffc00
#define QSM_VECTORNUM    65
#define TPU_BASE         0xfffe00
#define TPU_VECTORBASE   0x80 
#define I2CA_BASE	 0xffe200
#define I2CB_BASE	 0xffe000
#define I2CA_VECTORNUM	 27 // irq3
#define I2CB_VECTORNUM	 25 // irq1 /* propblem: irq mask set to 2 */
#define FASTBIN_BASE	 0xfe0000

#undef  I2C_INITCS

#else

#define EPROM_BASE       0xf00000  // -- 0xf1ffff
#define CPURAM_BASE      0xf20000  // -- 0xf207ff
#define DUART_BASE       0xffd800  // -- 0xffdfff

#define SIM_BASE         0xfffa00
#define PTIMER_VECTORNUM 64
#define CPURAMCTRL_BASE  0xfffb00
#define SCI_BASE         0xfffc00
#define QSM_VECTORNUM    65
#define TPU_BASE         0xfffe00
#define TPU_VECTORBASE   0x80 
#define I2CA_BASE	 0xffe000
#define I2CB_BASE	 0xffe800
#define I2CA_VECTORNUM	 27 // irq3
#define I2CB_VECTORNUM	 28 // irq4
#define FASTBIN_BASE	 0  // not there

#define  I2C_INITCS

#endif

#endif
