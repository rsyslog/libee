/**
 * @file event.h
 * @brief Representation of an event.
 * @class ee_event event.h
 *
 *//*
 *
 * Libee - An Event Expression Library inspired by CEE
 * Copyright 2010-2012 by Rainer Gerhards and Adiscon GmbH.
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
	unsigned objID;		/**< a magic number to prevent some memory adressing errors */
	ee_ctx	ctx;			/**< the library context */
	struct ee_tagbucket *tags;		/**< tags associated with this event */
	struct ee_fieldbucket *fields;	/**< fields contained in this event */
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
 * Create an event from a JSON string.
 *
 * @memberof ee_event
 * @public
 *
 * @param[in] ctx associated library context
 * @param[in] json JSON classical C-string to create event from
 *
 * @return new library context or NULL if an error occured
 */
struct ee_event* ee_newEventFromJSON(ee_ctx ctx, char *json);

/**
 * Destructor for the ee_event object.
 *
 * @memberof ee_event
 * @public
 *
 * @param event The event to be discarded.
 */
void ee_deleteEvent(struct ee_event *event);


/**
 * Assign a tag bucket to an event.
 *
 * A complete tag bucket is assigned to the event. Any previously
 * assigned tags are DISCARDED.
 *
 * @memberof ee_event
 * @public
 *
 * @param event event where tag shall be added
 * @param tagbucket  already-created tag bucket to be assigned to event.
 * 		The caller ceases control of this bucket. In particular,
 * 		we may destruct it at any time. If the caller intends to
 * 		continue access the tagbucket, it must properly create
 * 		a duplicate.
 *
 * @return	0 on success, something else otherwise.
 */
int ee_assignTagbucketToEvent(struct ee_event *event, struct ee_tagbucket *tagbucket);


/**
 * Add a tag to the event.
 *
 * The tag is provided as a string. If no tag bucket exists when
 * this method is called, one is created. 
 *
 * <b>This is part of the ezAPI for libee</b>
 *
 * @memberof ee_event
 * @public
 *
 * @param event event where tag shall be added
 * @param tag   string representing a tag name. The tag must \b not
 *              already exist inside the tagbucket. Libee will copy
 *              the string, so the caller must free it itself if required.
 *
 * @return	0 on success, something else otherwise.
 */
int ee_addTagToEvent(struct ee_event *event, es_str_t *tag);


/**
 * Add a string field name-value pair to the event. 
 *
 * This adds a name-value pair (NVField) to the event. The value must be
 * a string value. If no fieldbucket yet exists, one is created.
 *
 * <b>This is part of the ezAPI for libee</b>
 *
 * @memberof ee_event
 * @public
 *
 * @param event event where field shall be added
 * @param[in] fieldname Name of the field to be added. The field name must \b not
 *                  already exist inside the fieldbucket. Libee will copy
 *                  the field name, so the caller must free it itself if required.
 * @param[in] value value of the field to be added. 
 *
 * @return	0 on success, something else otherwise.
 */
int ee_addStrFieldToEvent(struct ee_event *event, char *fieldname, es_str_t *value);


/**
 * Add an already constructed field to the event. 
 *
 * @memberof ee_event
 * @public
 *
 * @param event event where field shall be added
 * @param[in] field field to be added
 *
 * @return	0 on success, something else otherwise.
 */
int ee_addFieldToEvent(struct ee_event *event, struct ee_field *field);


/**
 * Obtain a field with specified name from given event.
 *
 * @memberof ee_event
 * @public
 *
 * @param event event to search
 * @param[in] str name of field
 *
 * @return	NULL if field was not found (or an error occured);
 *              pointer to the field otherwise
 */
struct ee_field* ee_getEventField(struct ee_event *event, es_str_t *name);


/**
 * Obtain the string representaton of a field with specified name
 * from given event. The string representation is build in the current
 * default encoding (note: different encodings are NOT yet implemented
 * at the time of this writing).
 *
 * @memberof ee_event
 * @public
 *
 * @param event event to search
 * @param[in] name name of field
 * @param[out] strVal output string with field representation. If NULL
 *                    a new string is generated, the representation
 *                    is APPENDED to the existing string.
 *
 * @return	0 (EE_OK), if everything went well, EE_NOTFOUND if the
 * 		field could not be found and something else for other
 * 		errors.
 */
int ee_getEventFieldAsString(struct ee_event *event,
		es_str_t *name, es_str_t **strVal);


/**
 * Check if an event is classified via a specific tag.
 *
 * @memberof ee_event
 * @public
 *
 * @param[in] event event to look at
 * @param[in] str name of tag
 *
 * @return	0 if event is not classified with the tag, something
 * 		else otherwise
 */
int ee_EventHasTag(struct ee_event *event, es_str_t *tagname);


/**
 * Obtain the event's tagbucket.
 *
 * @memberof ee_event
 * @public
 *
 * @param[in] event event to process
 * @param[out] tagbucket associated tagbucket. May be NULL if none is
 *                       associated.
 */
void ee_EventGetTagbucket(struct ee_event *event, struct ee_tagbucket **tagbucket);


/**
 * Format an event in syslog RFC 5424 format.
 *
 * This method takes an event and creates a new string representation
 * in RFC5424 format. The string is passed to the caller, which then
 * is responsible for freeing it.
 *
 * <b>This is part of the ezAPI for libee</b>
 *
 * @memberof ee_event
 * @public
 *
 * @param event event to format
 * @param[out] str pointer to string with RFC5424 representation, caller must destruct

 * @return	0 on success, something else otherwise.
 */
int ee_fmtEventToRFC5424(struct ee_event *event, es_str_t **str);


/**
 * Format an event in JSON format.
 *
 * This method takes an event and creates a new string representation
 * in JSON format. The string is passed to the caller, which then
 * is responsible for freeing it.
 *
 * <b>This is part of the ezAPI for libee</b>
 *
 * @memberof ee_event
 * @public
 *
 * @param event event to format
 * @param[out] str pointer to string with JSON representation, caller must destruct

 * @return	0 on success, something else otherwise.
 */
int ee_fmtEventToJSON(struct ee_event *event, es_str_t **str);


/**
 * Format an event in XML format.
 *
 * This method takes an event and creates a new string representation
 * in XML format. The string is passed to the caller, which then
 * is responsible for freeing it.
 *
 * <b>This is part of the ezAPI for libee</b>
 *
 * @memberof ee_event
 * @public
 *
 * @param event event to format
 * @param[out] str pointer to string with XML representation, caller must destruct

 * @return	0 on success, something else otherwise.
 */
int ee_fmtEventToXML(struct ee_event *event, es_str_t **str);


/**
 * Format an event to CSV format.
 *
 * This method takes an event and creates a new string representation
 * in CSV format. The string is passed to the caller, which then
 * is responsible for freeing it. Note that this methods needs a string
 * specifying field names and order (as they shall be written to the
 * output).
 *
 * <b>This is part of the ezAPI for libee</b>
 *
 * @memberof ee_event
 * @public
 *
 * @param event event to format
 * @param[out] str pointer to string with XML representation, caller must destruct
 * @param[in] extraData string with field names (comma-delimited list)

 * @return	0 on success, something else otherwise.
 */
int ee_fmtEventToCSV(struct ee_event *event, es_str_t **str, es_str_t *extraData);

#endif /* #ifndef LIBEE_EVENT_H_INCLUDED */
