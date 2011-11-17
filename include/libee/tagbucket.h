/**
 * @file tagbucket.h
 * @brief the tagbucket object
 * @class ee_tagbucket tagbucket.h
 *
 *//*
 *
 * Libee - An Event Expression Library inspired by CEE
 * Copyright 2010-2011 by Rainer Gerhards and Adiscon GmbH.
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
	es_str_t *name;
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
	unsigned refCount;
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
 * Add an additional reference to the tagbucket. Use this whenever
 * an additional part of the code needs a READ-ONLY copy of the
 * tag bucket. Note: The delete function checks the reference count and
 * only deletes if it is down to zero.
 *
 * @memberof ee_tagbucket
 * @public
 *
 * @param[in] tagbucket the tagbucket to add ref for
 *
 * @return address of ref-added tagbucket (for convenience)
 */
struct ee_tagbucket* ee_addRefTagbucket(struct ee_tagbucket *tagbucket);

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
int ee_addTagToBucket(struct ee_tagbucket *tagbucket, es_str_t *tagname);

/**
 * Check if the tagbucket contains a specific tag.
 *
 * @memberof ee_tagbucket
 * @public
 *
 * @param[in] tagbucket	the tagbucket to check
 * @param[in] tagname	name of the tag
 *
 * @return 0 if tag not present, something else otherwise
 */
int ee_TagbucketHasTag(struct ee_tagbucket *tagbucket, es_str_t *tagname);

/**
 * Iterate over all tags inside a tag bucket.
 * On initial entry, cookie must be set to zero. The cookie returned
 * from each call must be passed into the next call. If the returned 
 * cookie is NULL, no more tags are available (this may happen immediately
 * if the tag bucket is empty). So the proper terminating condition to
 * check for is returned cookie == NULL. The actual return code
 * shall only be check for errors. Note that it is NOT permitted and
 * can lead to fatal errors if the tagbucket is changed between calls.
 * If the tagbucket changes, an initial call with a NULL-cookie must be
 * made.
 *
 * @memberof ee_value
 * @public
 *
 * @param[in] tagbucket tagbucket to process
 * @param[in/out] cookie cookie - for details see description
 * @param[out] tagname name of tag
 * @return 0 if tag not present, something else otherwise
 */
int ee_TagbucketGetNextTag(struct ee_tagbucket *tagbucket, void **cookie, es_str_t **tagname);

#endif /* #ifndef LIBEE_TAGBUCKET_H_INCLUDED */
