# libniftylog configure.ac 
# v0.2 - Daniel Hiepler (2010)

# --------------------------------
# Initialization macros.
# --------------------------------

#release versioning
define([MAJOR_VERSION], 0)
define([MINOR_VERSION], 1)
define([MICRO_VERSION], 1)
define([API_VERSION], 00001)
#                     |   |
#                     |   +-- increment by one if interfaces were added without 
#                     |       breaking the API (results in a warning)
#                     +------ increment by 10000 on API changes that break older versions
#                             (results in an error)

# naming
define([RELEASE_NAME], niftylog)
define([LIB_NAME], libniftylog)




# --------------------------------
# AC_INIT
# --------------------------------
AC_INIT([LIB_NAME],[MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION],[https://github.com/niftylight/niftylight/issues], [RELEASE_NAME], [http://wiki.niftylight.de/LIB_NAME])
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION, AC_PACKAGE_BUGREPORT, AC_PACKAGE_URL)
AC_CONFIG_MACRO_DIR([m4])
AC_LANG([C])

# Test for new silent rules and enable only if they are available
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

                
# --------------------------------
# Package information
# --------------------------------
GENERIC_LIBRARY_NAME=RELEASE_NAME
GENERIC_LIBRARY_VERSION=MAJOR_VERSION:MINOR_VERSION
GENERIC_API_VERSION=API_VERSION
GENERIC_VERSION=MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION
GENERIC_RELEASE=MAJOR_VERSION.MINOR_VERSION
VERSION=$GENERIC_VERSION
PACKAGE=$GENERIC_LIBRARY_NAME

AC_SUBST(PACKAGE)
AC_SUBST(VERSION)
AC_SUBST(GENERIC_API_VERSION)
AC_SUBST(GENERIC_LIBRARY_VERSION)
AC_SUBST(GENERIC_LIBRARY_NAME)
AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_VERSION)
AC_SUBST(LIBDIR)

AC_DEFINE(GENERIC_MAJOR_VERSION, MAJOR_VERSION, [major version])
AC_DEFINE(GENERIC_MINOR_VERSION, MINOR_VERSION, [minor version])
AC_DEFINE(GENERIC_MICRO_VERSION, MICRO_VERSION, [micro version])
AC_DEFINE(GENERIC_API_VERSION, API_VERSION, [api version])

# --------------------------------
# Check for programs
# --------------------------------
AC_PROG_CC
AM_PROG_CC_C_O
LT_INIT
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
AC_CONFIG_HEADERS([config.h])


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
	[  --disable-tests         Don't run tests before install ],
	[ tests=false ],
	[ tests=true])
AM_CONDITIONAL(RUN_TESTS, test x$tests = xtrue)


# --------------------------------
# Output
# --------------------------------
AC_CONFIG_FILES([Makefile \
	  include/Makefile \
	  include/log_version.h \
	  src/Makefile \
	  tests/Makefile \
          $GENERIC_LIBRARY_NAME.pc          
])
AC_OUTPUT
