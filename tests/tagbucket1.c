/**
 * @file tagbucket1.c
 * @brief A very basic test for the tagbucket class.
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
#include <stdio.h>
#include <getopt.h>
#include "config.h"
#include "libee.h"

static ee_ctx ctx;

void
TagOutputCB(void __attribute__((unused)) *cookie, char *tagname)
{
	printf("%s\n", tagname);
}

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


int main(int argc, char *argv[])
{
	int opt;
	FILE *fpIn = stdin;
	char lnbuf[1024];
	struct ee_tagbucket *tagbucket;

	while((opt = getopt(argc, argv, "i:")) != -1) {
		switch (opt) {
		case 'i':
			if((fpIn = fopen(optarg, "r")) == NULL) {
				perror(optarg);
				exit(1);
			}
			break;
		default:
			printf("invalid option '%c' or value missing - "
			       "terminating...\n", opt);
			exit (1);
			break;
		}
	}

	if(strcmp(VERSION, ee_version())) {
		char buf[1024];
		sprintf(buf, "loaded library version %s does not match "
		       "to-be-tested version %s.", VERSION, ee_version());
		errout(buf);
	}

	if((ctx = ee_initCtx()) == NULL) {
		errout("Could not initialize libee context");
	}

	ee_setDebugCB(ctx, dbgCallBack, NULL);

	if((tagbucket = ee_newTagbucket(ctx)) == NULL)
		errout("could not create tag bucket");
	/* add tags */
	while(!feof(fpIn)) {
		if(fgets(lnbuf, sizeof(lnbuf), fpIn) != NULL) {
			lnbuf[strlen(lnbuf)-1] = '\0'; /* strip '\n' */
			ee_addTagToBucket(tagbucket, lnbuf);
		}
	}

	ee_iterateOverBucketTags(tagbucket, TagOutputCB, NULL);
	ee_deleteTagbucket(tagbucket);

	ee_exitCtx(ctx);
	return 0;
}
