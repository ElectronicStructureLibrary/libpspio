#
# Copyright (C) 2012 M. Oliveira, Y. Pouillon
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
pio_fortran_f_srcs = \
  pspio_f90_lib.F90 \
  pspio_f90_types.F90

# Fortran objects
pio_fortran_f_objs = $(pio_fortran_f_srcs:F90=$(LTOBJEXT))

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
AM_CPPFLAGS = @pio_core_incs@ -I$(srcdir)/..

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
  test_fortran \
  test_fortran_error

test_fortran_SOURCES = test_fortran.F90
test_fortran_LDADD = -lpspio_fortran -L$(builddir)/../.libs -lpspio
test_fortran_CPPFLAGS = -I$(srcdir)/..
test_fortran_DEPENDENCIES = ../libpspio.la libpspio_fortran.la $(pio_fortran_f_objs)

test_fortran_error_SOURCES = test_fortran_error.F90
test_fortran_error_LDADD = -lpspio_fortran -L$(builddir)/../.libs -lpspio
test_fortran_error_CPPFLAGS = -I$(srcdir)/..
test_fortran_error_DEPENDENCIES = ../libpspio.la libpspio_fortran.la $(pio_fortran_f_objs)

                    # ------------------------------------ #

#
# Explicit dependencies
#

pspio_f90_lib.$(LTOBJEXT): pspio_f90_types.$(LTOBJEXT)

                    # ------------------------------------ #

#
# Explicit targets
#

# Fortran modules have to be cleaned explicitely
clean-local:
	rm -f $(pio_fortran_mods)
