#
# Copyright (C) 2011-2018 Yann Pouillon <devops@materialsevolution.es>
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

#
# Top makefile for Libpspio
#

# Autotools parameters
ACLOCAL_AMFLAGS = -I config/m4

# Main tasks
SUBDIRS = src tests
if DO_BUILD_FORTRAN
  SUBDIRS += fortran
endif

# Files to install for pkg-config
# See http://www.freedesktop.org/wiki/Software/pkg-config/ for details
pkgconfigdir = $(libdir)/pkgconfig
nodist_pkgconfig_DATA = config/data/libpspio.pc

# Some files are not removed by default and cause "make distclean" to fail
DISTCLEANFILES = \
  libpspio-*.tar.bz2 \
  libpspio-*.tar.gz \
  libpspio-*.tar.xz \
  config/data/libpspio.pc \
  config/data/libpspio-config.yml

# Source tarball
EXTRA_DIST = \
  Doxyfile \
  psp_references
