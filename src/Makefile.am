# libniftyconf Makefile.am - v0.2 - Daniel Hiepler <daniel@niftylight.de> 

INCLUDE_DIRS = -I$(top_srcdir)/include -I$(top_srcdir)/src


# module-subdirs to include
SUBDIRS =
EXTRA_DIST = 

# target library
lib_LTLIBRARIES = lib@GENERIC_LIBRARY_NAME@.la

# source-files
lib@GENERIC_LIBRARY_NAME@_la_SOURCES = \
	logger.c

# cflags 
lib@GENERIC_LIBRARY_NAME@_la_CFLAGS = -Wall $(INCLUDE_DIRS)

# linker flags
lib@GENERIC_LIBRARY_NAME@_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE) -lpthread -ldl -Wall -no-undefined 

# link in modules from subdirectories
#lib@GENERIC_LIBRARY_NAME@_la_LIBADD = $(SUBDIRS)
lib@GENERIC_LIBRARY_NAME@_la_LIBADD =
