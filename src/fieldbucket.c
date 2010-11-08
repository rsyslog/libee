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
	if((fieldbucket->ht = xmlHashCreate(ctx->fieldBucketSize)) == NULL) {
		free(fieldbucket);
		fieldbucket = NULL;
		goto done;
	}

done:
	return fieldbucket;
}



/* deallocator for freeing hash table content
 */
static void
deallocator(void *payload, xmlChar __attribute__((unused)) *name)
{
	ee_deleteField((struct ee_field*) payload);
}


void
ee_deleteFieldbucket(struct ee_fieldbucket *fieldbucket)
{
	assert(fieldbucket->objID == ObjID_FIELDBUCKET);
	fieldbucket->objID = ObjID_DELETED;
	xmlHashFree(fieldbucket->ht, deallocator);
	free(fieldbucket);
}


int
ee_addFieldToBucket(struct ee_fieldbucket *fieldbucket, struct ee_field *field)
{
	int r;
	assert(fieldbucket != NULL);assert(fieldbucket->objID == ObjID_FIELDBUCKET);
	assert(field != NULL);assert(field->objID == ObjID_FIELD);
	/* for the time being, we accept name "duplication" (it points to the same
	 * string in any case, so that's not too bad...)
	 */
	char *name;
	name = es_str2cstr(field->name, NULL);
	r = xmlHashAddEntry(fieldbucket->ht, (xmlChar*) name, field);
	return r;
}


/* we can pass only one pointer to libxml2, so we unfortunately
 * need to set up a structure for the two parameters we have.
 */
struct ptr_IteratoroverFieldTags {
	void (*f)(void *, char*);
	void *cookie;
};
static void IteratorOverFieldTags(void __attribute__((unused)) *payload,
				   void __attribute__((unused)) *data,
				   xmlChar *name)
{
	struct ptr_IteratoroverFieldTags *iter = (struct ptr_IteratoroverFieldTags*) data;
	iter->f(iter->cookie, (char*)name);
}


void
ee_iterateOverFieldTags(struct ee_fieldbucket *fieldbucket,
			 void(*f)(void*, char*), void *cookie)
{
	static struct ptr_IteratoroverFieldTags iter;
	assert(f != NULL);
	iter.f = f;
	iter.cookie = cookie;
	xmlHashScan(fieldbucket->ht, IteratorOverFieldTags, &iter);
}
