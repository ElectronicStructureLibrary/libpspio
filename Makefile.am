#                                                       -*- Automake -*-
# Top makefile for Libpspio
#

# Autotools parameters
ACLOCAL_AMFLAGS = -I config/m4

# Main tasks
SUBDIRS = src

# Source tarball
EXTRA_DIST = \
  Doxyfile \
  libxc \
  psp_references

dist-hook:
	rm -rf `find $(distdir) -name .svn`

