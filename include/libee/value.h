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
 * added, this union must be extended.
 * Note that we need to have a type indicator, as we do not always have a
 * loaded dictionary (or the library may be in non-validating mode!).
 */
struct ee_value {
	unsigned objID;
		/**< a magic number to prevent some memory adressing errors */
	enum {
		ee_valtype_none = 0,
		ee_valtype_str = 1,
		ee_valtype_nbr = 2
	} valtype;	/**< type of the value, selects union member */
	union {
		struct ee_timestamp ts;
		long long number;
		es_str_t *str;
	} val;		/**< the actual value */
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
struct ee_value* ee_newValue(ee_ctx ctx);


/**
 * Destructor for the ee_value object.
 *
 * @memberof ee_value
 * @public
 *
 * @param[in] value object to be destructed
 */
void ee_deleteValue(struct ee_value *value);

/**
 * Set the value to the provided string.
 * This functions is \b preliminary and must be
 * replace by one with better code / calling interface.
 *
 * @memberof ee_value
 * @public
 *
 */
int ee_setStrValue(struct ee_value *value, es_str_t *val);

/**
 * Encode the current value in syslog format and add it to the provided string.
 * If just the plain value is required, an empty string must be passed
 * in. The interface is defined in a away that prevents multiple write
 * operations when constructing event output formats, which usually
 * involve concatenation.
 *
 * 
 * @memberof ee_value
 * @public
 *
 * @param[in] val value to enocde
 * @param[out]  str string to wich the encoded value is to be added.
 * 		   Must have been allocated by the caller.
 * @returns 0 on success, something else otherwise
 */
int ee_addValue_Syslog(struct ee_value *value, es_str_t **str);


/**
 * Encode the current value in XML format and add it to the provided string.
 * If just the plain value is required, an empty string must be passed
 * in. The interface is defined in a away that prevents multiple write
 * operations when constructing event output formats, which usually
 * involve concatenation.
 *
 * 
 * @memberof ee_value
 * @public
 *
 * @param[in] val value to enocde
 * @param[out]  str string to wich the encoded value is to be added.
 * 		   Must have been allocated by the caller.
 * @returns 0 on success, something else otherwise
 */
int ee_addValue_XML(struct ee_value *value, es_str_t **str);

#endif /* #ifndef LIBEE_VALUE_H_INCLUDED */
