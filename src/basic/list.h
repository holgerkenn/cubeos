/* basic list handling */

#ifndef _LIST_H
#define _LIST_H
typedef struct list_s list;
typedef struct entry_s entry;
struct entry_s {
list * list;
entry * prev;
entry * next;
void * data;
int len;
} ;

struct list_s
{
entry * head;
entry * tail;
int entries;
};

entry *head(list *l);
entry *tail(list *l);
int entries(list *l);
void list_init(list * l);
void insert_after(entry * e,entry *x);
void insert_before(entry * e,entry *x);
void insert_head(list *l,entry *e);
void insert_tail(list *l,entry *e);
void delete(entry *e);

#endif
