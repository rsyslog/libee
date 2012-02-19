pkgconfigdir = $(libdir)/pkgconfig
#pkgconfig_DATA = eecollection.pc

##############################################################################
# collection
##############################################################################

lib_LTLIBRARIES = libeecollection.la
dist_pkgconfig_DATA = eecollection.pc
dist_include_HEADERS = \
    collection.h \
    collection_tools.h \
    collection_queue.h \
    collection_stack.h

libeecollection_la_SOURCES = \
    collection.c \
    collection_tools.c \
    collection_cnv.c \
    collection_queue.c \
    collection_stack.c \
    collection_cmp.c \
    collection_iter.c \
    collection_priv.h \
    trace/trace.h
libeecollection_la_LDFLAGS = \
    -version-info 3:0:0

check_PROGRAMS = \
    collection_ut \
    collection_stack_ut \
    collection_queue_ut
TESTS = \
    collection_ut \
    collection_stack_ut \
    collection_queue_ut

collection_ut_SOURCES = collection_ut.c
collection_ut_LDADD = libeecollection.la
collection_stack_ut_SOURCES = collection_stack_ut.c
collection_stack_ut_LDADD = libeecollection.la
collection_queue_ut_SOURCES = collection_queue_ut.c
collection_queue_ut_LDADD = libeecollection.la

collection-docs:
if HAVE_DOXYGEN
	cd collection; \
	$(DOXYGEN) collection.cfg.doxy
endif

