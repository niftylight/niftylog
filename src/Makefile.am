#############
# libniftylog Makefile.am
# v0.4 - Daniel Hiepler <daniel@niftylight.de>



# module-subdirs to include
SUBDIRS =


# directories to include
INCLUDE_DIRS = \
	-I$(top_srcdir)/include -I$(top_srcdir)/src -I$(srcdir) \
	-I$(top_builddir)/include -I$(top_builddir)/src -I$(builddir)


# extra files to include in distribution
EXTRA_DIST = \
        _mechanism.h \
        _mechanism-syslog.h \
        _mechanism-stderr.h \
        _mechanism-null.h


# source files
lib@PACKAGE@_la_SOURCES = \
	version.c \
	logger.c \
	mechanism.c \
	mechanism-stderr.c \
	mechanism-null.c \
	mechanism-syslog.c


# compile for debugging ?
if DEBUG
    DEBUG_CFLAGS = -DDEBUG
endif

# custom cflags
WARN_CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter


# target library
lib_LTLIBRARIES = lib@PACKAGE@.la

# cflags
lib@PACKAGE@_la_CFLAGS = \
	$(INCLUDE_DIRS) \
	$(WARN_CFLAGS) \
	$(DEBUG_CFLAGS) \
	-DPACKAGE_GIT_VERSION="\"`$(top_srcdir)/version --git`\""

# linker flags
lib@PACKAGE@_la_LDFLAGS = \
	-version-info @PACKAGE_API_CURRENT@:@PACKAGE_API_REVISION@:@PACKAGE_API_AGE@ \
	-Wall -no-undefined -no-allow-shlib-undefined \
	-export-symbols-regex [_]*\(nft_\|Nft\|NFT_\).*

# link in modules from subdirectories
lib@PACKAGE@_la_LIBADD = $(SUBDIRS)
