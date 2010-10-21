/**
 * @file fieldset.h
 * @brief The CEE FieldSet Object.
 * @class ee_fieldset fieldset.h
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
#ifndef LIBEE_FIELDSET_H_INCLUDED
#define	LIBEE_FIELDSET_H_INCLUDED

/**
 * The CEE FieldSet Object.
 * @extends ee_obj
 */
struct ee_fieldset {
	struct ee_obj o;	/*<< @protected base object */
};

/**
 * Constructor for the ee_fieldset object.
 *
 * @memberof ee_fieldset
 * @public
 *
 * @return new library context or NULL if an error occured
 */
struct ee_fieldset* ee_newFieldset(void);

/**
 * Destructor for the ee_fieldset object.
 *
 * @memberof ee_fieldset
 * @public
 *
 * @param fieldset The fieldset to be discarded.
 */
void ee_deleteFieldset(struct ee_fieldset *fieldset);


#endif /* #ifndef LIBEE_FIELDSET_H_INCLUDED */
