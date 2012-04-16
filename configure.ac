                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([libee], [0.4.1], [rgerhards@adiscon.com])
AM_INIT_AUTOMAKE
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_SRCDIR([src/ctx.c])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
if test "$GCC" = "yes"
then CFLAGS="$CFLAGS -W -Wall -Wformat-security -Wshadow -Wcast-align -Wpointer-arith -Wmissing-format-attribute -g"
fi

AC_PROG_LIBTOOL

# Checks for libraries.
AC_SEARCH_LIBS(pow, m)
rt_libs=$LIBS

# We CURRENTLY do NOT need libxml, but this will change at a later stage.
# If we need libxml, we need it for features which some may not find essential.
# The idea is to make these features optional, and thus save the libxml dependency
# under some circumstances. This will be added as need arises. As such, for
# the time being, we simply comment out the code. Please do not remove it,
# as we will  need it back in again some time in the future.
# rgerhards, 2011-04-01 (NOT an april fools day joke!)
#AC_CHECK_PROG(
#  [HAVE_LIBXML2_CONFIG],
#  [xml2-config],
#  [yes],,,
#)
#if test "x${HAVE_LIBXML2_CONFIG}" != "xyes"; then
#  AC_MSG_FAILURE([xml2-config not found in PATH - this means libxml2 development package is missing])
#fi
#AC_CHECK_LIB(
#  [xml2],
#  [xmlReadFile],
#  [LIBXML2_CFLAGS=`xml2-config --cflags`
#   LIBXML2_LIBS=`xml2-config --libs`
#  ],
#  [AC_MSG_FAILURE([libxml2 is missing])],
#  [`xml2-config --libs`]
#)
AC_SUBST(LIBXML2_CFLAGS)
AC_SUBST(LIBXML2_LIBS)

AC_SUBST(rt_libs)

# Checks for header files.
#AC_HEADER_STDC
#AC_CHECK_HEADERS([])

# Checks for typedefs, structures, and compiler characteristics.
#AC_C_CONST
#AC_TYPE_SIZE_T
#AC_HEADER_TIME
#AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_MALLOC
#AC_FUNC_SELECT_ARGTYPES
#AC_TYPE_SIGNAL
#AC_CHECK_FUNCS([])

LIBEE_CFLAGS="-I\$(top_srcdir)/include"
LIBEE_LIBS="\$(top_builddir)/src/libee.la -lm"
AC_SUBST(LIBEE_CFLAGS)
AC_SUBST(LIBEE_LIBS)

# modules we require
PKG_CHECK_MODULES(LIBESTR, libestr >= 0.0.0)

# enable/disable the testbench (e.g. because some important parts
# are missing)
AC_ARG_ENABLE(testbench,
        [AS_HELP_STRING([--enable-testbench],[file input module enabled @<:@default=yes@:>@])],
        [case "${enableval}" in
         yes) enable_testbench="yes" ;;
          no) enable_testbench="no" ;;
           *) AC_MSG_ERROR(bad value ${enableval} for --enable-testbench) ;;
         esac],
        [enable_testbench=yes]
)
AM_CONDITIONAL(ENABLE_TESTBENCH, test x$enable_testbench = xyes)


# debug mode settings
AC_ARG_ENABLE(debug,
        [AS_HELP_STRING([--enable-debug],[Enable debug mode @<:@default=no@:>@])],
        [case "${enableval}" in
         yes) enable_debug="yes" ;;
          no) enable_debug="no" ;;
           *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
         esac],
        [enable_debug="no"]
)
AM_CONDITIONAL(ENABLE_DEBUG, test x$enable_debug = xyes)
if test "$enable_debug" = "yes"; then
        AC_DEFINE(DEBUG, 1, [Defined if debug mode is enabled (it's easier to check in the code).])
fi
if test "$enable_debug" = "no"; then
        AC_DEFINE(NDEBUG, 1, [Defined if debug mode is disabled.])
fi



AC_CONFIG_FILES([Makefile \
		libee.pc \
		src/Makefile \
		include/Makefile \
		include/libee/Makefile \
		tests/Makefile])
AC_OUTPUT
AC_CONFIG_MACRO_DIR([m4])


echo "*****************************************************"
echo "libee will be compiled with the following settings:"
echo
echo "Debug mode enabled:          $enable_debug"
echo "Testbench enabled:           $enable_testbench"
