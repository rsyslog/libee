/**
 * @file tag.h
 * @brief The CEE tag object.
 * @class ee_tag tag.h
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
#ifndef LIBEE_TAG_H_INCLUDED
#define	LIBEE_TAG_H_INCLUDED

/**
 * The tag class.
 * @extends ee_obj
 */
struct ee_tag {
	struct ee_obj o;	/*<< the base object */
	/*struct ee_namelist *altNames;*/
	struct ee_tagSet *tagset;
	/* TODO?: add Tag Relation Element -- but first wait how CEE evolves */
};

/**
 * Constructor for the ee_tag object.
 *
 * @memberof ee_tag
 * @public
 *
 * @param[in] ctx associated library context
 *
 * @return new library context or NULL if an error occured
 */
struct ee_tag* ee_newTag(ee_ctx ctx);

/**
 * Destructor for the ee_tag object.
 *
 * @memberof ee_tag
 * @public
 *
 * @param tag The tag to be discarded.
 */
void ee_deleteTag(struct ee_tag *tag);


#endif /* #ifndef LIBEE_TAG_H_INCLUDED */
