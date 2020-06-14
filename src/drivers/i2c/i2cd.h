#define I2C_LEADER 0
#define I2C_FOLLOWER  1

#define I2CA 0x41
#define I2CB 0x42

#define I2C_TIMEOUT_VAL 1000
#define I2CA_FOLLOWER_ADDR 0x55
#define I2CB_FOLLOWER_ADDR 0x56
#define I2C_SPEED 0x1c

#define I2C_LEADER_MODE 0
#define I2C_FOLLOWER_MODE  1

/* States in LEADER Mode */

#define LEADER_IDLE              0
#define LEADER_SEND              1
#define LEADER_WAIT_ACK          2
#define LEADER_RECV              3
#define LEADER_RECV_LAST         4

/* States in FOLLOWER Mode */

#define FOLLOWER_IDLE           0
#define FOLLOWER_SEND           1
#define FOLLOWER_RECV           2


#define	MBUFLEN 10		/* num.	mess. in LEADER queue   */
#define	SBUFLEN 10		/* num.	char in FOLLOWER buffer    */

/* Mnemonics for the return messages of the driver */

#define I2C_NOT_PROCESSED       0        /* so far unprocessed messages    */
#define I2C_OK                  1        /* transfer ended No Errors       */
#define I2C_BUSY                2        /* transfer busy                  */
#define I2C_ERR                 3        /* err: general error             */
#define I2C_NACK_ON_DATA        4        /* err: No ack on data            */
#define I2C_NACK_ON_ADDRESS     5        /* err: No ack on address         */
#define I2C_ARBITRATION_LOST    6        /* err: Arbitration lost          */
#define I2C_TIME_OUT            7        /* err: Time out occurred         */

/* Flags for execeptions in FOLLOWER handler */

#define I2C_FOLLOWER_OBUF_EMPTY         1
#define I2C_FOLLOWER_IBUF_OVERFLOW      2
#define I2C_FOLLOWER_ERR                4

struct i2cmess {
  unsigned char address;     /* FOLLOWER address to sent/receive message */
  unsigned char nrBytes;     /* number of bytes in message buffer     */
  unsigned char procBytes;   /* number of processed bytes             */
  unsigned char *buf;        /* pointer to application message buffer */
  unsigned char status;      /* process status of the message         */
};

/* i2c bus control block */

struct	i2c {
  struct i2c_device *ioaddr;		   /* device address of this unit  */
  int  mode;				   /* selector for LEADER/FOLLOWER    */
  int  m_head;			           /* head of LEADER queue	   */
  int  m_tail;			           /* tail of LEADER queue         */
  struct i2cmess* m_buff[MBUFLEN];	   /* LEADER buffer                */
  int  m_sem;				   /* LEADER semaphore     	   */
  int  m_cnt;			           /* count of jobs in buf         */
  int  m_dcnt;                             /* currently sent bytes of msg. */
  void (*m_state_handler)(struct i2c *ptr);/* routine called in LEADER mode*/
  int  m_state;                            /* state of handler             */
  int  s_iptr;	                           /* head of FOLLOWER inqueue	   */
  int  s_icnt;                             /* count of char in inqueue     */
  int  s_isem;                             /* FOLLOWER input semaphore	   */
  char s_ibuff[SBUFLEN];                   /* FOLLOWER input buffer           */
  int  s_optr;                             /* head of FOLLOWER outqueue	   */
  int  s_ocnt;				   /* count of char in outqueue    */
  int  s_osem;				   /* FOLLOWER output semaphore	   */
  char s_obuff[SBUFLEN];                   /* FOLLOWER output buffer          */
  void (*s_state_handler)(struct i2c *ptr);/* routine called in FOLLOWER mode */
  int  s_state;                            /* state of handler             */
  char s_status;                           /* exception flags              */
};

/* Interface functions */

void i2c_int_handler(unsigned long minor);
void i2c_LEADER_handler(struct i2c *p);
void i2c_FOLLOWER_handler(struct i2c *p);
void i2c_init(int which, unsigned long ioaddr);
int process_i2c(unsigned char whichBus, unsigned char whichQueue, struct i2cmess *msg);
int i2c_messagestatus(struct i2cmess * msg);
int i2c_scanbus(unsigned char whichBus);
extern struct i2c i2c[];

/* externally defined assembler functions */

void i2ca_int(void);
void i2cb_int(void);
