/*
 * Copyright (c) 2018, Joyent, Inc.
 */
#include "llist.h"

void
llist_prepend(llist_t *list, void *data)
{
        llist_t *head, *tail;

        assert(list != NULL);
        head = list->ll_next;
        tail = list->ll_prev;

        if (head == NULL) {
                /* empty list */
                list->ll_next = list->ll_prev = (llist_t *)data;
        } else {
                ((llist_t *)data)->ll_next = head;
                head->ll_prev = (llist_t *)data;
                list->ll_next = (llist_t *)data;
        }
}

void
llist_append(llist_t *list, void *data)
{
        llist_t *head, *tail;

        assert(list != NULL);
        head = list->ll_next;
        tail = list->ll_prev;

        if (head == NULL) {
                /* empty list */
                list->ll_next = list->ll_prev = (llist_t *)data;
        } else {
                ((llist_t *)data)->ll_prev = tail;
                tail->ll_next = (llist_t *)data;
                list->ll_prev = (llist_t *)data;
        }
}

int
llist_walker(llist_t *list, int (*cbfunc)(llist_t *, void *), void *arg)
{
	llist_t *node;
	int ret;

	assert(list != NULL);
	node = list->ll_next;
	while (node != NULL) {
		ret = cbfunc(node, arg);
		if (ret == LL_WALK_DONE) {
			break;
		} else if (ret == LL_WALK_ERR) {
			return (-1);
		} else {
			node = node->ll_next;
		}
	}
	return (0);
}

int
llist_remove(llist_t *list, void *data)
{
        return (0);
}

void
llist_destroy(llist_t *list, int (*cbfunc)(llist_t *, void *), void *arg)
{
	llist_t *node, *victim;

	assert(list != NULL);
	node = list->ll_next;
	while (node != NULL) {
		/* destroy node */
		victim = node;
		node = node->ll_next;
		(void) cbfunc(victim, arg);
	}
}

