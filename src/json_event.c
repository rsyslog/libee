/**
 * @file json_event.c
 * Supports creating an event out of an JSON string.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libestr.h"
#include "libee/libee.h"
#include "libee/fieldbucket.h"
#include "cjson/cjson.h"

int
callback(struct ee_fieldbucket *fields, char *name,int type,cJSON *item)
{
	struct ee_field *f;
	struct ee_value *val;
	char *valstr = NULL;
	es_str_t *estr;

//printf("callback: type %d, name %s\n", type, name);
	if(type == cJSON_Object)
		return 1; // TODO: support!

	if(type == cJSON_String) {
		valstr = item->valuestring;
	} else if(type == cJSON_Number) {
		valstr = cJSON_print_number(item);
	} else if(type == cJSON_NULL) {
		valstr = "-";
	} else if(type == cJSON_False) {
		valstr = "false";
	} else if(type == cJSON_True) {
		valstr = "true";
	}
//printf("callback: string value %s\n", valstr);
	
	estr = es_newStrFromCStr(valstr, strlen(valstr));
	val = ee_newValue(fields->ctx);
	ee_setStrValue(val, estr);
	f = ee_newFieldFromNV(fields->ctx, name, val);
	ee_addFieldToBucket(fields, f);

	if(type == cJSON_Number)
		free(valstr);
	return 1;
}


void parse_and_callback(struct ee_fieldbucket *fields, cJSON *item, char *prefix)
{
	char *name;
	char *newprefix;
	int lenprefix;
	int bNeedFree;
	int dorecurse;
//printf("parse_and_callback, item %p, item->string %p, prefix(%d): '%s'\n", item, item->string, strlen(prefix), prefix);
	while (item)
	{
		lenprefix = strlen(prefix);
		if(lenprefix == 0) {
			newprefix = (item->string == NULL) ? "" : item->string;
			bNeedFree = 0;
		} else {
			name = (item->string == NULL) ? "*" : item->string;
			newprefix=malloc(strlen(prefix)+strlen(name)+2);
			sprintf(newprefix,"%s.%s",prefix,name);
			bNeedFree = 1;
		}
		dorecurse = callback(fields, newprefix, item->type, item);
		if (item->child && dorecurse)
			parse_and_callback(fields, item->child,newprefix);
		item=item->next;
		if(bNeedFree)
			free(newprefix);
	}
}

struct ee_event*
ee_newEventFromJSON(ee_ctx ctx, char *str)
{
	struct cJSON *json;
	struct ee_event *e = NULL;

	json = cJSON_Parse(str);
	if(json == NULL)
		goto done;
	e = ee_newEvent(ctx);
	e->fields = ee_newFieldbucket(ctx);
	parse_and_callback(e->fields, json, "");
	cJSON_Delete(json);
done:
	return e;
}
