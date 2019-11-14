/* Exception/Interrupt vector table -----------------------------------------*/

IVTABLE:
	.long   0       // Reset system stack location (not used).
	dc.l    SYM(start)      // Reset start address.
	dc.l    BUSSERR         // Buss error handler.
	dc.l    ADRSERR         // Address error handler.
	dc.l    ILLEGALINSTR    // Illigal instruction.
	dc.l    ZERODIVIDE      // Zero divide.
	dc.l    CHKS            // CHK, CHK2 instructions.
	dc.l    TRAPCCV         // TRAPcc, TRAPV instructions.
	dc.l    PRIVVIOLATED    // Privalage violation.
	dc.l    TRACE           // Trace.
	dc.l    UNUSEDVECT      // Line 1010 emulator.
	dc.l    UNUSEDVECT      // Line 1111 emulator.
	dc.l    HRDWAREBKPT     // Hardware breakpoint.
	dc.l    UNUSEDVECT      // Reserved (Motorola).
	dc.l    UNINITIALISED   // Format error and uninitialised interrupt.
	dc.l    UNINITIALISED   // Format error and uninitialised interrupt.
	.rept 8
	.long UNUSEDVECT    // All unused vectors.
	.endr
	dc.l    SPURIOUSINT     // Spuriouse interrupt.
	dc.l    AUTOVECT1       // Level 1 interrupt autovector.
	dc.l    AUTOVECT2       // Level 2 interrupt autovector.
	dc.l    AUTOVECT3       // Level 3 interrupt autovector.
	dc.l    AUTOVECT4       // Level 4 interrupt autovector.
	dc.l    AUTOVECT5       // Level 5 interrupt autovector.
	dc.l    AUTOVECT6       // Level 6 interrupt autovector.
	dc.l    AUTOVECT7       // Level 7 interrupt autovector.
// Trap Vectors
	dc.l	TRAP0		// Trap 0 vector
	dc.l	TRAP1		// Trap 1 vector
	dc.l	TRAP2		// Trap 2 vector
	dc.l	TRAP3		// Trap 3 vector
	dc.l	TRAP4		// Trap 4 vector
	dc.l	TRAP5		// Trap 5 vector
	dc.l	TRAP6		// Trap 6 vector
	dc.l	TRAP7		// Trap 7 vector
	dc.l	TRAP8		// Trap 8 vector
	dc.l	TRAP9		// Trap 9 vector
	dc.l	TRAP10		// Trap 10 vector
	dc.l	TRAP11		// Trap 11 vector
	dc.l	TRAP12		// Trap 12 vector
	dc.l	TRAP13		// Trap 13 vector
	dc.l	TRAP14		// Trap 14 vector
	dc.l	TRAP15		// Trap 15 vector
// Reserved vectors
	.rept 16
	.long UNUSEDVECT	
	.endr
// Vectors for devices that generate vectors during iack
	dc.l    PTIMERVEC	// Periodic Timer Vector (#64)
	.rept 191
	.long UNUSEDVECT
	.endr

BUSSERR:
	move.w  #1,sys_errorcode
	jsr     SYM(exp_handler)
ADRSERR:
	move.w  #2,sys_errorcode
	jsr     SYM(exp_handler)
ILLEGALINSTR:
	move.w  #3,sys_errorcode
	jsr     SYM(exp_handler)
ZERODIVIDE:
	move.w  #4,sys_errorcode
	jsr     SYM(exp_handler)
CHKS:
	move.w  #5,sys_errorcode
	jsr     SYM(exp_handler)
TRAPCCV:
	move.w  #6,sys_errorcode
	jsr     SYM(exp_handler)
PRIVVIOLATED:
	move.w  #7,sys_errorcode
	jsr     SYM(exp_handler)
TRACE:
	move.w  #8,sys_errorcode
	jsr     SYM(exp_handler)
UNUSEDVECT:
	move.w  #9,sys_errorcode
	jsr     SYM(exp_handler)
HRDWAREBKPT:
	move.w  #10,sys_errorcode
	jsr     SYM(exp_handler)
UNINITIALISED:
	move.w  #11,sys_errorcode
	jsr     SYM(exp_handler)
SPURIOUSINT:
	move.w  #12,sys_errorcode
	jsr     SYM(exp_handler)
AUTOVECT1:
	move.w  #13,sys_errorcode
	jsr     SYM(exp_handler)
AUTOVECT2:
	move.w  #14,sys_errorcode
	jsr     SYM(exp_handler)
AUTOVECT3:
	move.w  #15,sys_errorcode
	jsr     SYM(exp_handler)
AUTOVECT4:
	move.w  #16,sys_errorcode
	jsr     SYM(exp_handler)
AUTOVECT5:
	ori.w	IMM(0x0700),sr	// level 7 int mask
	move.l	a0,sp@-
	move.l	a1,sp@-
	move.l	d0,sp@-
	move.l	d1,sp@-
	jsr     SYM(duart_int)
	move.l	sp@+,d1
	move.l	sp@+,d0
	move.l	sp@+,a1
	move.l	sp@+,a0
	rte // rte resets sr to the old value
AUTOVECT6:
	move.w  #18,sys_errorcode
	jsr     SYM(exp_handler)
AUTOVECT7:
	move.w  #19,sys_errorcode
	jsr     SYM(exp_handler)
TRAP0:
	move.l	a0,sp@-
	move.l	a1,sp@-
	move.l	d0,sp@-
	move.l	d1,sp@-
	jsr     SYM(trap0)
	move.l	sp@+,d1
	move.l	sp@+,d0
	move.l	sp@+,a1
	move.l	sp@+,a0
	rte
TRAP1:
	move.w  #21,sys_errorcode
	jsr     SYM(exp_handler)
TRAP2:
	move.w  #22,sys_errorcode
	jsr     SYM(exp_handler)
TRAP3:
	move.w  #23,sys_errorcode
	jsr     SYM(exp_handler)
TRAP4:
	move.w  #24,sys_errorcode
	jsr     SYM(exp_handler)
TRAP5:
	move.w  #25,sys_errorcode
	jsr     SYM(exp_handler)
TRAP6:
	move.w  #26,sys_errorcode
	jsr     SYM(exp_handler)
TRAP7:
	move.w  #27,sys_errorcode
	jsr     SYM(exp_handler)
TRAP8:
	move.w  #28,sys_errorcode
	jsr     SYM(exp_handler)
TRAP9:
	move.w  #29,sys_errorcode
	jsr     SYM(exp_handler)
TRAP10:
	move.w  #30,sys_errorcode
	jsr     SYM(exp_handler)
TRAP11:
	move.w  #31,sys_errorcode
	jsr     SYM(exp_handler)
TRAP12:
	move.w  #32,sys_errorcode
	jsr     SYM(exp_handler)
TRAP13:
	move.w  #33,sys_errorcode
	jsr     SYM(exp_handler)
TRAP14:
	move.w  #34,sys_errorcode
	jsr     SYM(exp_handler)
TRAP15:
	move.w  #35,sys_errorcode
	jsr     SYM(exp_handler)
PTIMERVEC:
	ori.w	IMM(0x0700),sr	// level 7 int mask
	move.l	a0,sp@-
	move.l	a1,sp@-
	move.l	d0,sp@-
	move.l	d1,sp@-
	jsr     SYM(ptimer_int)
	cmpi	#1,d0 /* Returned 1 ? */
	bne	NO_SCHED /* No: Do not call Scheduler after rte */
	/* Stack looks now 
	SP-> d1   2 Word
	     d0   2
	     a1   2
	     a0   2
	     SR   1
	     PC   2 (of interrupted function)
	     VEC  1
	     ---- Rest is data of interrupted function ----

	Stack has to look like this:
	SP-> d1   2 Word
	     d0   2
	     a1   2
	     a0   2
	     SR   1
	     PC   2 (Adress of Scheduler)
	     VEC  1
	     PC   2 (of interrupted function)
	     ---- Rest is data of interrupted function ----
	*/
	MOVE.l	 sp,a0
	MOVE.l	 sp,a1
	adda     #(-4),a0
	MOVE.l   sp@+,a0@+
	MOVE.l   sp@+,a0@+
	MOVE.l   sp@+,a0@+
	MOVE.l   sp@+,a0@+
	MOVE.l   sp@+,a0@+
	MOVE.l   sp@+,a0@+
			 	/* Now everything is moved and SP is */
				/* pointing to the data of the interrupted function */
	MOVE.l   a1@(14),sp@-	/* Pushing the return adress */
	MOVE.l	 IMM(SCHEDWRAP),a1@(14) /* And replacing the pc with schedule() */
	adda     #(-4),a1	
	MOVE.l   a1,sp     	/* Moving SP to the end of the stack */    
	
NO_SCHED:
	move.l	sp@+,d1
	move.l	sp@+,d0
	move.l	sp@+,a1
	move.l	sp@+,a0
	rte // rte resets sr to the old value


SCHEDWRAP:
	move.w  sr,sp@-
	move.l	a0,sp@-
	move.l	a1,sp@-
	move.l	d0,sp@-
	move.l	d1,sp@-
	jsr     SYM(schedule)
	move.l	sp@+,d1
	move.l	sp@+,d0
	move.l	sp@+,a1
	move.l	sp@+,a0
	move.w  sp@+,sr
	rts 
