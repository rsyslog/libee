/**
 * @file apache.h
 * The file file format class.
 *
 * @class ee_apache apache.h
 *
 * This class represents the apache common log file format.
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
#ifndef LIBEE_APACHE_H_INCLUDED
#define	LIBEE_APACHE_H_INCLUDED
#include <libestr.h>

typedef struct ee_fieldListApache_s ee_fieldListApache_t;

/**
 * List with names for fields in log file (in that order).
 */
struct ee_fieldListApache_s {
	es_str_t *name;		/**< field name */
	ee_fieldListApache_t *next;	/**< list housekeeping, next node (or NULL) */
};


/* parse tree object
 */
struct ee_apache {
	ee_fieldListApache_t	*nroot; /**< root of field name list */
	ee_fieldListApache_t	*ntail; /**< tail of field name list */
};


/**
 * Allocates and initializes a new parse tree node.
 * @memberof ee_apache
 *
 * @param[in] ctx current library context
 * @param[in] parent parent node of the current tree (NULL if root)
 *
 * @return pointer to new node or NULL on error
 */
struct ee_apache* ee_newApache(ee_ctx ctx);


/**
 * Free a parse tree node and destruct all members.
 * @memberof ee_apache
 *
 * @param[in] ctx current library context
 * @param[in] tree pointer to apache to free
 */
void ee_deleteApache(struct ee_apache *tree);

 

int
ee_apacheNameList(ee_ctx ctx, struct ee_apache *apache, es_str_t *str);

/**
 * Decode an apache common log file into a CEE structure.
 *
 * The interface is heavily callback-based. The idea is to provide
 * greatest flexibility by doing so.
 *
 * @memberof ee_apache
 * @public
 *
 * @param[in] ctx library context to use
 * @param[in] cbGetLine get next line to be processed. Returns
 *            0 if all went well, EE_EOF at end of file and something
 *            else otherwise.
 * @param[in] cbNewEvt callback for function that receives newly created
 *            events. It must return 0 on success and something else otherwise.
 * @param[out] errStr printable error message, provided only if an error
 *             occurs. If so, the caller must delete the provided pointer.
 * @returns 0 on success, something else otherwise
 */
// TODO: define a new interface, so that we have a decoder object
// As I would like to finish this work, I am adding a poitner
// to the apache object as a dirty solution.
int ee_apacheDec(ee_ctx ctx, int (*cbGetLine)(es_str_t **ln),
              int (*cbNewEvt)(struct ee_event *event),
	      es_str_t **errMsg, struct ee_apache *apache);

#endif /* #ifndef LIBEE_APACHE_H_INCLUDED */
