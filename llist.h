/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2018, Joyent, Inc.
 */
#ifndef _LLIST_H
#define	_LLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct llist {
	struct llist *ll_prev;
	struct llist *ll_next;
} llist_t;

#define	LL_WALK_NEXT	1
#define	LL_WALK_ERR	2
#define	LL_WALK_DONE	3

void llist_prepend(llist_t *, void *);
void llist_append(llist_t *, void *);
int llist_walker(llist_t *, int (*)(llist_t *, void *), void *);
void llist_destroy(llist_t *, int (*)(llist_t *, void *), void *);

#endif
