/**
 * @file fieldtype.h
 * @brief The fieldtype object.
 * @class ee_fieldtype fieldtype.h
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
#ifndef LIBEE_FIELDTYPE_H_INCLUDED
#define	LIBEE_FIELDTYPE_H_INCLUDED

/**
 * The Fieldtype class.
 * This class describes properties of fields, like the syntax they have.
 * It represents a dictionary object.
 * 
 * In v0.5 of the interim CEE spec, this was called a "Field".
 *
 * @extends ee_obj
 */
struct ee_fieldtype {
	struct ee_obj o;	/*<< the base object */
	struct ee_tagset *fromTagSet;
	struct ee_fieldtypeset *fieldtypeSet;
};

/**
 * Constructor for the ee_fieldtype object.
 *
 * @memberof ee_fieldtype
 * @public
 *
 * @return new library context or NULL if an error occured
 */
struct ee_fieldtype* ee_newFieldtype(void);

/**
 * Destructor for the ee_fieldtype object.
 *
 * @memberof ee_fieldtype
 * @public
 *
 * @param fieldtype The fieldtype to be discarded.
 */
void ee_deleteFieldtype(struct ee_fieldtype *fieldtype);


#endif /* #ifndef LIBEE_FIELDTYPE_H_INCLUDED */
