/**
 * @file int2syslog1.c
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
#include "config.h"
#include "libee/libee.h"
#include "libee/int.h"

static ee_ctx ctx;
static FILE *fpIn;
static int verbose = 0;

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

	ee_fmtEventToRFC5424(event, &out);
	printf("Formatted event: '%s'\n", es_str2cstr(out, NULL));
	es_deleteStr(out);

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
	unsigned char *cstr;
	char errbuf[1024];

	fpIn = stdin;
	while((opt = getopt(argc, argv, "i:v")) != -1) {
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
		default:
			printf("invalid option '%c' or value missing - "
			       "terminating...\n", opt);
			exit (1);
			break;
		}
	}

	if(strcmp(VERSION, ee_version())) {
		sprintf(errbuf, "loaded library version %s does not match "
		       "to-be-tested version %s.", VERSION, ee_version());
		errout(errbuf);
	}

	if((ctx = ee_initCtx()) == NULL) {
		errout("Could not initialize libee context");
	}
	ee_setDebugCB(ctx, dbgCallBack, NULL);

	if((r = ee_intDec(ctx, cbGetLine, cbNewEvt, &errmsg)) != 0) {
		cstr = es_str2cstr(errmsg, NULL);
		snprintf(errbuf, sizeof(errbuf), "error %d in decoding stage: %s\n",
			 r, cstr);
		free(cstr);
		errout(errbuf);
	}

	ee_exitCtx(ctx);
	return 0;
}
