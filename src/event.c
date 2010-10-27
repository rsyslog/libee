/**
 * @file event.c
 * Implements event object methods.
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
#include "libee/nvfield.h"
#include "libee/value.h"

#define ERR_ABORT {r = 1; goto done; }

#define CHECK_FIELD \
	if(event->objID != ObjID_EVENT) { \
		r = -1; \
		goto done; \
	}


struct ee_event*
ee_newEvent(ee_ctx __attribute__((unused)) ctx)
{
	struct ee_event *event;
	if((event = malloc(sizeof(struct ee_event))) == NULL)
		goto done;

	event->objID = ObjID_EVENT;
	event->ctx = ctx;

done:
	return event;
}


void
ee_deleteEvent(struct ee_event *event)
{
	assert(event->objID == ObjID_EVENT);
	if(event->tags != NULL)
		ee_deleteTagbucket(event->tags);
	if(event->fields != NULL)
		ee_deleteFieldbucket(event->fields);
	free(event);
}


int
ee_addTagToEvent(struct ee_event *event, char *tag)
{
	int r = -1;

	assert(event->objID == ObjID_EVENT);
	if(event->tags == NULL)
		if((event->tags = ee_newTagbucket(event->ctx)) == NULL)
			goto done;

	r = ee_addTagToBucket(event->tags, strdup(tag));
	
done:
	return r;
}


int
ee_addStrFieldToEvent(struct ee_event *event, char *fieldname, char *value)
{
	int r = -1;
	struct ee_nvfield *nvfield = NULL;
	union ee_value *val = NULL;

	assert(event->objID == ObjID_EVENT);
	if(event->fields == NULL)
		if((event->fields = ee_newFieldbucket(event->ctx)) == NULL)
			goto done;
//printf("addStrField: %s/%s\n", fieldname, value);

	if((val = ee_newValue(event->ctx)) == NULL) goto done;
	if((r = ee_setStrValue(val, value)) != 0) goto done;
	if((nvfield = ee_newNVField(event->ctx, fieldname, val)) == NULL) goto done;
	if((r = ee_addNVFieldToBucket(event->fields, nvfield)) != 0) goto done;

done:
	if(r != 0) {
		/* central error cleanup */
		if(val != NULL)
			ee_deleteValue(val);
		if(nvfield != NULL)
			ee_deleteNVField(nvfield);
	}

	return r;
}


/* TODO: create much more solid code. This here is a HACK! */
static void copy2String(char *str, size_t *offs, char *toAdd)
{
	strcpy(str+*offs, toAdd);
	*offs = strlen(str);
}

/* we can pass only one pointer to libxml2, so we unfortunately
 * need to set up a structure for the two parameters we have.
 */
struct data_IteratorRFC5424 {
	char *buf;
	size_t lenBuf;
	size_t lenCurr;
};
/* callback used to build the strings */
static void IteratorRFC5424(void __attribute__((unused)) *payload,
				   void __attribute__((unused)) *data,
				   xmlChar *name)
{
	struct data_IteratorRFC5424 *iter = (struct data_IteratorRFC5424 *) data;
	char buf[10240];

printf("name=%s, value=%s\n", name, ((struct ee_nvfield*) payload)->val->str);
	sprintf(buf, " %s=\"%s\"", name, ((struct ee_nvfield*) payload)->val->str);
	copy2String(iter->buf, &iter->lenCurr, buf);
}
/* TODO: do a *real* implementation. The code below is just a
 * rough tester.
 */
/* Note: for efficiency reasons, we "break up" the object model a bit
 * here: In order to avoid inefficiency when calling the hashScan function,
 * I iterate over the hash table holding the fields here. Note that this
 * object here should *not* know it is actually dealing with a hash table.
 * rgerhards, 2010-10-27
 */
int
ee_fmtEventToRFC5424(struct ee_event *event, char **pbuf, size_t *plenBuf)
{
	int r = -1;
	struct data_IteratorRFC5424 iter;

	assert(event->objID == ObjID_EVENT);
	iter.buf = malloc(10240); // TODO: something real!
	iter.lenBuf = 10240;
	iter.lenCurr = 0;

	xmlHashScan(event->fields->ht, IteratorRFC5424, &iter);
	*pbuf = iter.buf;

done:
	return r;
}
