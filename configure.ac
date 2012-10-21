# libniftylog configure.ac 
# v0.3 - (c) Daniel Hiepler (2010-2012)


# --------------------------------
# Initialization macros.
# --------------------------------
AC_INIT([libniftylog], m4_esyscmd([./version.sh --short]),[http://github.com/niftylight/niftylog/issues], [niftylog], [http://wiki.niftylight.de/AC_PACKAGE_NAME])
AC_COPYRIGHT([Copyright (C) 2006-2012 Daniel Hiepler])

AM_INIT_AUTOMAKE([color-tests dist-bzip2 parallel-tests silent-rules -Wall])
AC_CONFIG_MACRO_DIR([m4])


# --------------------------------
#    information on the package
# --------------------------------

# infos
PACKAGE_DESCRIPTION="lightweight & simple library to implement logging/debugging output for new or existing applications"
AC_SUBST([PACKAGE_DESCRIPTION], [${PACKAGE_DESCRIPTION}], [package description])
AC_DEFINE_UNQUOTED(PACKAGE_DESCRIPTION, "${PACKAGE_DESCRIPTION}", [package description])

# versioning (s. version.sh)
AC_SUBST([PACKAGE_MAJOR_VERSION], m4_esyscmd([./version.sh --major]), [major version])
AC_SUBST([PACKAGE_MINOR_VERSION], m4_esyscmd([./version.sh --minor]), [minor version])
AC_SUBST([PACKAGE_MICRO_VERSION], m4_esyscmd([./version.sh --micro]), [micro version])
AC_SUBST([PACKAGE_API_REVISION], m4_esyscmd([./version.sh --api-revision]), [api revision])
AC_SUBST([PACKAGE_API_CURRENT], m4_esyscmd([./version.sh --api-current]), [api current])
AC_SUBST([PACKAGE_API_AGE], m4_esyscmd([./version.sh --api-age]), [api age])
AC_SUBST([PACKAGE_GIT_VERSION], m4_esyscmd([./version.sh --git]), [git version])



# --------------------------------
# Check for programs
# --------------------------------
LT_INIT
AM_PROG_CC_C_O
PKG_PROG_PKG_CONFIG([0.14])
if test -z "$PKG_CONFIG"; then
  AC_MSG_ERROR([*** pkg-config not found. See http://www.freedesktop.org/software/pkgconfig/ or check your distribution.])
fi
  

# --------------------------------
# Check for libs
# --------------------------------



# --------------------------------
# Check for headers
# --------------------------------


# --------------------------------
# Check for typesv
# --------------------------------


# --------------------------------
# Check for structures
# --------------------------------


# --------------------------------
# Check compiler characteristics
# --------------------------------


# --------------------------------
# Check for functions
# --------------------------------


# --------------------------------
# Check for system-services
# --------------------------------


# --------------------------------
#   compile process
# --------------------------------
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])


# --------------------------------
# Arguments for configure-script
# --------------------------------
AC_ARG_ENABLE(
        debug,
	[  --disable-debug         Don't compile for debugging ],
	[ debug=false ],
	[ debug=true])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)				


AC_ARG_ENABLE(
	tests,
	[  --disable-tests         Don't build tests for "make check" ],
	[ tests=false ],
	[ tests=true])
AM_CONDITIONAL(RUN_TESTS, test x$tests = xtrue)


# --------------------------------
# Output
# --------------------------------
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES(
[	  
          Doxyfile \
	  Makefile \
	  include/Makefile \
	  include/logger-version.h \
	  src/Makefile \
	  src/version.c \
	  tests/Makefile \
          $PACKAGE.pc \
	  debian/control.tmpl
])
AC_OUTPUT


echo "
Configure report:

    Package.....................:  ${PACKAGE_STRING}
    Description.................:  ${PACKAGE_DESCRIPTION}
    Revision....................:  ${PACKAGE_GIT_VERSION}
    Libtool version.............:  ${PACKAGE_API_CURRENT}:${PACKAGE_API_REVISION}:${PACKAGE_API_AGE}
    URL.........................:  ${PACKAGE_URL}
    Bugreports..................:  ${PACKAGE_BUGREPORT}

    Install prefix..............:  ${prefix}
    C compiler..................:  ${CC}
    C++ compiler................:  ${CPP}
    System CFLAGS...............:  ${CFLAGS}
    System CXXFLAGS.............:  ${CXXFLAGS}
    System LDFLAGS..............:  ${LDFLAGS}
"
