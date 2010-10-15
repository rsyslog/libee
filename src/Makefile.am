# Uncomment for debugging
DEBUG = -g
PTHREADS_CFLAGS = -pthread

#CFLAGS += $(DEBUG)

# we need to clean the sample up once we have reached a decent
# milestone (latest at initial release!)
#sbin_PROGRAMS = sample
#sample_SOURCES = sample.c \
#	libee.c ctx.c samp.c

lib_LTLIBRARIES = libee.la

libee_la_SOURCES = \
	ctx.c \

libee_la_CPPFLAGS = $(PTHREADS_CFLAGS)
libee_la_LIBADD = $(rt_libs)
libee_la_LDFLAGS = -version-info 0:0:0

include_HEADERS = libee.h ctx.h
