if ENABLE_TESTBENCH

TESTRUNS = 
check_PROGRAMS = \
	$(TESTRUNS) \
	genfile \
	ezapi1
#add when clear: tagbucket1

TESTS = $(TESTRUNS) 
#	tagbucket.sh

endif # if ENABLE_TESTBENCH

DISTCLEANFILES=
EXTRA_DIST = \
	tagbucket.sh

genfile_SOURCES = genfile.c

# The tagbucket concept is not totally clear at the moment, and
# so we do not support this test case for the time being. Once 
# the uncertainty has been cleared up, we will re-enable the test!
#tagbucket1_SOURCES = tagbucket1.c
#tagbucket1_CPPFLAGS =  -I$(top_srcdir) $(LIBEE_CFLAGS) $(LIBXML2_CFLAGS)
#tagbucket1_LDADD = $(LIBEE_LIBS) $(LIBXML2_LIBS) $(LIBESTR_LIBS)

ezapi1_SOURCES = ezapi1.c
ezapi1_CPPFLAGS =  -I$(top_srcdir) $(LIBEE_CFLAGS) $(LIBXML2_CFLAGS)
ezapi1_LDADD = $(LIBEE_LIBS) $(LIBXML2_LIBS) $(LIBESTR_LIBS)
