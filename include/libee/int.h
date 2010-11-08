/**
 * @file int.h
 * The internal file format class.
 *
 * @class ee_int int.h
 *
 * The internal format is a very basic but easy to write and handle
 * event representation. Its primary purpose is to help with testing
 * libee.
 *
 * Events are represented in plain text files, which should be handable as
 * usual text files.
 *
 * The format is line-oriented. Each line has exactly one meaning 
 * (being a field name or value) or is a comment. Position within file
 * is important.
 *
 * each line consits of three parts:
 * - a single-character record type indicator
 * - a colon (only used for visual appearence and some error checking
 * - a value, terminated by LF
 *
 * Field types:
 * # - comment
 * e - start of a new event, value is ignored
 * f - start of a new field, value is field name
 * v - value is a field value for the last defined field
 *
 * It is invalid to have a "v" record type without a f directly in front
 * of it. It is valid to have fields without values (assuming this
 * is permitted for the field in question in the schema, if validation 
 * is active).
 *
 * Values:
 * UTF-8 encoded, but characters 0x0-0x1f, 0x7f must be escaped. Within
 * the value, C escape sequences are supported, with the most
 * important ones being (Note: double backslashes are for Doxygen, of course
 * this is to be used with single backslashes):
 * - \\0 NUL
 * - \\a BEL
 * - \\b Backspace
 * - \\f FF
 * - \\n LF
 * - \\r CR
 * - \\t HT
 * - \\' singlu quotation mark
 * - \\" double quotation mark
 * - \\? question mark
 * - \\\\ backslash character
 * - \\ooo ASCII Character in octal notation (o being octal digit)
 * - \\xhh ASCC character in hexadecimal notation
 * - \\xhhhh Unicode characer in headecimal notation
 * All other escape sequences are undefined. Currently, this is
 * interpreted as the escape character itself, but this is not
 * guaranteed. Most importantly, a special meaning may be assigned
 * to any of the currently-unassigned characters in the future.
 *
 * Sample:
 * # event with two fields
 * e:
 * f:field1
 * v:This\tis a test\n
 * v:Multiple values
 * f:field2
 * v:single value for this field
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
#ifndef LIBEE_INT_H_INCLUDED
#define	LIBEE_INT_H_INCLUDED
#include <libestr.h>

/**
 * Decode an internal representation into a CEE structure.
 *
 * The interface is heavily callback-based. The idea is to provide
 * greatest flexibility by doing so.
 *
 * @memberof ee_int
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
int ee_intDec(ee_ctx ctx, int (*cbGetLine)(es_str_t **ln),
              int (*cbNewEvt)(struct ee_event *event),
	      es_str_t **errMsg);

#endif /* #ifndef LIBEE_INT_H_INCLUDED */
