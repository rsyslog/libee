/**
 * @file apache_dec.c
 * Decoder for apache common log format.
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
#include "libee/apache.h"
#include "libee/internal.h"


struct ee_apache*
ee_newApache(ee_ctx ctx)
{
	struct ee_apache *apache;

	if((apache = malloc(sizeof(struct ee_apache))) == NULL)
		goto done;
	
	apache->nroot = apache->ntail = NULL;
done:	return apache;
}


void
ee_deleteApache(struct ee_apache *apache)
{
	ee_fieldListApache_t *node, *nodeDel;

	if(apache == NULL)
		goto done;

	for(node = apache->nroot ; node != NULL ; ) {
		nodeDel = node;
		es_deleteStr(node->name);
		node = node->next;
		free(nodeDel);
	}

	free(apache);

done:	return;
}


static inline int
ee_apacheAddName(ee_ctx ctx, struct ee_apache *apache, es_str_t *name)
{
	int r;
	ee_fieldListApache_t *node;

	CHKN(node = malloc(sizeof(ee_fieldListApache_t)));
	node->next = NULL;
	node->name = name;

	/* enqueue */
	if(apache->nroot == NULL) {
		apache->nroot = apache->ntail = node;
	} else {
		apache->ntail->next = node;
		apache->ntail = node;
	}
	r = 0;

done:	return r;
}


int
ee_apacheNameList(ee_ctx ctx, struct ee_apache *apache, es_str_t *str)
{
	int r;
	es_size_t i = 0;
	unsigned char *c;
	es_str_t *name;

	c = es_getBufAddr(str);

	while(i < es_strlen(str)) {
		CHKN(name = es_newStr(16));
		while(i < es_strlen(str) && c[i] != ',' && c[i] != ' ') {
			CHKR(es_addChar(&name, c[i]));
			++i;
		}
		if(es_strlen(name) == 0) {
			r = -1 ; // TODO: error codee! 
			es_deleteStr(name);
			goto done;
		}
		/* add name to name list */
		CHKR(ee_apacheAddName(ctx, apache, name));
		if(i < es_strlen(str))	/* are we on ','? */
			++i;		/* "eat" it */
	}
	r = 0;

done:	return r;
}

static inline int
addField(ee_ctx ctx, struct ee_event *event, es_str_t *name, struct ee_value *value)
{
	int r;
	struct ee_field *field;

	CHKN(field = ee_newField(ctx));
	CHKR(ee_nameField(field, name));
	CHKR(ee_addValueToField(field, value));
	CHKR(ee_addFieldToEvent(event, field));
	r = 0;

done:	return r;
}


static inline int
processField(ee_ctx ctx, es_str_t *str, es_size_t *offs, struct ee_value **value)
{
	int r;
	int quoted;
	unsigned char *c;
	es_size_t i = *offs;
	es_str_t *val;

	CHKN(val = es_newStr(16));
	c = es_getBufAddr(str);
	CHKN(*value = ee_newValue(ctx));
	/* skip leading whitespace */
	while(i < es_strlen(str) && c[i] == ' ') {
		++i;
	}
	if(i == es_strlen(str))
		goto done;

	if(c[i] == '"') {
		quoted = 1;
		++i;
	} else if(c[i] == '[') {
		quoted = 2;
		++i;
	} else {
		quoted = 0;
	}

	while(i < es_strlen(str)) {
		if(   (quoted == 0 && c[i] == ' ')
		   || (quoted == 1 && c[i] == '"')
		   || (quoted == 2 && c[i] == ']')) {
			++i;
			break; /* end of field */
		}
		es_addChar(&val, c[i]);
		++i;
	}
	/* just a dash means this field is empty! */
	if(!es_strconstcmp(val, "-"))
		es_emptyStr(val);

	ee_setStrValue(*value, val);
	*offs = i;
	r = 0;

done:	return r;
}


/**
 * Process a log line.
 * @memberof ee_apache
 * @private
 * @returns 0 on success, something else otherwise.
 */
static inline int
processLn(ee_ctx ctx, struct ee_apache *apache, es_str_t *ln,
	  int (*cbNewEvt)(struct ee_event *event))
{
	int r;
	es_size_t i;
	struct ee_value *val;
	ee_fieldListApache_t *node;
	struct ee_value *value;
	struct ee_event *event;

	CHKN(event = ee_newEvent(ctx));
	i = 0;
	node = apache->nroot;
	while(node != NULL && i < es_strlen(ln)) {
		CHKR(processField(ctx, ln, &i, &val));
		CHKR(addField(ctx, event, node->name, val));
		node = node->next;
	}
	CHKR(cbNewEvt(event));
	r = 0;

done:	return r;
}


int
ee_apacheDec(ee_ctx ctx, int (*cbGetLine)(es_str_t **ln),
          int (*cbNewEvt)(struct ee_event *event),
	      es_str_t **errMsg, struct ee_apache *apache)
{
	int r;
	int lnNbr;
	es_str_t *ln = NULL;
	char errMsgBuf[1024];
	size_t errlen;
	
	lnNbr = 1;
	r = cbGetLine(&ln);
	while(r == 0) {
		if((r = processLn(ctx, apache, ln, cbNewEvt)) != 0) {
			errlen = snprintf(errMsgBuf, sizeof(errMsgBuf),
					  "error processing line %d", lnNbr);
			*errMsg = es_newStrFromCStr(errMsgBuf, errlen);
			goto done;
		}
		free(ln);
		r = cbGetLine(&ln);
		lnNbr++;
	}
	/* when we are done with the file, we need to check if there are
	 * any objects to submit (usually there are!)
	 */

	if(r == EE_EOF)
		r = 0;
done:
	return r;
}
/* vim :ts=4:sw=4 */
