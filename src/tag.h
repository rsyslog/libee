/**
 * @file tag.h
 * @brief The CEE tag object.
 * @class ee_tag tag.h
 *
 *//*
 *
 * Libee - a CEE-based event expression library.
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
 * The actual tag object data structure.
 */
struct ee_tag {
	unsigned objID;
		/**< a magic number to prevent some memory adressing errors */
	char *name;
	char *shortName;
	char *descr;
	struct ee_namelist *altNames;
	struct ee_tagSet *tagset;
	// TODO: add Tag Relation Element
	struct ee_timestamp updated;
};

/**
 * Constructor for the ee_tag object.
 *
 * @memberof ee_tag
 * @public
 *
 * @return new library context or NULL if an error occured
 */
struct ee_tag* ee_newTag(void);

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
