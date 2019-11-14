#include <stddef.h>
#include <list.h>

entry *
  head (list * l)
{
	if (l == NULL)
		return NULL;
	return l->head;
}

entry *
  tail (list * l)
{
	if (l == NULL)
		return NULL;
	return l->tail;
}

void list_init (list * l)
{
	if (l == NULL)
		return;
	l->head = NULL;
	l->tail = NULL;
	l->entries = 0;
}

int entries (list * l)
{
	if (l == NULL)
		return -1;
	return l->entries;
}

void insert_after (entry * e, entry * x)
{
	if (e == NULL)
		return;
	if (e->list)
		return;
	if (x == NULL)
		return;
	if (x->list == NULL)
		return;
	e->prev = x;
	e->next = x->next;
	x->next = e;
	if (x->list->tail == x)
		x->list->tail = e;
	e->list = x->list;
	e->list->entries++;
}

void insert_before (entry * e, entry * x)
{
	if (e == NULL)
		return;
	if (e->list)
		return;
	if (x == NULL)
		return;
	if (x->list == NULL)
		return;
	e->prev = x->prev;
	e->next = x;
	x->prev = e;
	if (x->list->head == x)
		x->list->head = e;
	e->list = x->list;
	e->list->entries++;
}

void insert_head (list * l, entry * e)
{
	if (l == NULL)
		return;
	if (e == NULL)
		return;
	if (e->list)
		return;
	if (l->head == NULL) {
		l->head = e;
		l->tail = e;
		e->prev = NULL;
		e->next = NULL;
		e->list = l;
		l->entries++;
	} else
		insert_before (e, l->head);
}

void insert_tail (list * l, entry * e)
{
	if (l == NULL)
		return;
	if (e == NULL)
		return;
	if (e->list)
		return;
	if (l->tail == NULL) {
		l->head = e;
		l->tail = e;
		e->prev = NULL;
		e->next = NULL;
		e->list = l;
		l->entries++;
	} else
		insert_after (e, l->tail);
}


void delete (entry * e)
{
	if (e == NULL)
		return;
	if (e->list == NULL)
		return;
	if (e->prev)
		e->prev->next = e->next;
	if (e->next)
		e->next->prev = e->prev;
	if (e == e->list->tail)
		e->list->tail = e->prev;
	if (e == e->list->head)
		e->list->head = e->next;
	e->list->entries--;
	e->list = NULL;
}
