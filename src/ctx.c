/* Libee - An Event Expression Library inspired by CEE
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
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "libee/libee.h"
#include "libee/internal.h"
#include "libee/ctx.h"

#define ERR_ABORT {r = 1; goto done; }

#define CHECK_CTX \
	if(ctx->objID != ObjID_CTX) { \
		r = -1; \
		goto done; \
	}

char *
ee_version(void)
{
	return VERSION;
}


ee_ctx
ee_initCtx(void)
{
	ee_ctx ctx;
	if((ctx = calloc(1, sizeof(struct ee_ctx_s))) == NULL)
		goto done;

	ctx->objID = ObjID_CTX;
	ctx->dbgCB = NULL;
	ctx->tagBucketSize = EE_DFLT_TAG_BCKT_SIZE;
	ctx->fieldBucketSize = EE_DFLT_FIELD_BCKT_SIZE;
done:
	return ctx;
}


int
ee_exitCtx(ee_ctx ctx)
{
	int r = 0;

	CHECK_CTX;

	ctx->objID = ObjID_None; /* prevent double free */
	free(ctx);
done:
	return r;
}

void
ee_setFlags(ee_ctx ctx, unsigned int flags)
{
	ctx->flags |= flags;
}

unsigned int ee_getFlags(ee_ctx ctx)
{
	return ctx->flags;
}

int
ee_setDebugCB(ee_ctx ctx, void (*cb)(void*, char*, size_t), void *cookie)
{
	int r = 0;

	CHECK_CTX;
	ctx->dbgCB = cb;
	ctx->dbgCookie = cookie;
done:
	return r;
}

/**
 * @internal
 * Generate some debug message and call the caller provided callback.
 *
 * Will first check if a user callback is registered. If not, returns
 * immediately.
 */
void
ee_dbgprintf(ee_ctx ctx, char *fmt, ...)
{
	va_list ap;
	char buf[8*1024];
	size_t lenBuf;

	if(ctx->dbgCB == NULL)
		goto done;
	
	va_start(ap, fmt);
	lenBuf = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	if(lenBuf >= sizeof(buf)) {
		/* prevent buffer overrruns and garbagge display */
		buf[sizeof(buf) - 5] = '.';
		buf[sizeof(buf) - 4] = '.';
		buf[sizeof(buf) - 3] = '.';
		buf[sizeof(buf) - 2] = '\n';
		buf[sizeof(buf) - 1] = '\0';
		lenBuf = sizeof(buf) - 1;
	}

	ctx->dbgCB(ctx->dbgCookie, buf, lenBuf);
done: 
	return;
}

