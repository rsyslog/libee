/**
 * @file csv_enc.c
 * Encoder for CSV format. Note: CEE currently think about what a
 * CEE-compliant CSV format may look like. As such, the format of
 * this output will most probably change once the final decision
 * has been made. At this time (2010-12), I do NOT even try to
 * stay inline with the discussion.
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

/* data structure for the field list */
typedef struct ee_fieldListCSV_s ee_fieldListCSV_t;
/**
 * List with names for fields in log file (in that order).
 */
struct ee_fieldListCSV_s {
	es_str_t *name;		/**< field name */
	ee_fieldListCSV_t *next;	/**< list housekeeping, next node (or NULL) */
};
/* field list object
 */
struct ee_FieldCSV {
	ee_fieldListCSV_t	*nroot; /**< root of field name list */
	ee_fieldListCSV_t	*ntail; /**< tail of field name list */
};


static inline int
ee_AddName(ee_ctx ctx, struct ee_FieldCSV *fields, es_str_t *name)
{
	int r;
	ee_fieldListCSV_t *node;

	CHKN(node = malloc(sizeof(ee_fieldListCSV_t)));
	node->next = NULL;
	node->name = name;

	/* enqueue */
	if(fields->nroot == NULL) {
		fields->nroot = fields->ntail = node;
	} else {
		fields->ntail->next = node;
		fields->ntail = node;
	}
	r = 0;

done:	return r;
}


/**
 * Build a name list. The name list is required in order for this encoder
 * to know in which sequence which fields need to be output.
 */
static inline struct ee_FieldCSV*
genNameList(ee_ctx ctx, es_str_t *str)
{
	struct ee_FieldCSV *fields = NULL;
	es_size_t i = 0;
	unsigned char *c;
	es_str_t *name;

	if((fields = malloc(sizeof(struct ee_FieldCSV))) == NULL) goto done;
	fields->nroot = fields->ntail = NULL;

	c = es_getBufAddr(str);
	while(i < es_strlen(str)) {
		if((name = es_newStr(16)) == NULL) {
			free(fields);
			fields = NULL;
			goto done;
		}
		while(i < es_strlen(str) && c[i] != ',' && c[i] != ' ') {
			if((es_addChar(&name, c[i])) != 0) goto done;
			++i;
		}
		if(es_strlen(name) == 0) {
			es_deleteStr(name);
			free(fields);
			fields = NULL;
			goto done;
		}
		/* add name to name list */
		if((ee_AddName(ctx, fields, name)) != 0) goto done;
		if(i < es_strlen(str))	/* are we on ','? */
			++i;		/* "eat" it */
	}

done:	return fields;
}


/* TODO: CSV encoding for Unicode characters is as of RFC4627 not fully
 * supported. The algorithm is that we must build the wide character from
 * UTF-8 (if char > 127) and build the full 4-octet Unicode character out
 * of it. Then, this needs to be encoded. Currently, we work on a
 * byte-by-byte basis, which simply is incorrect.
 * rgerhards, 2010-11-09
 */
int
ee_addValue_CSV(struct ee_value *value, es_str_t **str)
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
	r = 0;

	return r;
}


int
ee_addField_CSV(struct ee_field *field, es_str_t **str)
{
	int r;
	struct ee_valnode *valnode;

	assert(field != NULL);assert(field->objID== ObjID_FIELD);
	assert(str != NULL); assert(*str != NULL);

	if(field->nVals == 1) {
		CHKR(ee_addValue_CSV(field->val, str));
	} else { /* we have multiple values --> array */
		CHKR(es_addChar(str, '['));
		CHKR(ee_addValue_CSV(field->val, str));
		for(valnode = field->valroot ; valnode != NULL ; valnode = valnode->next) {
			CHKR(es_addChar(str, ','));
			CHKR(ee_addValue_CSV(valnode->val, str));
		}
		CHKR(es_addChar(str, ']'));
	}
	r = 0;

done:
	return r;
}


int
ee_fmtEventToCSV(struct ee_event *event, es_str_t **str, es_str_t *extraData)
{
	int r = -1;
	struct ee_FieldCSV *fields = NULL;
	struct ee_field* field;
	ee_fieldListCSV_t *node;

	assert(event != NULL);assert(event->objID == ObjID_EVENT);
	assert(extraData != NULL);
	if((*str = es_newStr(256)) == NULL) goto done;
	if((fields = genNameList(event->ctx, extraData)) == NULL) goto done;

	node = fields->nroot;
	while(node != NULL) {
		CHKR(es_addChar(str, '"'));
		if((field = ee_getEventField(event, node->name)) != NULL) {
			ee_addField_CSV(field, str);
		}
		CHKR(es_addChar(str, '"'));
		node = node->next;
		if(node != NULL) {
			CHKR(es_addChar(str, ','));
		}
	}
	r = 0;
done:
	return r;
}
/* vim :ts=4:sw=4 */
