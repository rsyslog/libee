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
#include "libee/valnode.h"

/**
 * The Field object.
 *
 * Note that in CEE terms, this is called a "nvfield". 
 *
 * This represents a name-value pair, whereby the name should correspond
 * to a valid field type. However, depending on compliance level, name
 * may not point to a valid field. For this reason, we do not require
 * a pointer to the proper field definition.
 *
 * Fields may contain a variable number of values. However, the by far
 * most common case is exactly one value. To support this effciently, we
 * store the first value directly within the structure and the 2nd+ in
 * a linked list.
 */
struct ee_field {
	unsigned objID;		/**< magic number to identify the object */
	ee_ctx ctx;		/**< associated library context */
	es_str_t *name;		/**< the field name */
	unsigned char nVals;	/**< number of values */
	struct ee_value *val;	/**< value assigned to this field */
	struct ee_valnode *valroot;	/**< list for 2nd+ values */
	struct ee_valnode *valtail;	/**< tail of the value list (for fast insert) */
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
struct ee_field* ee_newFieldFromNV(ee_ctx ctx, char *name, struct ee_value *val);

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


/**
 * Name a field.
 * Set the field name. This MUST NOT be called if a field name has
 * already been established. Note that if the library is in validating mode,
 * it may check the field name against the dictionary.
 *
 * @memberof ee_field
 * @public
 *
 * @param[in] ctx library context
 * @param[in] str field name
 *
 * @return 0 on success, something else otherwise
 */
int ee_nameField(struct ee_field *field, es_str_t *name);


/**
 * Add a value to a field.
 * Add the provided value to the list of field values. The value will
 * be added as the \b last member of the field list (so calling sequence
 * is important!). In validating mode, the library will check value
 * cardinality against the dictionary.
 *
 * @memberof ee_field
 * @public
 *
 * @param[in] ctx library context
 * @param[in] val value to add to field
 *
 * @return 0 on success, something else otherwise
 */
int ee_addValueToField(struct ee_field *field, struct ee_value *val);


/**
 * Add a string value to a field.
 * This is primarily a shortcut to add a string value. For more
 * details, see ee_addValueToField() doc, which also applies here.
 *
 * @memberof ee_field
 * @public
 *
 * @param[in] field field to update
 * @param[in] str string to add
 *
 * @return 0 on success, something else otherwise
 */
int ee_addStrValueToField(struct ee_field *field, es_str_t *str);


/**
 * Encode the current field with all its values in syslog format
 * and append this representation to the provided string.
 * 
 * @memberof ee_value
 * @public
 *
 * @param[in] field field to enocde
 * @param[out]  str string to wich the encoded value is to be added.
 * 		   Must have been allocated by the caller.
 * @returns 0 on success, something else otherwise
 */
int ee_addField_Syslog(struct ee_field *value, es_str_t **str);


/**
 * Encode the current field with all its values in XML format
 * and append this representation to the provided string.
 * 
 * @memberof ee_value
 * @public
 *
 * @param[in] field field to enocde
 * @param[out]  str string to wich the encoded value is to be added.
 * 		   Must have been allocated by the caller.
 * @returns 0 on success, something else otherwise
 */
int ee_addField_XML(struct ee_field *value, es_str_t **str);


/**
 * Get the number of values the field has.
 *
 * @memberof ee_value
 * @public
 *
 * @param[in] field relevant field
 * @returns number of values
 */
int ee_getNumFieldVals(struct ee_field *field);

/**
 * Get a specific value from this field as a string.
 *
 * @memberof ee_value
 * @public
 *
 * @param[in] field relevant field
 * @param[in] n number of the field to return, zero-based (like C arrays)
 *
 * @returns string representation of the n-th field value or NULL in
 * 	case of error
 */
es_str_t* ee_getFieldValueAsStr(struct ee_field *field, unsigned short n);

/**
 * Obtain the string representaton of a field.
 * The string representation is build in the current
 * default encoding (note: different encodings are NOT yet implemented
 * at the time of this writing).
 *
 * @memberof ee_value
 * @public
 *
 * @param[in] field relevant field
 * @param[out] str output string with field representation. If NULL
 *                    a new string is generated, the representation
 *                    is APPENDED to the existing string.
 *
 * @returns 0 (EE_OK) on success, error code otherwise
 */
int ee_getFieldAsString(struct ee_field *field, es_str_t **str);

#endif /* #ifndef LIBEE_FIELD_H_INCLUDED */
