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
  pspio_fortran_c.c \
  pspio_info.c \
  pspio_mesh.c \
  pspio_meshfunc.c \
  pspio_potential.c \
  pspio_projector.c \
  pspio_pspdata.c \
  pspio_qn.c \
  pspio_state.c \
  pspio_upf.c \
  pspio_xc.c \
  upf_read.c \
  upf_tag.c \
  upf_write.c \
  upf_xc.c \
  util.c


# Exported C headers - keep this in alphabetical order
pio_core_hdrs = \
  pspio.h \
  pspio_common.h \
  pspio_error.h \
  pspio_info.h \
  pspio_mesh.h \
  pspio_meshfunc.h \
  pspio_potential.h \
  pspio_projector.h \
  pspio_pspdata.h \
  pspio_qn.h \
  pspio_state.h \
  pspio_upf.h \
  pspio_xc.h


# Internal C headers - keep this in alphabetical order
pio_hidden_hdrs = \
  libxccodes.h \
  string_f.h \
  test_common.h \
  upf.h \
  util.h

# Fortran source files and modules
pio_fortran_srcs = pspio_fortran_f.F90
if F90_MOD_UPPERCASE
  pio_fortran_mods = PSPIO_F90_TYPES_M.@ax_cv_f90_modext@ \
                     PSPIO_F90_LIB_M.@ax_cv_f90_modext@
else
  pio_fortran_mods = pspio_f90_types_m.@ax_cv_f90_modext@ \
                     pspio_f90_lib_m.@ax_cv_f90_modext@
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

# Libraries
lib_LTLIBRARIES = libpspio.la

libpspio_la_LDFLAGS = -version-info 0:0:0
libpspio_la_SOURCES = $(pio_core_srcs)
if DO_BUILD_FORTRAN
  nodist_libpspio_la_SOURCES = $(pio_built_srcs)
endif

# Headers
include_HEADERS = $(pio_core_hdrs)
noinst_HEADERS = $(pio_hidden_hdrs)
if DO_BUILD_FORTRAN
  nodist_include_HEADERS = $(pio_built_hdrs)
endif

                    # ------------------------------------ #

#
# Test programs
#

noinst_PROGRAMS = \
  test_error \
  test_fortran \
  test_mesh \
  test_meshfunc \
  test_qn \
  test_potential \
  test_projector \
  test_state \
  test_upf \
  test_xc

test_error_SOURCES = test_error.c
test_error_LDADD = -lpspio -lm
test_error_CPPFLAGS = -I$(srcdir)/../src

test_fortran_SOURCES = test_fortran.F90
test_fortran_LDADD = -lpspio -lm
test_fortran_CPPFLAGS = -I$(srcdir)/../src

test_mesh_SOURCES = test_mesh.c
test_mesh_LDADD = -lpspio -lm
test_mesh_CPPFLAGS = -I$(srcdir)/../src

test_meshfunc_SOURCES = test_meshfunc.c
test_meshfunc_LDADD = -lpspio -lm
test_meshfunc_CPPFLAGS = -I$(srcdir)/../src

test_potential_SOURCES = test_potential.c
test_potential_LDADD = -lpspio -lm
test_potential_CPPFLAGS = -I$(srcdir)/../src

test_projector_SOURCES = test_projector.c
test_projector_LDADD = -lpspio -lm
test_projector_CPPFLAGS = -I$(srcdir)/../src

test_qn_SOURCES = test_qn.c
test_qn_LDADD = -lpspio -lm
test_qn_CPPFLAGS = -I$(srcdir)/../src

test_state_SOURCES = test_state.c
test_state_LDADD = -lpspio -lm
test_state_CPPFLAGS = -I$(srcdir)/../src

test_upf_SOURCES = test_upf.c
test_upf_LDADD = -lpspio -lm
test_upf_CPPFLAGS = -I$(srcdir)/../src

test_xc_SOURCES = test_xc.c
test_xc_LDADD = -lpspio -lm
test_xc_CPPFLAGS = -I$(srcdir)/../src

                    # ------------------------------------ #

#
# Explicit targets
#

# Fortran modules have to be cleaned explicitely
# Note: the 'dummy' pseudo-file is there in case the variable would be empty.
clean-local:
	rm -f $(pio_fortran_mods) dummy

