/**
 * @file field.h
 * @brief The CEE nvfield object.
 * @class ee_field field.h
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
#ifndef LIBEE_FIELD_H_INCLUDED
#define	LIBEE_FIELD_H_INCLUDED

/**
 * The Field object.
 *
 * Note that in CEE terms, this is called a "nvfield". 
 *
 * This represents a name-value pair, whereby the name should correspond
 * to a valid field type. However, depending on compliance level, name
 * may not point to a valid field. For this reason, we do not require
 * a pointer to the proper field definition.
 */
struct ee_field {
	int objID;		/**< magic number to identify the object */
	ee_ctx ctx;		/**< associated library context */
	char	*name;		/**< the field name */
	union ee_value *val;	/**< value assigned to this field */
};

/**
 * Constructor for the ee_field object.
 *
 * @memberof ee_field
 * @public
 *
 * @param[in] ctx library context
 *
 * @return pointer to new object or NULL if an error occured
 */
struct ee_field* ee_newField(ee_ctx ctx);


/**
 * Constructor an ee_field object from a name value pair.
 * TODO: is this legacy or do we need it in the future?
 *
 * @memberof ee_field
 * @public
 *
 * @param[in] ctx library context
 * @param[in] name field name
 * @param[in] val value
 *
 * @return new field or NULL if an error occured
 */
struct ee_field* ee_newFieldFromNV(ee_ctx ctx, char *name, union ee_value *val);

/**
 * Destructor for the ee_field object.
 *
 * @memberof ee_field
 * @public
 *
 * @param[in] field object to be destructed
 *
 * @param field The field to be discarded.
 */
void ee_deleteField(struct ee_field *field);


#endif /* #ifndef LIBEE_FIELD_H_INCLUDED */
