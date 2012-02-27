/**
 * @file json_dec.c
 * Decoder for JSON format.
 *//* Libee - An Event Expression Library inspired by CEE
 * Copyright 2012 by Rainer Gerhards and Adiscon GmbH.
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


/**
 * Process a log line.
 * @private
 * @returns 0 on success, something else otherwise.
 */
static inline int
processLn(ee_ctx ctx, es_str_t *ln,
	  int (*cbNewEvt)(struct ee_event *event))
{
	int r;
	struct ee_event *event;
	char *str;

	str = es_str2cstr(ln, NULL);
	CHKN(event = ee_newEventFromJSON(ctx, str));
	free(str);
	CHKR(cbNewEvt(event));
	r = 0;

done:	return r;
}


int
ee_jsonDec(ee_ctx ctx, int (*cbGetLine)(es_str_t **ln),
          int (*cbNewEvt)(struct ee_event *event),
	      es_str_t **errMsg)
{
	int r;
	int lnNbr;
	es_str_t *ln = NULL;
	char errMsgBuf[1024];
	size_t errlen;
	
	lnNbr = 1;
	r = cbGetLine(&ln);
	while(r == 0) {
		if((r = processLn(ctx, ln, cbNewEvt)) != 0) {
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
