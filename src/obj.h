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
#include <string.h> 	/* needed for memset() */
#include "ctx.h"
#include "timestamp.h"

/**
 * A base object. It contains the common plumbing which is present in
 * almost all other EE objects.
 */
struct ee_obj {
	unsigned objID;
		/**< a magic number to prevent some memory adressing errors */
	ee_ctx ctx;	/**< the library context */
	char *name;
	char *shortName;
	char *descr;
	struct ee_timestamp updated;
};

/**
 * Initializer for the ee_obj object.
 * This is much like the typical constructor, but it does not malloc() the object.
 * Rather it assumes the object must already exist. This is always the case, as the
 * ee_obj struct (and NOT a pointer) MUST be the first data element of any
 * derived object. Not malloc'ing it obviously safes us a lot of overhead
 * (also think about cache locality).
 *
 * @memberof ee_obj
 * @public
 *
 * @param[in] ctx associated library context
 * @param[in] pThis pointer to uninitialized ee_obj
 */
/* for now, we use an inline function (as it is only called from within
 * the library, but this may change depending on needs.
 */
static inline void
ee_initObj(ee_ctx ctx, struct ee_obj* pThis)
{
	memset(pThis, 0, sizeof(struct ee_obj));
	pThis->ctx = ctx;
}


/**
 * De-Initializer for the ee_obj object.
 * "Destructs" the object, but does not free any memory (this
 * is dual to ee_initObj().
 *
 * @memberof ee_obj
 * @public
 *
 * @param obj The obj to be discarded.
 */
/* for now, we use an inline function (as it is only called from within
 * the library, but this may change depending on needs.
 */
static inline void
ee_deinitObj(struct ee_obj __attribute__((unused)) *pThis)
{
	/* currently nothing to do */;
}


#endif /* #ifndef LIBEE_OBJ_H_INCLUDED */
