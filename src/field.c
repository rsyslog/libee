/**
 * @file field.c
 * Implements field object methods.
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
done:
	return field;
}


void
ee_deleteField(struct ee_field *field)
{
	assert(field->objID == ObjID_FIELD);
	free(field);
}

/* In this version of the method, we simply create a copy of the field name. In
 * later versions, depending on our state and compliance level, we may use
 * a pointer to an in-memory representation of the dictionary entity instead.
 * rgerhards, 2010-10-26
 */
struct ee_field*
ee_newFieldFromNV(ee_ctx __attribute__((unused)) ctx, char *name, union ee_value *val)
{
	struct ee_field *field;
	assert(val == ObjID_VALUE);
	if((field = ee_newField(ctx)) == NULL) goto done;

	if((field->name = strdup(name)) == NULL) {
		free(field);
		field = NULL;
		goto done;
	}

	field->val = val;

done:
	return field;
}

