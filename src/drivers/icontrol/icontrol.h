#ifndef _ICONTROL_H
#define _ICONTROL_H

struct position {
int xpos;
int ypos;
int angle;
};


#define LEFT_PWM 0  
#define LEFT_QD1 1
#define LEFT_QD2 12 /* TP Ext */
#define LEFT_D0 2  /* bit 2 DUART */
#define LEFT_D1 3  /* bit 3 DUART */


#define RIGHT_PWM 4
#define RIGHT_QD1 5
#define RIGHT_QD2 13 /* TP Ext */
#define RIGHT_D0 6  /* bit 6 DUART */
#define RIGHT_D1 7  /* bit 7 DUART */

#define KICK_PWM 2  
#define KICK_D0 4  /* bit 4 DUART */
#define KICK_D1 5  /* bit 5 DUART */



#define COUNTSPERREV 16
#define GEARRATIO (113/196)
#define WHEELDIA 42 /* in mm */
#define WHEELDIST 112 /* in mm */

#define PULSEFREQ 100

extern struct position pos;
extern int isrcount;

int init_icontrol ();
void do_pfield (void * field,int xpos, int ypos, int *newleft, int *newright);
void icontrol_int ();
void icontrol_set(int newleft, int newright);

#endif

