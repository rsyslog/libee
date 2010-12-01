/**
 * @file int_dec.c
 * Decoder for libee internal event format.
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
#include "libee/int.h"
#include "libee/internal.h"


/**
 * Decode a line into type and value. Value is NOT unescaped.
 * @memberof ee_int
 * @private
 * @returns 0 on success, something else otherwise.
 */
static inline int
decodeLn(es_str_t *ln, char *typ, es_str_t **value)
{
	int r ;

	if(es_strlen(ln) < 2) {
		r = EE_INVLDFMT;
		goto done;
	}

	*typ = es_getBufAddr(ln)[0];
	if(*typ != '#' && *typ != 'e' && *typ != 'f' && *typ != 'v') {
		r = EE_INVLDFMT;
		goto done;
	}

	if(es_getBufAddr(ln)[1] != ':') {
		r = EE_INVLDFMT;
		goto done;
	}
	if((*value = es_newStrFromSubStr(ln, 2, es_strlen(ln) - 2)) == NULL) {
		r = EE_NOMEM;
		goto done;
	}
	r = 0;
done:
	return r;
}


/**
 * Finish processing current field.
 * Add it to the current event and start a new one.
 * @memberof ee_int
 * @private
 * @returns 0 on success, something else otherwise.
 */
static inline int
finishField(struct ee_event *event, struct ee_field **field)
{
	int r;
	if(*field != NULL) {
		CHKR(ee_addFieldToEvent(event, *field));
		*field = NULL;
	}
	r = 0;
done:
	return r;
}


/**
 * Process a decoded line.
 * @memberof ee_int
 * @private
 * @returns 0 on success, something else otherwise.
 */
static inline int
processLn(ee_ctx ctx, char typ, es_str_t *value, struct ee_event **event,
		  struct ee_field **field,
          int (*cbNewEvt)(struct ee_event *event))
{
	int r;
	struct ee_value *val;

	switch(typ) {
	case '#':
		/* comment - ignore */
		break;
	case 'e':
		if(*event != NULL) {
			CHKR(finishField(*event, field));
			CHKR(cbNewEvt(*event));
		}
		CHKN(*event = ee_newEvent(ctx));
		break;
	case 'f':
		if(*event == NULL) {
			r = EE_INVLDFMT;
			goto done;
		}
		CHKR(finishField(*event, field));
		CHKN(*field = ee_newField(ctx));
		CHKR(ee_nameField(*field, value));
		break;
	case 'v':
		if(*field == NULL) {
			r = EE_INVLDFMT;
			goto done;
		}
		CHKN(val = ee_newValue(ctx));
		CHKR(ee_setStrValue(val, value));
		CHKR(ee_addValueToField(*field, val));
		break;
	}
	r = 0;

done:
	return r;
}


int
ee_intDec(ee_ctx ctx, int (*cbGetLine)(es_str_t **ln),
          int (*cbNewEvt)(struct ee_event *event),
	      es_str_t **errMsg)
{
	int r;
	int lnNbr;
	es_str_t *ln = NULL;
	char typ;
	es_str_t *value;
	struct ee_event *event = NULL;
	struct ee_field *field = NULL;
	char errMsgBuf[1024];
	size_t errlen;
	
	lnNbr = 1;
	r = cbGetLine(&ln);
	while(r == 0) {
		if((r = decodeLn(ln, &typ, &value)) != 0) {
			errlen = snprintf(errMsgBuf, sizeof(errMsgBuf),
					  "invalid format in line %d", lnNbr);
			*errMsg = es_newStrFromCStr(errMsgBuf, errlen);
			goto done;
		}
		if((r = processLn(ctx, typ, value, &event, &field, cbNewEvt)) != 0) {
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
	if(event != NULL) {
		if(field != NULL) {
			CHKR(finishField(event, &field));
		}
		CHKR(cbNewEvt(event));
	}

	if(r == EE_EOF)
		r = 0;
done:
	return r;
}
/* vim :ts=4:sw=4 */
