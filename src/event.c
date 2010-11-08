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
#include <libestr.h>

#include "libee/libee.h"
#include "libee/field.h"
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
	event->fields = NULL;
	event->tags = NULL;

done:
	return event;
}


void
ee_deleteEvent(struct ee_event *event)
{
	assert(event != NULL);assert(event->objID == ObjID_EVENT);
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

	assert(event != NULL);assert(event->objID == ObjID_EVENT);
	if(event->tags == NULL)
		if((event->tags = ee_newTagbucket(event->ctx)) == NULL)
			goto done;

	r = ee_addTagToBucket(event->tags, strdup(tag));
	
done:
	return r;
}


int
ee_addFieldToEvent(struct ee_event *event, struct ee_field *field)
{
	int r;

	assert(event != NULL);assert(event->objID == ObjID_EVENT);
	if(event->fields == NULL) {
		CHKN(event->fields = ee_newFieldbucket(event->ctx));
	}

	r = ee_addFieldToBucket(event->fields, field);
	
done:
	return r;
}


int
ee_addStrFieldToEvent(struct ee_event *event, char *fieldname, es_str_t *value)
{
	int r = -1;
	struct ee_field *field = NULL;
	struct ee_value *val = NULL;

	assert(event != NULL);assert(event->objID == ObjID_EVENT);
	if(event->fields == NULL)
		if((event->fields = ee_newFieldbucket(event->ctx)) == NULL)
			goto done;
//printf("addStrField: %s/%s\n", fieldname, es_str2cstr(value, NULL));

	if((val = ee_newValue(event->ctx)) == NULL) goto done;
	if((r = ee_setStrValue(val, value)) != 0) goto done;
	if((field = ee_newFieldFromNV(event->ctx, fieldname, val)) == NULL) goto done;
	if((r = ee_addFieldToBucket(event->fields, field)) != 0) goto done;

done:
	if(r != 0) {
		/* central error cleanup */
		if(val != NULL)
			ee_deleteValue(val);
		if(field != NULL)
			ee_deleteField(field);
	}

	return r;
}


/* callback used to build the strings */
static void IteratorRFC5424(void *payload, void *data,
				   xmlChar __attribute__((unused)) *name)
{
	struct ee_field *field = (struct ee_field*) payload;
	es_str_t **str = (es_str_t**) data;

	assert(field->objID == ObjID_FIELD);
	es_addChar(str, ' ');
	ee_addField_Syslog(field, str);
}
/* Note: for efficiency reasons, we "break up" the object model a bit
 * here: In order to avoid inefficiency when calling the hashScan function,
 * I iterate over the hash table holding the fields here. Note that this
 * object here should *not* know it is actually dealing with a hash table.
 * rgerhards, 2010-10-27
 */
int
ee_fmtEventToRFC5424(struct ee_event *event, es_str_t **str)
{
	int r = -1;

	assert(event != NULL);assert(event->objID == ObjID_EVENT);
	if((*str = es_newStr(256)) == NULL) goto done;

	es_addBuf(str, "[cee@...", 8);
	xmlHashScan(event->fields->ht, IteratorRFC5424, str);
	es_addChar(str, ']');

done:
	return r;
}
