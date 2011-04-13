/**
 * @file fieldbucket.c
 * Implements fieldbucket object methods.
 *//* Libee - An Event Expression Library inspired by CEE
 * Copyright 2010 by Rainer Gerhards and Adiscon GmbH.
 *
 * This file is part of libee.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * A copy of the LGPL v2.1 can be found in the file "COPYING" in this distribution.
 */
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include "libee/libee.h"
#include "libee/internal.h"

#define ERR_ABORT {r = 1; goto done; }

#define CHECK_FIELD \
	if(fieldbucket->objID != ObjID_FIELDBUCKET) { \
		r = -1; \
		goto done; \
	}


struct ee_fieldbucket*
ee_newFieldbucket(ee_ctx ctx)
{
	struct ee_fieldbucket *fieldbucket;
	if((fieldbucket = malloc(sizeof(struct ee_fieldbucket))) == NULL)
		goto done;

	fieldbucket->objID = ObjID_FIELDBUCKET;
	fieldbucket->ctx = ctx;
	fieldbucket->root = fieldbucket->tail = NULL;

done:	return fieldbucket;
}


void
ee_deleteFieldbucket(struct ee_fieldbucket *fieldbucket)
{
	struct ee_fieldbucket_listnode *node, *nodeDel;

	assert(fieldbucket->objID == ObjID_FIELDBUCKET);
	fieldbucket->objID = ObjID_DELETED;
	for(node = fieldbucket->root ; node != NULL ; ) {
		nodeDel = node;
		node = node->next;
		ee_deleteField(nodeDel->field);
		free(nodeDel);
	}
	free(fieldbucket);
}


/* TODO: when in validating mode, check duplicate field entries */
int
ee_addFieldToBucket(struct ee_fieldbucket *fieldb, struct ee_field *field)
{
	int r;
	struct ee_fieldbucket_listnode *node;
	assert(fieldb != NULL);assert(fieldb->objID == ObjID_FIELDBUCKET);
	assert(field != NULL);assert(field->objID == ObjID_FIELD);

	CHKN(node = malloc(sizeof(struct ee_fieldbucket_listnode)));
	node->field = field;
	node->next = NULL;
	if(fieldb->root == NULL) {
		fieldb->root = fieldb->tail = node;
	} else {
		fieldb->tail->next = node;
		fieldb->tail = node;
	}
	r = 0;

done:	return r;
}


/* Note: this function currently is quite performance-hungry. We should
 * replace all this searching by hashtable access some time in the future,
 * but for now the focus is on getting things done in a simple fashion, and
 * so the simple list search approach is good enough. But if you try to
 * optimize, do not optimize the list search but rather introduce the hash
 * table as second indexing structure! -- rgerhards, 2010-12-01
 */
struct ee_field*
ee_getBucketField(struct ee_fieldbucket *bucket, es_str_t *name)
{
	struct ee_fieldbucket_listnode *node;

	for(node = bucket->root ; node != NULL ; node = node->next) {
		if(!es_strcmp(name, node->field->name))
			break;
	}

	return((node == NULL) ? NULL : node->field);
}
