/**
 * @file fieldbucket.h
 * @brief the fieldbucket object
 * @class ee_fieldbucket fieldbucket.h
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
#ifndef LIBEE_FIELDBUCKET_H_INCLUDED
#define	LIBEE_FIELDBUCKET_H_INCLUDED

#include <libxml/hash.h>

/**
 * The fieldbucket object, a container to store fields and their values.
 */
struct ee_fieldbucket {
	unsigned objID;
		/**< a magic number to prevent some memory adressing errors */
	ee_ctx ctx;		/**< associated library context */
	xmlHashTablePtr ht;	/**< this *is* the actual bucket ;) */
};

/**
 * Constructor for the ee_fieldbucket object.
 *
 * @memberof ee_fieldbucket
 * @public
 *
 * @param[in] ctx the library context to use
 *
 * @return new library context or NULL if an error occured
 */
struct ee_fieldbucket* ee_newFieldbucket(ee_ctx ctx);

/**
 * Destructor for the ee_fieldbucket object.
 *
 * @memberof ee_fieldbucket
 * @public
 *
 * @param[in] fieldbucket the fieldbucket to destroy
 *
 * @param fieldbucket The fieldbucket to be discarded.
 */
void ee_deleteFieldbucket(struct ee_fieldbucket *fieldbucket);


#endif /* #ifndef LIBEE_FIELDBUCKET_H_INCLUDED */
