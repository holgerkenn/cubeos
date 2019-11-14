#define I2C_MASTER 0
#define I2C_SLAVE  1

#define I2CA 0x41
#define I2CB 0x42

#define I2C_TIMEOUT_VAL 1000
#define I2CA_SLAVE_ADDR 0x55
#define I2CB_SLAVE_ADDR 0x56
#define I2C_SPEED 0x1c

#define I2C_MASTER_MODE 0
#define I2C_SLAVE_MODE  1

/* States in Master Mode */

#define MASTER_IDLE              0
#define MASTER_SEND              1
#define MASTER_WAIT_ACK          2
#define MASTER_RECV              3
#define MASTER_RECV_LAST         4

/* States in Slave Mode */

#define SLAVE_IDLE           0
#define SLAVE_SEND           1
#define SLAVE_RECV           2


#define	MBUFLEN 10		/* num.	mess. in master queue   */
#define	SBUFLEN 10		/* num.	char in slave buffer    */

/* Mnemonics for the return messages of the driver */

#define I2C_NOT_PROCESSED       0        /* so far unprocessed messages    */
#define I2C_OK                  1        /* transfer ended No Errors       */
#define I2C_BUSY                2        /* transfer busy                  */
#define I2C_ERR                 3        /* err: general error             */
#define I2C_NACK_ON_DATA        4        /* err: No ack on data            */
#define I2C_NACK_ON_ADDRESS     5        /* err: No ack on address         */
#define I2C_ARBITRATION_LOST    6        /* err: Arbitration lost          */
#define I2C_TIME_OUT            7        /* err: Time out occurred         */

/* Flags for execeptions in slave handler */

#define I2C_SLAVE_OBUF_EMPTY         1
#define I2C_SLAVE_IBUF_OVERFLOW      2
#define I2C_SLAVE_ERR                4

struct i2cmess {
  unsigned char address;     /* slave address to sent/receive message */
  unsigned char nrBytes;     /* number of bytes in message buffer     */
  unsigned char procBytes;   /* number of processed bytes             */
  unsigned char *buf;        /* pointer to application message buffer */
  unsigned char status;      /* process status of the message         */
};

/* i2c bus control block */

struct	i2c {
  struct i2c_device *ioaddr;		   /* device address of this unit  */
  int  mode;				   /* selector for master/slave    */
  int  m_head;			           /* head of master queue	   */
  int  m_tail;			           /* tail of master queue         */
  struct i2cmess* m_buff[MBUFLEN];	   /* master buffer                */
  int  m_sem;				   /* master semaphore     	   */
  int  m_cnt;			           /* count of jobs in buf         */
  int  m_dcnt;                             /* currently sent bytes of msg. */
  void (*m_state_handler)(struct i2c *ptr);/* routine called in master mode*/
  int  m_state;                            /* state of handler             */
  int  s_iptr;	                           /* head of slave inqueue	   */
  int  s_icnt;                             /* count of char in inqueue     */
  int  s_isem;                             /* slave input semaphore	   */
  char s_ibuff[SBUFLEN];                   /* slave input buffer           */
  int  s_optr;                             /* head of slave outqueue	   */
  int  s_ocnt;				   /* count of char in outqueue    */
  int  s_osem;				   /* slave output semaphore	   */
  char s_obuff[SBUFLEN];                   /* slave output buffer          */
  void (*s_state_handler)(struct i2c *ptr);/* routine called in slave mode */
  int  s_state;                            /* state of handler             */
  char s_status;                           /* exception flags              */
};

/* Interface functions */

void i2c_int_handler(unsigned long minor);
void i2c_master_handler(struct i2c *p);
void i2c_slave_handler(struct i2c *p);
void i2c_init(int which, unsigned long ioaddr);
int process_i2c(unsigned char whichBus, unsigned char whichQueue, struct i2cmess *msg);
int i2c_messagestatus(struct i2cmess * msg);
int i2c_scanbus(unsigned char whichBus);
extern struct i2c i2c[];

/* externally defined assembler functions */

void i2ca_int(void);
void i2cb_int(void);
