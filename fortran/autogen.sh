#!/bin/sh
#
# Copyright (C) 2011-2017 Yann Pouillon <devops@materialsevolution.es>
#
# This file is part of Libpspio.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# version 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.
#
# Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for more
# details.

# Stop at first error encountered
set -e

# Check that we are in the right directory
if test ! -s "./configure.ac" -o ! -s "src/pspiof.F90"; then
  echo "This is not a Libpspio source tree - aborting now"
  exit 1
fi

# Update source code
python scripts/make-fortran-constants.py

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
