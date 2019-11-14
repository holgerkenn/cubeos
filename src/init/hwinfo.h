struct duart_info {
  unsigned int base;
  unsigned char mr1a;
  unsigned char mr1b;
  unsigned char mr2a;
  unsigned char mr2b;
  unsigned char csra;
  unsigned char csrb;
  unsigned char imr;
  unsigned char ivr;
  unsigned char opcr;
  unsigned char opr;
};

struct i2c_info {
  unsigned int base;
  unsigned char ownaddr;
  unsigned char ivr;
  unsigned char csr;
  unsigned char control;
};

struct hw_info {
  unsigned int clk;          /* in Hz */
  unsigned int board_type;
  unsigned int eprom_base;
  unsigned int eprom_size;   /* in Bytes */
  unsigned int ram_base;
  unsigned int ram_size;     /* in Bytes */
  unsigned short duart_avail;
  struct duart_info duart;
  unsigned short i2ca_avail;
  struct i2c_info i2ca;
  unsigned short i2cb_avail;
  struct i2c_info i2cb;
};

extern struct hw_info hwinfo;
