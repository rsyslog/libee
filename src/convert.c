/**
 * @file int2syslog.c
 * @brief A tool to convert internal representation to syslog format
 *
 * @author Rainer Gerhards <rgerhards@adiscon.com>
 *
 *//*
 * Libee - An Event Expression Library inspired by CEE
 * Copyright 2010 by Rainer Gerhards and Adiscon GmbH.
 *
 * This file is part of liblognorm.
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
#include <stdio.h>
#include <getopt.h>
#include <libestr.h>
#include <assert.h>
#include "config.h"
#include "libee/libee.h"
#include "libee/int.h"
#include "libee/apache.h"
#include "libee/internal.h"

/* private forward definition for decoders without headers */
int ee_jsonDec(ee_ctx ctx, int (*cbGetLine)(es_str_t **ln),
              int (*cbNewEvt)(struct ee_event *event),
	      es_str_t **errMsg);


static ee_ctx ctx;
static FILE *fpIn;
static int verbose = 0;
enum codec { f_all, f_syslog, f_json, f_xml, f_int, f_apache, f_csv };
static enum codec encoder = f_syslog;
static enum codec decoder = f_int;
static es_str_t *decFmt = NULL; /**< a format string for decoder use */
static es_str_t *encFmt = NULL; /**< a format string for encoder use */

void
dbgCallBack(void __attribute__((unused)) *cookie, char *msg,
	    size_t __attribute__((unused)) lenMsg)
{
	printf("libee: %s\n", msg);
}

void errout(char *errmsg)
{
	fprintf(stderr, "%s\n", errmsg);
	exit(1);
}


/* callback that receives newly created event
 */
static int cbNewEvt(struct ee_event *event)
{
	es_str_t *out;
	char *cstr;

	switch(encoder) {
	case f_syslog:
		ee_fmtEventToRFC5424(event, &out);
		cstr = es_str2cstr(out, NULL);
		printf("%s\n", cstr);
		free(cstr);
		es_deleteStr(out);
		break;
	case f_json:
		ee_fmtEventToJSON(event, &out);
		cstr = es_str2cstr(out, NULL);
		printf("%s\n", cstr);
		free(cstr);
		es_deleteStr(out);
		break;
	case f_xml:
		ee_fmtEventToXML(event, &out);
		cstr = es_str2cstr(out, NULL);
		printf("%s\n", cstr);
		free(cstr);
		es_deleteStr(out);
		break;
	case f_csv:
		ee_fmtEventToCSV(event, &out, encFmt);
		cstr = es_str2cstr(out, NULL);
		printf("%s\n", cstr);
		free(cstr);
		es_deleteStr(out);
		break;
	case f_all:
	// TODO: add CSV!
		printf("\n");
		ee_fmtEventToRFC5424(event, &out);
		cstr = es_str2cstr(out, NULL);
		printf("syslog: %s\n", cstr);
		free(cstr);
		es_deleteStr(out);
		ee_fmtEventToJSON(event, &out);
		cstr = es_str2cstr(out, NULL);
		printf("json..: %s\n", cstr);
		free(cstr);
		es_deleteStr(out);
		ee_fmtEventToXML(event, &out);
		cstr = es_str2cstr(out, NULL);
		printf("xml...: %s\n", cstr);
		free(cstr);
		es_deleteStr(out);
		break;
	default:
		assert(0); /* if this happens, we have a program error */
		return -1;
	}

	ee_deleteEvent(event);
	return 0;
}

/* callback to read a line
 */
static int
cbGetLine(es_str_t **ln)
{
	int r;
	size_t len;
	char buf[64*1024];
	if(fgets(buf, sizeof(buf), fpIn) == NULL) {
		r = (feof(fpIn)) ? EE_EOF : -1;
		goto done;
	}
	len = strlen(buf);
	if(len > 0)
		len--;
	buf[len] = '\0'; /* strip '\n' */
	if(verbose)
		printf("Read line[%u] '%s'\n", (unsigned) len, buf);
	if((*ln = es_newStrFromCStr(buf, len)) == NULL) {
		r = EE_NOMEM;
		goto done;
	}
	if(decoder != f_json)
		es_unescapeStr(*ln);
	r = 0;
done:
	return r;
}


int main(int argc, char *argv[])
{
	int r;
	int opt;
	es_str_t *errmsg;
	char *cstr;
	char errbuf[1024];

	fpIn = stdin;

	if(strcmp(VERSION, ee_version())) {
		sprintf(errbuf, "loaded library version %s does not match "
		       "to-be-tested version %s.", VERSION, ee_version());
		errout(errbuf);
	}

	if((ctx = ee_initCtx()) == NULL) {
		errout("Could not initialize libee context");
	}
	ee_setDebugCB(ctx, dbgCallBack, NULL);

	while((opt = getopt(argc, argv, "c:i:ve:E:d:D:")) != -1) {
		switch (opt) {
		case 'i':
			if((fpIn = fopen(optarg, "r")) == NULL) {
				perror(optarg);
				exit(1);
			}
			break;
		case 'v':
			verbose = 1;
			break;
		case 'e': /* encoder to use */
			if(!strcmp(optarg, "json")) {
				encoder = f_json;
			} else if(!strcmp(optarg, "xml")) {
				encoder = f_xml;
			} else if(!strcmp(optarg, "csv")) {
				encoder = f_csv;
			} else if(!strcmp(optarg, "syslog")) {
				encoder = f_syslog;
			} else if(!strcmp(optarg, "all")) {
				encoder = f_all;
			}
			break;
		case 'd': /* decoder to use */
			if(!strcmp(optarg, "int")) {
				decoder = f_int;
			} else if(!strcmp(optarg, "apache")) {
				decoder = f_apache;
			} else if(!strcmp(optarg, "json")) {
				decoder = f_json;
			}
			break;
		case 'D': /* decoder-specific format string (will be validated by decoder) */ 
			decFmt = es_newStrFromCStr(optarg, strlen(optarg));
			break;
		case 'E': /* encoder-specific format string (will be validated by encoder) */ 
			encFmt = es_newStrFromCStr(optarg, strlen(optarg));
			break;
		case 'c': /* compactness of encoding */
			if(!strcmp(optarg, "ultra")) {
				ee_setEncUltraCompact(ctx);
			}
			break;
		default:
			printf("invalid option '%c' or value missing - "
			       "terminating...\n", opt);
			exit (1);
			break;
		}
	}

	switch(decoder) {
	case f_int:
		if((r = ee_intDec(ctx, cbGetLine, cbNewEvt, &errmsg)) != 0) {
			cstr = es_str2cstr(errmsg, NULL);
			snprintf(errbuf, sizeof(errbuf), "error %d in decoding stage: %s\n",
				 r, cstr);
			free(cstr);
			errout(errbuf);
		}
		break;
	case f_json:
		if((r = ee_jsonDec(ctx, cbGetLine, cbNewEvt, &errmsg)) != 0) {
			cstr = es_str2cstr(errmsg, NULL);
			snprintf(errbuf, sizeof(errbuf), "error %d in decoding stage: %s\n",
				 r, cstr);
			free(cstr);
			errout(errbuf);
		}
		break;
	case f_apache:
		{
		struct ee_apache *apache;
		apache = ee_newApache(ctx);
		if(ee_apacheNameList(ctx, apache, decFmt) != 0) {
			errout("error applying decoder format string");
		}
		if((r = ee_apacheDec(ctx, cbGetLine, cbNewEvt, &errmsg, apache)) != 0) {
			cstr = es_str2cstr(errmsg, NULL);
			snprintf(errbuf, sizeof(errbuf), "error %d in decoding stage: %s\n",
				 r, cstr);
			free(cstr);
			errout(errbuf);
		}
		ee_deleteApache(apache);
		}
		break;
	default:
		errout("program error, decoder not yet supported");
	}

	ee_exitCtx(ctx);
	return 0;
}
