/**
 * @file nvfield.h
 * @brief The CEE nvfield object.
 * @class ee_nvfield nvfield.h
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
#ifndef LIBEE_NVFIELD_H_INCLUDED
#define	LIBEE_NVFIELD_H_INCLUDED

/**
 * The NVField object.
 *
 * This represents a name-value pair, whereby the name should correspond
 * to a valid field type. However, depending on compliance level, name
 * may not point to a valid field. For this reason, we do not require
 * a pointer to the proper field definition.
 */
struct ee_nvfield {
	int objID;		/**< magic number to identify the object */
	ee_ctx ctx;		/**< associated library context */
	char	*name;		/**< the field name */
	union ee_value *val;	/**< value assigned to this field */
};

/**
 * Constructor for the ee_nvfield object.
 *
 * @memberof ee_nvfield
 * @public
 *
 * @param[in] ctx library context
 *
 * @return new library context or NULL if an error occured
 */
struct ee_nvfield* ee_newNVField(ee_ctx ctx);


/**
 * Destructor for the ee_nvfield object.
 *
 * @memberof ee_nvfield
 * @public
 *
 * @param[in] nvfield object to be destructed
 *
 * @param nvfield The nvfield to be discarded.
 */
void ee_deleteNVField(struct ee_nvfield *nvfield);


#endif /* #ifndef LIBEE_NVFIELD_H_INCLUDED */
