/**
 * @file xml_enc.c
 * Encoder for XML format.
 *
 * This file contains code from all related objects that is required in 
 * order to encode this format. The core idea of putting all of this into
 * a single file is that this makes it very straightforward to write
 * encoders for different encodings, as all is in one place.
 *
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

static char hexdigit[16] =
	{'0', '1', '2', '3', '4', '5', '6', '7', '8',
	 '9', 'A', 'B', 'C', 'D', 'E', 'F' };

/* TODO: XML encoding for Unicode characters is as of RFC4627 not fully
 * supported. The algorithm is that we must build the wide character from
 * UTF-8 (if char > 127) and build the full 4-octet Unicode character out
 * of it. Then, this needs to be encoded. Currently, we work on a
 * byte-by-byte basis, which simply is incorrect.
 * rgerhards, 2010-11-09
 */
int
ee_addValue_XML(struct ee_value *value, es_str_t **str)
{
	int r;
	es_str_t *valstr;
	unsigned char *buf;
	unsigned char c;
	es_size_t i;
	char numbuf[4];
	int j;

	assert(str != NULL); assert(*str != NULL);
	assert(value != NULL); assert(value->objID == ObjID_VALUE);
	// TODO: support other types!
	assert(value->valtype == ee_valtype_str);
	valstr = value->val.str;
	es_addBuf(str, "<value>", 7);

	buf = es_getBufAddr(valstr);
	for(i = 0 ; i < es_strlen(valstr) ; ++i) {
		c = buf[i];
		switch(c) {
		case '\0':
			es_addBuf(str, "&#00;", 5);
			break;
#if 0
		case '\n':
			es_addBuf(str, "&#10;", 5);
			break;
		case '\r':
			es_addBuf(str, "&#13;", 5);
			break;
		case '\t':
			es_addBuf(str, "&x08;", 5);
			break;
		case '\"':
			es_addBuf(str, "&quot;", 6);
			break;
#endif
		case '<':
			es_addBuf(str, "&lt;", 4);
			break;
		case '&':
			es_addBuf(str, "&amp;", 5);
			break;
#if 0
		case ',':
			es_addBuf(str, "\\,", 2);
			break;
		case '\'':
			es_addBuf(str, "&apos;", 6);
			break;
#endif
		default:
			es_addChar(str, c);
#if 0
			/* TODO : proper Unicode encoding (see header comment) */
			for(j = 0 ; j < 4 ; ++j) {
				numbuf[3-j] = hexdigit[c % 16];
				c = c / 16;
			}
			es_addBuf(str, "\\u", 2);
			es_addBuf(str, numbuf, 4);
			break;
#endif
		}
	}
	es_addBuf(str, "</value>", 8);
	r = 0;

	return r;
}


int
ee_addField_XML(struct ee_field *field, es_str_t **str)
{
	int r;
	struct ee_valnode *valnode;

	assert(field != NULL);assert(field->objID== ObjID_FIELD);
	assert(str != NULL); assert(*str != NULL);
	CHKR(es_addBuf(str, "<Field name =\"", 14));
	CHKR(es_addStr(str, field->name));
	CHKR(es_addBuf(str, "\">", 2));
	if(field->nVals > 0) {
		if(field->nVals == 1) {
			CHKR(ee_addValue_XML(field->val, str));
		} else { /* we have multiple values --> array */
			CHKR(ee_addValue_XML(field->val, str));
			for(valnode = field->valroot ; valnode != NULL ; valnode = valnode->next) {
				CHKR(ee_addValue_XML(valnode->val, str));
			}
		}
	}
	CHKR(es_addBuf(str, "</Field>", 8));
	r = 0;

done:
	return r;
}


static inline int
ee_addTags_XML(struct ee_tagbucket *tags, es_str_t **str)
{
	int r = 0;
	struct ee_tagbucket_listnode *tag;

	CHKR(es_addBuf(str, "<event.tags>", 12));
	for(tag = tags->root ; tag != NULL ; tag = tag->next) {
		CHKR(es_addBuf(str, "<tag>", 5));
		CHKR(es_addStr(str, tag->name));
		CHKR(es_addBuf(str, "</tag>", 6));
	}
	CHKR(es_addBuf(str, "</event.tags>", 13));

done:	return r;
}


int
ee_fmtEventToXML(struct ee_event *event, es_str_t **str)
{
	int r = -1;
	struct ee_fieldbucket_listnode *node;

	assert(event != NULL);assert(event->objID == ObjID_EVENT);
	if((*str = es_newStr(256)) == NULL) goto done;

	es_addBuf(str, "<event>", 7);
	if(event->tags != NULL) {
		CHKR(ee_addTags_XML(event->tags, str));
	}
	if(event->fields != NULL) {
		for(node = event->fields->root ; node != NULL ; node = node->next) {
			assert(node->field->objID == ObjID_FIELD);
			ee_addField_XML(node->field, str);
		}
	}
	es_addBuf(str, "</event>", 8);

done:
	return r;
}
/* vim :ts=4:sw=4 */
