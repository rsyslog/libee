/**
 * @file tagbucket.c
 * Implements tagbucket object methods.
 *//* Libee - An Event Expression Library inspired by CEE
 * Copyright 2010-2011 by Rainer Gerhards and Adiscon GmbH.
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

#define CHECK_TAGBUCKET \
	if(tagbucket->objID != ObjID_TAGBUCKET) { \
		r = -1; \
		goto done; \
	}


struct ee_tagbucket*
ee_newTagbucket(ee_ctx ctx)
{
	struct ee_tagbucket *tagbucket;
	if((tagbucket = malloc(sizeof(struct ee_tagbucket))) == NULL)
		goto done;

	tagbucket->objID = ObjID_TAGBUCKET;
	tagbucket->ctx = ctx;
	tagbucket->root = tagbucket->tail = NULL;
	tagbucket->refCount = 1;

done:	return tagbucket;
}


/* Add an additional reference to the tag bucket.
 * TODO: atomic instructions for ref counting! -- rgerhards, 2011-04-06
 */
struct ee_tagbucket*
ee_addRefTagbucket(struct ee_tagbucket *tagbucket)
{
	assert(tagbucket->objID == ObjID_TAGBUCKET);
	tagbucket->refCount++;
	return tagbucket;
}


void
ee_deleteTagbucket(struct ee_tagbucket *tagbucket)
{
	int currRefCount;

	// TODO: use atomic instructions for reference counting!
	assert(tagbucket->objID == ObjID_TAGBUCKET);
	currRefCount = tagbucket->refCount--;
	if(currRefCount == 0) {
		tagbucket->objID = ObjID_DELETED;
		// TODO: free list (memleak)
		free(tagbucket);
	}
}


/* TODO: when in validating mode, check duplicate field entries */
int
ee_addTagToBucket(struct ee_tagbucket *tagbucket, es_str_t *tagname)
{
	int r;
	struct ee_tagbucket_listnode *node;
	assert(tagbucket != NULL);assert(tagbucket->objID == ObjID_TAGBUCKET);

	CHKN(node = malloc(sizeof(struct ee_tagbucket_listnode)));
	node->name = tagname;
	node->next = NULL;
	if(tagbucket->root == NULL) {
		tagbucket->root = tagbucket->tail = node;
	} else {
		tagbucket->tail->next = node;
		tagbucket->tail = node;
	}
	r = 0;

done:	return r;
}


int
ee_TagbucketGetNextTag(struct ee_tagbucket *tagbucket, void **cookie, es_str_t **tagname)
{
	struct ee_tagbucket_listnode **cookieIn;
	struct ee_tagbucket_listnode *tag;
	
	cookieIn = (struct ee_tagbucket_listnode **) cookie;
	if(*cookieIn == NULL) {
		tag = tagbucket->root;
	} else {
		tag = (*cookieIn)->next;
	}

	if(tag != NULL) {
		*tagname = tag->name;
	}
	*cookie = tag;

	return 0; /* right now, no errors can occur (but may in the future!) */
}


int
ee_TagbucketHasTag(struct ee_tagbucket *tagbucket, es_str_t *tagname)
{
	int r = 0;
	struct ee_tagbucket_listnode *tag;

	for(tag = tagbucket->root ; tag != NULL ; tag = tag->next) {
		if(!es_strcmp(tag->name, tagname)) {
			r = 1;
			goto done;
		}
	}

done:	return r;
}
