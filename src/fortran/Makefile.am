#
# Copyright (C) 2011-2012 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 3 of the License, or 
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# $Id: Makefile.am 256 2012-09-06 10:06:13Z micael $
#

#
# Makefile for the Fortran sources of Libpspio
#

                    # ------------------------------------ #

#
# Source files
#

# C source - keep this in alphabetical order
pio_fortran_c_srcs = pspio_fortran_c.c

# Fortran source - keep this in alphabetical order
pio_fortran_f_srcs = pspio_fortran_f.F90

# Fortran modules
if F90_MOD_UPPERCASE
  pio_fortran_mods = PSPIO_F90_TYPES_M.@ax_cv_f90_modext@ \
                     PSPIO_F90_LIB_M.@ax_cv_f90_modext@
else
  pio_fortran_mods = pspio_f90_types_m.@ax_cv_f90_modext@ \
                     pspio_f90_lib_m.@ax_cv_f90_modext@
endif

                    # ------------------------------------ #

#
# Build targets
#

# Essential build parameters
INCLUDES = @pio_core_incs@ -I$(srcdir)/..

# Libraries
lib_LTLIBRARIES = libpspio_fortran.la

libpspio_fortran_la_LDFLAGS = -version-info 0:0:0
libpspio_fortran_la_SOURCES = $(pio_fortran_c_srcs)
nodist_libpspio_fortran_la_SOURCES = $(pio_fortran_f_srcs)

# Headers
nodist_include_HEADERS = $(pio_fortran_mods)

                    # ------------------------------------ #

#
# Test programs
#

noinst_PROGRAMS = \
  test_fortran

test_fortran_SOURCES = test_fortran.F90
test_fortran_LDADD = -lpspio_fortran -L$(builddir)/../.libs -lpspio -lm
test_fortran_CPPFLAGS = -I$(srcdir)/..

                    # ------------------------------------ #

#
# Explicit targets
#

# Fortran modules have to be cleaned explicitely
# Note: the 'dummy' pseudo-file is there in case the variable would be empty.
clean-local:
	rm -f $(pio_fortran_mods) dummy

