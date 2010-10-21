/**
 * @file event.h
 * @brief Representation of an event.
 * @class ee_event event.h
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
#ifndef LIBEE_EVENT_H_INCLUDED
#define	LIBEE_EVENT_H_INCLUDED

/**
 * The event class.
 * This models an actual event as it happens.
 */
struct ee_event {
	ee_ctx	ctx;			/**< the library context */
	struct tagbucket *tags;		/**< tags associated with this event */
	struct fieldbucket *fields;	/**< fields contained in this event */
};

/**
 * Constructor for the ee_event object.
 *
 * @memberof ee_event
 * @public
 *
 * @param[in] ctx associated library context
 *
 * @return new library context or NULL if an error occured
 */
struct ee_event* ee_newEvent(ee_ctx ctx);

/**
 * Destructor for the ee_event object.
 *
 * @memberof ee_event
 * @public
 *
 * @param event The event to be discarded.
 */
void ee_deleteEvent(struct ee_event *event);


#endif /* #ifndef LIBEE_EVENT_H_INCLUDED */
