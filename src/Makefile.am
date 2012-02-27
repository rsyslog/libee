#SUBDIRS = collection
sbin_PROGRAMS = libee-convert

# Uncomment for debugging
DEBUG = -g
PTHREADS_CFLAGS = -pthread
LIBEE_CFLAGS=-I$(top_srcdir)/include

if ENABLE_DEBUG
CFLAGS += $(DEBUG)
endif

# we need to clean the sample up once we have reached a decent
# milestone (latest at initial release!)
#sbin_PROGRAMS = sample
#sample_SOURCES = sample.c \
#	libee.c ctx.c samp.c

lib_LTLIBRARIES = libee.la

libee_la_SOURCES = \
	cjson/cjson.c \
	ctx.c \
	tag.c \
	event.c \
	json_event.c \
	value.c \
	tagbucket.c \
	field.c \
	fieldbucket.c \
	primitivetype.c \
	int_dec.c \
	json_dec.c \
	apache_dec.c \
	syslog_enc.c \
	json_enc.c \
	csv_enc.c \
	xml_enc.c \
    collection/collection.c \
    collection/collection_tools.c \
    collection/collection_cnv.c \
    collection/collection_queue.c \
    collection/collection_stack.c \
    collection/collection_cmp.c \
    collection/collection_iter.c \
    collection/collection_priv.h \
    collection/trace/trace.h
EXTRA_DIST = \
	cjson/cjson.h

libee_la_CPPFLAGS = $(LIBXML2_CFLAGS) $(LIBESTR_CFLAGS) $(LIBEE_CFLAGS)
libee_la_LIBADD = $(LIBXML2_LIBS) $(LIBESTR_LIBS)
libee_la_LDFLAGS = -version-info 0:0:0

libee_convert_SOURCES = convert.c
libee_convert_CPPFLAGS =  -I$(top_srcdir) $(LIBEE_CFLAGS) $(LIBESTR_CFLAGS) $(LIBXML2_CFLAGS)
libee_convert_LDADD = $(LIBEE_LIBS) $(LIBXML2_LIBS) $(LIBESTR_LIBS)

include_HEADERS = 
