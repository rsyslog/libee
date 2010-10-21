/**
 * @file namelist.h
 * @brief An object to represent list of names.
 * @class ee_namelist namelist.h
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
#ifndef LIBEE_NAMELIST_H_INCLUDED
#define	LIBEE_NAMELIST_H_INCLUDED

/**
 * An object to represent list of names.
 *
 * @extends ee_obj
 *
 * This is NOT a core CEE object, but rather a libee helper entity.
 */
struct ee_namelist {
	unsigned objID;
		/**< a magic number to prevent some memory adressing errors */
	char *name;
	struct ee_namelist *next;
};

/**
 * Constructor for the ee_namelist object.
 *
 * @memberof ee_namelist
 * @public
 *
 * @return new library context or NULL if an error occured
 */
struct ee_namelist* ee_newNamelist(void);

/**
 * Destructor for the ee_namelist object.
 *
 * @memberof ee_namelist
 * @public
 *
 * @param namelist The namelist to be discarded.
 */
void ee_deleteNamelist(struct ee_namelist *namelist);


#endif /* #ifndef LIBEE_NAMELIST_H_INCLUDED */
