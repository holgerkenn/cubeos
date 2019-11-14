#ifndef KERROR_H
#define KERROR_H

#define ERR_PANIC   7
#define ERR_EMERG   6
#define ERR_ALERT   5
#define ERR_WARN    4
#define ERR_NOTICE  3
#define ERR_LOG     2
#define ERR_DEBUG   1


#define ERR_NO_MORE_PROCESSES 1


void complain(int i,char * text);

#endif
