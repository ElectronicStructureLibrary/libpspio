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
  abinit_util.c \
  abinit_xc.c \
  interpolation.c \
  pspio_abinit.c \
  pspio_error.c \
  pspio_fhi.c \
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
  pspio_abinit.h \
  pspio_common.h \
  pspio_error.h \
  pspio_fhi.h \
  pspio_info.h \
  pspio_mesh.h \
  pspio_meshfunc.h \
  pspio_potential.h \
  pspio_projector.h \
  pspio_pspdata.h \
  pspio_qn.h \
  pspio_state.h \
  pspio_upf.h \
  pspio_xc_funcs.h \
  pspio_xc.h


# Internal C headers - keep this in alphabetical order
pio_hidden_hdrs = \
  abinit.h \
  interpolation.h \
  string_f.h \
  test_common.h \
  upf.h \
  util.h

                    # ------------------------------------ #

#
# Build targets
#

# Essential build parameters
AM_CPPFLAGS = @pio_core_incs@

# Libraries
lib_LTLIBRARIES = libpspio.la

libpspio_la_LDFLAGS = -version-info 0:0:0
libpspio_la_SOURCES = $(pio_core_srcs)

# Headers
include_HEADERS = $(pio_core_hdrs)
noinst_HEADERS = $(pio_hidden_hdrs)

# Fortran
if DO_BUILD_FORTRAN
  SUBDIRS = . fortran
endif

                    # ------------------------------------ #

#
# Test programs
#

pio_basic_tests = \
  test_error \
  test_mesh \
  test_meshfunc \
  test_qn \
  test_potential \
  test_projector \
  test_state \
  test_xc
pio_xfail_tests = \
  test_io
pio_wrapped_tests = \
  test_io_multiple.sh

check_PROGRAMS = $(pio_basic_tests) $(pio_xfail_tests)
nodist_check_SCRIPTS = $(pio_wrapped_tests)
TESTS = $(pio_basic_tests) $(pio_xfail_tests) $(pio_wrapped_tests)
XFAIL_TESTS = $(pio_xfail_tests)

test_error_SOURCES = test_error.c
test_error_LDADD = -lpspio
test_error_CPPFLAGS = -I$(srcdir)/../src
test_error_DEPENDENCIES = libpspio.la

test_io_SOURCES = test_io.c
test_io_LDADD = -lpspio
test_io_CPPFLAGS = -I$(srcdir)/../src
test_io_DEPENDENCIES = libpspio.la

test_mesh_SOURCES = test_mesh.c
test_mesh_LDADD = -lpspio
test_mesh_CPPFLAGS = -I$(srcdir)/../src
test_mesh_DEPENDENCIES = libpspio.la

test_meshfunc_SOURCES = test_meshfunc.c
test_meshfunc_LDADD = -lpspio
test_meshfunc_CPPFLAGS = -I$(srcdir)/../src
test_meshfunc_DEPENDENCIES = libpspio.la

test_potential_SOURCES = test_potential.c
test_potential_LDADD = -lpspio
test_potential_CPPFLAGS = -I$(srcdir)/../src
test_potential_DEPENDENCIES = libpspio.la

test_projector_SOURCES = test_projector.c
test_projector_LDADD = -lpspio
test_projector_CPPFLAGS = -I$(srcdir)/../src
test_projector_DEPENDENCIES = libpspio.la

test_qn_SOURCES = test_qn.c
test_qn_LDADD = -lpspio
test_qn_CPPFLAGS = -I$(srcdir)/../src
test_qn_DEPENDENCIES = libpspio.la

test_state_SOURCES = test_state.c
test_state_LDADD = -lpspio
test_state_CPPFLAGS = -I$(srcdir)/../src
test_state_DEPENDENCIES = libpspio.la

test_xc_SOURCES = test_xc.c
test_xc_LDADD = -lpspio
test_xc_CPPFLAGS = -I$(srcdir)/../src
test_xc_DEPENDENCIES = libpspio.la

MOSTLYCLEANFILES = test_*.tmp
