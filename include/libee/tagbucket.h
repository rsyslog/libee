/**
 * @file tagbucket.h
 * @brief the tagbucket object
 * @class ee_tagbucket tagbucket.h
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
#ifndef LIBEE_TAGBUCKET_H_INCLUDED
#define	LIBEE_TAGBUCKET_H_INCLUDED

/**
 * Internal structure to represent linked list nodes for the tagbucket.
 */
struct ee_tagbucket_listnode {
	char *name;	/* TODO: use es_str_t! */
	struct ee_tagbucket_listnode *next;
};

/**
 * The tagbucket class, a container to store tags.
 */
struct ee_tagbucket {
	unsigned objID;		/**< a magic number to prevent some memory adressing errors */
	ee_ctx ctx;		/**< associated library context */
	struct ee_tagbucket_listnode *root; /**< root of our tags list */
	struct ee_tagbucket_listnode *tail; /**< list tail to speed up adding nodes */
};

/**
 * Constructor for the ee_tagbucket object.
 *
 * @memberof ee_tagbucket
 * @public
 *
 * @param[in] ctx the library context to use
 *
 * @return newly created object or NULL if an error occured
 */
struct ee_tagbucket* ee_newTagbucket(ee_ctx ctx);

/**
 * Destructor for the ee_tagbucket object.
 *
 * @memberof ee_tagbucket
 * @public
 *
 * @param[in] tagbucket The tagbucket to be discarded.
 */
void ee_deleteTagbucket(struct ee_tagbucket *tagbucket);

/**
 * Add a tag (string) to the bucket.
 *
 * @memberof ee_tagbucket
 * @public
 *
 * @param[in] tagbucket	the tagbucket to modify
 * @param[in] tagname	name of the tag to be added
 *
 * @return 0 on success, something else otherwise
 */
int ee_addTagToBucket(struct ee_tagbucket *tagbucket, char *tagname);


/**
 * Iterate over all tags inside the bucket and call a user-defined function
 * on each tag.
 *
 * @memberof ee_tagbucket
 * @public
 *
 * @param[in] tagbucket	the tagbucket to modify
 * @param[in] f	function to be called, must accept cookie as first param,
 * 		pointer to tag name as second.
 * @param[in] cookie cookie to be passed to user-defined function
 *
 * @return number of tags processed (= nbr of function calls)
 */
void ee_iterateOverBucketTags(struct ee_tagbucket *tagbucket, void(*f)(void*,char *), void *cookie);

#endif /* #ifndef LIBEE_TAGBUCKET_H_INCLUDED */
