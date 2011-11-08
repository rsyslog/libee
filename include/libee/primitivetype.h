/**
 * @file primitivetype.h
 * @brief Primitive data types.
 * @class ee_primitiveType primitivetype.h
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
#ifndef LIBEE_PRIMITIVETYPE_H_INCLUDED
#define	LIBEE_PRIMITIVETYPE_H_INCLUDED

/**
 * Representation of a primitive data type.
 *
 * @extends ee_obj
 *
 * A primitive data type is also called a "base type" in CEE. It is at the root
 * of all type hierarchie. Note that libee primitives include types which do
 * not seem to be very primitive from a generic programming language type of
 * view. For us, primitives are those that are very frequently used on logs.
 * We also try to avoid regexes, as these can be very time-consuming. So we
 * have things like IPv4Address or FQDN names as primitive types.
 */
struct ee_primitiveType {
	struct ee_obj o;	/*<< the base object */
	int (*parse)(ee_ctx ctx, char *p, struct ee_field* newField);
};

/**
 * Constructor for the ee_primitiveType object.
 *
 * @memberof ee_primitiveType
 * @public
 *
 * @return new library context or NULL if an error occured
 */
struct ee_primitiveType* ee_newPrimitiveType(void);

/**
 * Destructor for the ee_primitiveType object.
 *
 * @memberof ee_primitiveType
 * @public
 *
 * @param primitiveType The primitiveType to be discarded.
 */
void ee_deletePrimitiveType(struct ee_primitiveType *primitiveType);

/**
 * Parser interface.
 * @param[in] ctx current context
 * @param[in] str input string
 * @param[in/out] offs offset where parsing has to start inside str.
 * 	Updated on exist. \b Note: if the parser consumed all characters,
 * 	offs equals strlen(str) on exit. This is part of the interface and
 * 	the predicate for the caller to detect end of parsing.
 * @param[in] ed string with extra data
 * @param[out] newVal new value object created if parsing was successful
 * @return 0 on success, something else otherwise
 */


/** 
 * Parser for RFC5424 date.
 */
int ee_parseRFC5424Date(ee_ctx ctx, es_str_t *str, es_size_t *offs, es_str_t *ed, struct ee_value **newVal);

/** 
 * Parser for RFC3164 date.
 */
int ee_parseRFC3164Date(ee_ctx ctx, es_str_t *str, es_size_t *offs, es_str_t *ed, struct ee_value **newVal);

/** 
 * Parser for numbers.
 */
int ee_parseNumber(ee_ctx ctx, es_str_t *str, es_size_t *offs, es_str_t *ed, struct ee_value **newVal);


/** 
 * Parser for Words (SP-terminated strings).
 */
int ee_parseWord(ee_ctx ctx, es_str_t *str, es_size_t *offs, es_str_t *ed, struct ee_value **newVal);


/** 
 * Parse everything up to a specific character.
 */
int ee_parseCharTo(ee_ctx ctx, es_str_t *str, es_size_t *offs, es_str_t *ed, struct ee_value **newVal);


/** 
 * Parse a quoted string.
 */
int ee_parseQuotedString(ee_ctx ctx, es_str_t *str, es_size_t *offs, es_str_t *ed, struct ee_value **newVal);

/** 
 * Parse an ISO date.
 */
int ee_parseISODate(ee_ctx ctx, es_str_t *str, es_size_t *offs, es_str_t *ed, struct ee_value **newVal);


/** 
 * Parse a timestamp in 12hr format.
 */
int ee_parseTime12hr(ee_ctx ctx, es_str_t *str, es_size_t *offs, es_str_t *ed, struct ee_value **newVal);


/** 
 * Parse a timestamp in 24hr format.
 */
int ee_parseTime24hr(ee_ctx ctx, es_str_t *str, es_size_t *offs, es_str_t *ed, struct ee_value **newVal);

/** 
 * Parser for IPv4 addresses.
 */
int ee_parseIPv4(ee_ctx ctx, es_str_t *str, es_size_t *offs, es_str_t *ed, struct ee_value **newVal);

#endif /* #ifndef LIBEE_PRIMITIVETYPE_H_INCLUDED */
