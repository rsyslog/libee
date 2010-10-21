/**
 * @file tagbucket.c
 * Implements tagbucket object methods.
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

#include "libee.h"
#include "tagbucket.h"

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
	if((tagbucket->ht = xmlHashCreate(ctx->tagBucketSize)) == NULL) {
		free(tagbucket);
		tagbucket = NULL;
		goto done;
	}

done:
	return tagbucket;
}



/* deallocator for freeing hash table content
 */
static void
deallocator(void *payload, xmlChar __attribute__((unused)) *name)
{
}

void
ee_deleteTagbucket(struct ee_tagbucket *tagbucket)
{
	assert(fieldbucket->objID == ObjID_TAGBUCKET);
	tagbucket->objID = ObjID_DELETED;
	xmlHashFree(tagbucket->ht, deallocator);
	free(tagbucket);
}


int ee_addTagToBucket(struct ee_tagbucket *tagbucket, char *tagname)
{
	int r = 0;

	assert(tagname != NULL);
	CHECK_TAGBUCKET;
	// TODO: depending on compliance level, we need
	// to check if this is a valid tagname
	/* Note: payload (third parameter) MUST NOT be NULL, otherwise we will
	 * not see this entry when scanning over the table!
	 */
	r = xmlHashAddEntry(tagbucket->ht, (xmlChar*) tagname, "");
done:
	return r;
}


/* we can pass only one pointer to libxml2, so we unfortunately
 * need to set up a structure for the two parameters we have.
 */
struct ptr_IteratoroverBucketTags {
	void (*f)(void *, char*);
	void *cookie;
};
static void IteratorOverBucketTags(void __attribute__((unused)) *payload,
				   void __attribute__((unused)) *data,
				   xmlChar *name)
{
	struct ptr_IteratoroverBucketTags *iter = (struct ptr_IteratoroverBucketTags*) data;
	iter->f(iter->cookie, (char*)name);
}


void
ee_iterateOverBucketTags(struct ee_tagbucket *tagbucket,
			 void(*f)(void*, char*), void *cookie)
{
	static struct ptr_IteratoroverBucketTags iter;
	assert(f != NULL);
	iter.f = f;
	iter.cookie = cookie;
	xmlHashScan(tagbucket->ht, IteratorOverBucketTags, &iter);
}
