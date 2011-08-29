#!/bin/sh
#
# Copyright (C) 2011 Yann Pouillon
#
# This file is part of the Libpspio software package. For license information,
# please see the COPYING file in the top-level directory of the source
# distribution.
#

# Stop at first error encountered
set -e

# Check that we are in the right directory
if test ! -s "./configure.ac" -o ! -d "psp_references"; then
  echo "This is not a Libpspio source tree - aborting now"
  exit 1
fi

# Create possibly missing directories
mkdir -p config/gnu config/m4

# Generate M4 macros
#echo "Generating M4 macros..."
#echo "done."

# Generate makefiles
#echo "Generating makefiles..."
#echo "done."

# Generate M4 includes
echo "Generating aclocal.m4..."
aclocal -I config/m4
echo "done."

# Generate configure auxiliary files
echo "Generating config.h.in..."
autoheader
echo "done."

# Generate configure
echo "Generating configure script..."
autoconf
echo "done."

# Generate libtool scripts
echo "Generating libtool scripts..."
my_libtoolize="libtoolize"
${my_libtoolize} --version >/dev/null 2>&1
if test "${?}" != "0"; then 
  my_libtoolize="glibtoolize"
fi
${my_libtoolize} --version >/dev/null 2>&1
if test "${?}" != "0"; then 
  echo "Error: could not find a working version of libtoolize" >&2
  exit 1
fi
${my_libtoolize} --automake --copy --force
echo "done."

# Generate makefile inputs
# Do not use "automake --force-missing", as it overwrites the INSTALL file.
echo "Generating Makefile.in for each directory..."
automake --add-missing --copy
echo "done."
