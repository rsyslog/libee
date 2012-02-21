/**
 * @file field.c
 * Implements field object methods.
 *//* Libee - An Event Expression Library inspired by CEE
 * Copyright 2010-2011 by Rainer Gerhards and Adiscon GmbH.
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

#define ERR_ABORT {r = 1; goto done; }

#define CHECK_FIELD \
	if(field->objID != ObjID_FIELD) { \
		r = -1; \
		goto done; \
	}

struct ee_field*
ee_newField(ee_ctx ctx)
{
	struct ee_field *field;
	if((field = malloc(sizeof(struct ee_field))) == NULL) goto done;
	field->objID = ObjID_FIELD;
	field->ctx = ctx;
	field->name = NULL;
	field->nVals = 0;
	field->valroot = field->valtail = NULL;
done:
	return field;
}


void
ee_deleteField(struct ee_field *field)
{
	struct ee_valnode *node, *nodeDel;

	assert(field->objID == ObjID_FIELD);
	es_deleteStr(field->name);
	if(field->nVals > 0) {
		ee_deleteValue(field->val);
	}
	if(field->nVals > 1) {
		node = field->valroot;
		while(node != NULL) {
			nodeDel = node;
			ee_deleteValue(nodeDel->val);
			free(nodeDel);
		}
	}
	free(field);
}

struct ee_field*
ee_newFieldFromNV(ee_ctx __attribute__((unused)) ctx, char *name, struct ee_value *val)
{
	struct ee_field *field;
	assert(val->objID == ObjID_VALUE);
	if((field = ee_newField(ctx)) == NULL) goto done;

	if((field->name = es_newStrFromCStr(name, strlen(name))) == NULL) {
		free(field);
		field = NULL;
		goto done;
	}

	field->val = val;
	field->nVals = 1;

done:
	return field;
}


/* In this version of the method, we simply create a copy of the field name. In
 * later versions, depending on our state and compliance level, we may use
 * a pointer to an in-memory representation of the dictionary entity instead.
 * rgerhards, 2010-10-26
 */
int
ee_nameField(struct ee_field *field, es_str_t *name)
{
	int r;
	assert(field->objID == ObjID_FIELD);
	if(field->name != NULL) {
		r = EE_FIELDHASNAME;
		goto done;
	}
	CHKN(field->name = es_strdup(name));
	r = 0;
done:
	return r;
}


int
ee_addValueToField(struct ee_field *field, struct ee_value *val)
{
	int r;
	struct ee_valnode *valnode;
	assert(field != NULL);assert(field->objID== ObjID_FIELD);
	assert(val != NULL);assert(val->objID == ObjID_VALUE);

	if(field->nVals == 0) {
		field->nVals = 1;
		field->val = val;
	} else if(field->nVals == LIBEE_CEE_MAX_VALS_PER_FIELD) {
		r = EE_TOOMANYVALUES;
		goto done;
	} else {
		/* we need to add to the list of values */
		CHKN(valnode = ee_newValnode());
		valnode->val = val;
		++field->nVals;
		if(field->valtail == NULL) {
			field->valroot = field->valtail = valnode;
		} else {
			field->valtail->next = valnode;
			field->valtail = valnode;
		}
	}
	r = 0;
done:
	return r;
}


int
ee_addStrValueToField(struct ee_field *field, es_str_t *str)
{
	int r;
	struct ee_value *value;
	assert(field != NULL);assert(field->objID== ObjID_FIELD);

	CHKN(value = ee_newValue(field->ctx));
	CHKR(ee_setStrValue(value, str));
	r = ee_addValueToField(field, value);
done:
	return r;
}


int
ee_getNumFieldVals(struct ee_field *field)
{
	assert(field != NULL);
	return(field->nVals);
}


/* TODO: this function currently assumes that the field has a string
 * representation, which for now is always true. Needs to be changed if
 * we change the representation!
 */
es_str_t*
ee_getFieldValueAsStr(struct ee_field *field, unsigned short n)
{
	es_str_t *str;
	assert(field != NULL);

	if(n >= field->nVals) {
		str = NULL;
		goto done;
	}
	if(n == 0) {
		str = es_strdup(field->val->val.str);
	} else {
		assert(0); // TODO: implement!
	}
done:
	return str;
}


/* TODO: this function currently assumes that the field has a string
 * representation, which for now is always true. Needs to be changed if
 * we change the representation!
 * TODO: implement (default) encoder interface
 */
int
ee_getFieldAsString(struct ee_field *field, es_str_t **str)
{
	int r = EE_ERR;
	struct ee_valnode *node;
	assert(field != NULL);

	if(*str == NULL) {
		CHKN(*str = es_newStr(16));
	}

	if(field->nVals == 0) {
		goto done;
	}
	/* first value needs to be treated seperately */
	CHKR(es_addStr(str, field->val->val.str));

	/* on to the rest */
	for(node = field->valroot ; node != NULL ; node = node->next) {
		CHKR(es_addStr(str, node->val->val.str));
	}

done:	return r;
}
