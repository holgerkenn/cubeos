/* Radio State Machine */

#ifndef _RSM_H
#define _RSM_H
#define RSM_FRAMELEN 0x48

void process_rsm(unsigned char c);
int rsm_get_frame(unsigned char * buffer);
int rsm_send_frame(unsigned char * buffer);
int init_rsm_rx();
int init_rsm_tx();
int deinit_rsm_rx();
int deinit_rsm_tx();

extern int rsm_good;
extern int rsm_bad;
extern int rsm_ugly;
extern int rsm_bytes;

extern int rsm_uglycount;
extern unsigned char rsm_enabled;

#endif
