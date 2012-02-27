/**
 * @file syslog_enc.c
 * Encoder for syslog format.
 * This file contains code from all related objects that is required in 
 * order to encode syslog format. The core idea of putting all of this into
 * a single file is that this makes it very straightforward to write
 * encoders for different encodings, as all is in one place.
 *//* Libee - An Event Expression Library inspired by CEE
 * Copyright 2010-2012 by Rainer Gerhards and Adiscon GmbH.
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

#include "collection/collection.h"
#include "libee/libee.h"
#include "libee/internal.h"


int
ee_addValue_Syslog(struct collection_item *item, es_str_t **str)
{
	int r;
	unsigned char *c;
	es_size_t i;
	unsigned lenstr;

	assert(str != NULL); assert(*str != NULL);
	assert(item != NULL);
	// TODO: support other types!

	c = col_get_item_data(item);
	lenstr = col_get_item_length(item) - 1;
	for(i = 0 ; i < lenstr ; ++i) {
		switch(c[i]) {
		case '\0':
			es_addChar(str, '\\');
			es_addChar(str, '0');
			break;
		case '\n':
			es_addChar(str, '\\');
			es_addChar(str, 'n');
			break;
		/* TODO : add rest of control characters here... */
		case ',': /* comma is CEE-reserved for lists */
			es_addChar(str, '\\');
			es_addChar(str, ',');
			break;
#if 0 /* alternative encoding for discussion */
		case '^': /* CEE-reserved for lists */
			es_addChar(str, '\\');
			es_addChar(str, '^');
			break;
#endif
		/* at this layer ... do we need to think about transport
		 * encoding at all? Or simply leave it to the transport agent?
		 */
		case '\\': /* RFC5424 reserved */
			es_addChar(str, '\\');
			es_addChar(str, '\\');
			break;
		case ']': /* RFC5424 reserved */
			es_addChar(str, '\\');
			es_addChar(str, ']');
			break;
		case '\"': /* RFC5424 reserved */
			es_addChar(str, '\\');
			es_addChar(str, '\"');
			break;
		default:
			es_addChar(str, c[i]);
			break;
		}
	}
	r = 0;

	return r;
}


int
ee_addField_Syslog(struct collection_item *item, es_str_t **str)
{
	int r;
	int colType;
	char *propname;
	int proplen;

	assert(str != NULL); assert(*str != NULL);
	colType = col_get_item_type(item);
	if(colType != COL_TYPE_STRING) {
		goto done;
	}
	propname = (char*) col_get_item_property(item, &proplen);
	CHKR(es_addBuf(str, propname, proplen));
	CHKR(es_addBuf(str, "=\"", 2));
	CHKR(ee_addValue_Syslog(item, str));
#if 0
	if(field->nVals > 0) {
		/* TODO:  we need to handle arrays, if this is something that
		  we actually need to take care of. Remember that the CEE syslog
		  mapping currently solely bases on JSON...
		*/
		if(field->nVals > 1) {
			for(valnode = field->valroot ; valnode != NULL ; valnode = valnode->next) {
				CHKR(es_addChar(str, ','));
				CHKR(ee_addValue_Syslog(valnode->val, str));
			}
		}
	}
#endif
	CHKR(es_addChar(str, '\"'));
	r = 0;

done:
	return r;
}


static inline int
ee_addTags_Syslog(struct ee_tagbucket *tags, es_str_t **str)
{
	int r = 0;
	struct ee_tagbucket_listnode *tag;
	int needComma = 0;

	CHKR(es_addBuf(str, " event.tags=\"", 13));
	for(tag = tags->root ; tag != NULL ; tag = tag->next) {
		if(needComma)
			es_addChar(str, ',');
		else
			needComma = 1;
		CHKR(es_addStr(str, tag->name));
	}
	es_addChar(str, '"');

done:	return r;
}


int
ee_fmtEventToRFC5424(struct ee_event *event, es_str_t **str)
{
	int r = -1;
	struct collection_iterator *iterator;
	struct collection_item *item;
	int error;

	assert(event != NULL);assert(event->objID == ObjID_EVENT);
	if((*str = es_newStr(256)) == NULL) goto done;

	es_addBuf(str, "[cee@115", 8);
	if(event->tags != NULL) {
		CHKR(ee_addTags_Syslog(event->tags, str));
	}

	CHKR(col_bind_iterator(&iterator, event->data, COL_TRAVERSE_DEFAULT));
	do {
		error = col_iterate_collection(iterator, &item);
		if (error) {
			col_unbind_iterator(iterator);
			goto done;
		}
		if(item == NULL)
			break;
		if(!strcmp(col_get_item_property(item, NULL), "[ROOT]"))
			continue; /* quick hack to get rid of root object... */

		es_addChar(str, ' ');
		ee_addField_Syslog(item, str);
	} while(1);
	es_addChar(str, ']');

done:
	return r;
}
/* vim :ts=4:sw=4 */
