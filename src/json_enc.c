/**
 * @file json_enc.c
 * Encoder for JSON format.
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


int
ee_addValue_JSON(struct ee_value *value, es_str_t **str)
{
	int r;
	es_str_t *valstr;
	unsigned char *c;
	size_t i;

	assert(str != NULL); assert(*str != NULL);
	assert(value != NULL); assert(value->objID == ObjID_VALUE);
	// TODO: support other types!
	assert(value->valtype == ee_valtype_str);
	valstr = value->val.str;
	es_addChar(str, '\"');

	c = es_getBufAddr(valstr);
	for(i = 0 ; i < es_strlen(valstr) ; ++i) {
		switch(c[i]) {
		case '\0':
			es_addBuf(str, "\\u0000", 6);
			break;
		case '\"':
			es_addBuf(str, "\\\"", 2);
			break;
		case '/':
			es_addBuf(str, "\\/", 2);
			break;
		case '\\':
			es_addBuf(str, "\\\\", 2);
			break;
		case '\n':
			es_addBuf(str, "\\n", 2);
			break;
		/* TODO : add rest of control characters here... */
		default:
			es_addChar(str, c[i]);
			break;
		}
	}
	es_addChar(str, '\"');
	r = 0;

	return r;
}


int
ee_addField_JSON(struct ee_field *field, es_str_t **str)
{
	int r;
	struct ee_valnode *valnode;

	assert(field != NULL);assert(field->objID== ObjID_FIELD);
	assert(str != NULL); assert(*str != NULL);
	CHKR(es_addChar(str, '\"'));
	CHKR(es_addStr(str, field->name));
	CHKR(es_addBuf(str, "\": ", 3));
	if(field->nVals == 1) {
		CHKR(ee_addValue_JSON(field->val, str));
	} else if(field->nVals > 1) {
		CHKR(es_addChar(str, '['));
		CHKR(ee_addValue_JSON(field->val, str));
		for(valnode = field->valroot ; valnode != NULL ; valnode = valnode->next) {
			CHKR(es_addChar(str, ','));
			CHKR(ee_addValue_JSON(valnode->val, str));
		}
		CHKR(es_addChar(str, ']'));
	}
	r = 0;

done:
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
	ee_addField_JSON(field, str);
	es_addChar(str, ',');
}
/* Note: for efficiency reasons, we "break up" the object model a bit
 * here: In order to avoid inefficiency when calling the hashScan function,
 * I iterate over the hash table holding the fields here. Note that this
 * object here should *not* know it is actually dealing with a hash table.
 * rgerhards, 2010-10-27
 */
int
ee_fmtEventToJSON(struct ee_event *event, es_str_t **str)
{
	int r = -1;

	assert(event != NULL);assert(event->objID == ObjID_EVENT);
	if((*str = es_newStr(256)) == NULL) goto done;

	es_addBuf(str, "{", 8);
	xmlHashScan(event->fields->ht, IteratorRFC5424, str);
	es_addChar(str, (unsigned char) '}');

done:
	return r;
}
/* vim :ts=4:sw=4 */
