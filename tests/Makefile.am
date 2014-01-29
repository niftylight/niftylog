#############
# libniftylog Makefile.am
# v0.4 - Daniel Hiepler <daniel@niftylight.de>


EXTRA_DIST = \
	tests.env

# directories to include
INCLUDE_DIRS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/include \
	-I$(srcdir)

# files to clean on "make distclean"
DISTCLEANFILES = 

# custom cflags
WARN_CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter


TESTCFLAGS = \
	$(INCLUDE_DIRS) \
	$(WARN_CFLAGS)

TESTLDFLAGS = \
	-Wall -no-undefined

TESTLDADD = \
	$(top_builddir)/src/libniftylog.la



check_PROGRAMS = \
	list_mechanisms \
	logging

TESTS = $(check_PROGRAMS)
AM_TESTS_ENVIRONMENT = $(srcdir)/tests.env;


list_mechanisms_SOURCES = list_mechanisms.c
list_mechanisms_CFLAGS = $(TESTCFLAGS)
list_mechanisms_LDFLAGS = $(TESTLDFLAGS)
list_mechanisms_LDADD = $(TESTLDADD)

logging_SOURCES = logging.c
logging_CFLAGS = $(TESTCFLAGS)
logging_LDFLAGS = $(TESTLDFLAGS)
logging_LDADD = $(TESTLDADD)
