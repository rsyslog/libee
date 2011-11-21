/**
 * @file ctx.h
 * @brief The library context object.
 * @class ee_ctx ctx.h
 *
 * The library context object is at the top of libee's class
 * hierarchy. It serves as the "environement" (acutally context ;))
 * in which all other ee library objects operate.
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
#ifndef LIBEE_EE_H_INCLUDED
#define	LIBEE_EE_H_INCLUDED

/* some configuration-defined values (TODO: autoconf!)
 */
#define EE_DFLT_TAG_BCKT_SIZE	5
	/**< default size for tag buckets (extensible) */
#define EE_DFLT_FIELD_BCKT_SIZE	11
	/**< default size for field buckets (extensible) */

#define ObjID_None		0xFDFD0000
#define ObjID_CTX		0xFDFD0001
#define ObjID_TAG		0xFDFD0002
#define ObjID_TAGBUCKET		0xFDFD0003
#define ObjID_FIELDTYPE		0xFDFD0004
#define ObjID_FIELD		0xFDFD0005
#define ObjID_FIELDBUCKET	0xFDFD0006
#define ObjID_EVENT		0xFDFD0007
#define ObjID_VALUE		0xFDFD0008
#define ObjID_VALNODE		0xFDFD0009
#define ObjID_DELETED		0xFDFDFFFF

/**
 * The compliance level in which this context runs.
 * Most importantly, the compliance level defines how picky the
 * lib is about verifying semantical correctness of the objects
 * in question.
 */
enum ee_compLevel {
	ee_cl_NONE = 0,	/**< everything is allowed */
	ee_cl_FULL	/**< full verification is required */
};

#define EE_CTX_FLAG_ENC_ULTRACOMPACT 1
#define EE_CTX_FLAG_INCLUDE_FLAT_TAGS 2

struct ee_ctx_s {
	unsigned objID;	/**< a magic number to prevent some memory adressing errors */
	void (*dbgCB)(void *cookie, char *msg, size_t lenMsg);
					/**< user-provided debug output callback */
	void *dbgCookie;		/**< cookie to be passed to debug callback */
	enum ee_compLevel compLevel;	/**< our compliance level */
	unsigned short flags;		/**< flags modifying behavior */
	int fieldBucketSize;		/**< default size for field buckets */
	int tagBucketSize;		/**< default size for field buckets */
};


/**
 * The library context descriptor.
 * This is used to permit multiple independednt instances of the
 * library to be called within a single program. This is most 
 * useful for plugin-based architectures.
 * @note
 * For the current library version, multi-threaded applications
 * must use \b different library contexts for concurrent access.
 */
typedef struct ee_ctx_s* ee_ctx;

/**
 * Return library version string.
 *
 * Returns the version of the currently used library.
 *
 * @return Zero-Terminated library version string.
 */
/* Note: this MUST NOT be inline to make sure the actual library
 * has the right version, not just what was used to compile!
 */
char *ee_version(void);

/**
 * Initialize a library context.
 *
 * To prevent memory leaks, ee_exitCtx() must be called on a library
 * context that is no longer needed.
 *
 * @memberof ee_ctx
 * @public
 *
 * @return new library context or NULL if an error occured
 */
ee_ctx ee_initCtx(void);

/**
 * Discard a library context.
 *
 * Free's the ressources associated with the given library context. It
 * MUST NOT be accessed after calling this function.
 *
 * @memberof ee_ctx
 * @public
 *
 * @param ctx The context to be discarded.
 *
 * @return Returns zero on success, something else otherwise.
 */
int ee_exitCtx(ee_ctx ctx);

/**
 * Set context flags.
 * Note that all previsouly set flags are overridden when this
 * method is called. If that is not desired, call ee_getFlags()
 * first and manipulate the result as desired.
 *
 * @memberof ee_ctx
 * @public
 *
 * @param ctx The context to be updated
 * @param flags Flags to be set. Note that currently an integer is
 * 		passed in, but we operate with short internally.
 */
void ee_setFlags(ee_ctx ctx, unsigned int flags);

/**
 * Get context flags.
 *
 * @memberof ee_ctx
 * @public
 *
 * @param ctx The context to query
 * @return currently set flags
 *
 */
unsigned int ee_getFlags(ee_ctx ctx);


/**
 * Set encoding mode to ultra compact.
 * In this mode the encoders will generated the shortest
 * string presentation possible, even if that means it will
 * be very hard to read by a human.
 *
 * @memberof ee_ctx
 * @public
 *
 * @param ctx context to modify
 */
static inline void
ee_setEncUltraCompact(ee_ctx ctx)
{
	ctx->flags |= EE_CTX_FLAG_ENC_ULTRACOMPACT;
}

/**
 * Set a debug message handler (callback).
 *
 * Libee can provide helpful information for debugging
 * - it's internal processing
 * - the way a log message is being normalized
 *
 * It does so by emiting "interesting" information about its processing
 * at various stages. A caller can obtain this information by registering
 * an entry point. When done so, ctx will call the entry point
 * whenever it has something to emit. Note that debugging can be rather
 * verbose.
 *
 * The callback will be called with the following three parameters in that order:
 * - the caller-provided cookie
 * - a zero-terminated string buffer
 * - the length of the string buffer, without the trailing NUL byte
 *
 * @note
 * The provided callback function <b>must not</b> call any ctx
 * APIs except when specifically flagged as safe for calling by a debug
 * callback handler.
 *
 * @memberof ee_ctx
 * @private
 *
 * @param[in] ctx The library context to apply callback to.
 * @param[in] cb The function to be called for debugging
 * @param[in] cookie Opaque cookie to be passed down to debug handler. Can be
 *                   used for some state tracking by the caller. This is defined as
 *                   void* to support pointers. To play it safe, a pointer should be
 *                   passed (but advantorous folks may also use an unsigned).
 *
 * @return Returns zero on success, something else otherwise.
 */
int ee_setDebugCB(ee_ctx ctx, void (*cb)(void*, char*, size_t), void *cookie);

/* internal functions */
void ee_dbgprintf(ee_ctx ctx, char *fmt, ...) __attribute__((format(printf, 2, 3)));

static inline int
ee_ctxIsEncUltraCompact(ee_ctx ctx)
{
	return ctx->flags & EE_CTX_FLAG_ENC_ULTRACOMPACT;
}

#endif /* #ifndef LIBEE_EE_H_INCLUDED */
