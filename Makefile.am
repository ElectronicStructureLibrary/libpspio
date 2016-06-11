#
# Copyright (C) 2011-2015 Yann Pouillon <notifications@materialsevolution.es>
#
# This file is part of Libpspio.
#
# Libpspio is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, version 3 of the License, or (at your option) any later
# version.
#
# Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
# the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
# 02110-1301  USA.

#
# Top makefile for Libpspio
#

# Autotools parameters
ACLOCAL_AMFLAGS = -I config/m4

# Main tasks
SUBDIRS = src
if DO_BUILD_FORTRAN
  SUBDIRS += fortran
endif

# Source tarball
EXTRA_DIST = \
  Doxyfile \
  psp_references

DISTCLEANFILES = libpspio-*.tar.gz
