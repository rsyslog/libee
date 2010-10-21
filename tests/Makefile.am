if ENABLE_TESTBENCH

TESTRUNS = 
check_PROGRAMS = \
	$(TESTRUNS) \
	genfile \
	tagbucket1

TESTS = $(TESTRUNS) \
	tagbucket.sh

endif # if ENABLE_TESTBENCH

DISTCLEANFILES=
EXTRA_DIST = \
	tagbucket.sh

genfile_SOURCES = genfile.c

tagbucket1_SOURCES = tagbucket1.c
tagbucket1_CPPFLAGS =  -I$(top_srcdir) $(LIBEE_CFLAGS) $(LIBXML2_CFLAGS)
tagbucket1_LDADD = $(LIBEE_LIBS) $(LIBXML2_LIBS)
