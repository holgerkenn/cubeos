/*
 * crt0.S -- startup file for m68k-coff
 */

#include "asm.h"
#include "global_def.h"
#include "mc68332.h"
#include "mc68681.h"
#include "taskconst.h" /* for TASK_INIT_SR */

	.title "crt0.S for m68k-coff"

/*
 * Define an empty environment.
 */
        .bss
        .align 4
SYM (environ):
        .long 0

	.text

/*
 * set up address where the kernel jumps to
 */

	.extern SYM(exp_handler)
	.extern SYM(duart_int)
	.extern SYM(init_tty)
	.extern SYM(init_scheduler)
	.extern SYM(init_ptimer)
	.extern SYM(schedule)

#include "ivtable.S"

/*
 * These symbols are defined in C code, so they need to always be
 * named with SYM because of the difference between object file formats.
 */

/* These are defined in C code. */
        .extern SYM(main)
        .extern SYM(exit)
        .extern SYM(atexit)
        .extern SYM(__do_global_dtors)
	.extern SYM(Blocking_Serial_Out)

/*
 * set things up so the application will run. This *must* be called start.
 */

vbr_save:	
	.long  0
stack_save:	
	.long  0
frame_save:	
	.long  0
		
start:
/*	move.w	IMM(0x5c7e),SIM_CSOR0
	move.w	IMM(0x3c7e),SIM_CSOR1 */
	movec	vbr,d0
	movel	d0,vbr_save
	movel	fp,frame_save
	move.l	IMM(IVTABLE),d0   //
	movec	d0,vbr	          // set vector base register

/* set up initial stack frame */
	movel	sp,stack_save
      	movel	IMM(__stack), sp
	link	fp, IMM(0)

/*
 * zero out the bss section.
 */
        move.l  IMM(__bss_start),a0
        cmpa.l  IMM(_end),a0
        jbeq    3f
2:
        clr.l	(a0)+
        cmpa.l  IMM(_end),a0
        jmi     2b
3:

/*
 * call the main routine from the boot monitor to get it going.
 * main (argc, argv, environ)
 * we pass argv as a pointer to NULL.
 */

#ifdef ADD_DTORS
/* put __do_global_dtors in the atexit list so the destructors get run */
        movel   IMM (SYM(__do_global_dtors)),sp@-
        jsr     SYM (atexit)
	addql	IMM(4),sp
#endif
        movel   IMM (__FINI_SECTION__),sp@-
        jsr     SYM (atexit)
	addql	IMM(4),sp

        jsr     __INIT_SECTION__

	moveb	IMM(1),Blocking_Serial_Out
	jsr	init_tty
	jsr	init_scheduler
	jsr	init_ptimer  
	move.w	IMM(TASK_INIT_SR),sr	// as defined in cubeos.h
        pea     0
        pea     SYM (environ)
        pea     sp@(4)
        pea     0
	jsr	SYM (main)
	addl	IMM(10),sp
/*
 * drop down into exit incase the user doesn't. This should drop
 * control back to the ROM monitor, if there is one. This calls the
 * exit() from the C library so the C++ tables get cleaned up right.
 */
	movel	d0, sp@-    // return value from main
        jsr     _exit
	addql	IMM(4),sp
	moveb	IMM(1),Blocking_Serial_Out
	moveb	IMM(0x20),sp@-
	jsr	SYM(outchar)  // wait until all output from main is done
	addql	IMM(4),sp
	movel	(vbr_save),d0
	movec	d0,vbr
	unlk	fp
	movel	(stack_save),sp
	rts

