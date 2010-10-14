/**
 * @mainpage
 * Libee - a CEE-based event expression library.
 * Copyright 2010 by Rainer Gerhards and Adiscon GmbH.
 *
 * In essence, it provides a small but hopefully convenient API
 * layer above the CEE standard. Also, processing speed is definitely
 * a goal of this library. As such, it tries to generate in-memory
 * representations of CEE object that will be fast to work with.
 * Multi-threaded applications and plugin architectures are fully
 * supported. For the time being, this means that multiple threads
 * are safe as long as different library contexts are being used. It
 * is \b not safe to use the same context concurrently. In the future,
 * ctx may support in-context concurreny via specific options, but
 * this is outside the scope of the initial effort.
 *
 * More information on CEE can be found: http://cee.mitre.org/
 *
 * @note
 * <b>API stability</b>
 * 
 * At the time of this writing, CEE is a moving target. While I try
 * to keep things consistent, I do NOT yet guarantee stability of the
 * ctx API. Do not expect a stable version before ctx version 1.x.x
 * is announced. As I used ctx in my own projects, I obviously will
 * do my best to break as few things as possible. But depending on
 * CEE movements, some things may break. You have been warned ;)
 * 
 * On the name "ctx": while ctx aims to be a full implementation
 * of the CEE standard, I could not guarantee this at the time I
 * initiated this project. At this time, the CEE standard was far from
 * being completed and frequent changes were anticipated.
 * I was under a number of resource constraints and so could not commit
 * to keep the library in line with CEE no matter where CEE would head
 * to. While it was tempting to call the library "libcee", and many
 * suggested so, I used a similar, but different name to reduce potential
 * friction out of different needs of the CEE standards body and my
 * development project. If things go well, ctx may evolve into
 * something that becomes the "official" libcee.
 *
 * Libee fully supports Unicode. Like most Linux tools, it operates
 * on UTF-8 natively, called "passive mode". This was decided because we
 * so can keep the size of data structures small while still supporting
 * all of the world's languages (actually more than when we did UCS-2).
 * This is the same concept used by libxml2, on which ctx heavily 
 * relies for all XML-based functionality.
 *
 *//*
 *
 * ctx - a CEE-based event expression library
 * Copyright 2010 by Rainer Gerhards and Adiscon GmbH.
 *
 * This file is part of ctx.
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
#ifndef LIBEE_H_INCLUDED
#define	LIBEE_H_INCLUDED
#include <stdlib.h>	/* we need size_t */

#endif /* #ifndef EE_H_INCLUDED */
