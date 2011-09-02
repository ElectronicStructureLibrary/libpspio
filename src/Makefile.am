#
# Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete
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
# $Id$
#

#
# Makefile for the sources of Libpspio
#

                    # ------------------------------------ #

#
# Source files
#

# C source - keep this in alphabetical order
pio_core_srcs = \
  ab2libxc.c \
  pspio_error.c \
  pspio_mesh.c \
  pspio_meshfunc.c \
  pspio_potential.c \
  pspio_projector.c \
  pspio_pspdata.c \
  pspio_nlcc.c \
  pspio_qn.c \
  pspio_state.c \
  read_upf.c \
  test.c

# Exported C headers - keep this in alphabetical order
pio_core_hdrs = \
  pspio.h \
  pspio_common.h \
  pspio_error.h \
  pspio_mesh.h \
  pspio_meshfunc.h \
  pspio_potential.h \
  pspio_projector.h \
  pspio_pspdata.h \
  pspio_nlcc.h \
  pspio_qn.h \
  pspio_state.h

# Internal C headers - keep this in alphabetical order
pio_hidden_hdrs = \
  libxccodes.h \
  read_upf.h


# Test program
noinst_PROGRAMS = test
test_SOURCES = test.c
test_LDADD =  -lpspio -lm
test_CPPFLAGS = -I$(srcdir)/../src/


# Fortran source files and modules
if DO_BUILD_FORTRAN
  pio_fortran_srcs = pspio_fortran.F90
if F90_MOD_UPPERCASE
  pio_fortran_mods = PSPIO_FORTRAN.$(F90_MOD_EXT)
else
  pio_fortran_mods = pspio_fortran.$(F90_MOD_EXT)
endif
else
  pio_fortran_srcs =
  pio_fortran_mods =
endif

# Script-generated file lists
pio_built_srcs = $(pio_fortran_srcs)
pio_built_hdrs = $(pio_fortran_mods)

                    # ------------------------------------ #

#
# Build targets
#

# Essential build parameters
INCLUDES = @pio_core_incs@

# If there is any Fortran source file in the library under some
# circumstances (here when using --enable-fortran), Libtool will annoyingly
# select the Fortran linker in ALL cases, which causes the shared object
# to be silently screwed, the error appearing only at install time. The
# following trick should solve this issue.
pio_fortran_objs = $(pio_fortran_srcs:.F90=.$(LTOBJEXT))
pio_built_objs = $(pio_fortran_objs)

# Libraries
lib_LTLIBRARIES = libpspio.la

libpspio_la_LDFLAGS = -version-info 0:0:0
libpspio_la_SOURCES = $(pio_core_srcs)
if DO_BUILD_FORTRAN
  libpspio_la_LIBADD = $(pio_built_objs)
endif

# Headers
include_HEADERS = $(pio_core_hdrs)
noinst_HEADERS = $(pio_hidden_hdrs)
if DO_BUILD_FORTRAN
  nodist_include_HEADERS = $(pio_built_hdrs)
endif

# Fortran modules have to be cleaned explicitely
clean-local:
	rm -f $(pio_fortran_mods) dummy

                    # ------------------------------------ #

# Explicit dependencies (if needed, e.g. with Fortran)
# Use $(LTOBJEXT) as extension for object files
if DO_BUILD_FORTRAN
pspio_fortran.$(LTOBJEXT): pspio_fortran.F90
	@echo "FIXME"
	@false
endif
