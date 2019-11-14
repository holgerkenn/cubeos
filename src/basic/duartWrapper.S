#include <asm.h>

        .text

        .extern SYM(duart_int)
        .global SYM(duartWrapper)

duartWrapper:
        ori.w   IMM(0x0700),sr  // level 7 int mask
        move.l  a0,sp@-
        move.l  a1,sp@-
        move.l  d0,sp@-
        move.l  d1,sp@-
        jsr     SYM(duart_int)
        move.l  sp@+,d1
        move.l  sp@+,d0
        move.l  sp@+,a1
        move.l  sp@+,a0
        rte

