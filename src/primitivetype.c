/**
 * @file primitivetype.c
 * Implementation of the primitiveType class.
 *//*
 * Libee - An Event Expression Library inspired by CEE
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
#include <ctype.h>

#include "libee/libee.h"

#define ERR_ABORT {r = 1; goto done; }

#define CHECK_CTX \
	if(ctx->objID != ObjID_CTX) { \
		r = -1; \
		goto done; \
	}

ee_ctx
ee_initPrimitiveType(void)
{
	ee_ctx ctx;
	if((ctx = calloc(1, sizeof(struct ee_ctx_s))) == NULL)
		goto done;

	ctx->objID = ObjID_CTX;
	ctx->dbgCB = NULL;
done:
	return ctx;
}

int
ee_exitPrimitiveType(ee_ctx ctx)
{
	int r = 0;

	CHECK_CTX;

	ctx->objID = ObjID_None; /* prevent double free */
	free(ctx);
done:
	return r;
}

/* some helpers */
static inline int
hParseInt(unsigned char **buf, size_t *lenBuf)
{
	unsigned char *p = *buf;
	size_t len = *lenBuf;
	int i = 0;
	
	while(len > 0 && isdigit(*p)) {
		i = i * 10 + *p - '0';
		++p;
		--len;
	}

	*buf = p;
	*lenBuf = len;
	return i;
}

/* parsers for the primitive types
 *
 * All parsers receive 
 *
 * @param[in] ctx context object
 * @param[in] str the to-be-parsed string
 * @param[in/out] an offset into the string
 * @param[out] newVal newly created value
 *
 * They will try to parse out "their" object from the string. If they
 * succeed, they:
 *
 * create a nw ee_value (newVal) and store the obtained value into it
 * update buf and lenBuf to reflect the parsing carried out
 *
 * returns 0 on success and EE_WRONGPARSER if this parser could
 *           not successfully parse (but all went well otherwise) and something
 *           else in case of an error.
 */
//int ee_parse##ParserName(es_str_t *str, size_t *offs, 
#define BEGINParser(ParserName) \
int ee_parse##ParserName(ee_ctx __attribute__((unused)) ctx, es_str_t *str, size_t *offs, \
                      struct ee_value **value) \
{ \
	size_t r = EE_WRONGPARSER;

#define ENDParser \
	return r; \
}


/**
 * Parse a RFC3164 Date.
 */
BEGINParser(RFC3164Date)
	unsigned char *p;
	size_t len, orglen;
	/* variables to temporarily hold time information while we parse */
	int month;
	int day;
	int year = 0; /* 0 means no year provided */
	int hour; /* 24 hour clock */
	int minute;
	int second;

	assert(*offs < es_strlen(str));

	p = es_getBufAddr(str) + *offs;
	orglen = len = es_strlen(str) - *offs;
	/* If we look at the month (Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec),
	 * we may see the following character sequences occur:
	 *
	 * J(an/u(n/l)), Feb, Ma(r/y), A(pr/ug), Sep, Oct, Nov, Dec
	 *
	 * We will use this for parsing, as it probably is the
	 * fastest way to parse it.
	 */
	if(len < 3)
		goto fail;

	switch(*p++)
	{
	case 'j':
	case 'J':
		if(*p == 'a' || *p == 'A') {
			++p;
			if(*p == 'n' || *p == 'N') {
				++p;
				month = 1;
			} else
				goto fail;
		} else if(*p == 'u' || *p == 'U') {
			++p;
			if(*p == 'n' || *p == 'N') {
				++p;
				month = 6;
			} else if(*p == 'l' || *p == 'L') {
				++p;
				month = 7;
			} else
				goto fail;
		} else
			goto fail;
		break;
	case 'f':
	case 'F':
		if(*p == 'e' || *p == 'E') {
			++p;
			if(*p == 'b' || *p == 'B') {
				++p;
				month = 2;
			} else
				goto fail;
		} else
			goto fail;
		break;
	case 'm':
	case 'M':
		if(*p == 'a' || *p == 'A') {
			++p;
			if(*p == 'r' || *p == 'R') {
				++p;
				month = 3;
			} else if(*p == 'y' || *p == 'Y') {
				++p;
				month = 5;
			} else
				goto fail;
		} else
			goto fail;
		break;
	case 'a':
	case 'A':
		if(*p == 'p' || *p == 'P') {
			++p;
			if(*p == 'r' || *p == 'R') {
				++p;
				month = 4;
			} else
				goto fail;
		} else if(*p == 'u' || *p == 'U') {
			++p;
			if(*p == 'g' || *p == 'G') {
				++p;
				month = 8;
			} else
				goto fail;
		} else
			goto fail;
		break;
	case 's':
	case 'S':
		if(*p == 'e' || *p == 'E') {
			++p;
			if(*p == 'p' || *p == 'P') {
				++p;
				month = 9;
			} else
				goto fail;
		} else
			goto fail;
		break;
	case 'o':
	case 'O':
		if(*p == 'c' || *p == 'C') {
			++p;
			if(*p == 't' || *p == 'T') {
				++p;
				month = 10;
			} else
				goto fail;
		} else
			goto fail;
		break;
	case 'n':
	case 'N':
		if(*p == 'o' || *p == 'O') {
			++p;
			if(*p == 'v' || *p == 'V') {
				++p;
				month = 11;
			} else
				goto fail;
		} else
			goto fail;
		break;
	case 'd':
	case 'D':
		if(*p == 'e' || *p == 'E') {
			++p;
			if(*p == 'c' || *p == 'C') {
				++p;
				month = 12;
			} else
				goto fail;
		} else
			goto fail;
		break;
	default:
		goto fail;
	}

	len -= 3;

	/* done month */

	if(len == 0 || *p++ != ' ')
		goto fail;
	--len;

	/* we accept a slightly malformed timestamp when receiving. This is
	 * we accept one-digit days
	 */
	if(*p == ' ') {
		--len;
		++p;
	}

	day = hParseInt(&p, &len);
	if(day < 1 || day > 31)
		goto fail;

	if(len == 0 || *p++ != ' ')
		goto fail;
	--len;

	/* time part */
	hour = hParseInt(&p, &len);
	if(hour > 1970 && hour < 2100) {
		/* if so, we assume this actually is a year. This is a format found
		 * e.g. in Cisco devices.
		 *
		year = hour;
		*/

		/* re-query the hour, this time it must be valid */
		if(len == 0 || *p++ != ' ')
			goto fail;
		--len;
		hour = hParseInt(&p, &len);
	}

	if(hour < 0 || hour > 23)
		goto fail;

	if(len == 0 || *p++ != ':')
		goto fail;
	--len;
	minute = hParseInt(&p, &len);
	if(minute < 0 || minute > 59)
		goto fail;

	if(len == 0 || *p++ != ':')
		goto fail;
	--len;
	second = hParseInt(&p, &len);
	if(second < 0 || second > 60)
		goto fail;

	/* we provide support for an extra ":" after the date. While this is an
	 * invalid format, it occurs frequently enough (e.g. with Cisco devices)
	 * to permit it as a valid case. -- rgerhards, 2008-09-12
	 */
	if(len > 0 && *p == ':') {
		++p; /* just skip past it */
		--len;
	}

	/* we had success, so update parse pointer and caller-provided timestamp
	 * fields we do not have are not updated in the caller's timestamp. This
	 * is the reason why the caller must pass in a correct timestamp.
	 */
	size_t usedLen =  orglen - len;
	es_str_t *valstr = es_newStrFromSubStr(str, *offs, usedLen);
	*value = ee_newValue(ctx);
	ee_setStrValue(*value, valstr);
	*offs += usedLen;
printf("RFC3164 date parser: offs %u, len %u, usedLen %u\n", (unsigned) *offs, (unsigned) len,
	(unsigned) usedLen);
	r = 0; /* parsing was successful */
#if 0 /* TODO: see how we represent the actual timestamp */
	pTime->month = month;
	if(year > 0)
		pTime->year = year; /* persist year if detected */
	pTime->day = day;
	pTime->hour = hour;
	pTime->minute = minute;
	pTime->second = second;
#endif
fail:
ENDParser


/**
 * Parse a Number.
 * Note that a number is an abstracted concept. We always represent it
 * as 64 bits (but may later change our mind if performance dictates so).
 */
BEGINParser(Number)
	unsigned char *p;
	size_t len, orglen;
	long long n;


printf("parseNumber got '%s'\n", es_str2cstr(str, NULL)+ *offs);
	p = es_getBufAddr(str) + *offs;
	orglen = len = es_strlen(str) - *offs;

	n = hParseInt(&p, &len);
	if(p == es_getBufAddr(str))
		goto fail;

	if((*value = ee_newValue(ctx)) == NULL) {
		r = EE_NOMEM;
		goto fail;
	}

	/* success, persist */
	size_t usedLen =  orglen - len;
printf("in parseNumber offs %u, usedLen %u, orglen %d, len %u\n", (int) *offs, (int)usedLen, (int)orglen, (int)len);
	es_str_t *valstr = es_newStrFromSubStr(str, *offs, usedLen);
	ee_setStrValue(*value, valstr);
	*offs += usedLen;
	r = 0;
printf("in parseNumber offs %u, usedLen %u, orglen %d, len %u\n", (int) *offs, (int)usedLen, (int)orglen, (int)len);
	//(*newVal)->valtype = ee_valtype_nbr;
	//(*newVal)->val.number = n;
fail:
ENDParser


/**
 * Parse a word.
 * A word is a SP-delimited entity. The parser always works, except if
 * the offset is position on a space upon entry.
 */
BEGINParser(Word)
	unsigned char *c;
	size_t i;
	size_t len;	/**< length of substring we finally extract */
	es_str_t *valstr;

	assert(str != NULL);
	assert(offs != NULL);
	c = es_getBufAddr(str);
	i = *offs;

	/* search end of word */
	while(i < es_strlen(str) && c[i] != ' ') 
		i++;

	if(i == *offs) {
		r = EE_WRONGPARSER;
		goto done;
	}

	/* success, persist */
	len =  i - *offs;
	if(c[i] != ' ') { /* special case at end of string! */
		++len;
	}
	CHKN(*value = ee_newValue(ctx));
	CHKN(valstr = es_newStrFromSubStr(str, *offs, len));
	ee_setStrValue(*value, valstr);
	*offs = i;
	r = 0;

done:	return r;
ENDParser
