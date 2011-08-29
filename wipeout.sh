#!/bin/sh
#
# Copyright (C) 2011 Yann Pouillon
#
# This file is part of the Libpspio software package. For license
# information, please see the COPYING file in the top-level directory of
# the source distribution.
#

#
# IMPORTANT NOTE:
#
#   For maintainer use only!
#
#   PLEASE DO NOT EDIT THIS FILE, AS IT COULD CAUSE A SERIOUS LOSS OF DATA.
#   *** YOU HAVE BEEN WARNED! ***
#

# Check that we are in the right directory
if test ! -s "./configure.ac" -o ! -d "psp_references"; then
  echo "wipeout: Cowardly refusing to remove something from here!"
  exit 1
fi

# Remove temporary directories and files
echo "Removing temporary directories and files..."
rm -rf tmp*
find . -depth -name 'tmp-*' -exec rm -rf {} \;
echo "done."

# Remove autotools files
echo "Removing autotools files..."
rm -f core config.log config.status stamp-h1 config.h config.h.in*
rm -rf aclocal.m4 autom4te.cache configure confstat*
(cd config/gnu && rm -f config.guess config.sub depcomp install-sh ltmain.sh missing)
(cd config/m4 && rm -f libtool.m4 ltoptions.m4 ltsugar.m4 ltversion.m4 lt~obsolete.m4)
echo "done."

# Remove Makefiles and machine-generated files
echo "Removing files produced by the configure script..."
rm -f libtool
find . -name Makefile -exec rm {} \;
find . -name Makefile.in -exec rm {} \;
echo "done."

# Remove object files, libraries and programs
echo "Removing object files, libraries and programs..."
find . -depth -name '.deps' -o -name '.libs' -exec rm -r {} \;
find . -name '*.la' -o -name '*.lo' -exec rm {} \;
find . -name '*.a' -o -name '*.o' -exec rm {} \;
echo "done."
