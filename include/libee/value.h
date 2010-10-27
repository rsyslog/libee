/**
 * @file value.h
 * @brief Values that can be stored for fields.
 * @class ee_value value.h
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
#ifndef LIBEE_VALUE_H_INCLUDED
#define	LIBEE_VALUE_H_INCLUDED

/**
 * The value class.
 * This represents a value that is to be stored together with a CEE field.
 * We do not have a typed system, but we need efficient representations for
 * various simple types. As such, we define one big union that contains
 * definitions for all value types envisible. If a new primitive type is
 * added, this union must be extended. There is NO type indicateor inside the
 * value class itself, because a value is always assigned to a field and so
 * the field type defines the value type as well (via its restrictions).
 * @note
 * This sounds a bit ugly and hard to debug. We should probably reconsider
 * this decision once we got a bit more experience with the system.
 */
union ee_value {
	struct ee_timestamp ts;
	long long number;
	char *str;
};


/**
 * Constructor for the ee_value object.
 *
 * @memberof ee_value
 * @public
 *
 * @param[in] ctx library context
 *
 * @return newly created object or NULL if an error occured
 */
union ee_value* ee_newValue(ee_ctx ctx);


/**
 * Destructor for the ee_value object.
 *
 * @memberof ee_value
 * @public
 *
 * @param[in] value object to be destructed
 */
void ee_deleteValue(union ee_value *value);


#endif /* #ifndef LIBEE_VALUE_H_INCLUDED */
