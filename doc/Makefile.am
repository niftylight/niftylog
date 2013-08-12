#############
# libniftylog Makefile.am
# v0.4 - Daniel Hiepler <daniel@niftylight.de>

htmldir = \
	$(docdir)/html

html_DATA = \
	html/*

DISTCLEANFILES = \
	Doxyfile \
	-r html

CLEANFILES = \
	-r html \
	doxyfile.stamp


html/*:
	$(DOXYGEN) Doxyfile

doxyfile.stamp: html
	echo stamp > doxyfile.stamp

all-local: doxyfile.stamp

deb:
	cp "$(top_builddir)/debian/docbase" "$(DEBSRCDIR)/debian/libniftylog-dev.doc-base"
