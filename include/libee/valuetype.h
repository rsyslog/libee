/**
 * @file valuetype.h
 * @brief The CEE valueType object.
 * @class ee_valueType valuetype.h
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
#ifndef LIBEE_VALUETYPE_H_INCLUDED
#define	LIBEE_VALUETYPE_H_INCLUDED

/**
 * The CEE ValueType object.
 * @extends ee_obj
 */
struct ee_valueType {
	struct ee_obj o;	/*<< the base object */
	// TODO: restrictions and base types
};

/**
 * Constructor for the ee_valueType object.
 *
 * @memberof ee_valueType
 * @public
 *
 * @return new library context or NULL if an error occured
 */
struct ee_valueType* ee_newValueType(void);

/**
 * Destructor for the ee_valueType object.
 *
 * @memberof ee_valueType
 * @public
 *
 * @param valueType The valueType to be discarded.
 */
void ee_deleteValueType(struct ee_valueType *valueType);


#endif /* #ifndef LIBEE_VALUETYPE_H_INCLUDED */
