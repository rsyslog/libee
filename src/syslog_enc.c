/**
 * @file syslog_enc.c
 * Encoder for syslog format.
 * This file contains code from all related objects that is required in 
 * order to encode syslog format. The core idea of putting all of this into
 * a single file is that this makes it very straightforward to write
 * encoders for different encodings, as all is in one place.
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


int
ee_addValue_Syslog(struct ee_value *value, es_str_t **str)
{
	int r;
	es_str_t *valstr;
	unsigned char *c;
	es_size_t i;

	assert(str != NULL); assert(*str != NULL);
	assert(value != NULL); assert(value->objID == ObjID_VALUE);
	// TODO: support other types!
	assert(value->valtype == ee_valtype_str);
	valstr = value->val.str;

	c = es_getBufAddr(valstr);
	for(i = 0 ; i < es_strlen(valstr) ; ++i) {
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
ee_addField_Syslog(struct ee_field *field, es_str_t **str)
{
	int r;
	struct ee_valnode *valnode;

	assert(field != NULL);assert(field->objID== ObjID_FIELD);
	assert(str != NULL); assert(*str != NULL);
	CHKR(es_addStr(str, field->name));
	CHKR(es_addBuf(str, "=\"", 2));
	if(field->nVals > 0) {
		CHKR(ee_addValue_Syslog(field->val, str));
		if(field->nVals > 1) {
			for(valnode = field->valroot ; valnode != NULL ; valnode = valnode->next) {
				CHKR(es_addChar(str, ','));
				CHKR(ee_addValue_Syslog(valnode->val, str));
			}
		}
	}
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
	struct ee_fieldbucket_listnode *node;

	assert(event != NULL);assert(event->objID == ObjID_EVENT);
	if((*str = es_newStr(256)) == NULL) goto done;

	es_addBuf(str, "[cee@115", 8);
	if(event->tags != NULL) {
		CHKR(ee_addTags_Syslog(event->tags, str));
	}
	if(event->fields != NULL) {
		for(node = event->fields->root ; node != NULL ; node = node->next) {
			assert(node->field->objID == ObjID_FIELD);
			es_addChar(str, ' ');
			ee_addField_Syslog(node->field, str);
		}
	}
	es_addChar(str, ']');

done:
	return r;
}
/* vim :ts=4:sw=4 */
