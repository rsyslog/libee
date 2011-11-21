//#define NO_EMPTY_FIELDS /* undef for "normal" behaviour */
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
#include "libee/internal.h"

static char hexdigit[16] =
	{'0', '1', '2', '3', '4', '5', '6', '7', '8',
	 '9', 'A', 'B', 'C', 'D', 'E', 'F' };

/* TODO: JSON encoding for Unicode characters is as of RFC4627 not fully
 * supported. The algorithm is that we must build the wide character from
 * UTF-8 (if char > 127) and build the full 4-octet Unicode character out
 * of it. Then, this needs to be encoded. Currently, we work on a
 * byte-by-byte basis, which simply is incorrect.
 * rgerhards, 2010-11-09
 */
int
ee_addValue_JSON(struct ee_value *value, es_str_t **str)
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
	es_addChar(str, '\"');

	buf = es_getBufAddr(valstr);
	for(i = 0 ; i < es_strlen(valstr) ; ++i) {
		c = buf[i];
		if(   (c >= 0x23 && c <= 0x5b)
		   || (c >= 0x5d /* && c <= 0x10FFFF*/)
		   || c == 0x20 || c == 0x21) {
			/* no need to escape */
			es_addChar(str, c);
		} else {
			/* we must escape, try RFC4627-defined special sequences first */
			switch(c) {
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
			case '\010':
				es_addBuf(str, "\\b", 2);
				break;
			case '\014':
				es_addBuf(str, "\\f", 2);
				break;
			case '\n':
				es_addBuf(str, "\\n", 2);
				break;
			case '\r':
				es_addBuf(str, "\\r", 2);
				break;
			case '\t':
				es_addBuf(str, "\\t", 2);
				break;
			default:
				/* TODO : proper Unicode encoding (see header comment) */
				for(j = 0 ; j < 4 ; ++j) {
					numbuf[3-j] = hexdigit[c % 16];
					c = c / 16;
				}
				es_addBuf(str, "\\u", 2);
				es_addBuf(str, numbuf, 4);
				break;
			}
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
#ifdef NO_EMPTY_FIELDS
if(field->nVals == 0) {
	r = 1;
	goto done;
} else if(field->nVals == 1 && es_strlen(field->val->val.str) == 0) {
	r = 1;
	goto done;
}
#endif
	CHKR(es_addChar(str, '\"'));
	CHKR(es_addStr(str, field->name));
	if(ee_ctxIsEncUltraCompact(field->ctx)) {
		CHKR(es_addBuf(str, "\":", 2));
	} else {
		CHKR(es_addBuf(str, "\": ", 3));
	}
	if(field->nVals == 0) {
		if(ee_ctxIsEncUltraCompact(field->ctx)) {
			CHKR(es_addChar(str, '\"'));
		} else {
			CHKR(es_addBuf(str, "\"\"", 2));
		}
	} else if(field->nVals == 1) {
		CHKR(ee_addValue_JSON(field->val, str));
	} else { /* we have multiple values --> array */
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


static inline int
ee_addTags_JSON(struct ee_tagbucket *tags, es_str_t **str)
{
	int r = 0;
	struct ee_tagbucket_listnode *tag;
	int needComma = 0;

	CHKR(es_addBuf(str, "\"event.tags\":[", 14));
	for(tag = tags->root ; tag != NULL ; tag = tag->next) {
		if(needComma)
			es_addChar(str, ',');
		else
			needComma = 1;
		es_addChar(str, '"');
		CHKR(es_addStr(str, tag->name));
		es_addChar(str, '"');
	}
	es_addChar(str, ']');

done:	return r;
}


int
ee_fmtEventToJSON(struct ee_event *event, es_str_t **str)
{
	int r = -1;
	struct ee_fieldbucket_listnode *node;
	int bNeedComma = 0;

	assert(event != NULL);assert(event->objID == ObjID_EVENT);
	if((*str = es_newStr(256)) == NULL) goto done;

	es_addChar(str, '{');
	if(   event->ctx->flags & EE_CTX_FLAG_INCLUDE_FLAT_TAGS
	   && event->tags != NULL) {
		CHKR(ee_addTags_JSON(event->tags, str));
		bNeedComma = 1;
	}
	if(event->fields != NULL) {
		for(node = event->fields->root ; node != NULL ; node = node->next) {
			assert(node->field->objID == ObjID_FIELD);
			if(bNeedComma) {
				CHKR(es_addBuf(str, ", ", 2));
			} else {
				bNeedComma = 1;
			}
#ifdef NO_EMPTY_FIELDS
			if(ee_addField_JSON(node->field, str) == 1)
				continue;
#else
			ee_addField_JSON(node->field, str);
#endif
		}
	}
	es_addChar(str, '}');

done:
	return r;
}
/* vim :ts=4:sw=4 */
