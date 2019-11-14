/* tpu.h Motorola TPU register offset */
/* global_def.h has to be included before this ! */
#ifndef TPU_H

#define TPU_H

#ifndef GLOBAL_DEF_H
#error global_def.h not included !!!!
#endif

#define TPU_ADDR(nr)  (TPU_BASE + nr)

#define TPU_TPUMCR	TPU_ADDR(0x0)  /* TPU Module Configuration R. */
#define TPU_TICR	TPU_ADDR(0x08) /* TPU Interrupt Configuration R. */
#define TPU_CIER	TPU_ADDR(0x0a) /* Channel Interrupt Enable R. */
#define TPU_CFSR	TPU_ADDR(0x0c) /* Channel Function Select Space*/
#define TPU_CFSR0	TPU_ADDR(0x0c) /* Channel Function Select R. 0 */
#define TPU_CFSR1	TPU_ADDR(0x0e) /* Channel Function Select R. 1 */
#define TPU_CFSR2	TPU_ADDR(0x10) /* Channel Function Select R. 2 */
#define TPU_CFSR3	TPU_ADDR(0x12) /* Channel Function Select R. 3 */
#define TPU_HSQR	TPU_ADDR(0x14) /* Host Sequence Space */
#define TPU_HSQR0	TPU_ADDR(0x14) /* Host Sequence R. 0 */
#define TPU_HSQR1	TPU_ADDR(0x16) /* Host Sequence R. 1 */
#define TPU_HSRR	TPU_ADDR(0x18) /* Host Service Request Space */
#define TPU_HSRR0	TPU_ADDR(0x18) /* Host Service Request R. 0 */
#define TPU_HSRR1	TPU_ADDR(0x1a) /* Host Service Request R. 1 */
#define TPU_CPR		TPU_ADDR(0x1c) /* Channel Priority Space */
#define TPU_CPR0	TPU_ADDR(0x1c) /* Channel Priority R. 0 */
#define TPU_CPR1	TPU_ADDR(0x1e) /* Channel Priority R. 1 */
#define TPU_CISR	TPU_ADDR(0x20) /* Channel Interrupt Status R. */
#define TPU_PARAMETER   TPU_ADDR(0x100) /* TPU Parameter Space */


#define setTPUMCR(val)	writeshort(TPU_TPUMCR,val)
#define setTICR(val)	writeshort(TPU_TICR,val)
#define setCIER(ch,val) writeshortpos(TPU_CIER,val,0x1,ch)
#define setCFSR(ch,val)	writeshortpos(TPU_CFSR + ((3-(ch/4))<<1),val,0xf,(ch&0x3)<<2)
#define setHSQR(ch,val) writeshortpos(TPU_HSQR+((1-(ch/8))<<1),val,0x3,(ch&0x7)<<1)
#define setHSRR(ch,val) writeshortpos(TPU_HSRR+((1-(ch/8))<<1),val,0x3,(ch&0x7)<<1)
#define setCPR(ch,val)  writeshortpos(TPU_CPR+((1-(ch/8))<<1),val,0x3,(ch&0x7)<<1)
#define clearCISR(ch)	writeshortpos(TPU_CISR,0x0,0x1,ch)
#define setPAR(ch,nr,val) writeshort(TPU_PARAMETER+(ch*16)+(nr<<1),val)

#define getTPUMCR()	readshort(TPU_TPUMCR)
#define getTICR()	readshort(TPU_TICR)
#define getCIER(ch)	readshortpos(TPU_CIER,0x1,ch)
#define getCFSR(ch)	readshortpos(TPU_CFSR + ((3-(ch/4))<<1),0xf,(ch&0x3)<<2)
#define getHSQR(ch)	readshortpos(TPU_HSQR+((1-(ch/8))<<1),0x3,(ch&0x7)<<1)
#define getHSRR(ch)	readshortpos(TPU_HSRR+((1-(ch/8))<<1),0x3,(ch&0x7)<<1)
#define getCPR(ch)	readshortpos(TPU_CPR+((1-(ch/8))<<1),0x3,(ch&0x7)<<1)
#define getCISR(ch)	readshortpos(TPU_CISR,0x1,ch)
#define getPAR(ch,nr)	readshort(TPU_PARAMETER+(ch*16)+(nr<<1))

#define TPU_FKT_PPWA 0xf
#define TPU_FKT_OC   0xe
#define TPU_FKT_SM   0xd
#define TPU_FKT_PSP  0xc
#define TPU_FKT_PMA  0xb
#define TPU_FKT_ITC  0xa
#define TPU_FKT_PWM  0x9
#define TPU_FKT_DIO  0x8
#define TPU_FKT_SPWM 0x7
#define TPU_FKT_QDEC 0x6

#define TPU_QDEC_EDGE_TIME 0
#define TPU_QDEC_POS_COUNT 1
#define TPU_QDEC_TCR1_VAL 2
#define TPU_QDEC_CHAN_PINSTATE 3
#define TPU_QDEC_CORR_PINSTATE_ADDR 4
#define TPU_QDEC_EDGE_TIME_LSB_ADDR 5

#endif
