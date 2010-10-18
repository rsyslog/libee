/**
 * @file obj.h
 * @brief A base object containing some common plumbing
 * @class ee_obj obj.h
 *
 *//*
 *
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
#ifndef LIBEE_OBJ_H_INCLUDED
#define	LIBEE_OBJ_H_INCLUDED
#include "timestamp.h"

/**
 * A base object. It contains the common plumbing which is present in
 * almost all other EE objects.
 */
struct ee_obj {
	unsigned objID;
		/**< a magic number to prevent some memory adressing errors */
	char *name;
	char *shortName;
	char *descr;
	struct ee_timestamp updated;
};

/**
 * Constructor for the ee_obj object.
 *
 * @memberof ee_obj
 * @public
 *
 * @return new library context or NULL if an error occured
 */
struct ee_obj* ee_newObj(void);

/**
 * Destructor for the ee_obj object.
 *
 * @memberof ee_obj
 * @public
 *
 * @param obj The obj to be discarded.
 */
void ee_deleteObj(struct ee_obj *obj);


#endif /* #ifndef LIBEE_OBJ_H_INCLUDED */
