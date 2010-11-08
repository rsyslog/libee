# Uncomment for debugging
DEBUG = -g
PTHREADS_CFLAGS = -pthread
LIBEE_CFLAGS=-I$(top_srcdir)/include

#CFLAGS += $(DEBUG)

# we need to clean the sample up once we have reached a decent
# milestone (latest at initial release!)
#sbin_PROGRAMS = sample
#sample_SOURCES = sample.c \
#	libee.c ctx.c samp.c

lib_LTLIBRARIES = libee.la

libee_la_SOURCES = \
	ctx.c \
	tag.c \
	event.c \
	value.c \
	tagbucket.c \
	field.c \
	fieldbucket.c \
	primitivetype.c \
	int_dec.c

libee_la_CPPFLAGS = $(LIBXML2_CFLAGS) $(LIBEE_CFLAGS)
libee_la_LIBADD = $(LIBXML2_LIBS)
libee_la_LDFLAGS = -version-info 0:0:0

include_HEADERS = 
