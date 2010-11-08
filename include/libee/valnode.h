/**
 * @file valnode.h
 * @brief An object to represent a value node inside a list of values.
 * @class ee_valnode valnode.h
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
#ifndef LIBEE_VALNODE_H_INCLUDED
#define	LIBEE_VALNODE_H_INCLUDED

/**
 * An object to represent node inside a list of values.
 * This probably will only be used inside fields, but I have
 * created a separate class because I envision some other potential
 * uses for it.
 * This is NOT a core CEE object, but rather a libee helper entity.
 */
struct ee_valnode {
	unsigned objID;
		/**< a magic number to prevent some memory adressing errors */
	struct ee_value *val;
	struct ee_valnode *next;
};

/**
 * Constructor for the ee_valnode object.
 *
 * @memberof ee_valnode
 * @public
 *
 * @return new object or NULL if an error occured
 */
static inline struct ee_valnode*
ee_newValnode(void)
{
	struct ee_valnode* valnode;
	if((valnode = malloc(sizeof(struct ee_valnode))) == NULL) goto done;
	valnode->objID = ObjID_VALNODE;
	valnode->next = NULL;
done:
	return valnode;
}


/**
 * Destructor for the ee_valnode object.
 * Note: only the single object is destructed, \b not the complete node.
 * So the caller must ensure the last will not be broken.
 *
 * @memberof ee_valnode
 * @public
 *
 * @param valnode The valnode to be discarded.
 */
void ee_deleteValnode(struct ee_valnode *valnode);


#endif /* #ifndef LIBEE_VALNODE_H_INCLUDED */
