#include <asm.h>

        .text

        .extern SYM(i2c_int_handler)
        .global SYM(i2ca_int)

i2ca_int:
        ori.w   IMM(0x0700),sr  // level 7 int mask
        move.l  a0,sp@-
        move.l  a1,sp@-
        move.l  d0,sp@-
        move.l  d1,sp@-
        move.l  IMM(0),sp@-
        jsr     SYM(i2c_int_handler)
        addql   IMM(4),sp
        move.l  sp@+,d1
        move.l  sp@+,d0
        move.l  sp@+,a1
        move.l  sp@+,a0
        rte
