/**
 * @file field.h
 * @brief The CEE field object.
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
 * The CEE Field object.
 * @extends ee_obj
 */
struct ee_field {
	struct ee_obj o;	/*<< the base object */
	struct ee_tagset *fromTagSet;
	struct ee_fieldset *fieldSet;
};

/**
 * Constructor for the ee_field object.
 *
 * @memberof ee_field
 * @public
 *
 * @return new library context or NULL if an error occured
 */
struct ee_field* ee_newField(void);

/**
 * Destructor for the ee_field object.
 *
 * @memberof ee_field
 * @public
 *
 * @param field The field to be discarded.
 */
void ee_deleteField(struct ee_field *field);


#endif /* #ifndef LIBEE_FIELD_H_INCLUDED */
