/* Writable Flags in Control register */

#define ACK     0x01    /* Acknowledge cycle */
#define STO     0x02    /* Stop condition */
#define STA     0x04    /* Start condition */
#define ENI     0x08    /* Enable interrupt */
#define ESO     0x40    /* Enable i2c bus interface */

/* Readable Flags in Control Register */

#define BB      0x01    /* i2c bus busy */
#define LAB     0x02    /* lost arbitration */
#define AAS     0x04    /* Addressed as slave */
#define LRB     0x08    /* last received bit */
#define BER     0x10    /* bus error */
#define STS     0x20    /* stop condition detected */
#define PIN     0x80    /* pending interrupt (active low) */

struct i2c_device {
  unsigned char	data;
  unsigned char	control;
};
