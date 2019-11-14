#include "asm.h"
#include "global_def.h"

/* This is the context switch code */
/* Stolen from xinu */


	.extern SYM(context_srsave)

/* void contextsw(void * oldarea,void * newarea) */
        .text
	.align 2
	.global SYM (contextsw)
SYM (contextsw):
        move.l    a0,     sp@-           // Save A0 onto old stack
        move.l    sp@(8),a0              // Move address of old area into A0 
        movem.l   #0xffff,a0@            // Save all registers
        move.l    sp@,   a0@(32)         // Put original A0 in old savearea
        addq.l    #8,     a0@(60)        // Move SP beyond return address
                                         //       as if a return has occured
        add.l     #64,    a0             // Skip past registers d0-7,a0-7
        move.w    #0,     a0@+           // Pad SR savearea, since SR is a word
        move.w    context_srsave,a0@+    // Save SR in old savearea
        move.l    sp@(4), a0@+           // Save PC in old savearea


//Save image:d0,d1,d2,d3,d4,d5,d6,d7,a0,a1,a2,a3,a4,a5,a6,ssp,0x00, sr, pc
//            0  4  8 12 16 20 24 28 32 36 40 44 48 52 56  60  64   66  68

        move.l    sp@(12),a0              // Move address of new area into A0
        movem.l   a0@,#0x7fff             // Restore all regs (even A0) except SP
        move.l    sp@(12), a0             // Move address of new area into A0 again
        move.l    a0@(60),sp              // Put  SSP into kernel stack
        move.l    a0@(68),sp@-            // Move PC onto current stack
        move.w    a0@(66),sr              // Restore the status register

        move.l    a0@(32),a0              // Restore A0 from new area
        rts                               // since we pushed the pc onto the stack,
                                          //   we just pretend to return
	.data

