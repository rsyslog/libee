/**
 * @file tagset.h
 * @brief The CEE tagset object.
 * @class ee_tagset tagset.h
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
#ifndef LIBEE_TAGSET_H_INCLUDED
#define	LIBEE_TAGSET_H_INCLUDED

/**
 * The actual tagset object data structure.
 * 
 * @extends ee_obj
 *
 * TODO: is it correct that there is no short name in the CEE spec?
 */
struct ee_tagset {
	struct ee_obj o;	/*<< the base object */
};

/**
 * Constructor for the ee_tagset object.
 *
 * @memberof ee_tagset
 * @public
 *
 * @return new library context or NULL if an error occured
 */
struct ee_tagset* ee_newTagset(void);

/**
 * Destructor for the ee_tagset object.
 *
 * @memberof ee_tagset
 * @public
 *
 * @param tagset The tagset to be discarded.
 */
void ee_deleteTagset(struct ee_tagset *tagset);


#endif /* #ifndef LIBEE_TAGSET_H_INCLUDED */
