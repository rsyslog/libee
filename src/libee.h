/**
 * @mainpage
 * Libee - An Event Expression Library inspired by CEE
 *
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
 * @section dataStructures Data Structures
 * @subsection ctx The Library Context
 *
 * All functions in this library depend on the so-called library context.
 * It provides support for using multiple independent instances of libee
 * inside a single program, e.g. in plugin-based systems. The library context
 * stores things that usually go into global variables, things like
 * a global debug handler, compliance levels, tuning paramters and so
 * on. A typical, non-plugin based program should never need more than
 * a single library context. Think of it as an additional abstraction
 * layer to facilitate working with multiple independend instances of libee
 * at the same time within the same program.
 *
 * The context must be provided upon each object creation. Once an object
 * has been created, it remains bound to the context used during creation.
 * <b>No object can be shared between multiple contextes</b>. During creation,
 * the context is stored within each object. All successive calls use this
 * in-object context. This is especially important as some libxml2 functions,
 * on which we heavily rely, do not permit to pass more than one object pointer.
 * As such, this object must contain the required context object, as we need
 * it for proper operation.
 *
 * For obvious reasons, a libray context must only be destructed after \b all
 * objects that use it have been destructed as well. The context itsefl
 * <b>does not</b> keep track of which other objects use it (this is a
 * decision based on performance reasons).
 *
 * @subsection setBucket On Sets and Buckets
 *
 * The CEE standard uses the term "set" to organize/group related tags and
 * fields. So this term is not available for any other use. However, we need
 * to have a container that can hold one or many of these objects. We have
 * named these containers "buckets". So we have tag and field buckets, each
 * of which contain a number of tags or fields.
 *
 * The idea behind this is that we may sometimes need to group all of an
 * event's tags and fields together. One reason may be that e.g. the tag
 * bucket can be created before the actual event. One other may be that 
 * a tag bucketmay even be used without an event record already be created
 * (e.g. as a performance optimization, where the tag bucket is created
 * on a global level and then re-used to populate each actual event instance
 * -- something that is NOT YET IMPLEMENTED!).
 *
 * Introduction of the bucket classes provide an easy way to handle this
 * situation.
 *
 * @section appendix Appendix
 * @subsection ceeCompliance CEE Compliance
 * While I try to keep compatible with CEE for obvious reasons, the standard
 * is not yet finished. Many details are not even decided. For this reason,
 * libee will evolve in parallel and not necessarily be aligned to the
 * latest CEE spec.
 *
 * @subsection apiStability API Stability
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
#include "obj.h"
#include "ctx.h"
#include "timestamp.h"
#include "value.h"
#include "field.h"
#include "nvfield.h"
#include "fieldbucket.h"
#include "primitivetype.h"
#include "tagbucket.h"

/* some private error codes (always negative)
 */
#define E_ERR -1
#define E_NOMEM -2
#endif /* #ifndef EE_H_INCLUDED */
