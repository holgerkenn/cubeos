#include <stddef.h>
//#include <assert.h>
#include <list.h>
#include "listtest.h"
#include "testmak.h"
int ListTest ()
{
	int i;
	list li;
	list *l;
	entry en[100];

	l = &li;
	list_init (l);
/* now we have a list */

	for (i = 0; i < 100; i++) {
		en[i].data = (void *) i;
		en[i].list = NULL;
	}

	for (i = 0; i < 100; i++)
		insert_head (l, &en[i]);

	checkif (entries (l) == 100, "lost an entry???");

	for (i = 99; i > 0; i--) {
		checkif (i == (int) (head (&li)->data), "malformed data");
		delete (head (l));
	}

	checkif (entries (l) == 1, "wrong number of entries");
	checkif (head (l) == &en[0], "unexpected head");

	delete (head (l));
	checkif (head (l) == NULL, "list not empty");	/* list is now empty */
	checkif (entries (l) == 0, "counter out of sync");

	delete (head (l));	/* delete something */
	delete (&en[0]);	/* delete something else */
	insert_before (&en[0], NULL);
	insert_after (&en[0], NULL);
	checkif (tail (l) == NULL, "list not empty");	/* list is still empty */

	for (i = 0; i < 100; i++)
		insert_tail (l, &en[i]);
	checkif (entries (l) == 100, "wrong number of entries");

	for (i = 0; i < 100; i++) {
		checkif (i == (int) (head (&li)->data), "malformed data");
		delete (head (l));
	}
	checkif (entries (l) == 0, "list not empty");

	for (i = 0; i < 100; i++)
		checkif (en[i].list == NULL, "entry still in list");

	for (i = 0; i < 99; i++)
		insert_tail (l, &en[i]);
	checkif (entries (l) == 99, "wrong number of entries");

	insert_before (&en[99], &en[98]);
	checkif (tail (l) == &en[98], "wrong tail");
	checkif (entries (l) == 100, "wrong mumber of entries");

	delete (tail (l));
	checkif (entries (l) == 99, "wrong number of entries");
	checkif (tail (l) == &en[99], "wrong tail");

	insert_after (&en[98], &en[0]);
	checkif (head (l) == &en[0], "wrong head");
	checkif (entries (l) == 100, "wrong number of entries");

	delete (head (l));
	checkif (head (l) == &en[98], "wrong head");
	checkif (entries (l) == 99, "wrong number of entries");

	for (i = 99; i > 0; i--) {
		checkif (i == entries (l), "wrong number of entries");
		delete (head (l));
	}

	printf ("Passed\n");
	return (0);
}
