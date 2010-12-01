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

/**
 * Internal structure to represent linked list nodes for the fieldbucket.
 */
struct ee_fieldbucket_listnode {
	struct ee_field *field;
	struct ee_fieldbucket_listnode *next;
};

/**
 * The fieldbucket object, a container to store fields and their values.
 * Note that fields are stored inside a linked list in the field bucket.
 * This is good to preserve sequence and easily iterate over them, but
 * it does not offer good performance for random access. For the time
 * being, we do not have any random access at all, so we ignore this issue.
 * However, the cure is simple: as soon as we need random access, we add
 * a hashtable, and store key/pointer to listnode (or so) in it. That
 * way, we have some overhead, but otherwise the best of both worlds.
 */
struct ee_fieldbucket {
	unsigned objID;
		/**< a magic number to prevent some memory adressing errors */
	ee_ctx ctx;		/**< associated library context */
	struct ee_fieldbucket_listnode *root; /**< root of our field list */
	struct ee_fieldbucket_listnode *tail; /**< list tail to speed up adding nodes */
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


/**
 * Add a field to the bucket.
 *
 * @memberof ee_fieldbucket
 * @public
 *
 * @param[in] bucket	the bucket to modify
 * @param[in] field	field to be added
 *
 * @return 0 on success, something else otherwise
 */
int ee_addFieldToBucket(struct ee_fieldbucket *fieldbucket, struct ee_field *field);


/**
 * Obtain a field with specified name from given bucket.
 *
 * @memberof ee_fieldbucket
 * @public
 *
 * @param bucket bucket to search
 * @param[in] str name of field
 *
 * @return	NULL if field was not found (or an error occured);
 *              pointer to the field otherwise
 */
struct ee_field* ee_getBucketField(struct ee_fieldbucket *bucket, es_str_t *name);

#endif /* #ifndef LIBEE_FIELDBUCKET_H_INCLUDED */
